#include "engine/platform/normal_system_config.h"
#include <thread>

namespace platform
{
    constexpr unsigned int NormalSystemConfig::GetSystemHardwareConcurrency()
    {
        return std::thread::hardware_concurrency();
    }
}


