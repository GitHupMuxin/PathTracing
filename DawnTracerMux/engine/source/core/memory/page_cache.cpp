#include <assert.h>
#include <iostream>
#include "engine/platform/platform_utils.h"
#include "engine/core/memory/memory_utils.h"
#include "engine/core/memory/page_cache.h"


namespace core
{
    namespace memory
    {
        PageCache::PageCache() { }

        PageCache* PageCache::GetInstance() noexcept
        {
            static PageCache instance;
            return &instance;
        }
        
        Span* PageCache::OfferASpan(size_t pageCount)
        {
            assert(pageCount > 0 && pageCount <= PAGE_BUCKET_SIZE);

            std::lock_guard lg(this->lock_);

            Span* result = nullptr;
            if (!this->bucket_[pageCount - 1].Empty())
                result = this->bucket_[pageCount - 1].PopFront();
            else 
            {
                for (size_t i = pageCount + 1; i <= PAGE_BUCKET_SIZE; i++)   
                {
                    if (!this->bucket_[i - 1].Empty())
                    {
                        Span* nSpan = this->bucket_[i - 1].PopFront();
                        Span* kSpan = new Span;

                        kSpan->pageID_ = nSpan->pageID_;
                        kSpan->n_ = pageCount;

                        nSpan->pageID_ += pageCount;
                        nSpan->n_ -= pageCount;

                        this->idMapSpan[nSpan->pageID_] = nSpan;
                        this->idMapSpan[nSpan->pageID_ + nSpan->n_ - 1] = nSpan;

                        this->bucket_[nSpan->n_ - 1].PushFront(nSpan);

                        result = kSpan;
                    }
                }

                if (result == nullptr)
                {
                    void* ptr = platform::PlatformUtils::SystemAllocaPageMemory(PAGE_BUCKET_SIZE * PAGE_CACHE_PAGE_SIZE);
                    
                    Span* span = new Span;
                    span->pageID_ = ((PageID)(ptr)) >> PAGE_CACHE_PAGE_SIZE_SHIFT;
                    span->n_ = PAGE_BUCKET_SIZE;
                    
                    this->bucket_[span->n_ - 1].PushFront(span);

                    result = this->OfferASpan(pageCount);
                }
            }

            for (int i = 0; i < result->n_; i++)
                this->idMapSpan.insert(std::make_pair(result->pageID_ + i, result));

            result->used_ = true;
            return result;
        }


        Span* PageCache::MapObjectToSpan(void* object)
        {
            PageID id = (((PageID)(object)) >> PAGE_CACHE_PAGE_SIZE_SHIFT);

            auto span = this->idMapSpan.find(id);
            if (span == this->idMapSpan.end())
            {
                assert(false);
                return nullptr;
            }

            return span->second;
        }

        void PageCache::ReleaseSpanToPageCache(Span* span)
        {
            std::lock_guard lg(this->lock_);   
            while (1)
            {
                PageID leftID = span->pageID_ - 1;
                auto it = this->idMapSpan.find(leftID);
                if (it == this->idMapSpan.end())
                    break;
                Span* leftSpan = it->second;
                if (leftSpan->used_ || leftSpan->n_ + span->n_ >= PAGE_BUCKET_SIZE)
                    break;
                span->pageID_ = leftSpan->pageID_;
                span->n_ += leftSpan->n_;

                this->bucket_[leftSpan->n_ - 1].Erase(leftSpan);
                this->idMapSpan.erase(leftID);
                this->idMapSpan.erase(leftID - leftSpan->n_);
                this->bucket_[span->n_ - 1].PushFront(span);
                delete leftSpan;
            }

            while (1)
            {
                PageID rightID = span->pageID_ + span->n_;
                auto it = this->idMapSpan.find(rightID);
                if (it == this->idMapSpan.end())
                    break;
                Span* rightSpan = it->second;
                if (rightSpan->used_ || rightSpan->n_ + span->n_ > PAGE_BUCKET_SIZE)
                    break;
                span->pageID_ = rightSpan->pageID_;
                span->n_ += rightSpan->n_;
                
                this->bucket_[rightSpan->n_ - 1].Erase(rightSpan);
                this->idMapSpan.erase(rightID);
                this->idMapSpan.erase(rightID + rightSpan->n_ - 1);
                this->bucket_[span->n_ - 1].PushFront(span);
                delete rightSpan;
            }
        }

        void PageCache::Show() const noexcept
        {
            std::cout << "Show Page Cache :" << std::endl;
            std::cout << "--- bucket ---" << std::endl;
            for (int i = 0; i < PAGE_BUCKET_SIZE; i++)
            {
                std::cout << "bucket " << i + 1 << ". :" << std::endl;
                this->bucket_[i].Show();
            }

            std::cout << std::endl;

            std::cout << "--- PageID Map Span ---" << std::endl;

            for (auto it : this->idMapSpan)
                std::cout << "PageID : " << it.first << "   " << "Span" << it.second << std::endl; 
            
            std::cout << std::endl;
        }



    }
}



