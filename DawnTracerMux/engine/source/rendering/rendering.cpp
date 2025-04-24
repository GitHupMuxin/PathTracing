#include <string>
#include <chrono>
#include <atomic>
#include "engine/core/math/vector_defination.h"
#include "engine/core/math/random_queue.h"
#include "engine/core/thread/thread_pool.h"
#include "engine/resource/object.h"
#include "engine/scene/bounding_tree.h"
#include "engine/rendering/rendering.h"
#include "engine/rendering/rendering_utils.h"

#define EPSILON 0.0001

namespace rendering
{

    scene::Intersection RenderPipeline::Light::Sample(const core::Vector3f& shaderPoint, float& pdf)
    {
        scene::Intersection intersection = {};

        float totalArea = 0.0f;
    
        std::vector<float> tempArea(this->lightObjectData_.size());

        for (int i = 0; i < this->lightObjectData_.size(); i++)
        {
            RenderPipeline::Light::LightTriangleData& data = lightObjectData_[i];
            core::Vector3f w = (shaderPoint - data.inner_).GetNormalize();
            core::Vector3f n = data.triangle_.normal_;
            float cosen = core::DotProduction(w, n);
            tempArea[i] = cosen * data.triangle_.area_;
            if (tempArea[i] > 0)
                totalArea += tempArea[i];        
        }

        float randomNum = core::RandomQueue::GetInstance()->GetRandomNum() * totalArea;

        float currentArea = 0.0f;
        int i = 0;
        while (i < this->lightObjectData_.size())
        {
            currentArea += tempArea[i] > 0 ? tempArea[i] : 0.0f;
            if (randomNum < currentArea)
                break;
            i++;
        }    

        this->lightObjectData_[i].triangle_.Sample(intersection, pdf);    

        pdf = 1.0f / totalArea;

        return intersection;
    }

    scene::Intersection RenderPipeline::Light::SampleNormal(const core::Vector3f& shaderPoint, float& pdf)
    {
        scene::Intersection intersection = {};

        float totalArea = 0.0f;

        for (int i = 0; i < this->lightObjectData_.size(); i++)
            totalArea += this->lightObjectData_[i].triangle_.area_;        

        float randomNum = core::ThreadSafeRandomQueue::GetInstance()->GetRandomNum() * totalArea;

        float currentArea = 0.0f;
        int i = 0;
        while (i < this->lightObjectData_.size())
        {
            currentArea += this->lightObjectData_[i].triangle_.area_;
            if (randomNum <= currentArea)
                break;
            i++;
        }    

        this->lightObjectData_[i].triangle_.Sample(intersection, pdf);    

        return intersection;
    }

    RenderPipeline::Light::LightTriangleData::LightTriangleData(const scene::Triangle& triangle) : triangle_(triangle)
    {
        this->inner_ = triangle_.GetInner();
    }

    void RenderPipeline::Light::AddTriangle(const scene::Triangle& triangle)
    {
        this->lightObjectData_.emplace_back(triangle);
    }

    RenderPipeline::RenderPipeline() : RussianRoulette_(0.8f), maxDepth_(16) { }

    RenderPipeline::RenderPipeline(scene::Scene& scene)
    {
        this->PushSceneInPipeline(scene);
    }

    void RenderPipeline::PreCompute()
    {
        //init triangleLists and use the transform
        for (resource::Obj* object : this->scene_->objectList_)
        {
            for (resource::Mesh& mesh : object->meshes_)
            {
                //set the unique Id to each mesh
                mesh.SetUniqueID(scene::Scene::GetUniqueID());

                auto& vertices = mesh.vertices_;
                auto& material = mesh.material_;
                int i = 0, n = vertices.size() / 3 * 3;
                while (i < n)
                {
                    scene::Triangle triangle(vertices[i], vertices[i + 1], vertices[i + 2], material);
                    triangle.SetUniqueID(mesh.GetUniqueID()); 


                    this->triangleList_.emplace_back(triangle);
                    i += 3;
                }
            }   
        }

        triangleOfLight_.resize(this->scene_->lightList_.size());
        std::vector<resource::Mesh* >& lightList = this->scene_->lightList_;
        for (int lightIndex = 0; lightIndex < lightList.size(); lightIndex++)
        {
            this->triangleOfLight_[lightIndex].ID_ = lightList[lightIndex]->GetUniqueID();
            auto& vertices = lightList[lightIndex]->vertices_;
            auto& material = lightList[lightIndex]->material_;
            int i = 0, n = vertices.size() / 3 * 3;
            while (i < n)
            {
                scene::Triangle triangle(vertices[i], vertices[i + 1], vertices[i + 2], material);
                triangle.SetUniqueID(lightList[lightIndex]->GetUniqueID()); 


                this->triangleOfLight_[lightIndex].AddTriangle(triangle);            
                i += 3;
            }
        }

    }

