#ifndef BUCKET_CONTAINER_ELEMENT_H
#define BUCKET_CONTAINER_ELEMENT_H

#include <stdio.h> //for snprintf
/********************************************************************************
 * This is the building block of the accumilated data:
 * It holds the stack trace (of a single frame)
 * and the hit-counter.
 * It is also responsible to flush the data
 ********************************************************************************/
#define HIT_DELEMETER  "="

template <typename KEY>
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

   void flush (int iLogFD) 
   {
      if (!isEmpty())
      {
         static size_t maxElementLength = KEY::valLength + sizeof (HIT_DELEMETER) + 10 /* int max size */ + 1;
         char elementStr[maxElementLength];
         size_t length = mData.toString(elementStr);
         length += snprintf (&elementStr[length], maxElementLength - length, "%s", HIT_DELEMETER);
         length += snprintf (&elementStr[length], maxElementLength - length, "%d\n", m_hitCounter);

         write (iLogFD, elementStr, length);

         markAsEmpty();
      }
   }

   void set (const KEY& iKey)
   {
      assert (isEmpty());
      mData.set (iKey);
      m_hitCounter = 1;
   }

   unsigned int getHitCounter () const
   {
      return m_hitCounter;
   }

   const KEY& getData() const
   {
      return mData;
   }

   static unsigned int hashToBucket (const KEY& iKey)
   {
      return KEY::hashToBucket(iKey);
   }

 private:
   KEY mData;
   unsigned int m_hitCounter;
};

#endif
