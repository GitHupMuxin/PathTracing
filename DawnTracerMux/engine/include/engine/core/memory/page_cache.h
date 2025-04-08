#pragma once
#include <stddef.h>
#include <mutex>
#include <memory>
#include <unordered_map>
#include "span.h"

namespace core
{
    namespace memory
    {
        typedef SpanList PageSpanList;
        const static size_t PAGE_BUCKET_SIZE = 128;
        
        class PageCache
        {
            private:
                PageSpanList bucket_[PAGE_BUCKET_SIZE];        
                std::unordered_map<PageID, Span* > idMapSpan;
                mutable std::recursive_mutex lock_;

                PageCache();
                PageCache(const PageCache& pageCache) = delete;
                PageCache& operator = (const PageCache& pageCache) = delete;
            public:
                static std::shared_ptr<PageCache> instance_;

                static std::shared_ptr<PageCache> GetInstance() noexcept;
                Span* OfferASpan(size_t pageCount);
                Span* MapObjectToSpan(void* object);
                void ReleaseSpanToPageCache(Span* span);
                void Show() const noexcept;
        };
    }
}



