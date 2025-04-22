#include "engine/resource/resource_maneger.h"
#include "engine/resource/shaders/shaders_maneger.h"

namespace resource
{

    ShadersManeger* ShadersManeger::GetInstance()
    {
        static std::unique_ptr<ShadersManeger> instance;
        return instance.get();
    }

    void ShadersManeger::RegisterShader(const std::string& str, std::shared_ptr<ShaderBase> shader)
    {
        ResourceManeger::GetInstance()->AddShader(str, shader);  
    }



}


