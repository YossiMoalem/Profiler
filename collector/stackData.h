#ifndef STACK_DATA_H
#define STACK_DATA_H

#include "stackwalker.h"
#define FRAME_DELEMETER ":"

#ifndef BACKTRACE_LENGTH
#define BACKTRACE_LENGTH 5
#endif //BACKTRACE_LENGTH

#define NUM_OF_INTERNAL_FRAMES 2

typedef Stackwalker::stackFrameAddr RawStack[BACKTRACE_LENGTH] ;
struct OneStack
{
   public:

      void set (const RawStack& iNewAddr)
      {
         memcpy (mStack, iNewAddr, sizeof (mStack));
      }

      void set (const OneStack& iNewStack)
      {
         memcpy (mStack, iNewStack.mStack, sizeof(mStack));
      }

      void flush ()
      {
         for (int i = 0; i < BACKTRACE_LENGTH; ++i)
         {
            if (i > 0)
               std::cout  <<FRAME_DELEMETER <<mStack[i];
            else 
               std::cout <<mStack[i];
         }
      }

      /********************************************************************************
       * Get the bucket index for a stack
       ********************************************************************************/
      static unsigned int hashToBucket (const OneStack& iStack)
      {
         return (unsigned int)iStack.mStack[0] % NUM_OF_BUCKETS;
      }

      bool operator == (const OneStack& iOther) const
      {
         return (0 == memcmp (mStack, iOther.mStack, sizeof (RawStack)));
      }

   private: 
      RawStack mStack ;
};

#endif
