#include <assert.h>
#include <algorithm>
#include <iostream>
#include "engine/core/memory/thread_cache.h"
#include "engine/core/memory/memory_utils.h"

namespace core
{
    namespace memory
    {

        size_t ThreadCache::MemoryAlignment(size_t size) noexcept
        {
            if (size <= 128)
                return MemoryAlignment(size, 8);
            else if (size <= 1024)
                return MemoryAlignment(size, 16);
            else if (size <= 8 * 1024)
                return MemoryAlignment(size, 128);
            else if (size <= 64 * 1024)
                return MemoryAlignment(size, 1024);
            else if (size <= 256 * 1024)
                return MemoryAlignment(size , 8 * 1024);
            else 
            {
                assert(false);
                return -1;
            }
        }
          
        size_t ThreadCache::MemoryAlignment(size_t size, size_t alignNum) noexcept
        {
            return (size + alignNum - 1) & ~(alignNum - 1);    
        }

        
        

        void ThreadCache::AskMemoryForCenterCache(size_t index, size_t alignSize)
        {
            size_t number = 0;
            size_t upperBound = this->buckets_[index].GetUpperBound_();
            size_t clampSize = MemoryUtils::NumOfAllocateSize(alignSize);
            
            if (upperBound < clampSize)
            {
                number = upperBound;
                this->buckets_[index].SetUpperBound_(upperBound + 1);
            }
            else 
                number = clampSize;
            
            void* star = nullptr;
            void* end = nullptr;

            size_t recvSize = CentralCache::GetInstance().get()->OfferRangeMemory(star, end, number, alignSize);

            this->buckets_[index].PushRange(star, end, recvSize);
        }

        void ThreadCache::DeallocateAll()
        {
            for (int i = 0; i < MAX_FREE_LIST_NUM; i++)
            {
                if (this->buckets_[i].IsEmpty()) continue;

                void* star = nullptr;
                void* end = nullptr;
                this->buckets_[i].PopRange(star, end, this->buckets_[i].freeSize_);
                CentralCache::GetInstance().get()->ReleaseListToSpansByIndex(star, i);
            }
        }

        ThreadCache::ThreadCache() { }

        ThreadCache::~ThreadCache()
        {
            this->DeallocateAll();
        }

        void* ThreadCache::Allocate(size_t size)
        {
            size_t alignSize = ThreadCache::MemoryAlignment(size);
            size_t index = MemoryUtils::SizeToIndex(size);
            if (this->buckets_[index].IsEmpty())
                this->AskMemoryForCenterCache(index, alignSize);
            return this->buckets_[index].Pop();
        }
       
        void ThreadCache::Deallocate(void* memory, size_t size) noexcept
        {
            assert(memory);

            size_t index = MemoryUtils::SizeToIndex(size);
            this->buckets_[index].Push(memory);

            if (this->buckets_[index].Size() >= this->buckets_[index].GetUpperBound_())
                this->GiveBackToCentralCache(this->buckets_[index], size);     

        }
        
        void ThreadCache::GiveBackToCentralCache(FreeList& list, size_t size)
        {
            void* star = nullptr;
            void* end = nullptr;
            list.PopRange(star, end, list.GetUpperBound_());

            CentralCache::GetInstance().get()->ReleaseListToSpans(star, size);
        }
        
        void ThreadCache::Show() const noexcept
        {
            std::cout << "Show the ThreadCache." << std::endl;
            for (int i = 0; i < MAX_FREE_LIST_NUM; i++)
            {
                std::cout << "buckets number " << i + 1 << ": ";
                this->buckets_[i].Show();
            }   
            std::cout << std::endl;
        }
        
            
    }
}
