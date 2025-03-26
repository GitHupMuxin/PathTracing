#include <random>
#include "engine/core/random_queue.h"

namespace core
{
    RandomQueue* RandomQueue::instance = nullptr;

    RandomQueue* RandomQueue::GetInstance()
    {
        if (instance == nullptr)
            instance = new RandomQueue();
        return instance;
    } 

    void RandomQueue::PushRandomNum()
    {
        int t = 10;
        while (t--)
        {

            static std::random_device rd;
            static std::mt19937 gen(rd());
            static std::uniform_real_distribution<float> dis(0.0f, 1.0f);

            this->numQue.push(dis(gen));
        }
    }

    float RandomQueue::GetRandomNum()
    {
        if (this->numQue.empty())
            this->PushRandomNum();
        float temp = this->numQue.front();
        this->numQue.pop();
        return temp;
    }

    RandomQueue::RandomQueue() { }

    RandomQueue::~RandomQueue()
    {
        if (instance != nullptr)
            delete instance;
    }



}



