#pragma once
#include "MATH.h"
#include "Ray.h"

class BoundingBox
{
    public:
        static BoundingBox Union(const BoundingBox& b1, const BoundingBox& b2);
        MATH::Vector3f pMax; 
        MATH::Vector3f pMin;

        BoundingBox();
        BoundingBox(const MATH::Vector3f& p1, const MATH::Vector3f& p2);
        BoundingBox(const MATH::Vector3f& p1, const MATH::Vector3f& p2, const MATH::Vector3f& P3);

        MATH::Vector3f  Centroid();
        bool            getIntersection(const Ray& ray);
};

