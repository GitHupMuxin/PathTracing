#pragma once
#include <string>
#include "MATH.h"
#include <regex>
#include <queue>

namespace Tool
{
    class stringTool
    {
        public:
            static std::vector<std::string> stringSplit(const std::string& str, char delim);  
    };

    void DrawToFile(std::string& path, const int& width, const int& height, std::vector<MATH::Vector3f>& frameBuffer);

    void SRGBDrawToFile(std::string& path, const int& width, const int& height, std::vector<MATH::Vector3f>& frameBuffer);
    
    void execution_progress(float progress);

    MATH::Vector3f toWorldSpace(const MATH::Vector3f& dir, const MATH::Vector3f& n);

    class randomQueue
    {
        private:
            static randomQueue*     instance;
            std::queue<float>       numQue; 

            randomQueue();
            randomQueue(const randomQueue& r) = delete;
            randomQueue(const randomQueue&& r) = delete;
            void                    pushRandomNum();

        public:
            static randomQueue*     getInstance();
            float                   getRandomNum();

            ~randomQueue();
    };

}