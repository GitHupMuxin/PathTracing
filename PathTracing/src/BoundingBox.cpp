#include "BoundingBox.h"
#include "MATH.h"
#include <limits>

BoundingBox::BoundingBox()
{
    float minNum = std::numeric_limits<float>::lowest();
    float maxNum = std::numeric_limits<float>::max();

    pMax = MATH::Vector3f(minNum);
    pMin = MATH::Vector3f(maxNum);
}

BoundingBox::BoundingBox(const MATH::Vector3f& p1, const MATH::Vector3f& p2)
{
    pMax = MATH::Vector3f(std::max(p1.x, p2.x), std::max(p1.y, p2.y), std::max(p1.z, p2.z));
    pMin = MATH::Vector3f(std::min(p1.x, p2.x), std::min(p1.y, p2.y), std::min(p1.z, p2.z));
}

BoundingBox::BoundingBox(const MATH::Vector3f& p1, const MATH::Vector3f& p2, const MATH::Vector3f& p3)
{
    pMax = MATH::Vector3f(std::max(p1.x, std::max(p2.x, p3.x)),
                          std::max(p1.y, std::max(p2.y, p3.y)),
                          std::max(p1.z, std::max(p2.z, p3.z))
    );
    
    pMin = MATH::Vector3f(std::min(p1.x, std::min(p2.x, p3.x)),
                          std::min(p1.y, std::min(p2.y, p3.y)),
                          std::min(p1.z, std::min(p2.z, p3.z))
    );
}

BoundingBox BoundingBox::Union(const BoundingBox &b1, const BoundingBox &b2)
{
    BoundingBox result;
    result.pMax = MATH::Vector3f(std::max(b1.pMax.x, b2.pMax.x), std::max(b1.pMax.y, b2.pMax.y), std::max(b1.pMax.z, b2.pMax.z));
    result.pMin = MATH::Vector3f(std::min(b1.pMin.x, b2.pMin.x), std::min(b1.pMin.y, b2.pMin.y), std::min(b1.pMin.z, b2.pMin.z));
    return result;
}

MATH::Vector3f BoundingBox::Centroid()
{
    return (pMax + pMin) * 0.5; 
}

bool BoundingBox::getIntersection(const Ray& ray)
{
    MATH::Vector3f t1 = (this->pMax - ray.oir) * ray.dir_inv;
    MATH::Vector3f t2 = (this->pMin - ray.oir) * ray.dir_inv;

    MATH::Vector3f tMax = MATH::process2Vector(t1, t2, MATH::Vector3_Tool<float>::MAX());    
    MATH::Vector3f tMin = MATH::process2Vector(t1, t2, MATH::Vector3_Tool<float>::MIN());

    float tmin = std::fmax(std::fmax(tMin.x, tMin.y), tMin.z);
    float tmax = std::fmin(std::fmin(tMax.x, tMax.y), tMax.z);

    bool result = tmin <= tmax && tmax > 0;
    return result;
}


