#include "engine/core/core_utils.h"

namespace core
{
    Vector3f CoreUtils::ToWorldSpace(const Vector3f& dir, const Vector3f& n)
    {
        // MATH::Vector3f T(0.0f), B(0.0f);
        // if (n.x < 0.99f)
        // {
        //     T = MATH::Vector3f(1.0, 1.0, 0.0);
        //     B = MATH::crossProduction(n, T).normalize();
        //     T = MATH::crossProduction(B , n).normalize();
        // }
        // else 
        // {
        //     T = MATH::Vector3f(0.0f, 1.0f, 0.0f);
        //     B = MATH::crossProduction(n, T).normalize();
        //     T = MATH::crossProduction(B, n).normalize();
        // }

        // return dir.x * T + dir.y * B + dir.z * n;

        Vector3f B, C;
        if (std::fabs(n.x) > std::fabs(n.y)){
            float invLen = 1.0f / std::sqrt(n.x * n.x + n.z * n.z);
            C = Vector3f(n.z * invLen, 0.0f, -n.x *invLen);
        }
        else {
            float invLen = 1.0f / std::sqrt(n.y * n.y + n.z * n.z);
            C = Vector3f(0.0f, n.z * invLen, -n.y *invLen);
        }
        B = CrossProduction(C, n);
        return dir.x * B + dir.y * C + dir.z * n;

    }
}



