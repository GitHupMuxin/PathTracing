#include <cstdio>
#include <iostream>
#include <string>
#include "engine/platform/platform_utils.h"
#include "engine/core/math/vector_defination.h"
#include "engine/core/math/random_queue.h"
#include "engine/core/thread/thread_pool.h"
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

    //lode the mesh
    std::string executionPath = platform::PlatformUtils::GetExecutionPath();
    
#ifdef _MSC_VER
    executionPath = executionPath.substr(0, executionPath.find_last_of('/'));
#endif // msvc


    resource::ResourceManeger::GetInstance()->LodeSceneFile(executionPath + "/assets/scene.json");

    //init Scene
    int width = resource::ResourceManeger::GetInstance()->GetScreenWidth();
    int height = resource::ResourceManeger::GetInstance()->GetScreenHeight();
    scene::Scene scene(width, height);

    // scene.SetCamera(camera);
    scene.SetCamera(resource::ResourceManeger::GetInstance()->GetCamera());

    //build Scene
    auto& objList = resource::ResourceManeger::GetInstance()->GetAllObjects();
    for (auto& object : objList)
    {
        scene.Add(object.get());
    }
    //scene.Show();

    //create a raytrace pipeline
    rendering::RenderPipeline raytracePipeline;    
    
    //push Scene into pipeline
    raytracePipeline.PushSceneInPipeline(scene);

    //generate frambuffer
    std::vector<core::Vector3f> frameBuffer(scene.width_ * scene.height_);

    //rendering
    int spp = resource::ResourceManeger::GetInstance()->GetSpp();
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

    std::string outPutFilePath = executionPath + "/out/testImageSpp";
    outPutFilePath += std::to_string(spp);
    outPutFilePath += ".ppm";

    rendering::RenderingUtils::SRGBDrawToFile(outPutFilePath, scene.width_, scene.height_, frameBuffer);

#ifdef _MSC_VER
    getchar();
#endif // msvc

    return 0;
}