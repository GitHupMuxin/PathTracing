#include "engine/core/math/random_queue.h"
#include "engine/core/core_utils.h"
#include "engine/resource/shaders/shader.h"
#include "engine/resource/object.h"

#ifndef M_PI
# define M_PI		3.14159265358979323846	/* pi */
#endif // !M_PI


namespace resource
{

    core::Vector3f ShaderBase::Sample(const core::Vector3f& wi, const core::Vector3f& n)
    {
        core::Vector3f wo(0.0f);
        float r1 = core::ThreadSafeRandomQueue::GetInstance()->GetRandomNum();
        float r2 = core::ThreadSafeRandomQueue::GetInstance()->GetRandomNum();

        float phi = 2.0f * M_PI * r1;
        float cosTheta = std::fabs(1.0f - 2.0f * r2);
        float sinTheta = std::sqrt(1.0f- (cosTheta * cosTheta));

        wo.x = sinTheta * std::cos(phi);
        wo.y = sinTheta * std::sin(phi);
        wo.z = cosTheta;

        wo = core::CoreUtils::ToWorldSpace(wo, n);

        return wo;    
    }

    float ShaderBase::Pdf(const core::Vector3f& wi, const core::Vector3f& wo, const core::Vector3f& n)
    {
        float result = 0.0f;
        result = 1.0 / M_PI * 0.5;
        return result;
    }

    core::Vector3f ShaderBase::GetBRDF(const core::Vector3f& wi, const core::Vector3f& wo, const core::Vector3f& n, const Material* m)
    {
        core::Vector3f result(0.0f);
        result = core::Vector3f(m->Kd_ / M_PI);
        return result;
    }

    core::Vector3f DiffuseShader::Sample(const core::Vector3f& wi, const core::Vector3f& n)
    {
        core::Vector3f wo(0.0f);
        float r1 = core::ThreadSafeRandomQueue::GetInstance()->GetRandomNum();
        float r2 = core::ThreadSafeRandomQueue::GetInstance()->GetRandomNum();

        float phi = 2.0f * M_PI * r1;
        float cosTheta = std::fabs(1.0f - 2.0f * r2);
        float sinTheta = std::sqrt(1.0f- (cosTheta * cosTheta));

        wo.x = sinTheta * std::cos(phi);
        wo.y = sinTheta * std::sin(phi);
        wo.z = cosTheta;

        wo = core::CoreUtils::ToWorldSpace(wo, n);

        return wo;    
    }

    float DiffuseShader::Pdf(const core::Vector3f& wi, const core::Vector3f& wo, const core::Vector3f& n)
    {
        float result = 0.0f;
        result = 1.0 / M_PI * 0.5;
        return result;
    }

    core::Vector3f DiffuseShader::GetBRDF(const core::Vector3f& wi, const core::Vector3f& wo, const core::Vector3f& n, const Material* m)
    {
        core::Vector3f result(0.0f);
        result = core::Vector3f(m->Kd_ / M_PI);
        return result;
    }
    
}



