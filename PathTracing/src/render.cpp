#include "render.h"
#include "BoundingTree.h"
#include "MATH.h"
#include "Object.h"
#include <chrono>
#include <string>
#include "Tool.h"

#define EPSILON 0.0001

Intersection Renderpipeline::Light::Sample(const MATH::Vector3f& shaderPoint, float& pdf)
{
    Intersection intersection = {};

    float totalArea = 0.0f;
    
    std::vector<float> tempArea(this->lightObjectData.size());

    for (int i = 0; i < this->lightObjectData.size(); i++)
    {
        Renderpipeline::Light::LightTriangleData& data = lightObjectData[i];
        MATH::Vector3f w = (shaderPoint - data.inner).getNormalize();
        MATH::Vector3f n = data.triangle.normal;
        float cosen = MATH::dotProduction(w, n);
        tempArea[i] = cosen * data.triangle.area;
        if (tempArea[i] > 0)
            totalArea += tempArea[i];        
    }

    float randomNum = Tool::randomQueue::getInstance()->getRandomNum() * totalArea;

    float currentArea = 0.0f;
    int i = 0;
    while (i < this->lightObjectData.size())
    {
        currentArea += tempArea[i] > 0 ? tempArea[i] : 0.0f;
        if (randomNum < currentArea)
            break;
        i++;
    }    

    this->lightObjectData[i].triangle.Sample(intersection, pdf);    

    pdf = 1.0f / totalArea;

    return intersection;
}

Intersection Renderpipeline::Light::SampleNormal(const MATH::Vector3f& shaderPoint, float& pdf)
{
    Intersection intersection = {};

    float totalArea = 0.0f;

    for (int i = 0; i < this->lightObjectData.size(); i++)
        totalArea += this->lightObjectData[i].triangle.area;        

    float randomNum = Tool::randomQueue::getInstance()->getRandomNum() * totalArea;

    float currentArea = 0.0f;
    int i = 0;
    while (i < this->lightObjectData.size())
    {
        currentArea += this->lightObjectData[i].triangle.area;
        if (randomNum <= currentArea)
            break;
        i++;
    }    

    this->lightObjectData[i].triangle.Sample(intersection, pdf);    

    return intersection;
}

Renderpipeline::Light::LightTriangleData::LightTriangleData(const OBJECT::Triangle& triangle_) : triangle(triangle_)
{
    this->inner = triangle.getInner();
}

void Renderpipeline::Light::addTriangle(const OBJECT::Triangle& triangle)
{
    this->lightObjectData.emplace_back(triangle);
}

Renderpipeline::Renderpipeline() : RussianRoulette(0.8f), maxDepth(16) { }

Renderpipeline::Renderpipeline(Scene& scene)
{
    this->pushSceneInPipeline(scene);
}

void Renderpipeline::PreCompute()
{
    //init triangleLists and use the transform
    for (OBJECT::OBJ* object : this->scene->objectList)
    {
        for (OBJECT::Mesh& mesh : object->meshes)
        {
            //set the unique Id to each mesh
            mesh.setUniqueID(Scene::getUniqueID());

            auto& vertices = mesh.vertices;
            auto& material = mesh.material;
            int i = 0, n = vertices.size() / 3 * 3;
            while (i < n)
            {
                OBJECT::Triangle triangle(vertices[i], vertices[i + 1], vertices[i + 2], material);
                triangle.setUniqueID(mesh.getUniqueID()); 


                this->triangleList.emplace_back(triangle);
                i += 3;
            }
        }   
    }

    triangleOfLight.resize(this->scene->lightList.size());
    std::vector<OBJECT::Mesh* >& lightList = this->scene->lightList;
    for (int lightIndex = 0; lightIndex < lightList.size(); lightIndex++)
    {
        this->triangleOfLight[lightIndex].ID = lightList[lightIndex]->getUniqueID();
        auto& vertices = lightList[lightIndex]->vertices;
        auto& material = lightList[lightIndex]->material;
        int i = 0, n = vertices.size() / 3 * 3;
        while (i < n)
        {
            OBJECT::Triangle triangle(vertices[i], vertices[i + 1], vertices[i + 2], material);
            triangle.setUniqueID(lightList[lightIndex]->getUniqueID()); 


            this->triangleOfLight[lightIndex].addTriangle(triangle);            
            i += 3;
        }
    }

}

