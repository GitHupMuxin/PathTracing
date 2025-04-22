#include <iostream>
#include "engine/core/thread/thread_pool.h"

int main()
{
    unsigned int loopTime = 1e9;
    auto func1 = [&]() -> void {
        std::cout << "in the first function.\n";
        unsigned int loop = loopTime;
        while (loop--) ;
    };

    auto func2 = [&]() -> void {
        std::cout << "in the second function.\n";
        unsigned int loop = loopTime;
        while (loop--) ;
    };

    auto func3 = [&]() -> void {
        std::cout << "in the third function.\n";
        unsigned int loop = loopTime;
        while (loop--) ;
    };

    for (int i = 0; i < 1000; i++)
        core::ThreadPool::GetInstance()->Commit(func1);
    for (int i = 0; i < 1000; i++)
        core::ThreadPool::GetInstance()->Commit(func2);
    for (int i = 0; i < 1000; i++)
        core::ThreadPool::GetInstance()->Commit(func3);
   
    std::cout << "wait\n" << std::endl;
    core::ThreadPool::GetInstance()->WaiteAll();

    core::ThreadPool::GetInstance()->ShutDownAllThreads();
    return EXIT_SUCCESS;
}




