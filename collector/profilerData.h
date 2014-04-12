#ifndef PROFILER_DATA_H
#define PROFILER_DATA_H

#include <iostream>
#ifndef BACKTRACE_LENGTH
#define BACKTRACE_LENGTH 5
#endif //BACKTRACE_LENGTH


#define NUM_OF_BUCKETS 1024
#define ELEMENTS_IN_BUCKET 4
#define NUM_OF_INTERNAL_FRAMES 2

#define FRAME_DELEMETER ":"
#define HIT_DELEMETER  "="

typedef void* StackElement[BACKTRACE_LENGTH]; 

/********************************************************************************
 * This is the class that hold all the collected stacktrace data:
 * It holds predefined number of backets. Each backet holds predefined number of stacks.
 * When a new stack is recieved, we calculate to which bucket it bellongs:
 *  If we already have this stack in this bucket - we increment the hit counter.
 *  If we do not have it, byut we have space in the bucket - we add it to the bucket
 *  otherwise we flush a stuck, and write the new one.
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
      StackElement m_stack;
      unsigned int m_hitCounter;

      DataElement () : m_hitCounter (0) {}

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
            for (int i = 0; i < BACKTRACE_LENGTH; ++i)
            {
               if (i > 0)
               std::cout  <<FRAME_DELEMETER <<m_stack[i];
               else 
                  std::cout <<m_stack[i];

            }

            std::cout <<HIT_DELEMETER<<m_hitCounter <<std::endl;
            markAsEmpty();
         }
      }

      void set (const StackElement     i_stack)
      {
         for (int i = 0; i < BACKTRACE_LENGTH; ++i)
         {
            m_stack[i] = i_stack[i];
         }
         m_hitCounter = 1;
      }
   };

   public :

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
   void addStack (const StackElement i_stack)
   {
      unsigned int bucketIndex = hash (i_stack);
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
    * Get the bucket index for a stack
    ********************************************************************************/
   unsigned int hash (const StackElement i_stack) const 
   {
      return (unsigned int)i_stack[0] % NUM_OF_BUCKETS;
   }

   /********************************************************************************
    * Basically, this is operator == for StackElement...
    ********************************************************************************/
   unsigned int isSameStack (const StackElement i_first, const StackElement i_second) const
   {
      bool isSame = true;
      for (unsigned int i = 0; i < BACKTRACE_LENGTH && isSame ; ++i)
      {
         isSame = (i_first[i] == i_second[i]);
      }
      return isSame;
   }


   /********************************************************************************
    * This function is responsible of finding the place in the bucket where 
    * the frame should bw written to (see the class description for details)
    * It will ALWAYS return an index to write to. If needed this index will be flushed
    * so we can overwrite it.
    ********************************************************************************/
   unsigned int getWritePosInBacket (int bucketIndex, const StackElement i_stack ) 
   {
      unsigned int minHitCounter    = m_data[bucketIndex][0].m_hitCounter;
      unsigned int minConterIndex   = 0;

      for (unsigned int i = 0; i < ELEMENTS_IN_BUCKET; ++i)
      {
         if (m_data[bucketIndex][i].isEmpty())
         {
            return i;
         }
         if (isSameStack(i_stack, m_data[bucketIndex][i].m_stack))
         {
            return i;
         }
         if (m_data[bucketIndex][i].m_hitCounter < minHitCounter)
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
