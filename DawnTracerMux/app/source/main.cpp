#include <iostream>
#include <string>
#include "engine/platform/platform_utils.h"
#include "engine/core/math/vector_defination.h"
#include "engine/core/math/random_queue.h"
#include "engine/core/core_utils.h"
#include "engine/resource/object.h"
#include "engine/resource/resource_maneger.h"
#include "engine/resource/camera.h"
#include "engine/scene/scene.h"
#include "engine/rendering/rendering.h"
#include "engine/rendering/rendering_utils.h"
#include "third_party/nlohmann/json/json.hpp"

#ifndef M_PI
# define M_PI		3.14159265358979323846	/* pi */
#endif // !M_PI


int main()
{
    std::cout << "wellcome to my project" << std::endl;
    //std::cout << "create a vector3f show: " << vec << std::endl;

    //init Scene
    scene::Scene scene(784, 784);

    //lode the mesh
    std::string executionPath = platform::PlatformUtils::GetExecutionPath();
    resource::Obj floor(executionPath + "/assets/models/cornellbox/floor.obj");
    resource::Obj left(executionPath + "/assets/models/cornellbox/left.obj");
    resource::Obj right(executionPath + "/assets/models/cornellbox/right.obj");
    resource::Obj shortbox(executionPath + "/assets/models/cornellbox/shortbox.obj");
    resource::Obj tallbox(executionPath + "/assets/models/cornellbox/tallbox.obj");
    resource::Obj light(executionPath + "/assets/models/cornellbox/light.obj");

    resource::ResourceManeger::GetInstance().get()->LodeSceneFile(executionPath + "/assets/scene.json");

    //bing transform to mesh

    const std::string materialName = "red";
    //init materials
    resource::Material Red("red", core::Vector3f(0.0f), core::Vector3f(0.63f, 0.065f, 0.05f), core::Vector3f(0.0), core::Vector3f(0.0), 0.0f, 0.0f, 0.0f, 0);
    resource::Material Green("green", core::Vector3f(0.0f), core::Vector3f(0.14f, 0.45f, 0.091f), core::Vector3f(0.0), core::Vector3f(0.0), 0.0f, 0.0f, 0.0f, 0);
    resource::Material White("white", core::Vector3f(0.0f), core::Vector3f(0.725f, 0.71f, 0.68f), core::Vector3f(0.0), core::Vector3f(0.0), 0.0f, 0.0f, 0.0f, 0);
    resource::Material Light("light", core::Vector3f(0.0f), core::Vector3f(0.65), core::Vector3f(0.0), core::Vector3f(0.0), 0.0f, 0.0f, 0.0f, 0);
    Light.self_illuminating_ = true;
    Light.illuminate_ = 8.0f * core::Vector3f(0.747f + 0.058f, 0.747f + 0.258f, 0.747f) + 15.6f * core::Vector3f(0.740f + 0.287f, 0.740f + 0.160f, 0.740f) + 18.4f * core::Vector3f(0.737f + 0.642f, 0.737f + 0.159f, 0.737f);


    //initialize and bind shader
    resource::Shader diffuseShader;
    diffuseShader.shader_ = [](resource::Shader::ShaderInput& input) -> resource::Shader::ShaderOutput    
    {
        resource::Shader::ShaderOutput output;

        float r1 = core::RandomQueue::GetInstance()->GetRandomNum();
        float r2 = core::RandomQueue::GetInstance()->GetRandomNum();

        float phi = 2.0f * M_PI * r1;
        float cosTheta = std::fabs(1.0f - 2.0f * r2);
        float sinTheta = 1 - (cosTheta * cosTheta);

        output.wo.x = sinTheta * std::cos(phi);
        output.wo.y = sinTheta * std::sin(phi);
        output.wo.z = cosTheta;

        output.wo = core::CoreUtils::ToWorldSpace(output.wo, input.n); 

        output.BRDF = core::Vector3f(1.0f / M_PI);
        output.pdf = 1.0f / M_PI * 0.5f;

        return output;
    };

    diffuseShader.Sample = [](const core::Vector3f& wi, const core::Vector3f& n) -> core::Vector3f 
    {
        core::Vector3f wo(0.0f);
        float r1 = core::RandomQueue::GetInstance()->GetRandomNum();
        float r2 = core::RandomQueue::GetInstance()->GetRandomNum();

        float phi = 2.0f * M_PI * r1;
        float cosTheta = std::fabs(1.0f - 2.0f * r2);
        float sinTheta = std::sqrt(1.0f- (cosTheta * cosTheta));

        wo.x = sinTheta * std::cos(phi);
        wo.y = sinTheta * std::sin(phi);
        wo.z = cosTheta;

        wo = core::CoreUtils::ToWorldSpace(wo, n);

        return wo;    
    };

    diffuseShader.Pdf = [](const core::Vector3f& wi, const core::Vector3f& wo,  const core::Vector3f& n) -> float
    {
        float result = 0.0f;
        result = 1.0 / M_PI * 0.5;
        return result;
    };

    diffuseShader.GetBRDF = [](const core::Vector3f& wi, const core::Vector3f& wo, const core::Vector3f& n, const resource::Material* m) -> core::Vector3f
    {
        core::Vector3f result(0.0f);
        result = core::Vector3f(m->Kd_ / M_PI);
        return result;
    };


    Red.shader_ = &diffuseShader;
    White.shader_ = &diffuseShader;
    Green.shader_ = &diffuseShader;
    Light.shader_ = &diffuseShader;

    //bind materials to mesh
    floor.ResetMaterial(&White);
    left.ResetMaterial(&Red);
    right.ResetMaterial(&Green);
    shortbox.ResetMaterial(&White);
    tallbox.ResetMaterial(&White);
    light.ResetMaterial(&Light);


    //generate camera
    resource::Camera camera;
    camera.eye_point_ = core::Vector3f(278, 273, -800);
    camera.lookAt_ = core::Vector3f(278, 273, -799);
    camera.up_ = core::Vector3f(0.0, 1.0, 0.0);
    camera.fov_ = 40;

    scene.SetCamera(camera);

    //build Scene
    scene.Add(&floor);
    scene.Add(&left);
    scene.Add(&right);
    scene.Add(&shortbox);
    scene.Add(&tallbox);
    scene.Add(&light);

    //scene.Show();

    //create a raytrace pipeline
    rendering::Renderpipeline raytracePipeline;    
    
    //push Scene into pipeline
    raytracePipeline.PushSceneInPipeline(scene);

    //generate frambuffer

    std::vector<core::Vector3f> frameBuffer(scene.width_ * scene.height_);

    //rendering
    int spp = 1;
    std::cout << "now render with spp = " << spp << std::endl;
    raytracePipeline.DrawCall(frameBuffer, spp);

    // for (int i = 0; i < scene.width; i++)
    // {
    //     for (int j = 0; j < scene.height; j++)
    //     {
    //         int index = i * scene.width + j;
    //         std::cout << frameBuffer[index] << std::endl;
    //     }
    // }

    std::string outPutFilePath = executionPath + "/out/imageSpp";
    outPutFilePath += std::to_string(spp);
    outPutFilePath += ".ppm";

    rendering::RenderingUtils::SRGBDrawToFile(outPutFilePath, scene.width_, scene.height_, frameBuffer);

    return 0;
}