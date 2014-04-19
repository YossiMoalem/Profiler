#include <sys/time.h>//for setitimer
#include <string.h> //for memset
#include <iostream>//for std::cerr

#include "profiler.h"
#include "stackwalker.h"


/********************************************************************************
 * Signal Handler. Just call the report() method
 ********************************************************************************/
static void sigProfHandler (int )
{
   Profiler::instance()->report () ;
}

Profiler* Profiler::s_inst = NULL;

Profiler* Profiler::instance ()
{
   if (s_inst == NULL)
   {
      s_inst = new Profiler();
   }
   return s_inst;
}

/********************************************************************************
 * Init:
 *  Set the signal handler
 *  start the timer.
 ********************************************************************************/
int Profiler::init ()
{
   struct sigaction handler;
   memset(&handler, 0, sizeof(handler));
   handler.sa_handler = sigProfHandler;
   handler.sa_flags = SA_RESTART;

   sigaction (SIGALRM, &handler ,&m_oldHandler);
   
   // Start profiling 
   struct itimerval tout_val;
   tout_val.it_value.tv_sec = 0; 
   tout_val.it_value.tv_usec = 1000;/* start after 1 ms */
   tout_val.it_interval.tv_sec = 0; 
   tout_val.it_interval.tv_usec = 1000;/* repeat every 1 ms */
   //setitimer(ITIMER_PROF, &tout_val,0);
   setitimer(ITIMER_REAL, &tout_val,0);

   return 0;
}

/********************************************************************************
 * Finalize
 *  Stop the timer
 *  Remove the signal handler (restore old)
 *  flush all collected data
 ********************************************************************************/
int Profiler::finilize ()
{
   // Stop profiling 
   struct itimerval tout_val;
   tout_val.it_value.tv_sec = 0; 
   tout_val.it_value.tv_usec = 0;
   tout_val.it_interval.tv_sec = 0; 
   tout_val.it_interval.tv_usec = 0;
   setitimer(ITIMER_REAL, &tout_val,0);

   // Restore old handler
   sigaction (SIGPROF, &m_oldHandler, NULL);
   
   //Pour all the collected data
   m_data.flush();
   return 0;
}

/********************************************************************************
 * Report
 *  Get stacktrace, and add it to the data
 ********************************************************************************/
void Profiler::report ()
{
   static RawStack curRowStack;
   static OneStack curStack;
   //TODO: compare this to backtrace(). Anyone has a good profiler???? :-)
   Stackwalker::getStacktrace(BACKTRACE_LENGTH, curRowStack, 2 /*2 internal, FW frames */);
   curStack.set(curRowStack);
   m_data.addHit(curStack);
}

/********************************************************************************
 * getLogFD
 * Create log file. This should be passed to the data container to flusg data to.
 ********************************************************************************/
int Profiler::getLogFD()
{
   //Open log file
   char filename[100];
   snprintf(filename, 100, "Prof_%d_%ld.prf", getpid(), time(0));
   int logFD = open (filename, O_WRONLY | O_CREAT | O_EXCL | O_NOATIME );
   if (logFD == -1)
   {
      if (errno == EEXIST)
      {
         std::cerr <<"Error: The file: "<<filename <<" already exists. Cowerdly exiting.." <<std::endl;
      } else if (errno == EACCES){
         std::cerr <<"Error: Cannot open log file "<<filename<<" (Permission Denied). Exiting..."<<std::endl; 
      } else {
         std::cerr <<"Error: Cannot open log file "<<filename<<"(errno = " <<errno <<" ). Exiting..."<<std::endl;
      }
      exit(1);
   }
    return logFD;
}

/********************************************************************************
 * Profiler default C'tor
 ********************************************************************************/
Profiler::Profiler () : m_data(getLogFD())
{ }

/********************************************************************************
 * ProfilerStarter
 *  This class is created in order to create one static instance of it.
 *  this is used to start the profiler when the app starts, and stop it,
 *  when it finishes.
 ********************************************************************************/
class ProfilerStarter 
{
   public:
   ProfilerStarter()
   {
      Profiler::instance()->init();
   }
   ~ProfilerStarter ()
   {
      Profiler::instance()->finilize();
   }
};

static ProfilerStarter sProfilerSTarter;
