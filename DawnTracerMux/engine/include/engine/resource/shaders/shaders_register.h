#pragma once

namespace resource
{
    class ShaderBase;

    template<typename T>
    class AutoRegisterar
    {
        public:
            AutoRegisterar(std::string names, std::shared_ptr<ShaderBase> shader);
    };

    #define CONCAT_IMPL(a, b) a##b
    #define CONCAT(a, b) CONCAT_IMPL(a, b)
    #define REGISTER_SHADER(shaderName, shader) \
        namespace{ \
            struct CONCAT(UniqueTag, __COUNTER__) {}; \
            static AutoRegisterar<struct CONCAT(UniqueTag, __COUNTER__)> CONCAT(registerar_, __COUNTER__)(shaderName, shader); \
        } \

}

#include "shaders_register_inl.h"

