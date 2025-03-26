#include <string>
#include "engine/core/vector_defination.h"

namespace rendering
{
    class RenderingUtils
    {
        public:
            static void DrawToFile(std::string& path, const int& width, const int& height, std::vector<core::Vector3f>& frameBuffer);

            static void SRGBDrawToFile(std::string& path, const int& width, const int& height, std::vector<core::Vector3f>& frameBuffer);
    
            static void ExecutionProgress(float progress);

    };
}



