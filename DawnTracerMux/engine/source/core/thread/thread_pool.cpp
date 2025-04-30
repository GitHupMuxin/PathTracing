#include <assert.h>
#include <iostream>
#include "engine/platform/normal_system_config.h"
#include "engine/core/thread/thread_pool.h"
#include "engine/tools/test_block.h"

namespace core
{
    unsigned int ThreadPool::Worker::uniqueWorkerID = 0;

    ThreadPool::Worker::Worker(std::function<void()>&& workerMain)
    {      
        this->workerID_ = Worker::workerID_++;
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

        std::thread::id id = std::this_thread::get_id();
        std::cout << "--Thread shutdown. Finish task:" << taskCount << "\n";
        std::cout << "thread id: " << id << std::endl;
    } 

    ThreadPool::Job::Job(std::unique_ptr<std::function<void()>>&& func)
    {
        this->func_ = std::move(func);
    }

    void ThreadPool::Job::operator() ()
    {
        (*this->func_.get())();
    }

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

        std::cout << "Total ray count:" << tool::TotalTestBlock::GetInstance()->data.totalRay.load() << std::endl;
        std::cout << "Total ray time count:" << tool::TotalTestBlock::GetInstance()->data.totalRayTime.load() << std::endl;
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
        static ThreadPool instance;
        return &instance;        
    }

    const unsigned int ThreadPool::GetThreadCount()
    {
        return this->workers_.size();
    }

}




