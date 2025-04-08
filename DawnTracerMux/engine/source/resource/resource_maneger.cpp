#include "engine/platform/platform_utils.h"
#include "engine/resource/resource_maneger.h"
#include "engine/resource/resource_utils.h"
#include "third_party/nlohmann/json/json.hpp"
#include "third_party/pugixml/include/pugixml.hpp"

namespace resource
{
    std::shared_ptr<ResourceManeger> ResourceManeger::instance_(new ResourceManeger);

    void ResourceManeger::LodeJson(const std::string& path)
    {
        try {
            // 1. 读取 JSON 文件
            std::ifstream file(path);
            if (!file.is_open()) {
                throw std::runtime_error("无法打开文件" + path);
            }

            std::string subPath = path.substr(0, path.find_last_of('/'));

            // 2. 解析 JSON 数据
            nlohmann::json scene_data = nlohmann::json::parse(file);

            // 3. 访问场景配置
            std::string scene_name = scene_data["scene"]["name"];
            int width = scene_data["scene"]["width"];
            int height = scene_data["scene"]["height"];
            float spp = scene_data["spp"];

            std::cout << "scene name: " << scene_name << std::endl;
            std::cout << "width:" << width << "    height:" << height << std::endl;

            const auto& CameraJson = scene_data["scene"]["camera"].front();            
            std::cout << CameraJson.contains("eye_point") << "\n";
            this->camera.eye_point_ = core::Vector3f(CameraJson["eye_point"].get<std::vector<float>>());
            this->camera.lookAt_ = core::Vector3f(CameraJson["lookAt"].get<std::vector<float>>());
            this->camera.up_ = core::Vector3f(CameraJson["up"].get<std::vector<float>>());
            this->camera.fov_ = CameraJson["fov"].get<float>();

            for (const auto& material : scene_data["scene"]["material"])
            {
                std::string materialName = material["name"];

                bool self_illuminating = material.contains("self_illuminating") ? true : false;

                core::Vector3f illuminate = material.contains("illuminate") ? core::Vector3f(material["illuminate"].get<std::vector<float>>()) : core::Vector3f(0.0);




                std::shared_ptr<Material> m = std::make_shared<Material>(materialName,               
                                    core::Vector3f(material["ka"].get<std::vector<float>>()),  
                                    core::Vector3f(material["kd"].get<std::vector<float>>()),
                                    core::Vector3f(material["ks"].get<std::vector<float>>()),
                                    core::Vector3f(material["ke"].get<std::vector<float>>()),
                                    float(material["ns"].get<float>()),           
                                    float(material["ni"].get<float>()),
                                    float(material["d"].get<float>()),
                                    float(material["i"].get<float>()),
                                    self_illuminating,
                                    illuminate
                                    );

                this->materialsMap.insert(std::make_pair(materialName, std::move(m)));
            }

            for (const auto& object : scene_data["scene"]["objects"])
            {
                std::string objectName = object["name"];
                std::string PATH = object["path"];
                std::shared_ptr<Obj> obj(new Obj);
                obj->ObjFileLoder(subPath + "/" + PATH);
                if (object.contains("material"))
                    obj->ResetMaterial(this->materialsMap[object["material"].get<std::string>()].get());
                this->objectsMap.insert(std::make_pair(objectName, std::move(obj)));
            }

            
        } 
        catch (const std::exception& e) 
        {
            std::cerr << "错误: " << e.what() << std::endl;
            assert(false);
        }
    }

    void ResourceManeger::LodeXML(const std::string& path)
    {
        //pugi::xml_document doc;
    
        //// 1. 加载 XML 文件
        //const std::string EXECUTION_PATH = platform::PlatformUtils::GetExecutionPath();
        //pugi::xml_parse_result result = doc.load_file((EXECUTION_PATH + "/test_xml.xml").c_str());
        //if (!result) {
        //    std::cerr << "XML 解析错误: " << result.description() << std::endl;
        //}

        //// 2. 获取根节点
        //pugi::xml_node scene = doc.child("scene");
        //std::string scene_name = scene.attribute("name").as_string();
        //int width = scene.attribute("width").as_int();
        //int height = scene.attribute("height").as_int();

        //std::cout << "场景名称: " << scene_name << std::endl;
        //std::cout << "分辨率: " << width << "x" << height << std::endl;

        //// 3. 遍历对象节点
        //pugi::xml_node objects = scene.child("objects");
        //for (pugi::xml_node obj : objects.children("object")) {
        //std::string type = obj.attribute("type").as_string();
        //pugi::xml_node pos = obj.child("position");
        //float x = pos.attribute("x").as_float();
        //float y = pos.attribute("y").as_float();
        //float z = pos.attribute("z").as_float();
        //float scale = obj.child("scale").text().as_float();

        //std::cout << "\n对象类型: " << type << std::endl;
        //std::cout << "位置: (" << x << ", " << y << ", " << z << ")" << std::endl;
        //std::cout << "缩放: " << scale << std::endl;
        //}


    }


    std::shared_ptr<ResourceManeger> ResourceManeger::GetInstance()
    {
        return ResourceManeger::instance_;
    }

    void ResourceManeger::LodeSceneFile(const std::string& path)
    {
        std::cout << "--lode scene file--\n" << "scene file path:" << path << "\n";
        size_t n = path.length();
        if (path.substr(n - 5, n) == ".json")
            this->LodeJson(path);
        else if (path.substr(n - 4, n) == ".xml")
            this->LodeJson(path);
        else 
            throw std::runtime_error("wrong path Input.");
    }

    const std::shared_ptr<Obj> ResourceManeger::FindObject(const std::string& name) const noexcept
    {
        if (this->objectsMap.count(name))
            return this->objectsMap.find(name)->second;
        return nullptr;
    }

    const std::shared_ptr<Material> ResourceManeger::FindMaterial(const std::string& name) const noexcept
    {
        if (this->materialsMap.count(name))
            return this->materialsMap.find(name)->second;
        return nullptr;
    }

    const std::vector<std::shared_ptr<Obj>> ResourceManeger::FindObject(const std::vector<std::string>& nameLists) const noexcept
    {
        std::vector<std::shared_ptr<Obj>> result = { };
        for (const std::string& name : nameLists)
        {
            std::shared_ptr<Obj> Object(this->FindObject(name));
            if (Object.get())
                result.emplace_back(Object);
        }
        return result;
    }

    const std::vector<std::shared_ptr<Material>> ResourceManeger::FindMaterial(const std::vector<std::string>& nameLists) const noexcept
    {
        std::vector<std::shared_ptr<Material>> result = { };
        for (const std::string& name : nameLists)
        {
            std::shared_ptr<Material> material(this->FindMaterial(name));
            if (material.get())
                result.emplace_back(material);
        }
        return result;
    }

    const std::vector<std::shared_ptr<Obj>> ResourceManeger::GetAllObjects() const noexcept
    {
        std::vector<std::shared_ptr<Obj>> result = { };
        for (auto& it : this->objectsMap)
            if (it.second)
                result.emplace_back(it.second);
        return result;
    }

    const std::vector<std::shared_ptr<Material>> ResourceManeger::GetAllMaterial() const noexcept
    {
        std::vector<std::shared_ptr<Material>> result = { };
        for (auto& it : this->materialsMap)
            if (it.second)
                result.emplace_back(it.second);   
        return result;
    }

    const int ResourceManeger::GetResourceObjectSize() const noexcept
    {
        return this->objectsMap.size();
    }
    
    const int ResourceManeger::GetResourceMaterialSize() const noexcept
    {
        return this->materialsMap.size();   
    }


}

