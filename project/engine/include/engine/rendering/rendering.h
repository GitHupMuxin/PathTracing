#pragma once
#include <vector>
#include "engine/resource/object.h"
#include "engine/scene/bounding_tree.h"
#include "engine/scene/scene.h"

namespace rendering
{

    class Renderpipeline
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

            int                                 maxDepth_;
            float                               RussianRoulette_;
            scene::Scene*                       scene_;
            std::vector<scene::Triangle>       triangleList_;
            std::vector<Renderpipeline::Light>  triangleOfLight_;
            scene::BaseBoundingTree*                   boundingTree_;

            void            PreCompute();
            void            BuildBoundingBoxTree();
            core::Vector3f  RayTrace(core::Ray ray, int depth);
        public:
            Renderpipeline();
            Renderpipeline(scene::Scene& scene);
        
            void            PushSceneInPipeline(scene::Scene& scene);
            void            DrawCall(std::vector<core::Vector3f>& framebuffer, int spp);
            void            triangleListShow() const;
        
    };


}