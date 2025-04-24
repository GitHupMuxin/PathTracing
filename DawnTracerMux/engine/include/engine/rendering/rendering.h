#pragma once
#include <vector>
#include <mutex>
#include "engine/resource/object.h"
#include "engine/scene/bounding_tree.h"
#include "engine/scene/scene.h"

namespace rendering
{

    class RenderPipeline
    {
        private:
            class Light
            {
                private:
                    class LightTriangleData
                    {
                        public:
                            scene::Triangle             triangle_;
                            core::Vector3f              inner_;
                            LightTriangleData(const scene::Triangle& triangle);
                    };
                public:
                    unsigned int                    ID_;
                    std::vector<LightTriangleData>  lightObjectData_;
                    scene::Intersection Sample(const core::Vector3f& shaderPoint, float& pdf);
                    scene::Intersection SampleNormal(const core::Vector3f& shaderPoint, float& pdf);
                    void AddTriangle(const scene::Triangle& triangle); 
            };

            int                                     maxDepth_;
            float                                   RussianRoulette_;
            scene::Scene*                           scene_;
            std::vector<scene::Triangle>            triangleList_;
            std::vector<RenderPipeline::Light>      triangleOfLight_;
            scene::BaseBoundingTree*                boundingTree_;

            void                    PreCompute();
            void                    BuildBoundingBoxTree();
            core::Vector3f          RayTrace(core::Ray ray, int depth);
            core::Vector3f          ThreadSafeRayTrace(core::Ray ray, int depth);
            void                    DrawCallImp_V1(std::vector<core::Vector3f>& framebuffer, int spp);
            void                    DrawCallImp_V2(std::vector<core::Vector3f>& framebuffer, int spp);
            std::function<void()>   GenerateRenderTask(core::Vector3f*result, core::Vector3f oir, core::Vector3f centerPoint, core::Vector3f delta_u, core::Vector3f delta_v, core::Vector3f d_delta_u, core::Vector3f d_delta_v, int multiSample, int centerSample, int sqrtSpp, int spp);
        public:
            RenderPipeline();
            RenderPipeline(scene::Scene& scene);
        
            void                    PushSceneInPipeline(scene::Scene& scene);
            void                    DrawCall(std::vector<core::Vector3f>& framebuffer, int spp);
            void                    TriangleListShow() const;
    };


}