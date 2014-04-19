#ifndef PROFILER_H
#define PROFILER_H


#include <signal.h>

#include "profilerData.h"
#include "stackData.h"

static void sigProfHandler (int );

/********************************************************************************
 *  This is the class that activated the Profiler.
 *  It sets signal handler, which takes a snapshot of the backtrace, and collects it
 *  And sets a timer to call this signal handler, at predefined time.
 ********************************************************************************/
class Profiler
{
   friend void sigProfHandler (int );
   public:
   static Profiler* instance ();
   int init();
   int finilize();

   private:
   void report();
   int  getLogFD();
   Profiler ();

   private:
   static Profiler*         s_inst;
   ProfilerData<OneStack>   m_data;
   struct sigaction         m_oldHandler;

};


#endif //PROFILER_H
