#include <cmath>
#include <iostream>
#include <math.h>
#include "engine/scene/scene.h"
#include "engine/core/math/vector_defination.h"

#ifndef M_PI
    # define M_PI		3.14159265358979323846	/* pi */
#endif

namespace scene
{
    unsigned int Scene::MeshID_ = 1;

    const unsigned int Scene::GetUniqueID()
    {
        return Scene::MeshID_++;
    }    

    Scene::Scene(int w, int h, int depth, core::Vector3f bGroundColor) : width_(w), height_(h), maxDetph_(depth), backGroundColor_(bGroundColor) { }

    void Scene::SetCamera(const resource::Camera& camera)
    {
        this->camera_.eye_point_ = camera.eye_point_;
        this->camera_.lookAt_ = camera.lookAt_;
        this->camera_.up_ = camera.up_;
        this->camera_.fov_ = camera.fov_;
        this->InitImageView();
    } 

    void Scene::InitImageView()
    {
        float theta = this->camera_.fov_ * 0.5 * M_PI / 180.0f;
        float tanTheta = tan(theta);
        float imageAspectRatio = (double)this->width_ / (double)this->height_;
    
        float imageView_h = 1.0f * tanTheta * 2;
        float imageView_w = imageView_h * imageAspectRatio;

        core::Vector3f w = (this->camera_.lookAt_ - this->camera_.eye_point_).Normalize();
        this->camera_.up_ = w.y == 1.0 ? core::Vector3f(0.0, 0.0, -1.0) : this->camera_.up_;

        core::Vector3f u = core::CrossProduction(this->camera_.up_, w).Normalize();
        core::Vector3f v = core::CrossProduction(w, u).Normalize();

        this->camera_.imageView_.delta_u_ = u * imageView_w / (float)this->width_ * -1.0f;
        this->camera_.imageView_.delta_v_ = v * imageView_h / (float)this->height_ * -1.0f;
        this->camera_.imageView_.step_u_ = this->width_;
        this->camera_.imageView_.step_v_ = this->height_;

        this->camera_.imageView_.starPoint_ = this->camera_.eye_point_ + w
                                           - this->camera_.imageView_.delta_u_ * (float)this->camera_.imageView_.step_u_ * 0.5f
                                           - this->camera_.imageView_.delta_v_ * (float)this->camera_.imageView_.step_v_ * 0.5f
                                           + 0.5 * this->camera_.imageView_.delta_u_ + 0.5 * this->camera_.imageView_.delta_v_;


    }

    void Scene::ReferenceLightObject()
    {
        for (auto& OBJ : this->objectList_)
        {
            for (auto& mesh : OBJ->meshes_)
            {
                if (mesh.material_->self_illuminating_)
                    this->lightList_.emplace_back(&mesh);
            }
        }       
    }

    void Scene::Add(resource::Obj* object)
    {
        this->objectList_.emplace_back(object);
        for (auto& mesh : object->meshes_)
        {
            if (mesh.material_->self_illuminating_)
                this->lightList_.emplace_back(&mesh);
        }
    }

    void Scene::Show() const
    {
        std::cout << "Scene:\n"
                  <<"width :" << this->width_ << "   hight :" << this->height_ << "\n"
                  << "    maxDetph :" << this->maxDetph_ << "\n"
                  << "backGroundColor :" << backGroundColor_ << "\n" 
                  << "Camera :\n";
        this->camera_.Show();
        std::cout << "Objects :\n";
        for (int i = 0; i < objectList_.size(); i++)
        {
            std::cout << i + 1 << ". ";
            objectList_[i]->Show();
        }
    }



}

