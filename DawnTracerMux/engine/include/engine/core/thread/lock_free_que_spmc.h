#pragma once
#include <atomic>

namespace core
{
    template<typename T>
    class LockFreeQueueSPMC
    {
        private:
            static const unsigned int DEFAULTQUEUESZIE = 4096;

            unsigned int size_;
            unsigned int mask_;
            std::atomic<unsigned int> bottom_;
            std::atomic<unsigned int> top_;
            T** lockFreeQueue_;

        public:
            LockFreeQueueSPMC(int size = LockFreeQueueSPMC::DEFAULTQUEUESZIE);
            void Push(T* t);
            T* Pop();
    };

}



