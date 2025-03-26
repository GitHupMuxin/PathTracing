#include "Tool.h"
#include "MATH.h"
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <random>

namespace Tool
{
    std::vector<std::string> stringTool::stringSplit(const std::string & str, char delim)
    {
        std::stringstream ss(str);
        std::vector<std::string> elems;
        std::string s;
        while (getline(ss, s, delim))
            elems.emplace_back(s);
        return elems;
    }



    void DrawToFile(std::string& path, const int& width, const int& height, std::vector<MATH::Vector3f>& frameBuffer)
    {
        FILE* fd = fopen(path.c_str(), "wb");
        fprintf(fd, "P6\n%d %d\n255\n", width, height);
        unsigned char color[3];
        for (int i = 0; i < width * height; i++)
        {
            color[0] = (unsigned char)(255 * std::clamp(frameBuffer[i].r, 0.0f, 1.0f));
            color[1] = (unsigned char)(255 * std::clamp(frameBuffer[i].g, 0.0f, 1.0f));
            color[2] = (unsigned char)(255 * std::clamp(frameBuffer[i].b, 0.0f, 1.0f));
            fwrite(color, 1, 3, fd);
        }
        fclose(fd);
    }

    void SRGBDrawToFile(std::string& path, const int& width, const int& height, std::vector<MATH::Vector3f>& frameBuffer)
    {
        FILE* fd = fopen(path.c_str(), "wb");
        fprintf(fd, "P6\n%d %d\n255\n", width, height);
        unsigned char color[3];
        for (int i = 0; i < width * height; i++)
        {
            color[0] = (unsigned char)(255 * std::pow(std::clamp(frameBuffer[i].r, 0.0f, 1.0f), 0.6f));
            color[1] = (unsigned char)(255 * std::pow(std::clamp(frameBuffer[i].g, 0.0f, 1.0f), 0.6f));
            color[2] = (unsigned char)(255 * std::pow(std::clamp(frameBuffer[i].b, 0.0f, 1.0f), 0.6f));
            fwrite(color, 1, 3, fd);
        }
        fclose(fd);
    }

    void execution_progress(float progress)
    {
        std::cout << "[";
        int x = 0;
        int scale = 100;
        int t = progress * scale;
        while (x < t)
        {
            std::cout << ">";
            x++;
        }
        while (x < scale)
        {
            std::cout << "-";
            x++;
        }

        std::cout << "]" << (int)(progress * 100) << "%";
        if (progress != 1)
            std::cout << "\r";
        else 
            std::cout << std::endl;
        std::cout.flush();
    }

    MATH::Vector3f toWorldSpace(const MATH::Vector3f& dir, const MATH::Vector3f& n)
    {
        // MATH::Vector3f T(0.0f), B(0.0f);
        // if (n.x < 0.99f)
        // {
        //     T = MATH::Vector3f(1.0, 1.0, 0.0);
        //     B = MATH::crossProduction(n, T).normalize();
        //     T = MATH::crossProduction(B , n).normalize();
        // }
        // else 
        // {
        //     T = MATH::Vector3f(0.0f, 1.0f, 0.0f);
        //     B = MATH::crossProduction(n, T).normalize();
        //     T = MATH::crossProduction(B, n).normalize();
        // }

        // return dir.x * T + dir.y * B + dir.z * n;

        MATH::Vector3f B, C;
        if (std::fabs(n.x) > std::fabs(n.y)){
            float invLen = 1.0f / std::sqrt(n.x * n.x + n.z * n.z);
            C = MATH::Vector3f(n.z * invLen, 0.0f, -n.x *invLen);
        }
        else {
            float invLen = 1.0f / std::sqrt(n.y * n.y + n.z * n.z);
            C = MATH::Vector3f(0.0f, n.z * invLen, -n.y *invLen);
        }
        B = MATH::crossProduction(C, n);
        return dir.x * B + dir.y * C + dir.z * n;

    }

    randomQueue* randomQueue::instance = nullptr;

    randomQueue* randomQueue::getInstance()
    {
        if (instance == nullptr)
            instance = new randomQueue();
        return instance;
    } 

    void randomQueue::pushRandomNum()
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

    float randomQueue::getRandomNum()
    {
        if (this->numQue.empty())
            this->pushRandomNum();
        float temp = this->numQue.front();
        this->numQue.pop();
        return temp;
    }

    randomQueue::randomQueue() { }

    randomQueue::~randomQueue()
    {
        if (instance != nullptr)
            delete instance;
    }


}