#include <algorithm>
#include "engine/core/math/vector_defination.h"
#include "engine/rendering/rendering_utils.h"

namespace rendering
{
    void RenderingUtils::DrawToFile(std::string& path, const int& width, const int& height, std::vector<core::Vector3f>& frameBuffer)
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

    void RenderingUtils::SRGBDrawToFile(std::string& path, const int& width, const int& height, std::vector<core::Vector3f>& frameBuffer)
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

    void RenderingUtils::ExecutionProgress(float progress)
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

    


}


