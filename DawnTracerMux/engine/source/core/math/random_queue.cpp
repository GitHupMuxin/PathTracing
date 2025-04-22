#include <random>
#include "engine/core/math/random_queue.h"

namespace core
{
    RandomQueue* RandomQueue::GetInstance()
    {
        static RandomQueue instance;
        return &instance;
    } 

    void RandomQueue::PushRandomNum()
    {
        int t = 100;
        while (t--)
        {

            static std::random_device rd;
            static std::mt19937 gen(rd());
            static std::uniform_real_distribution<float> dis(0.0f, 1.0f);

            this->numQue_.push(dis(gen));
        }
    }

    float RandomQueue::GetRandomNum()
    {
        if (this->numQue_.empty())
            this->PushRandomNum();
        float temp = this->numQue_.front();
        this->numQue_.pop();
        return temp;
    }

    ThreadSafeRandomQueue* ThreadSafeRandomQueue::GetInstance()
    {
        static ThreadSafeRandomQueue instance;
        return &instance;
    } 

    void ThreadSafeRandomQueue::PushRandomNum()
    {
        int t = 100;
        while (t--)
        {

            static std::random_device rd;
            static std::mt19937 gen(rd());
            static std::uniform_real_distribution<float> dis(0.0f, 1.0f);

            this->numQue_.push(dis(gen));
        }
    }

    float ThreadSafeRandomQueue::GetRandomNum()
    {
        std::unique_lock gl(this->mtx);
        if (this->numQue_.empty())
            this->PushRandomNum();
        float temp = this->numQue_.front();
        this->numQue_.pop();
        return temp;
    }

}



