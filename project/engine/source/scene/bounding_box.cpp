#include <limits>
#include "engine/scene/bounding_box.h"

BoundingBox::BoundingBox()
{
    float minNum = std::numeric_limits<float>::lowest();
    float maxNum = std::numeric_limits<float>::max();

    pMax_ = core::Vector3f(minNum);
    pMin_ = core::Vector3f(maxNum);
}

BoundingBox::BoundingBox(const core::Vector3f& p1, const core::Vector3f& p2)
{
    pMax_ = core::Vector3f(std::max(p1.x, p2.x), std::max(p1.y, p2.y), std::max(p1.z, p2.z));
    pMin_ = core::Vector3f(std::min(p1.x, p2.x), std::min(p1.y, p2.y), std::min(p1.z, p2.z));
}

BoundingBox::BoundingBox(const core::Vector3f& p1, const core::Vector3f& p2, const core::Vector3f& p3)
{
    pMax_ = core::Vector3f(std::max(p1.x, std::max(p2.x, p3.x)),
                          std::max(p1.y, std::max(p2.y, p3.y)),
                          std::max(p1.z, std::max(p2.z, p3.z))
    );
    
    pMin_ = core::Vector3f(std::min(p1.x, std::min(p2.x, p3.x)),
                          std::min(p1.y, std::min(p2.y, p3.y)),
                          std::min(p1.z, std::min(p2.z, p3.z))
    );
}

BoundingBox BoundingBox::Union(const BoundingBox &b1, const BoundingBox &b2)
{
    BoundingBox result;
    result.pMax_ = core::Vector3f(std::max(b1.pMax_.x, b2.pMax_.x), std::max(b1.pMax_.y, b2.pMax_.y), std::max(b1.pMax_.z, b2.pMax_.z));
    result.pMin_ = core::Vector3f(std::min(b1.pMin_.x, b2.pMin_.x), std::min(b1.pMin_.y, b2.pMin_.y), std::min(b1.pMin_.z, b2.pMin_.z));
    return result;
}

core::Vector3f BoundingBox::Centroid()
{
    return (pMax_ + pMin_) * 0.5; 
}

bool BoundingBox::GetIntersection(const core::Ray& ray)
{
    core::Vector3f t1 = (this->pMax_ - ray.oir_) * ray.dir_inv_;
    core::Vector3f t2 = (this->pMin_ - ray.oir_) * ray.dir_inv_;

    core::Vector3f tMax = core::Process2Vector(t1, t2, core::Vector3_Tool<float>::MAX());    
    core::Vector3f tMin = core::Process2Vector(t1, t2, core::Vector3_Tool<float>::MIN());

    float tmin = std::fmax(std::fmax(tMin.x, tMin.y), tMin.z);
    float tmax = std::fmin(std::fmin(tMax.x, tMax.y), tMax.z);

    bool result = tmin <= tmax && tmax > 0;
    return result;
}


