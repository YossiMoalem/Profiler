#include <stdio.h>
#include <signal.h>
#include <sys/time.h>

#include "profiler.h"
struct timespec sleepTime;
#define SLEEP_  nanosleep(&sleepTime,NULL);\
               std::cout <<"l";

#define SLEEP for(int i = 0; i < 999; i++){\
               for (int ii = 0; ii < 999; ii++ ) {\
                   }}

void f1111 ()
{
   SLEEP ;
}

void f111 ()
{
   SLEEP ;
   f1111();
}

void f211 ()
{
   SLEEP ;
   f1111();
}

void f311 ()
{
   SLEEP ;
   f1111();
}

void f11()
{
   SLEEP ;
   f111();
}


void f12()
{
   SLEEP ;
   f111();
}

void f21()
{
   SLEEP ;
   f211();
}


void f22()
{
   SLEEP ;
   f211();
}

void f31()
{
   SLEEP ;
   f311();
}


void f32()
{
   SLEEP ;
   f311();
}

void f1 ()
{
   SLEEP ;
   f11();
   f12();
}

void f2 ()
{
   SLEEP ;
   f21();
   f22();
}

void f3 ()
{
   SLEEP ;
   f31();
   f32();
}

int main ()
{
   sleepTime.tv_sec = 1;
   sleepTime.tv_nsec = 0; //1000 * 1000 *1000;

   for (int i  = 0; i < 1; ++i)
   {
   SLEEP ;
   f1();
   f2();
   f3();
   }

   return 0;

}
