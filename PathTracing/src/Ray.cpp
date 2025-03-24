#include "Ray.h"
#include "MATH.h"

Ray::Ray(const MATH::Vector3f& o, const MATH::Vector3f& d) : oir(o), dir(d) 
{
    this->dir_inv = d.getInv();
}



MATH::Vector3f Ray::trace(const float& t) const
{
    return this->oir + this->dir * t;
}