#ifndef PROFILER_DATA_H
#define PROFILER_DATA_H

#include <string.h> //for memcmp
#include <assert.h>
#include <time.h> //for time
#include <fcntl.h>//for open
#include <stdlib.h> //for exit
#include <unistd.h> //for getpid
#include <errno.h>

#include "bucketContainerElement.h"

#define NUM_OF_BUCKETS 1024

#define ELEMENTS_IN_BUCKET 4


/********************************************************************************
 * This is the class that hold data in buckets:
 * It holds predefined number of backets. (NUM_OF_BUCKETS)
 * Each backet holds predefined number of keys. (ELEMENTS_IN_BUCKET)
 * When a new key is recieved, we check to which bucket it bellongs:
 *  If we already have this key in this bucket - we increment the hit counter. //TODO:
 *  If we do not have it, but we have space in the bucket - we add it to the bucket
 *  otherwise we flush one element, and write the new one.
 *
 *   For the profiler:
 *   This does not needs to be TS. We anly call it fron the signal handler, that sends
 *   one thread only...
 ********************************************************************************/
template <typename KEY>
class ProfilerData
{

   public:

   ProfilerData(int iLogFD) : mLogFD(iLogFD)
   { }

   /********************************************************************************
    * Flush all data (should only be called, when profiler is stoped
    ********************************************************************************/
   void flush () 
   {
      for (int i = 0; i < NUM_OF_BUCKETS; ++i)
      {
         for (int ii = 0; ii < ELEMENTS_IN_BUCKET; ++ii)
         {
            m_data[i][ii].flush(mLogFD);
         }
      }
   }

   /********************************************************************************
    * Add one stack frame
    * Implements the logic described in the class comment
    ********************************************************************************/
   void addHit (const KEY& iKey)
   {
      unsigned int bucketIndex = KEY::hashToBucket(iKey);
      unsigned int index = getWritePosInBacket(bucketIndex, iKey);

      if ( m_data[bucketIndex][index].isEmpty())
      {
         m_data[bucketIndex][index].set(iKey); 
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
   unsigned int getWritePosInBacket (int bucketIndex, const KEY& iKey )// const
   {
      unsigned int minHitCounter    = m_data[bucketIndex][0].getHitCounter();
      unsigned int minConterIndex   = 0;

      for (unsigned int i = 0; i < ELEMENTS_IN_BUCKET; ++i)
      {
         if (m_data[bucketIndex][i].isEmpty())
         {
            return i;
         }
         if (m_data[bucketIndex][i].getData() == iKey)
         {
            return i;
         }
         if (m_data[bucketIndex][i].getHitCounter() < minHitCounter)
         {
            minConterIndex = i;
         }
      }
      m_data[bucketIndex][minConterIndex].flush(mLogFD);
      return minConterIndex;
   }


   private:
   DataElement<KEY> m_data [NUM_OF_BUCKETS][ELEMENTS_IN_BUCKET];
   int              mLogFD;

};


#endif //PROFILER_DATA_H
