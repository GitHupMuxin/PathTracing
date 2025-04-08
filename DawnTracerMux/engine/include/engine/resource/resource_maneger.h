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
            void LodeJson(const std::string& path);
            void LodeXML(const std::string& path);

            ResourceManeger() = default;
            ResourceManeger(const ResourceManeger&) = delete;
            void operator = (const ResourceManeger& ) = delete;
            std::unordered_map<std::string, std::shared_ptr<Obj>> objectsMap;
            std::unordered_map<std::string, std::shared_ptr<Material>> materialsMap;
            std::unordered_map<std::string, core::Vector3f> porperties;
            Camera camera;

            
            static std::shared_ptr<ResourceManeger> instance_;
        public:
            static std::shared_ptr<ResourceManeger>                 GetInstance();
            void                                                    LodeSceneFile(const std::string& path);
            const std::shared_ptr<Obj>                              FindObject(const std::string& name) const noexcept;
            const std::shared_ptr<Material>                         FindMaterial(const std::string& name) const noexcept;
            const std::vector<std::shared_ptr<Obj>>                 FindObject(const std::vector<std::string>& nameLists) const noexcept;
            const std::vector<std::shared_ptr<Material>>            FindMaterial(const std::vector<std::string>& nameLists) const noexcept;
            const std::vector<std::shared_ptr<Obj>>                 GetAllObjects() const noexcept;
            const std::vector<std::shared_ptr<Material>>            GetAllMaterial() const noexcept;

            const int                                               GetResourceObjectSize() const noexcept;
            const int                                               GetResourceMaterialSize() const noexcept;

            template <typename T>
            T                                                       GetPorperties(const std::string& porpertyName) const noexcept
            {
                T result; 
                if (this->porperties.count(porpertyName))
                    result = static_cast<T>(this->porperties.find(porpertyName)); 
                return result;
            }
    };
}




