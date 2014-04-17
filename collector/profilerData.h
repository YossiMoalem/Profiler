#ifndef PROFILER_DATA_H
#define PROFILER_DATA_H

#include <iostream>
#include <string.h> //for memcmp
#include <assert.h>

#include "stackwalker.h"

#ifndef BACKTRACE_LENGTH
#define BACKTRACE_LENGTH 5
#endif //BACKTRACE_LENGTH


#define NUM_OF_BUCKETS 1024
#define ELEMENTS_IN_BUCKET 4
#define NUM_OF_INTERNAL_FRAMES 2

#define FRAME_DELEMETER ":"
#define HIT_DELEMETER  "="

typedef   Stackwalker::stackFrameAddr StackData[BACKTRACE_LENGTH] ;
struct OneStack
{
   public:

      void set (const StackData& iNewAddr)
      {
         memcpy (mStack, iNewAddr, sizeof (mStack));
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
      static unsigned int hashToBucket (const StackData& iStack)
      {
         return (unsigned int)iStack[0] % NUM_OF_BUCKETS;
      }

      bool operator == (const StackData& iOther) const
      {
         return (0 == memcmp (mStack, iOther, sizeof (StackData)));
      }

   private: 
      StackData mStack ;
};

/********************************************************************************
 * This is the class that hold all the collected stacktrace data:
 * It holds predefined number of backets. Each backet holds predefined number of stacks.
 * When a new stack is recieved, we calculate to which bucket it bellongs:
 *  If we already have this stack in this bucket - we increment the hit counter.
 *  If we do not have it, byut we have space in the bucket - we add it to the bucket
 *  otherwise we flush a stuck, and write the new one.
 *
 *   This does not needs to be TS. We anly call it fron the signal handler, that sends
 *   one thread only...
 ********************************************************************************/
class ProfilerData
{
   /********************************************************************************
    * This is the building block of the accumilated data:
    * It holds the stack trace (of a single frame)
    * and the hit-counter.
    * It is also responsible to flush the data
    ********************************************************************************/
   struct DataElement
   {
      DataElement () : m_hitCounter (0)
      { }

      void addHit ()
      {
         ++m_hitCounter;
      }

      bool isEmpty () const
      {
         return m_hitCounter == 0;
      }

      void markAsEmpty()
      {
         m_hitCounter = 0;
      }

      void flush () 
      {
         if (!isEmpty())
         {
            m_stack.flush();
            std::cout <<HIT_DELEMETER<<m_hitCounter <<std::endl;
            markAsEmpty();
         }
      }

      void set (const StackData& i_stack)
      {
         assert (isEmpty());
         m_stack.set (i_stack);
         m_hitCounter = 1;
      }

      unsigned int getHitCounter () const
      {
         return m_hitCounter;
      }

      const OneStack& getStack () const
      {
         return m_stack;
      }

      private:
      OneStack m_stack;
      unsigned int m_hitCounter;
   };

   public:
   /********************************************************************************
    * Flush all data (should only be called, when profiler is stoped
    ********************************************************************************/
   void flush () 
   {
      for (int i = 0; i < NUM_OF_BUCKETS; ++i)
      {
         for (int ii = 0; ii < ELEMENTS_IN_BUCKET; ++ii)
         {
            m_data[i][ii].flush();
         }
      }
   }

   /********************************************************************************
    * Add one stack frame
    * Implements the logic described in the class comment
    ********************************************************************************/
   void addStack (const StackData& i_stack)
   {
      unsigned int bucketIndex = OneStack::hashToBucket(i_stack);
      unsigned int index = getWritePosInBacket(bucketIndex, i_stack);

      if ( m_data[bucketIndex][index].isEmpty())
      {
         m_data[bucketIndex][index].set(i_stack); 
      } else {
         m_data[bucketIndex][index].addHit();
      }
   }

   private:

   /********************************************************************************
    * This function is responsible of finding the place in the bucket where 
    * the frame should bw written to (see the class description for details)
    * It will ALWAYS return an index to write to. If needed this index will be flushed
    * so we can overwrite it.
    ********************************************************************************/
   unsigned int getWritePosInBacket (int bucketIndex, const StackData& i_stack )// const
   {
      unsigned int minHitCounter    = m_data[bucketIndex][0].getHitCounter();
      unsigned int minConterIndex   = 0;

      for (unsigned int i = 0; i < ELEMENTS_IN_BUCKET; ++i)
      {
         if (m_data[bucketIndex][i].isEmpty())
         {
            return i;
         }
         if (m_data[bucketIndex][i].getStack() == i_stack)
         {
            return i;
         }
         if (m_data[bucketIndex][i].getHitCounter() < minHitCounter)
         {
            minConterIndex = i;
         }
      }
      m_data[bucketIndex][minConterIndex].flush();
      return minConterIndex;
   }


   private:
   DataElement m_data [NUM_OF_BUCKETS][ELEMENTS_IN_BUCKET];

};


#endif //PROFILER_DATA_H
