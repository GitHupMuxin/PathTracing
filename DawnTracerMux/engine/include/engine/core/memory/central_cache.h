#pragma once
#include <memory>
#include "span.h"

namespace core
{
    namespace memory
    {
        static const unsigned int MAX_FREE_LIST_NUM = 208;
        class CentralCache
        {
            private:
                SpanList buckets_[MAX_FREE_LIST_NUM];

                CentralCache();
                CentralCache(const CentralCache& centralCache) = delete;
                CentralCache& operator= (const CentralCache& centralCache) = delete;

                Span* GetOneSpan(SpanList& list, size_t size);
            public:
                static std::shared_ptr<CentralCache> instance_;

                static std::shared_ptr<CentralCache> GetInstance() noexcept;

                size_t OfferRangeMemory(void* & star, void* & end, size_t n, size_t size);

                void ReleaseListToSpans(void* star, size_t size) noexcept;
                void ReleaseListToSpansByIndex(void* star, size_t index) noexcept;
                void Show() const noexcept;

        };
    }
}



