#pragma once
#include "engine/core/vector_defination.h"
#include "engine/resource/object.h"

namespace scene
{

    class Camera
    {
        private:
            class ImageView
            {
                public:
                    int             step_u_;
                    int             step_v_;
                    core::Vector3f  starPoint_;
                    core::Vector3f  delta_u_;
                    core::Vector3f  delta_v_;
                
                    ImageView() = default;
            };

        public:
            ImageView       imageView_;

            float           fov_;
            core::Vector3f  eye_point_;
            core::Vector3f  lookAt_;
            core::Vector3f  up_;
            Camera() = default;

            void Show() const;    
    };

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
            Camera                              camera_;

            Scene(int w = 1280, int h = 960, int depth = 1, core::Vector3f bGroundColor = core::Vector3f(0.235294, 0.67451, 0.843137));

            void                        SetCamera(const Camera& camera);
            void                        InitImageView();
            void                        ReferenceLightObject();
            void                        Add(resource::Obj* object);
            void                        Show() const;
            static const unsigned int   GetUniqueID();
    };




}


