#include <assert.h>
#include "engine/platform/normal_system_config.h"
#include "engine/core/thread/thread_pool.h"

namespace core
{

    ThreadPool::Worker::Worker(std::function<void()> workerMain)
    {      
        this->worker_ = std::make_unique<std::thread>(workerMain);
    } 


    void ThreadPool::WorkerMain()
    {
        while (!this->shutdown_)
        {
            std::unique_lock<std::mutex> uniqueLock(this->mtx);
            this->notifyJobQueue_.wait(uniqueLock, [this]{ return !this->jobsQueue_.Empty() && !this->shutdown_; });
            Job job;
            auto getJob = this->jobsQueue_.Pop(job);

            if (getJob)
                job();
        }
    } 

    ThreadPool::Job::Job(std::unique_ptr<std::function<void()>>& func)
    {
        this->func_ = std::move(func);
    }

    void ThreadPool::Job::operator() ()
    {
        (*this->func_.get())();
    }

    std::unique_ptr<ThreadPool> ThreadPool::instance_ = std::make_unique<ThreadPool>();

    ThreadPool::ThreadPool()
    {
        unsigned int ThreadCount = platform::NormalSystemConfig::GetSystemHardwareConcurrency();
        while (ThreadCount--)
            this->workers_.emplace_back(static_cast<std::function<void()>>(std::bind(&ThreadPool::WorkerMain, this)));
    }

    ThreadPool* ThreadPool::GetInstance()
    {
        return ThreadPool::instance_.get();        
    }




}




