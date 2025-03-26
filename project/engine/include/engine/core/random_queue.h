#include <queue>

namespace core
{
    class RandomQueue
    {
        private:
            static RandomQueue*     instance;
            std::queue<float>       numQue; 

            RandomQueue();
            RandomQueue(const RandomQueue& r) = delete;
            RandomQueue(const RandomQueue&& r) = delete;
            void                    PushRandomNum();

        public:
            static RandomQueue*     GetInstance();
            float                   GetRandomNum();

            ~RandomQueue();
    };


}



