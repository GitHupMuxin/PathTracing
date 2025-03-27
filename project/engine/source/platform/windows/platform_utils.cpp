#ifdef _WIN32
#include <windows.h>
#include "engine/platform/platform_utils.h"

namespace platform
{
    const std::string PlatformUtils::GetExecutionPath()
    {
        std::string result;
        
        char buffer[MAX_PATH];
        GetModuleFileNameA(NULL, buffer, MAX_PATH);
        result = buffer;
        for (auto& c : result)
            if (c == '\\') 
                c = '/';

        size_t last_slash = result.find_last_of("/");
        if (last_slash != std::string::npos)
            result = result.substr(0, last_slash);

        return result;
    }
}
#endif

