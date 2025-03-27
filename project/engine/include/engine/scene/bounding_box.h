#pragma once
#include "engine/core/vector_defination.h"
#include "engine/core/ray.h"

namespace scene
{
    class BoundingBox
    {
        public:
            static BoundingBox Union(const BoundingBox& b1, const BoundingBox& b2);
            core::Vector3f pMax_; 
            core::Vector3f pMin_;

            BoundingBox();
            BoundingBox(const core::Vector3f& p1, const core::Vector3f& p2);
            BoundingBox(const core::Vector3f& p1, const core::Vector3f& p2, const core::Vector3f& P3);

            core::Vector3f  Centroid();
            bool            GetIntersection(const core::Ray& ray);
    };    


}