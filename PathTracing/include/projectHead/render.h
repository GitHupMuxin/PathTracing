#pragma once
#include <vector>
#include "Object.h"
#include "BoundingTree.h"

class Renderpipeline
{
    private:
        class Light
        {
            private:
                class LightTriangleData
                {
                    public:
                        OBJECT::Triangle            triangle;
                        MATH::Vector3f              inner;
                        LightTriangleData(const OBJECT::Triangle& triangle_);
                };
            public:
                unsigned int                    ID;
                std::vector<LightTriangleData>  lightObjectData;
                Intersection Sample(const MATH::Vector3f& shaderPoint, float& pdf);
                Intersection SampleNormal(const MATH::Vector3f& shaderPoint, float& pdf);
                void addTriangle(const OBJECT::Triangle& triangle); 
        };

        int                                 maxDepth;
        float                               RussianRoulette;
        Scene*                              scene;
        std::vector<OBJECT::Triangle>       triangleList;
        std::vector<Renderpipeline::Light>  triangleOfLight;
        BaseBoundingTree*                   boundingTree;

        void            PreCompute();
        void            BuildBoundingBoxTree();
        MATH::Vector3f  rayTrace(Ray ray, int depth);
    public:
        Renderpipeline();
        Renderpipeline(Scene& scene);
        
        void            pushSceneInPipeline(Scene& scene);
        void            DrawCall(std::vector<MATH::Vector3f>& framebuffer, int spp);
        void            triangleListShow() const;
        
};