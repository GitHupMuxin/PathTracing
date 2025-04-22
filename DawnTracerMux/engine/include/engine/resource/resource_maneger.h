#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include <vector>
#include "object.h"
#include "camera.h"

namespace resource
{
    class ResourceManeger
    {
        private:
            int                                                             spp_;
            int                                                             width_;
            int                                                             height_;
            Camera                                                          camera_;
            std::unordered_map<std::string, std::shared_ptr<Obj>>           objectsMap_;
            std::unordered_map<std::string, std::shared_ptr<Material>>      materialsMap_;
            std::unordered_map<std::string, std::shared_ptr<ShaderBase>>    shadersMap_;
            std::unordered_map<std::string, core::Vector3f>                 porpertiesMap_;

            ResourceManeger() = default;
            ResourceManeger(const ResourceManeger&) = delete;
            void                                                            operator = (const ResourceManeger& ) = delete;

            void                                                            LodeJson(const std::string& path);
            void                                                            LodeXML(const std::string& path);

        public:
            static ResourceManeger*                                         GetInstance();
            void                                                            LodeSceneFile(const std::string& path);
            void                                                            AddShader(const std::string& str, std::shared_ptr<ShaderBase> shader);
            const std::shared_ptr<Obj>                                      FindObject(const std::string& name) const noexcept;
            const std::shared_ptr<Material>                                 FindMaterial(const std::string& name) const noexcept;
            const std::shared_ptr<ShaderBase>                               FindShader(const std::string& name) const noexcept;
            const std::vector<std::shared_ptr<Obj>>                         FindObject(const std::vector<std::string>& nameLists) const noexcept;
            const std::vector<std::shared_ptr<Material>>                    FindMaterial(const std::vector<std::string>& nameLists) const noexcept;
            const std::vector<std::shared_ptr<ShaderBase>>                  FindShader(const std::vector<std::string>& nameLists) const noexcept;
            const std::vector<std::shared_ptr<Obj>>                         GetAllObjects() const noexcept;
            const std::vector<std::shared_ptr<Material>>                    GetAllMaterial() const noexcept;
            const Camera                                                    GetCamera() const noexcept;
            const int                                                       GetSpp() const noexcept;
            const int                                                       GetScreenWidth() const noexcept;
            const int                                                       GetScreenHeight() const noexcept;
            const int                                                       GetResourceObjectSize() const noexcept;
            const int                                                       GetResourceMaterialSize() const noexcept;

            template <typename T>
            T                                                               GetPorperties(const std::string& porpertyName) const noexcept
            {
                T result; 
                if (this->porpertiesMap_.count(porpertyName))
                    result = static_cast<T>(this->porpertiesMap_.find(porpertyName)); 
                return result;
            }
    };
}




