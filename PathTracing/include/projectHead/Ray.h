#pragma once 
#include "MATH.h"

class Ray
{
    public:
        MATH::Vector3f oir;
        MATH::Vector3f dir;
        MATH::Vector3f dir_inv;

        Ray(const MATH::Vector3f& o, const MATH::Vector3f& d);

        MATH::Vector3f trace(const float& t) const;
};


