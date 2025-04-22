#ifdef _WIN32
#include <windows.h>
#include <iostream>
#include "engine/platform/platform_utils.h"
#include <vector>

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


    const long int PlatformUtils::GetSystemPageSize()
    {
        SYSTEM_INFO systemInfo;
        GetSystemInfo(&systemInfo);
        return systemInfo.dwPageSize;
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
        SYSTEM_INFO systemInfo;
        GetSystemInfo(&systemInfo);
        size_t pageSize = GetSystemPageSize();
        size_t numPages = (size + pageSize - 1) / pageSize;

        void* ptr = VirtualAlloc(nullptr, numPages * pageSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

        if(!ptr)
        {
            std::cerr << "VirtualAlloc failed: " << GetLastError() << std::endl;
        }

        return ptr;
    }

    void PlatformUtils::SystemFreePageMemory(void* ptr, size_t size)
    {
        (void)size;
        VirtualFree(ptr, 0, MEM_RELEASE);
    }

    size_t PlatformUtils::GetCacheLineSize()
    {
        DWORD buffer_size = 0;
        std::vector<SYSTEM_LOGICAL_PROCESSOR_INFORMATION> buffer;

        // ��һ�ε��û�ȡ���軺������С
        if (!GetLogicalProcessorInformation(nullptr, &buffer_size)) {
            if (GetLastError() != ERROR_INSUFFICIENT_BUFFER) {
                return 64; // Ĭ��ֵ
            }
        }

        // ���仺����
        buffer.resize(buffer_size / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION));
        if (!GetLogicalProcessorInformation(buffer.data(), &buffer_size)) {
            return 64; // Ĭ��ֵ
        }

        // ���������߼���������Ϣ
        for (const auto& info : buffer) {
            if (info.Relationship == RelationCache) {
                const CACHE_DESCRIPTOR& cache = info.Cache;
                // ���� L1 ���ݻ���
                if (cache.Level == 1 && cache.Type == CacheData) {
                    return cache.LineSize;
                }
            }
        }

        return 64; // Ĭ��ֵ������ x86 �ܹ���
    }

}
#endif

