#pragma once
#include <memory>
#include "engine/core/math/vector_defination.h"
#include "engine/core/ray.h"
#include "engine/resource/object.h"
#include "scene_object.h"

namespace scene
{

    struct alignas(64) BoundingTreeNode
    {
        BoundingBox                     box_;
        BoundingTreeNode*               left_;
        BoundingTreeNode*               right_;
        scene::Object*                  object_;

        BoundingTreeNode();
    };

    class BaseBoundingTree
    {
        public:
            virtual void                BuildBVH(std::vector<Object* > objects) = 0;
            virtual Intersection        GetIntersectionObject(const core::Ray& ray) = 0; 
            virtual BoundingTreeNode*   GetBoundingTree() = 0;
            virtual void                Show() const = 0;
    };

    class SimpleBVH : public BaseBoundingTree
    {
        private:
            BoundingTreeNode*           BVHHead_;

            Intersection                GetLastNodeIntersection(BoundingTreeNode* node, const core::Ray& ray);
            BoundingTreeNode*           Build(std::vector<Object* > objects);        
        public:
            SimpleBVH();
            void                        BuildBVH(std::vector<Object *> objects) override;
            Intersection                GetIntersectionObject(const core::Ray& ray) override;
            BoundingTreeNode*           GetBoundingTree() override;
            void                        Show() const override;
            ~SimpleBVH();
    };


    class BoundingTreeFactory
    {
        private:
            BoundingTreeFactory();
            BoundingTreeFactory(const BoundingTreeFactory& ) = delete;
            BoundingTreeFactory&            operator = (const BoundingTreeFactory& ) = delete;

        public:
            enum TypeOfBoundingTree { BVH };

            static BoundingTreeFactory*     GetInstance();
            BaseBoundingTree*               GetBoundingTree(TypeOfBoundingTree t);    
    };


}