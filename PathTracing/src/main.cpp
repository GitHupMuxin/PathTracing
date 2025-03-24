#include <iostream>
#include <string>
#include "MATH.h"
#include "Scene.h"
#include "Tool.h"
#include "render.h"
#include "Object.h"

int main()
{
    std::cout << "wellcome to my project" << std::endl;
    //std::cout << "create a vector3f show: " << vec << std::endl;

    //init Scene
    Scene scene(784, 784);

    //lode the mesh
    OBJECT::OBJ floor("models/cornellbox/floor.obj");
    OBJECT::OBJ left("models/cornellbox/left.obj");
    OBJECT::OBJ right("models/cornellbox/right.obj");
    OBJECT::OBJ shortbox("models/cornellbox/shortbox.obj");
    OBJECT::OBJ tallbox("models/cornellbox/tallbox.obj");
    OBJECT::OBJ light("models/cornellbox/light.obj");

    //bing transform to mesh

    const std::string materialName = "red";
    //init materials
    OBJECT::Material Red("red", MATH::Vector3f(0.0f), MATH::Vector3f(0.63f, 0.065f, 0.05f), MATH::Vector3f(0.0), MATH::Vector3f(0.0), 0.0f, 0.0f, 0.0f, 0);
    OBJECT::Material Green("green", MATH::Vector3f(0.0f), MATH::Vector3f(0.14f, 0.45f, 0.091f), MATH::Vector3f(0.0), MATH::Vector3f(0.0), 0.0f, 0.0f, 0.0f, 0);
    OBJECT::Material White("white", MATH::Vector3f(0.0f), MATH::Vector3f(0.725f, 0.71f, 0.68f), MATH::Vector3f(0.0), MATH::Vector3f(0.0), 0.0f, 0.0f, 0.0f, 0);
    OBJECT::Material Light("light", MATH::Vector3f(0.0f), MATH::Vector3f(0.65), MATH::Vector3f(0.0), MATH::Vector3f(0.0), 0.0f, 0.0f, 0.0f, 0);
    Light.self_illuminating = true;
    Light.illuminate = 8.0f * MATH::Vector3f(0.747f + 0.058f, 0.747f + 0.258f, 0.747f) + 15.6f * MATH::Vector3f(0.740f + 0.287f, 0.740f + 0.160f, 0.740f) + 18.4f * MATH::Vector3f(0.737f + 0.642f, 0.737f + 0.159f, 0.737f);

    //initialize and bind shader
    OBJECT::Shader diffuseShader;
    diffuseShader.shader = [](OBJECT::Shader::ShaderInput& input) -> OBJECT::Shader::ShaderOutput    
    {
        OBJECT::Shader::ShaderOutput output;

        float r1 = Tool::randomQueue::getInstance()->getRandomNum();
        float r2 = Tool::randomQueue::getInstance()->getRandomNum();

        float phi = 2.0f * M_PI * r1;
        float cosTheta = std::fabs(1.0f - 2.0f * r2);
        float sinTheta = 1 - (cosTheta * cosTheta);

        output.wo.x = sinTheta * std::cos(phi);
        output.wo.y = sinTheta * std::sin(phi);
        output.wo.z = cosTheta;

        output.wo = Tool::toWorldSpace(output.wo, input.n); 

        output.BRDF = MATH::Vector3f(1.0f / M_PI);
        output.pdf = 1.0f / M_PI * 0.5f;

        return output;
    };

    diffuseShader.sample = [](const MATH::Vector3f& wi, const MATH::Vector3f& n) -> MATH::Vector3f 
    {
        MATH::Vector3f wo(0.0f);
        float r1 = Tool::randomQueue::getInstance()->getRandomNum();
        float r2 = Tool::randomQueue::getInstance()->getRandomNum();

        float phi = 2.0f * M_PI * r1;
        float cosTheta = std::fabs(1.0f - 2.0f * r2);
        float sinTheta = std::sqrt(1.0f- (cosTheta * cosTheta));

        wo.x = sinTheta * std::cos(phi);
        wo.y = sinTheta * std::sin(phi);
        wo.z = cosTheta;

        wo = Tool::toWorldSpace(wo, n);

        return wo;    
    };

    diffuseShader.pdf = [](const MATH::Vector3f& wi, const MATH::Vector3f& wo,  const MATH::Vector3f& n) -> float
    {
        float result = 0.0f;
        result = 1.0 / M_PI * 0.5;
        return result;
    };

    diffuseShader.getBRDF = [](const MATH::Vector3f& wi, const MATH::Vector3f& wo, const MATH::Vector3f& n, const OBJECT::Material* m) -> MATH::Vector3f
    {
        MATH::Vector3f result(0.0f);
        result = MATH::Vector3f(m->Kd / M_PI);
        return result;
    };


    Red.shader = &diffuseShader;
    White.shader = &diffuseShader;
    Green.shader = &diffuseShader;
    Light.shader = &diffuseShader;

    //bind materials to mesh
    floor.resetMaterial(&White);
    left.resetMaterial(&Red);
    right.resetMaterial(&Green);
    shortbox.resetMaterial(&White);
    tallbox.resetMaterial(&White);
    light.resetMaterial(&Light);


    //generate camera
    Camera camera;
    camera.eye_point = MATH::Vector3f(278, 273, -800);
    camera.lookAt = MATH::Vector3f(278, 273, -799);
    camera.up = MATH::Vector3f(0.0, 1.0, 0.0);
    camera.fov = 40;

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
    Renderpipeline raytracePipeline;    
    
    //push Scene into pipeline
    raytracePipeline.pushSceneInPipeline(scene);

    //generate frambuffer

    std::vector<MATH::Vector3f> frameBuffer(scene.width * scene.height);

    //rendering
    int spp = 64;
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

    std::string outPutFilePath = "out/imageSpp";
    outPutFilePath += std::to_string(spp);
    outPutFilePath += ".ppm";

    Tool::SRGBDrawToFile(outPutFilePath, scene.width, scene.height, frameBuffer);

    return 0;
}