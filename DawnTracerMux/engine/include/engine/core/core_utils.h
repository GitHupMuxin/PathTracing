#pragma once
#include "math/vector_defination.h"

namespace core
{
    class CoreUtils
    {
        public:
            static Vector3f ToWorldSpace(const Vector3f& dir, const Vector3f& n);
    };


}
