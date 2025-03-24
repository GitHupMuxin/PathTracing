#include "Scene.h"
#include "MATH.h"
#include <cmath>
#include <iostream>
#include <math.h>

unsigned int Scene::MeshID = 1;

const unsigned int Scene::getUniqueID()
{
    return Scene::MeshID++;
}

void Camera::Show() const
{
    std::cout << "camera eye_point :" << this->eye_point << "\n"
              << "       lookAt :" << this->lookAt << "\n"
              << "       up :" << this->up << std::endl;
              std::cout << std::endl;             
}

Scene::Scene(int w, int h, int depth, MATH::Vector3f bGroundColor) : width(w), height(h), maxDetph(depth), backGroundColor(bGroundColor) { }

void Scene::SetCamera(const Camera& camera)
{
    this->camera.eye_point = camera.eye_point;
    this->camera.lookAt = camera.lookAt;
    this->camera.up = camera.up;
    this->camera.fov = camera.fov;
    this->InitImageView();
} 

void Scene::InitImageView()
{
    float theta = this->camera.fov * 0.5 * M_PI / 180.0f;
    float tanTheta = tan(theta);
    float imageAspectRatio = (double)this->width / (double)this->height;
    
    float imageView_h = 1.0f * tanTheta * 2;
    float imageView_w = imageView_h * imageAspectRatio;

    MATH::Vector3f w = (this->camera.lookAt - this->camera.eye_point).normalize();
    this->camera.up = w.y == 1.0 ? MATH::Vector3f(0.0, 0.0, -1.0) : this->camera.up;

    MATH::Vector3f u = MATH::crossProduction(this->camera.up, w).normalize();
    MATH::Vector3f v = MATH::crossProduction(w, u).normalize();

    this->camera.imageView.delta_u = u * imageView_w / (float)this->width * -1.0f;
    this->camera.imageView.delta_v = v * imageView_h / (float)this->height * -1.0f;
    this->camera.imageView.step_u = this->width;
    this->camera.imageView.step_v = this->height;

    this->camera.imageView.starPoint = this->camera.eye_point + w
                                       - this->camera.imageView.delta_u * (float)this->camera.imageView.step_u * 0.5f
                                       - this->camera.imageView.delta_v * (float)this->camera.imageView.step_v * 0.5f
                                       + 0.5 * this->camera.imageView.delta_u + 0.5 * this->camera.imageView.delta_v;


}

void Scene::referenceLightObject()
{
    for (auto& OBJ : this->objectList)
    {
        for (auto& mesh : OBJ->meshes)
        {
            if (mesh.material->self_illuminating)
                this->lightList.emplace_back(&mesh);
        }
    }       
}

void Scene::Add(OBJECT::OBJ* object)
{
    this->objectList.emplace_back(object);
    for (auto& mesh : object->meshes)
    {
        if (mesh.material->self_illuminating)
            this->lightList.emplace_back(&mesh);
    }
}

void Scene::Show() const
{
    std::cout << "Scene:\n"
              <<"width :" << this->width << "   hight :" << this->height << "\n"
              << "    maxDetph :" << this->maxDetph << "\n"
              << "backGroundColor :" << backGroundColor << "\n" 
              << "Camera :\n";
    this->camera.Show();
    std::cout << "Objects :\n";
    for (int i = 0; i < objectList.size(); i++)
    {
        std::cout << i + 1 << ". ";
        objectList[i]->Show();
    }
}



