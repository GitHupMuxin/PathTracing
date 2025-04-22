#include <iostream>
#include "engine/core/memory/thread_cache.h"
#include "engine/core/memory/page_cache.h"

int main()
{
    
    long long int* arr[10];
    for (int i = 0; i < 5; i++)
    {
        arr[i] = (long long int*)core::memory::LOCAL_THREAD_CACHE->Allocate(sizeof(long long int));

    } 

    //core::memory::LOCAL_THREAD_CACHE.get()->Show();
    //core::memory::CentralCache::GetInstance().get()->Show();
    //core::memory::PageCache::GetInstance().get()->Show();

    for (int i = 0; i < 5; i++)
    {
        core::memory::LOCAL_THREAD_CACHE->Deallocate(arr[i], sizeof(long long int));
    }
    core::memory::LOCAL_THREAD_CACHE->DeallocateAll();
    //core::memory::LOCAL_THREAD_CACHE.get()->Show();
    //core::memory::CentralCache::GetInstance().get()->Show();
    core::memory::PageCache::GetInstance()->Show();

    return EXIT_SUCCESS;

}



