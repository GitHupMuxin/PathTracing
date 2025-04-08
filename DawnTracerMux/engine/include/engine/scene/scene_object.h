#pragma once
#include "engine/core/math/vector_defination.h"
#include "engine/resource/object.h"

#include "bounding_box.h"

namespace scene
{

    class Object;

    struct Intersection
    {
        bool                happened;
        unsigned int        id;
        core::Vector3f      coords;
        core::Vector2f      tcoords;
        core::Vector3f      normal;
        double              distance;
        Object*     obj;
        resource::Material*   m;

        Intersection();

    };

   class Object
    {
        public:
            virtual BoundingBox&        GetBoundingBox() = 0;
            virtual Intersection        GetIntersetion(const core::Ray& ray) = 0;           
            virtual float               GetArea() = 0;
            virtual const unsigned int  GetUniqueID() = 0; 
    };

    class Triangle : public Object
    {
        private:
            unsigned int    id_;
        public:
            core::Vector3f  v0_, v1_, v2_;
            core::Vector2f  t0_, t1_, t2_;
            core::Vector3f  e1_, e2_;
            core::Vector3f  normal_;
            BoundingBox     boundingBox_;
            resource::Material*       m_;
            float           area_;

            Triangle(const resource::Vertex& vv0, const resource::Vertex& vv1, const resource::Vertex& vv2, resource::Material* mm);
            BoundingBox&        GetBoundingBox() override;
            Intersection        GetIntersetion(const core::Ray& ray) override;
            float               GetArea() override;
            const unsigned int  GetUniqueID() override;
            void                SetUniqueID(unsigned int ID);
            core::Vector3f      GetInner() const;
            void                Sample(Intersection& intersection, float& pdf);
    };

}

