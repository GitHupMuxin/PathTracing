#pragma once

#include <string>

namespace platform
{
    class PlatformUtils
    {   
        public:
            static const std::string    GetExecutionPath();
            static const long int       GetSystemPageSize();
            static const long int       GetSystemPageShift();
            static void*                SystemAllocaPageMemory(size_t size);
            static void                 SystemFreePageMemory(void* ptr, size_t size);
            static size_t               GetCacheLineSize();     
    };
}


