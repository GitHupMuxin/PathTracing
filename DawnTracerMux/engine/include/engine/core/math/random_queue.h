#pragma once
#include <queue>
#include "engine/core/thread/thread_safe_que.h"

namespace core
{
    class RandomQueue
    {
        private:
            std::queue<float>       numQue_; 

            RandomQueue() = default;
            ~RandomQueue() = default;
            RandomQueue(const RandomQueue& r) = delete;
            RandomQueue(const RandomQueue&& r) = delete;
            void                    PushRandomNum();

        public:
            static RandomQueue*     GetInstance();
            float                   GetRandomNum();

    };

    class ThreadSafeRandomQueue
    {
        private:
            std::mutex mtx;
            std::queue<float> numQue_;

            ThreadSafeRandomQueue() = default;
            ~ThreadSafeRandomQueue() = default;
            ThreadSafeRandomQueue(const ThreadSafeRandomQueue& r) = delete;
            ThreadSafeRandomQueue(const ThreadSafeRandomQueue&& r) = delete;
            void                            PushRandomNum();
        public:
            static ThreadSafeRandomQueue*   GetInstance(); 
            float                           GetRandomNum();

    };


}



