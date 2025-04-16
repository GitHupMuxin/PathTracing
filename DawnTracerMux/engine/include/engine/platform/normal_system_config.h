#pragma once
#include <stddef.h>

namespace platform
{
    class NormalSystemConfig
    {
        public:
            static unsigned int        GetSystemHardwareConcurrency();
    };

}

