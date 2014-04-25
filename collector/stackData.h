#ifndef STACK_DATA_H
#define STACK_DATA_H

#include <assert.h>//for assert 
#include <string.h>//for memcpy
#include <errno.h>//for errno

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
     //first we calculate the size of address in bits. 
     //Every 4 bits is represented by i char.
     //Then we nees to add 2 for the 0x
     static const int SizeOfAddressAsString = (sizeof(void*) * 8 / 4) + 2 ; 
     static const size_t valLength = (SizeOfAddressAsString + sizeof(FRAME_DELEMETER)) * BACKTRACE_LENGTH;

      void set (const RawStack& iNewAddr)
      {
         memcpy (mStack, iNewAddr, sizeof (mStack));
      }

      void set (const OneStack& iNewStack)
      {
         memcpy (mStack, iNewStack.mStack, sizeof(mStack));
      }

      const char* toString()
      {
         static char buff [valLength];
         size_t length = snprintf (buff, valLength, "%p", mStack[0]);
         
         for (int i = 1; i < BACKTRACE_LENGTH; ++i)
         {
            length += snprintf (&buff[length], valLength - length, "%s%p", FRAME_DELEMETER, mStack[i]);
         }
         assert (length < valLength);
         return buff;
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
