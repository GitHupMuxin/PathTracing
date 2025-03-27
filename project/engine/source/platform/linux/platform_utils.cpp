#ifdef __linux__
#include <unistd.h>
#include <limits.h>
#include "engine/platform/platform_utils.h"

namespace platform
{
    const std::string PlatformUtils::GetExecutionPath()
    {
        std::string result;
        char buffer[PATH_MAX];
        ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
        if (len != -1)
        {
            buffer[len] = '\0';
            result = buffer;   
        }

        size_t last_slash = result.find_last_of("/");
        if (last_slash != std::string::npos)
            result = result.substr(0, last_slash);
        return result;
    }
}

#endif
