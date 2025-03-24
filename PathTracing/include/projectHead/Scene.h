#pragma once
#include "MATH.h"
#include "Object.h"

class Camera
{
    private:
        class ImageView
        {
            public:
                int             step_u;
                int             step_v;
                MATH::Vector3f  starPoint;
                MATH::Vector3f  delta_u;
                MATH::Vector3f  delta_v;
                
                ImageView() = default;
        };

    public:
        ImageView       imageView;

        float           fov;
        MATH::Vector3f  eye_point;
        MATH::Vector3f  lookAt;
        MATH::Vector3f  up;
        Camera() = default;

        void Show() const;    
};

class Scene
{
    private:
        static unsigned int             MeshID;
    public:
        int                             width, height;
        int                             maxDetph;
        MATH::Vector3f                  backGroundColor;
        std::vector<OBJECT::OBJ* >      objectList;
        std::vector<OBJECT::Mesh* >     lightList;
        Camera camera;

        Scene(int w = 1280, int h = 960, int depth = 1, MATH::Vector3f bGroundColor = MATH::Vector3f(0.235294, 0.67451, 0.843137));

        void                        SetCamera(const Camera& camera);
        void                        InitImageView();
        void                        referenceLightObject();
        void                        Add(OBJECT::OBJ* object);
        void                        Show() const;
        static const unsigned int   getUniqueID();
};







