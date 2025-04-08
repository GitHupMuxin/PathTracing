#pragma once
#include "engine/core/math/vector_defination.h"
#include "engine/resource/object.h"
#include "engine/resource/camera.h"

namespace scene
{

    class Scene
    {
        private:
            static unsigned int                 MeshID_;
        public:
            int                                 width_;
            int                                 height_;
            int                                 maxDetph_;
            core::Vector3f                      backGroundColor_;
            std::vector<resource::Obj* >        objectList_;
            std::vector<resource::Mesh* >       lightList_;
            resource::Camera                              camera_;

            Scene(int w = 1280, int h = 960, int depth = 1, core::Vector3f bGroundColor = core::Vector3f(0.235294, 0.67451, 0.843137));

            void                        SetCamera(const resource::Camera& camera);
            void                        InitImageView();
            void                        ReferenceLightObject();
            void                        Add(resource::Obj* object);
            void                        Show() const;
            static const unsigned int   GetUniqueID();
    };




}


