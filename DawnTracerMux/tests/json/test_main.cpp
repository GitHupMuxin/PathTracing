#include <iostream>
#include <fstream>
#include "engine/platform/platform_utils.h"
#include "third_party/nlohmann/json/json.hpp" // 需要确保路径正确

using json = nlohmann::json;

int main() {
    try {
        // 1. 读取 JSON 文件
        std::string executionPath = platform::PlatformUtils::GetExecutionPath();
        std::ifstream file(executionPath + "/scene.json");
        if (!file.is_open()) {
            throw std::runtime_error("无法打开文件" + executionPath + "/scene.json");
        }

        // 2. 解析 JSON 数据
        json scene_data = json::parse(file);

        // 3. 访问场景配置
        std::string scene_name = scene_data["scene"]["name"];
        int width = scene_data["scene"]["width"];
        int height = scene_data["scene"]["height"];

        std::cout << "场景名称: " << scene_name << std::endl;
        std::cout << "分辨率: " << width << "x" << height << std::endl;

        // 4. 遍历场景中的对象
        for (const auto& obj : scene_data["scene"]["objects"]) {
            std::string type = obj["type"];
            std::vector<float> position = obj["position"];
            float scale = obj["scale"];

            std::cout << "\n对象类型: " << type << std::endl;
            std::cout << "位置: (" 
                      << position[0] << ", " 
                      << position[1] << ", " 
                      << position[2] << ")" << std::endl;
            std::cout << "缩放: " << scale << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "错误: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}