void Renderpipeline::BuildBoundingBoxTree()
{
    //build objects list;
    std::vector<OBJECT::Object* > objects(this->triangleList.size());
    //this->triangleListShow();

    std::cout << "-- Star to build the bounding tree.\n";
    std::chrono::high_resolution_clock::time_point star = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < this->triangleList.size(); i++)
    {
        objects[i] = &this->triangleList[i];
    }

    this->boundingTree = BoundingTreeFactory::Instance().getBoundingTree(BoundingTreeFactory::type::BVH);      
    this->boundingTree->buildBVH(objects); 
    //this->boundingTree->Show();

    std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();

    int64_t diff = std::chrono::duration_cast<std::chrono::milliseconds>(end - star).count();

    std::cout << "-- End of building bounding tree, used time :" << diff << "ms" << std::endl;
}


MATH::Vector3f Renderpipeline::rayTrace(Ray ray, int depth)
{
    Intersection intersection;
    intersection = this->boundingTree->getIntersectionObject(ray);

    if (!intersection.happened || depth > this->maxDepth)
        return MATH::Vector3f(0.0);

    MATH::Vector3f result(0.0);

    if (intersection.m->self_illuminating && !depth)
        return intersection.m->illuminate;

    MATH::Vector3f shaderPoint = intersection.coords;
    MATH::Vector3f n = intersection.normal.normalize();
    MATH::Vector3f wo = ray.dir;
    
    //sample light
    MATH::Vector3f firstLight = MATH::Vector3f(0.0f);
    for (Renderpipeline::Light& light : triangleOfLight)
    {
        float pdfLight = 0.0;
        Intersection lightIntersection = light.SampleNormal(shaderPoint, pdfLight);
        MATH::Vector3f xToP = lightIntersection.coords - shaderPoint;
        MATH::Vector3f ws = xToP.getNormalize();
        Intersection intersectionFromLight = this->boundingTree->getIntersectionObject(Ray(shaderPoint, ws));
        if (intersectionFromLight.happened && intersectionFromLight.id == light.ID)
        {
            MATH::Vector3f ns = lightIntersection.normal.normalize();
            OBJECT::Material* lightMaterial = intersectionFromLight.m;
            MATH::Vector3f BRDF = intersection.m->shader->getBRDF(ws, -wo, ns, intersection.m);
            float cosen1 = MATH::dotProduction(ws, n);
            float cosen2 = MATH::dotProduction(-ws, ns);
            float lengthPow2 = MATH::dotProduction(xToP, xToP);
            firstLight += lightMaterial->illuminate * BRDF * cosen1 * cosen2 / lengthPow2 / pdfLight;
        }
    }

    result += firstLight;

    //sample other
    if (Tool::randomQueue::getInstance()->getRandomNum() > this->RussianRoulette)
        return result;

    MATH::Vector3f secondLight = MATH::Vector3f(0.0f);

    MATH::Vector3f wi = intersection.m->shader->sample(wo, n).normalize();

    float pdf = intersection.m->shader->pdf(-wo, wi, n);
    float cosen = MATH::dotProduction(n, wi);   
    MATH::Vector3f BRDF = intersection.m->shader->getBRDF(-wo, wi, n, intersection.m);

    secondLight = this->rayTrace(Ray(shaderPoint ,wi), depth + 1) * BRDF * cosen / pdf / this->RussianRoulette;

    result += secondLight;
    
    return result;
}


void Renderpipeline::pushSceneInPipeline(Scene& scene)
{
    this->scene = &scene;

    //prepare for the rendering
    this->PreCompute();

    //build boundingBoxTree
    this->BuildBoundingBoxTree();
}

