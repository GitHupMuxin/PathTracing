#pragma once
#include <mutex>
#include <atomic>

namespace tool
{
    class TestBlock
    {
        public:
            struct TestData
            {
                unsigned int    rayCount;
                unsigned int    rayTimeCount;
            };

            TestData                data;

            

            TestBlock();
            inline void addRayCount()
            {
                data.rayCount++;    
            }
            ~TestBlock();
    };

    static thread_local TestBlock* TEST_BLOCK = new TestBlock;

    class TotalTestBlock
    {
        private:
            TotalTestBlock();
            TotalTestBlock(const TotalTestBlock&) = delete;
            void operator = (const TotalTestBlock&) = delete;
        public:
            struct TotalTestData
            {
                std::atomic<uint64_t> totalRay{0};
                std::atomic<uint64_t> totalRayTime{0};
            };
            
            TotalTestData           data;
            static TotalTestBlock*   GetInstance();
            ~TotalTestBlock();
    };

}


