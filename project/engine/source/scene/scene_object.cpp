#include "engine/scene/scene_object.h"
#include "engine/core/random_queue.h"

namespace scene
{
    Triangle::Triangle(const resource::Vertex& vv0, const resource::Vertex& vv1, const resource::Vertex& vv2, resource::Material* mm) : v0_(vv0.GetPosition()), v1_(vv1.GetPosition()), v2_(vv2.GetPosition()),
                                                                t0_(vv0.GetTexCoord()), t1_(vv1.GetTexCoord()), t2_(vv2.GetTexCoord()),
                                                                m_(mm)
    {
        this->e1_ = (v1_ - v0_);
        this->e2_ = (v2_ - v0_);
        this->area_ = core::CrossProduction(this->e1_, this->e2_).Lenght() * 0.5;
        this->e1_ = this->e1_.Normalize();
        this->e2_ = this->e2_.Normalize();
        this->normal_ = core::CrossProduction(this->e1_, this->e2_).Normalize();
        this->boundingBox_ = BoundingBox(v0_, v1_, v2_);

    }

    BoundingBox& Triangle::GetBoundingBox()
    {
        return boundingBox_;
    }

    Intersection Triangle::GetIntersetion(const core::Ray& ray)
    {
        Intersection inter;

        if (core::DotProduction(ray.dir_, this->normal_) > 0)
            return inter;

        core::Vector3f E1 = this->v1_ - this->v0_;
        core::Vector3f E2 = this->v2_ - this->v0_;
        core::Vector3f S =  ray.oir_ - this->v0_;
        core::Vector3f S1 = core::CrossProduction(ray.dir_, E2);
        core::Vector3f S2 = core::CrossProduction(S, E1);

        float div = core::DotProduction(S1, E1);
        if (div == 0.0f)
        {
            inter.happened = false;
            return inter;    
        }

        div = 1.0 / div;

        float t = core::DotProduction(S2, E2) * div;
        float u = core::DotProduction(S1, S) * div;
        float v = core::DotProduction(S2, ray.dir_) * div;

        inter.happened = (u >= 0.0f && u <= 1.0f) && (v >= 0.0f && v <= 1.0f) && ((1.0f - u - v) >= 0.0f && (1.0f - u - v <= 1.0f));
        if (!inter.happened)
            return inter;

        inter.distance = t;
        inter.coords = ray.Trace(t);
        inter.tcoords = this->t0_ * u + this->t1_ * v + this->t2_ * (1 - u - v);
        inter.normal = this->normal_;
        inter.obj = this;
        inter.m = this->m_;
        inter.id = this->id_;

        return inter;

    } 



    float Triangle::GetArea()
    {
        return this->area_;    
    }

    const unsigned int Triangle::GetUniqueID()
    {
        return this->id_;
    }

    void Triangle::SetUniqueID(unsigned int ID)
    {
        this->id_ = ID;
    }

    core::Vector3f Triangle::GetInner() const
    {
        float a = (this->v1_ - this->v2_).Lenght();
        float b = (this->v2_ - this->v0_).Lenght();
        float c = (this->v0_ - this->v1_).Lenght();
        float divSum = 1.0f / (a + b + c);

        core::Vector3f O = a * this->v0_ + b * this->v1_ + c * this->v2_ * divSum;    

        return O;
    }

    void Triangle::Sample(Intersection& intersection, float& pdf)
    {
        float r1 = core::RandomQueue::GetInstance()->GetRandomNum();
        float r2 = core::RandomQueue::GetInstance()->GetRandomNum();
        
        r1 = std::sqrt(r1);

        core::Vector3f P = this->v0_ * (1.0f - r1) + this->v1_ * (r1 * (1.0f - r2)) + this->v2_ * r1 * r2;
        intersection.coords = P;
        intersection.normal = this->normal_;
        pdf = 1.0f / this->area_;
    }


    Intersection::Intersection()
    {
        this->happened = false;
        this->coords = core::Vector3f(0.0f);
        this->tcoords = core::Vector2f(0.0f);
        this->normal = core::Vector3f(0.0f);
        this->distance = std::numeric_limits<double>::max();
        this->obj = nullptr;
        this->m = nullptr;
        this->id = 0;
    }


}

