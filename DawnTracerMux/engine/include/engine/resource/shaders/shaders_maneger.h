#pragma once
#include <memory>
#include "shader.h"


namespace resource
{
    class ShadersManeger
    {
        public:
            static ShadersManeger*      GetInstance();
            void                        RegisterShader(const std::string& str, std::shared_ptr<ShaderBase> shader);
    };

    

}