    void RenderPipeline::BuildBoundingBoxTree()
    {
        //build objects list;
        std::vector<scene::Object* > objects(this->triangleList_.size());
        //this->triangleListShow();

        std::cout << "-- Star to build the bounding tree.\n";
        std::chrono::high_resolution_clock::time_point star = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < this->triangleList_.size(); i++)
        {
            objects[i] = &this->triangleList_[i];
        }

        this->boundingTree_ = scene::BoundingTreeFactory::GetInstance()->GetBoundingTree(scene::BoundingTreeFactory::TypeOfBoundingTree::BVH);      
        this->boundingTree_->BuildBVH(objects); 
        //this->boundingTree->Show();

        std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();

        int64_t diff = std::chrono::duration_cast<std::chrono::milliseconds>(end - star).count();

        std::cout << "-- End of building bounding tree, used time :" << diff << "ms" << std::endl;  
    }   


    core::Vector3f RenderPipeline::RayTrace(core::Ray ray, int depth)
    {
        scene::Intersection intersection;
        intersection = this->boundingTree_->GetIntersectionObject(ray);

        if (!intersection.happened || depth > this->maxDepth_)
            return core::Vector3f(0.0);

        core::Vector3f result(0.0);

        if (intersection.m->self_illuminating_)
        {
            if (!depth)
                return intersection.m->illuminate_;
            else 
                return core::Vector3f(0.0f);
        }


        core::Vector3f shaderPoint = intersection.coords;
        core::Vector3f n = intersection.normal.Normalize();
        core::Vector3f wo = ray.dir_;
    
        //sample light
        core::Vector3f firstLight = core::Vector3f(0.0f);
        for (RenderPipeline::Light& light : triangleOfLight_)
        {
            float pdfLight = 0.0;
            scene::Intersection lightIntersection = light.SampleNormal(shaderPoint, pdfLight);
            core::Vector3f xToP = lightIntersection.coords - shaderPoint;
            core::Vector3f ws = xToP.GetNormalize();
            scene::Intersection intersectionFromLight = this->boundingTree_->GetIntersectionObject(core::Ray(shaderPoint, ws));
            if (intersectionFromLight.happened && intersectionFromLight.id == light.ID_)
            {
                core::Vector3f ns = lightIntersection.normal.Normalize();
                resource::Material* lightMaterial = intersectionFromLight.m;
                core::Vector3f BRDF = intersection.m->shader_->GetBRDF(ws, -wo, ns, intersection.m);
                float cosen1 = core::DotProduction(ws, n);
                float cosen2 = core::DotProduction(-ws, ns);
                float lengthPow2 = core::DotProduction(xToP, xToP);
                firstLight += lightMaterial->illuminate_ * BRDF * cosen1 * cosen2 / lengthPow2 / pdfLight;
            }
        }

        result += firstLight;

        //sample other
        if (core::RandomQueue::GetInstance()->GetRandomNum() > this->RussianRoulette_)
            return result;

        core::Vector3f secondLight = core::Vector3f(0.0f);

        core::Vector3f wi = intersection.m->shader_->Sample(wo, n).Normalize();

        float pdf = intersection.m->shader_->Pdf(-wo, wi, n);
        float cosen = core::DotProduction(n, wi);   
        core::Vector3f BRDF = intersection.m->shader_->GetBRDF(-wo, wi, n, intersection.m);

        secondLight = this->RayTrace(core::Ray(shaderPoint ,wi), depth + 1) * BRDF * cosen / pdf / this->RussianRoulette_;

        result += secondLight;
    
        return result;
    }

    core::Vector3f RenderPipeline::ThreadSafeRayTrace(core::Ray ray, int depth)
    {
        scene::Intersection intersection;
        intersection = this->boundingTree_->GetIntersectionObject(ray);

        if (!intersection.happened || depth > this->maxDepth_)
            return core::Vector3f(0.0);

        core::Vector3f result(0.0);

        if (intersection.m->self_illuminating_)
        {
            if (!depth)
                return intersection.m->illuminate_;
            else 
                return core::Vector3f(0.0f);
        }


        core::Vector3f shaderPoint = intersection.coords;
        core::Vector3f n = intersection.normal.Normalize();
        core::Vector3f wo = ray.dir_;
    
        //sample light
        core::Vector3f firstLight = core::Vector3f(0.0f);
        for (RenderPipeline::Light& light : triangleOfLight_)
        {
            float pdfLight = 0.0;
            scene::Intersection lightIntersection = light.SampleNormal(shaderPoint, pdfLight);
            core::Vector3f xToP = lightIntersection.coords - shaderPoint;
            core::Vector3f ws = xToP.GetNormalize();
            scene::Intersection intersectionFromLight = this->boundingTree_->GetIntersectionObject(core::Ray(shaderPoint, ws));
            if (intersectionFromLight.happened && intersectionFromLight.id == light.ID_)
            {
                core::Vector3f ns = lightIntersection.normal.Normalize();
                resource::Material* lightMaterial = intersectionFromLight.m;
                core::Vector3f BRDF = intersection.m->shader_->GetBRDF(ws, -wo, ns, intersection.m);
                float cosen1 = core::DotProduction(ws, n);
                float cosen2 = core::DotProduction(-ws, ns);
                float lengthPow2 = core::DotProduction(xToP, xToP);
                firstLight += lightMaterial->illuminate_ * BRDF * cosen1 * cosen2 / lengthPow2 / pdfLight;
            }
        }

        result += firstLight;

        //sample other
        if (core::ThreadSafeRandomQueue::GetInstance()->GetRandomNum() > this->RussianRoulette_)
            return result;

        core::Vector3f secondLight = core::Vector3f(0.0f);

        core::Vector3f wi = intersection.m->shader_->Sample(wo, n).Normalize();

        float pdf = intersection.m->shader_->Pdf(-wo, wi, n);
        float cosen = core::DotProduction(n, wi);   
        core::Vector3f BRDF = intersection.m->shader_->GetBRDF(-wo, wi, n, intersection.m);

        secondLight = this->ThreadSafeRayTrace(core::Ray(shaderPoint ,wi), depth + 1) * BRDF * cosen / pdf / this->RussianRoulette_;

        result += secondLight;
    
        return result;
    }


    void RenderPipeline::DrawCallImp_V1(std::vector<core::Vector3f>& framebuffer, int spp)
    {
        core::Vector3f& oir = this->scene_->camera_.eye_point_;
        core::Vector3f& starPoint = this->scene_->camera_.imageView_.starPoint_;
        core::Vector3f& delta_u = this->scene_->camera_.imageView_.delta_u_;
        core::Vector3f& delta_v = this->scene_->camera_.imageView_.delta_v_;


        int& step_u = this->scene_->camera_.imageView_.step_u_;
        int& step_v = this->scene_->camera_.imageView_.step_v_;

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

        core::Vector3f d_delta_u = delta_u / (float)sqrtSpp;
        core::Vector3f d_delta_v = delta_v / (float)sqrtSpp;

        std::cout << "-- Star to trace ray.\n";
        std::chrono::high_resolution_clock::time_point star = std::chrono::high_resolution_clock::now();

        std::cout << "Execution progress :\n";

        for (int j = 0; j < step_v; j++)
        {
            for (int i = 0; i < step_u; i++)
            {
                core::Vector3f temp(0.0f);
                core::Vector3f centerPoint = starPoint + delta_u * i + delta_v * j;
                int index = j * this->scene_->width_ + i;
                core::Vector3f upLeft = centerPoint - delta_u * 0.5 - delta_v * 0.5;
                for (int x = 0; x < sqrtSpp; x++)
                {
                    for (int y = 0; y < sqrtSpp; y++)
                    {
                        core::Vector3f dir = (upLeft + d_delta_u * x + d_delta_v * y - oir).Normalize();
                        for (int z = 0; z < multiSample; z++)
                            temp += RayTrace(core::Ray(oir, dir), 0);
                    }
                }

                core::Ray centerRay(oir, (centerPoint - oir).Normalize());
                for (int center = 0; center < centerSample; center++)
                    temp += RayTrace(centerRay, 0);

                framebuffer[index] = temp / spp;   
                count++;
            }
            float progress = count / maxCount;
            RenderingUtils::ExecutionProgress(progress);
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

    void RenderPipeline::DrawCallImp_V2(std::vector<core::Vector3f>& framebuffer, int spp)
    {

        core::Vector3f& oir = this->scene_->camera_.eye_point_;
        core::Vector3f& starPoint = this->scene_->camera_.imageView_.starPoint_;
        core::Vector3f& delta_u = this->scene_->camera_.imageView_.delta_u_;
        core::Vector3f& delta_v = this->scene_->camera_.imageView_.delta_v_;


        int& step_u = this->scene_->camera_.imageView_.step_u_;
        int& step_v = this->scene_->camera_.imageView_.step_v_;

        int sqrtSpp = std::sqrt(spp);
        int sampleBoxCount = sqrtSpp * sqrtSpp;
        int multiSample = spp / sampleBoxCount;
        int centerSample = spp - multiSample * sampleBoxCount;

        if (sqrtSpp == 1)
        {
            sqrtSpp = 0;
            centerSample = spp;
        }

        core::Vector3f d_delta_u = delta_u / (float)sqrtSpp;
        core::Vector3f d_delta_v = delta_v / (float)sqrtSpp;

        std::cout << "-- Star to trace ray.\n";
        std::chrono::high_resolution_clock::time_point star = std::chrono::high_resolution_clock::now();

        std::cout << "Execution progress :\n";
         
        int count = 0;

        for (int j = 0; j < step_v; j++)
        {
            for (int i = 0; i < step_u; i++)
            {
                int index = j * this->scene_->width_ + i;
                core::Vector3f centerPoint = starPoint + delta_u * i + delta_v * j;
                core::ThreadPool::GetInstance()->Commit(this->GenerateRenderTask(&framebuffer[index], oir, centerPoint, delta_u, delta_v, d_delta_u, d_delta_v, multiSample, centerSample, sqrtSpp, spp));
            }
        }

        std::cout << "Finish Generate Render Task" << std::endl;

        core::ThreadPool::GetInstance()->WaiteAll();     
        core::ThreadPool::GetInstance()->ShutDownAllThreads();  

        std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();

        int64_t diff = std::chrono::duration_cast<std::chrono::milliseconds>(end - star).count();

        std::cout << "-- End of tracing ray, used time :" << diff << "ms" << std::endl;
        std::cout << "                                 :" << diff / 1000 << "s" << std::endl;
        std::cout << "                                 :" << diff / 1000 / 60 << "minute" << std::endl;
        std::cout << "                                 :" << diff / 1000 / 60 / 60 << "hour" << std::endl;


    }

    std::function<void()> RenderPipeline::GenerateRenderTask(core::Vector3f*result, core::Vector3f oir, core::Vector3f centerPoint, core::Vector3f delta_u, core::Vector3f delta_v, core::Vector3f d_delta_u, core::Vector3f d_delta_v, int multiSample, int centerSample, int sqrtSpp, int spp)
    {
        auto task = [this](core::Vector3f*result, core::Vector3f oir, core::Vector3f centerPoint, core::Vector3f delta_u, core::Vector3f delta_v, core::Vector3f d_delta_u, core::Vector3f d_delta_v, int multiSample, int centerSample, int sqrtSpp, int spp){

            core::Vector3f upLeft = centerPoint - delta_u * 0.5 - delta_v * 0.5;
            for (int x = 0; x < sqrtSpp; x++)
            {
                for (int y = 0; y < sqrtSpp; y++)
                {
                    core::Vector3f dir = (upLeft + d_delta_u * x + d_delta_v * y - oir).Normalize();
                    for (int multiTime = 0; multiTime < multiSample; multiTime++)
                    {
                        core::Vector3f r = this->ThreadSafeRayTrace(core::Ray(oir, dir), 0);            
                        result->x += r.x / spp;
                        result->y += r.y / spp;
                        result->z += r.z / spp;
                    }
                }
            }

            core::Ray centerRay(oir, (centerPoint - oir).Normalize());
            for (int center = 0; center < centerSample; center++)
            {
                core::Vector3f r = this->ThreadSafeRayTrace(centerRay, 0);
                result->x += r.x / spp;
                result->y += r.y / spp;
                result->z += r.z / spp;
            }
        };    
        return std::bind(task, result, oir, centerPoint, delta_u, delta_v, d_delta_u, d_delta_v, multiSample, centerSample, sqrtSpp, spp);    
    }

    void RenderPipeline::PushSceneInPipeline(scene::Scene& scene)
    {
        this->scene_ = &scene;

        //prepare for the rendering
        this->PreCompute();

        //build boundingBoxTree
        this->BuildBoundingBoxTree();
    }

    void RenderPipeline::DrawCall(std::vector<core::Vector3f>& framebuffer, int spp)
    {        
        //this->DrawCallImp_V1(framebuffer, spp);
        this->DrawCallImp_V2(framebuffer, spp);
    }

    void RenderPipeline::TriangleListShow() const
    {
        for (auto& triangle : this->triangleList_)
        {
            std::cout << "point 1:" << triangle.v0_ << "    point 2 :" << triangle.v1_ << "   point 3 :" << triangle.v2_ << "\n";
            std::cout << "normal :" << triangle.normal_ << "\n";
            std::cout << "Bounding Box, point 1 :" << triangle.boundingBox_.pMax_ << "    point 2 :" << triangle.boundingBox_.pMin_ << std::endl;
        }
    }


}





