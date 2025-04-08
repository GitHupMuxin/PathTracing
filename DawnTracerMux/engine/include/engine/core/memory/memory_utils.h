#pragma once
#include <stddef.h>
#include "engine/platform/platform_utils.h"

namespace core
{
    namespace memory
    {
        static const size_t THREAD_CACHE_MAX_SIZE = 256 * 1024;
        static const size_t PAGE_CACHE_PAGE_SIZE = platform::PlatformUtils::GetSystemPageSize();
        static const size_t PAGE_CACHE_PAGE_SIZE_SHIFT = platform::PlatformUtils::GetSystemPageShift();
        static const size_t BUCKET_SIZE[5] = {0, 0 + 16, 0 + 16 + 56, 0 + 16 + 56 + 56, 0 + 16 + 56 + 56 + 56};

        typedef size_t PageID;
        class MemoryUtils
        {
            public:
                static void*& GetNextObject(void* listNode) noexcept;       
                
                static size_t SizeToIndex(size_t size) noexcept;
                static size_t SizeToIndex(size_t size, size_t alignShift) noexcept; 

                static size_t SizeToPage(size_t size) noexcept;

                static size_t NumOfAllocateSize(size_t size) noexcept;
        };
    }
}


