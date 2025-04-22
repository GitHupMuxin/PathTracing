#include <memory>
#include <assert.h>
#include <iostream>
#include "engine/core/memory/central_cache.h"
#include "engine/core/memory/page_cache.h"
#include "engine/core/memory/memory_utils.h"

namespace core
{
    namespace memory
    {
        CentralCache::CentralCache() { }

        Span* CentralCache::GetOneSpan(SpanList& list, size_t size)
        {
            Span* result = nullptr;
            Span* it = list.Begin();
            while (it != list.End())
            {
                if (it->freeList_ == nullptr)
                    it = it->nextSpan_;
                else 
                {
                    result = it;
                    break;
                }
            }
            if (result == nullptr)
            {
                size_t pageSize = MemoryUtils::SizeToPage(size);
                Span* span = PageCache::GetInstance()->OfferASpan(pageSize);

                char* star = (char*)(span->pageID_ << PAGE_CACHE_PAGE_SIZE_SHIFT);
                char* end = (char*)(star + (span->n_ << PAGE_CACHE_PAGE_SIZE_SHIFT));

                span->freeList_ = star;
                void* tail = star;
                star += size;

                while (star < end)
                {
                    MemoryUtils::GetNextObject(tail) = star;
                    star += size;
                    tail = MemoryUtils::GetNextObject((void*)tail);
                }

                MemoryUtils::GetNextObject(tail) = nullptr;
                list.PushFront(span);
                result = span;
            }


            return result;
        }

        CentralCache* CentralCache::GetInstance() noexcept
        {
            static CentralCache instance;
            return &instance;
        }


        size_t CentralCache::OfferRangeMemory(void* & star, void* & end, size_t n, size_t size)
        {
            size_t index = MemoryUtils::SizeToIndex(size);
            size_t count = 1;

            std::lock_guard lg(this->buckets_[index].lock_);
        
            Span* span = this->GetOneSpan(this->buckets_[index], size);
                
            assert(span);
            assert(span->freeList_);

            star = end = span->freeList_;
            while (count < n && MemoryUtils::GetNextObject(end) != nullptr)
            {
                end = MemoryUtils::GetNextObject(end);
                count++;
            }
            span->freeList_ = MemoryUtils::GetNextObject(end);
            MemoryUtils::GetNextObject(end) = nullptr;            

            span->usedCount_ += count;

            return count;
        }

        void CentralCache::ReleaseListToSpans(void* star, size_t size) noexcept
        {   
            size_t index = MemoryUtils::SizeToIndex(size);
            std::lock_guard lg(this->buckets_[index].lock_);
            while (star)
            {
                void* next = MemoryUtils::GetNextObject(star);

                Span* span = PageCache::GetInstance()->MapObjectToSpan(star);

                MemoryUtils::GetNextObject(star) = span->freeList_;
                span->freeList_ = star;

                span->usedCount_--;
                if (span->usedCount_ == 0)
                {
                    this->buckets_[index].Erase(span);
                    span->freeList_ = nullptr;
                    span->nextSpan_ = nullptr;
                    span->previousSpan_ = nullptr;
                    span->used_ = false;
                    PageCache::GetInstance()->ReleaseSpanToPageCache(span);
                }

                star = next;
            }
        }

        void CentralCache::ReleaseListToSpansByIndex(void* star, size_t index) noexcept
        {
            std::lock_guard lg(this->buckets_[index].lock_);
            while (star)
            {
                void* next = MemoryUtils::GetNextObject(star);

                Span* span = PageCache::GetInstance()->MapObjectToSpan(star);

                MemoryUtils::GetNextObject(star) = span->freeList_;
                span->freeList_ = star;

                span->usedCount_--;
                if (span->usedCount_ == 0)
                {
                    this->buckets_[index].Erase(span);
                    span->freeList_ = nullptr;
                    span->nextSpan_ = nullptr;
                    span->previousSpan_ = nullptr;
                    span->used_ = false;
                    PageCache::GetInstance()->ReleaseSpanToPageCache(span);
                }

                star = next;
            }
        }

        void CentralCache::Show() const noexcept
        {
            std::cout << "Show the CentralCache." << std::endl;
            for (int i = 0; i < MAX_FREE_LIST_NUM; i++)
            {
                std::cout << "SpanLists " << i + 1 << ". ";
                this->buckets_[i].Show();
            }
            std::cout << std::endl;
        }

    }
}

