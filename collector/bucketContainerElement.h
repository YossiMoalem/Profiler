#ifndef BUCKET_CONTAINER_ELEMENT_H
#define BUCKET_CONTAINER_ELEMENT_H

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

   void flush () 
   {
      if (!isEmpty())
      {
         mData.flush();
         std::cout <<HIT_DELEMETER<<m_hitCounter <<std::endl;
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
