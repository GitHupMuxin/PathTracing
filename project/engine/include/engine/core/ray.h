#pragma once 
#include "vector_defination.h"

namespace core
{

    class Ray
    {
        public:
            core::Vector3f oir_;
            core::Vector3f dir_;
            core::Vector3f dir_inv_;

            Ray(const core::Vector3f& o, const core::Vector3f& d);

            core::Vector3f Trace(const float& t) const;
    };  

}