void Renderpipeline::DrawCall(std::vector<MATH::Vector3f>& framebuffer, int spp)
{
    MATH::Vector3f& oir = this->scene->camera.eye_point;
    MATH::Vector3f& starPoint = this->scene->camera.imageView.starPoint;
    MATH::Vector3f& delta_u = this->scene->camera.imageView.delta_u;
    MATH::Vector3f& delta_v = this->scene->camera.imageView.delta_v;


    int& step_u = this->scene->camera.imageView.step_u;
    int& step_v = this->scene->camera.imageView.step_v;

    float maxCount = framebuffer.size(), count = 0;

    int sqrtSpp = std::sqrt(spp);
    int sampleBoxCount = sqrtSpp * sqrtSpp;
    int multiSample = spp / sampleBoxCount;
    int centerSample = spp - multiSample * sampleBoxCount;

    if (sqrtSpp == 1)
    {
        sqrtSpp = 0;
        centerSample = spp;
    }

    MATH::Vector3f d_delta_u = delta_u / (float)sqrtSpp;
    MATH::Vector3f d_delta_v = delta_v / (float)sqrtSpp;

    std::cout << "-- Star to trace ray.\n";
    std::chrono::high_resolution_clock::time_point star = std::chrono::high_resolution_clock::now();

    std::cout << "Execution progress :\n";

    for (int j = 0; j < step_v; j++)
    {
        for (int i = 0; i < step_u; i++)
        {
            MATH::Vector3f temp(0.0f);
            MATH::Vector3f centerPoint = starPoint + delta_u * i + delta_v * j;
            int index = j * this->scene->width + i;
            MATH::Vector3f upLeft = centerPoint - delta_u * 0.5 - delta_v * 0.5;
            for (int x = 0; x < sqrtSpp; x++)
            {
                for (int y = 0; y < sqrtSpp; y++)
                {
                    MATH::Vector3f dir = (upLeft + d_delta_u * x + d_delta_v * y - oir).normalize();
                    for (int z = 0; z < multiSample; z++)
                        temp += rayTrace(Ray(oir, dir), 0);
                }
            }

            Ray centerRay(oir, (centerPoint - oir).normalize());
            for (int center = 0; center < centerSample; center++)
                temp += rayTrace(centerRay, 0);

            framebuffer[index] = temp / spp;   
            count++;
        }
        float progress = count / maxCount;
        Tool::execution_progress(progress);
    }

    // float scale = tan(scene->camera.fov * 0.5 * M_PI / 180);
    // float imageAspectRatio = scene->width / (float)scene->height;
    // int m = 0;
    // MATH::Vector3f eye(278, 273, -800);
    // for (int j = 0; j < scene->height; j++)
    // {
    //     for (int i = 0; i < scene->width; i++)
    //     {
    //         count++;
    //         float x = (2 * (i + 0.5) / (float)scene->width - 1) * imageAspectRatio * scale;
    //         float y = (1 - 2 * (j + 0.5) / (float)scene->height) * scale;

    //         MATH::Vector3f dir = MATH::Vector3f(-x, y, 1).normalize();
    //         int ssaaTime = (int)sqrt(spp);
    //         int checkTime = spp / ssaaTime / ssaaTime;
    //         if (ssaaTime <= 1)
    //             for (int k = 0; k < spp; k++)
    //             {
    //                 framebuffer[m] += this->rayTrace(Ray(eye, dir), 0) / spp; 
    //             }
    //         else
    //         {
    //             float overflood = 1.0 / (float)ssaaTime;
    //             for (int xIndex = 0; xIndex < ssaaTime; xIndex++)
    //             {
    //                 for (int yIndex = 0; yIndex < ssaaTime; yIndex++)
    //                 {
    //                     for (int sampleTime = 0; sampleTime < checkTime; sampleTime++)
    //                     {
    //                         float xOfferset = xIndex * overflood + 0.5 * overflood;
    //                         float yOfferset = yIndex * overflood + 0.5 * overflood;
    //                         float sampleX = (2 * (i + xOfferset) / (float)scene->width - 1) * imageAspectRatio * scale;
    //                         float sampleY = (1 - 2 * (j + yOfferset) / (float)scene->height) * scale;
    //                         MATH::Vector3f sampleDir = MATH::Vector3f(-sampleX, sampleY, 1).normalize();
    //                         MATH::Vector3f result = this->rayTrace(Ray(oir, sampleDir), 0);
    //                         framebuffer[m] += result;
    //                     }
    //                 }
    //             }
    //             framebuffer[m++] /= spp;
    //         }
    //     }
    //     float progress = count / maxCount;
    //     Tool::execution_progress(progress);
    // }

    std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();

    int64_t diff = std::chrono::duration_cast<std::chrono::milliseconds>(end - star).count();

    std::cout << "-- End of tracing ray, used time :" << diff << "ms" << std::endl;
    std::cout << "                                 :" << diff / 1000 << "s" << std::endl;
    std::cout << "                                 :" << diff / 1000 / 60 << "minute" << std::endl;
    std::cout << "                                 :" << diff / 1000 / 60 / 60 << "hour" << std::endl;

}


void Renderpipeline::triangleListShow() const
{
    for (auto& triangle : this->triangleList)
    {
        std::cout << "point 1:" << triangle.v0 << "    point 2 :" << triangle.v1 << "   point 3 :" << triangle.v2 << "\n";
        std::cout << "normal :" << triangle.normal << "\n";
        std::cout << "Bounding Box, point 1 :" << triangle.boundingBox.pMax << "    point 2 :" << triangle.boundingBox.pMin << std::endl;
    }
}











