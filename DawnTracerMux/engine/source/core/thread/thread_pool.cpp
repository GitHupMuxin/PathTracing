#include <assert.h>
#include <iostream>
#include "engine/platform/normal_system_config.h"
#include "engine/core/thread/thread_pool.h"

namespace core
{

    ThreadPool::Worker::Worker(std::function<void()>&& workerMain)
    {      
        this->worker_ = std::make_unique<std::thread>(std::move(workerMain));
    } 


    void ThreadPool::WorkerMain()
    {
        unsigned int taskCount = 0;
        while (!this->shutdown_)
        {
            if (this->jobsQueue_.Empty())
            {
                std::unique_lock<std::mutex> uniqueLock(this->mtx);
                this->notifyJobQueue_.wait(uniqueLock, [this]{ return !this->jobsQueue_.Empty() || this->shutdown_; });
            }
            if (this->shutdown_)
                break;
            Job job;
            auto getJob = this->jobsQueue_.Pop(job);

            if (getJob)
            {
                job();
                taskCount++;
            }
            if (this->jobsQueue_.Empty())
                this->condition.notify_one();
        }

        std::cout << "--Thread shutdown. Finish task:" << taskCount << "\n";
    } 

    ThreadPool::Job::Job(std::unique_ptr<std::function<void()>>&& func)
    {
        this->func_ = std::move(func);
    }

    void ThreadPool::Job::operator() ()
    {
        (*this->func_.get())();
    }

    std::unique_ptr<ThreadPool> ThreadPool::instance_ = std::make_unique<ThreadPool>();

    ThreadPool::ThreadPool() : shutdown_(false)
    {
        unsigned int ThreadCount = platform::NormalSystemConfig::GetSystemHardwareConcurrency();
        while (ThreadCount--)
            this->workers_.emplace_back(std::move(std::bind(&ThreadPool::WorkerMain, this)));
    }

    ThreadPool::~ThreadPool()
    {
        this->ShutDownAllThreads();
        for (auto& it : this->workers_)
        {
            if (it.worker_->joinable())
                it.worker_->join();
        }
    }

    void ThreadPool::WaiteAll()
    {
        std::unique_lock ul(this->wiatAllMtx);
        this->condition.wait(ul, [this]() { return this->jobsQueue_.Empty(); });
    }

    void ThreadPool::ShutDownAllThreads()
    {
        this->shutdown_ = true;
        this->notifyJobQueue_.notify_all();
        for (auto& it : this->workers_)
        {
            if (it.worker_->joinable())
                it.worker_->join();
        }
    }
    
    ThreadPool* ThreadPool::GetInstance()
    {
        return ThreadPool::instance_.get();        
    }

}




