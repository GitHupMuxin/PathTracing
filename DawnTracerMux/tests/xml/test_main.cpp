#include <iostream>
#include <string>
#include "engine/platform/platform_utils.h"
#include "third_party/pugixml/include/pugixml.hpp" // 确保包含路径正确

int main() {
    pugi::xml_document doc;
    
    // 1. 加载 XML 文件
    const std::string EXECUTION_PATH = platform::PlatformUtils::GetExecutionPath();
    pugi::xml_parse_result result = doc.load_file((EXECUTION_PATH + "/test_xml.xml").c_str());
    if (!result) {
        std::cerr << "XML 解析错误: " << result.description() << std::endl;
        return EXIT_FAILURE;
    }

    // 2. 获取根节点
    pugi::xml_node scene = doc.child("scene");
    std::string scene_name = scene.attribute("name").as_string();
    int width = scene.attribute("width").as_int();
    int height = scene.attribute("height").as_int();

    std::cout << "场景名称: " << scene_name << std::endl;
    std::cout << "分辨率: " << width << "x" << height << std::endl;

    // 3. 遍历对象节点
    pugi::xml_node objects = scene.child("objects");
    for (pugi::xml_node obj : objects.children("object")) {
        std::string type = obj.attribute("type").as_string();
        pugi::xml_node pos = obj.child("position");
        float x = pos.attribute("x").as_float();
        float y = pos.attribute("y").as_float();
        float z = pos.attribute("z").as_float();
        float scale = obj.child("scale").text().as_float();

        std::cout << "\n对象类型: " << type << std::endl;
        std::cout << "位置: (" << x << ", " << y << ", " << z << ")" << std::endl;
        std::cout << "缩放: " << scale << std::endl;
    }

    return EXIT_SUCCESS;
}