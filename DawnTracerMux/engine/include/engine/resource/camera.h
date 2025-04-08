#pragma once
#include "engine/core/math/vector_defination.h"

namespace resource
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
}




