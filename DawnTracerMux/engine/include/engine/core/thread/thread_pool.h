#pragma once
#include <vector>
#include <memory>
#include <thread>
#include <condition_variable>
#include <future>
#include <functional>
#include "thread_safe_que.h"

namespace core
{
    class ThreadPool
    {
        private:
            class Worker
            {
                private:
                    static unsigned int                             uniqueWorkerID;
                public:
                    unsigned int                                    workerID_;
                    ThreadPool*                                     threadPool_;
                    std::unique_ptr<std::thread>                    worker_;
                    Worker(std::function<void()>&& workerMain);
            };
            void                                                    WorkerMain();
            
            class Job
            {
                private:
                    std::unique_ptr<std::function<void()>> func_;        
                    
                public:
                    Job() = default;
                    Job(std::unique_ptr<std::function<void()>>&& func);
                    void operator() ();
            };

            bool                                                    shutdown_;
            mutable std::mutex                                      mtx;
            mutable std::mutex                                      wiatAllMtx;
            std::condition_variable                                 notifyJobQueue_;
            ThreadSafeQueue<Job>                                    jobsQueue_;
            std::vector<Worker>                                     workers_;
            std::condition_variable                                 condition;

            ThreadPool();
        public:
            ~ThreadPool();
            void                                                    WaiteAll();
            void                                                    ShutDownAllThreads();

            static ThreadPool*                                      GetInstance();
            const unsigned int                                      GetThreadCount();

            template<typename F, typename ...Args>
            auto Commit(F&& f, Args&& ...args) -> std::future<decltype(f(args...))>
            {
                using jobFunction = std::function<void()>;
                using returnType = decltype(f(args...));

                std::function<returnType()> func = std::bind(std::forward<F>(f), std::forward<Args>(args)...);

                auto task = std::make_shared<std::packaged_task<returnType()>>(func);
               
                Job job(std::make_unique<jobFunction>([task](){ (*task)(); }));

                this->jobsQueue_.Push(std::move(job));

                this->notifyJobQueue_.notify_one();

                return task->get_future();
            }


    };

}




