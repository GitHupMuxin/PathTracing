#pragma once
#include <memory>
#include "engine/core/math/vector_defination.h"
#include "shaders_register.h"

namespace resource
{
    class Material;

    class ShaderBase
    {
        public:
            virtual core::Vector3f      Sample(const core::Vector3f& wi, const core::Vector3f& n);
            virtual float               Pdf(const core::Vector3f& wi, const core::Vector3f& wo, const core::Vector3f& n);
            virtual core::Vector3f      GetBRDF(const core::Vector3f& wi, const core::Vector3f& wo, const core::Vector3f& n, const Material* m); 
    };

    REGISTER_SHADER("default", std::move(std::make_shared<ShaderBase>()));

    class DiffuseShader : public ShaderBase
    {
        public:
            core::Vector3f              Sample(const core::Vector3f& wi, const core::Vector3f& n);
            float                       Pdf(const core::Vector3f& wi, const core::Vector3f& wo, const core::Vector3f& n);
            core::Vector3f              GetBRDF(const core::Vector3f& wi, const core::Vector3f& wo, const core::Vector3f& n, const Material* m); 
    };

    REGISTER_SHADER("diffuse", std::make_shared<DiffuseShader>());

    

}



