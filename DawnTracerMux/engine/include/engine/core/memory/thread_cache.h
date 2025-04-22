#pragma once
#include <stddef.h>
#include "free_list.h"
#include "engine/core/memory/central_cache.h"

namespace core
{
    namespace memory
    {

        class ThreadCache
        {
            private:
                FreeList buckets_[MAX_FREE_LIST_NUM];

                static size_t       MemoryAlignment(size_t size) noexcept;
                static size_t       MemoryAlignment(size_t size, size_t alignNum) noexcept;


                void                AskMemoryForCenterCache(size_t index, size_t alignNum);
            public:
                ThreadCache();
                ~ThreadCache();
                void                DeallocateAll();
                void*               Allocate(size_t size);
                void                Deallocate(void* memory, size_t size) noexcept;
                void                GiveBackToCentralCache(FreeList& list, size_t size);
                void                Show() const noexcept;
            
        };


        static thread_local std::shared_ptr<ThreadCache> LOCAL_THREAD_CACHE(new ThreadCache());


    }

}


