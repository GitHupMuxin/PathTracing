#include "lock_free_que_spmc.h"

namespace core
{
    template<typename T>
    LockFreeQueueSPMC<T>::LockFreeQueueSPMC(int size)
    {
        this->size_ = size;
        this->mask_ = 0;
        this->top_ = 0;
        this->bottom_ = 0;
        this->lockFreeQueue_ = new (T*)()
    }

    template<typename T>
    void  LockFreeQueueSPMC<T>::Push(T* t)
    {
        unsigned int nBottom = this->bottom_.load();
        this->lockFreeQueue_[nBottom & this->mask_] = t;
        this->bottom_++;
    }

    template<typename T>
    T* LockFreeQueueSPMC<T>::Pop()
    {
        unsigned int nTop = this->top_.load();
        unsigned int nBottom = this->bottom_.load();
        if (nBottom - nTop > 0)
        {
            if (this->top_.compare_exchange_strong(nTop, nTop + 1))
                return this->lockFreeQueue_[nTop];
        }

        return nullptr;
    }
}


