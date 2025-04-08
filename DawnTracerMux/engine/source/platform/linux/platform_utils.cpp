#ifdef __linux__
#include <unistd.h>
#include <limits.h>
#include <sys/mman.h>
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


    const long int PlatformUtils::GetSystemPageSize()
    {
        return sysconf(_SC_PAGE_SIZE);
    }


    const long int PlatformUtils::GetSystemPageShift()
    {
        long int i = 0;
        long int x = PlatformUtils::GetSystemPageSize();
        while (!(x & (1 << i)))
            i++;
        return i;
    }

    void* PlatformUtils::SystemAllocaPageMemory(size_t size)
    {
        size_t pageSize = GetSystemPageSize();
        size_t numPage = (size + pageSize - 1) / pageSize;

        void* ptr = mmap(nullptr, numPage * pageSize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

        if (ptr == MAP_FAILED)
        {
            perror("mmap failed");
            ptr = nullptr;
        }

        return ptr;
    }

    void PlatformUtils::SystemFreePageMemory(void* ptr, size_t size)
    {
        size_t pageSize = GetSystemPageSize();
        size_t numPage = (size + pageSize - 1) / pageSize;
        munmap(ptr, numPage * pageSize);
    }


}

#endif
