#include "shaders_maneger.h"

namespace resource
{
    template<typename T>
    AutoRegisterar<T>::AutoRegisterar(std::string names, std::shared_ptr<ShaderBase> shader)
    {
        ShadersManeger::GetInstance()->RegisterShader(names, std::move(shader));
    }   
}


