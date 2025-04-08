#include <assert.h>
#include <algorithm>
#include "engine/core/memory/memory_utils.h"

namespace core
{
    namespace memory
    {
        size_t MemoryUtils::SizeToIndex(size_t size, size_t alignShift) noexcept
        {
            return ((size + (1 << alignShift) - 1) >> alignShift) - 1;
        }


        size_t MemoryUtils::SizeToIndex(size_t size) noexcept
        {
            size_t result = 0;
            if (size < 128)
                //8 -> 3
                result = MemoryUtils::SizeToIndex(size, 3) + BUCKET_SIZE[0];
            else if (size < 1024)
                //16 -> 4
                result = MemoryUtils::SizeToIndex(size - 128, 4) + BUCKET_SIZE[1];
            else if (size < 8 * 1024)
                //128 -> 7
                result = MemoryUtils::SizeToIndex(size - 1024, 7) + BUCKET_SIZE[2];
            else if (size < 64 * 1024)
                //1024 -> 10
                result = MemoryUtils::SizeToIndex(size - 8 * 1024, 10) + BUCKET_SIZE[3];
            else if (size < 256 * 1024)
                //8192 -> 13
                result = MemoryUtils::SizeToIndex(size - 64 * 1024, 13) + BUCKET_SIZE[4];
            else 
            {
                assert(false);
                result = -1;
            }

            return result;
        }

        void*& MemoryUtils::GetNextObject(void* listNode) noexcept
        {
            return *((void**)listNode);
        } 

        size_t MemoryUtils::NumOfAllocateSize(size_t size) noexcept
        {
            assert(size > 0);
            int number = THREAD_CACHE_MAX_SIZE / size;
            number = std::clamp(number, 2, 512);
            return number;
        }

        size_t MemoryUtils::SizeToPage(size_t size) noexcept
        {
            size_t number = MemoryUtils::NumOfAllocateSize(size);

            size_t nPage = number * size;

            nPage /= PAGE_CACHE_PAGE_SIZE;
            
            return nPage == 0 ? 1 : nPage;
        }
    }
}



