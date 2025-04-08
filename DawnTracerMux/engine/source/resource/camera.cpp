#include "engine/resource/camera.h"

namespace resource
{
    void Camera::Show() const
    {
        std::cout << "camera eye_point :" << this->eye_point_ << "\n"
                  << "       lookAt :" << this->lookAt_ << "\n"
                  << "       up :" << this->up_ << std::endl;
                  std::cout << std::endl;             
    }
}




