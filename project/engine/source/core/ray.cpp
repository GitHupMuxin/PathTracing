#include "engine/core/ray.h"

namespace core
{

    Ray::Ray(const Vector3f& o, const Vector3f& d) : oir_(o), dir_(d) 
    {
        this->dir_inv_ = d.GetInv();
    }



    Vector3f Ray::Trace(const float& t) const
    {
        return this->oir_ + this->dir_ * t;
    }

}


