#include <sys/time.h>
#include <string.h>

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
   static StackElement curStack;
   //TODO: compare this to backtrace(). Anyone has a good profiler???? :-)
   Stackwalker::getStacktrace(BACKTRACE_LENGTH, curStack, 2 /*2 internal, FW frames */);
   m_data.addStack(curStack);
}

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
