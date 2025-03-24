#pragma once
#include "BoundingBox.h"
#include "MATH.h"
#include "Object.h"
#include "Ray.h"
#include <memory>
#include "Scene.h"



struct BoundingTreeNode
{
    BoundingBox         box;
    BoundingTreeNode*   left;
    BoundingTreeNode*   right;
    OBJECT::Object*     object;

    BoundingTreeNode();
};

class BaseBoundingTree
{
    public:
        virtual void                buildBVH(std::vector<OBJECT::Object* > objects) = 0;
        virtual Intersection        getIntersectionObject(const Ray& ray) = 0; 
        virtual BoundingTreeNode*   getBoundingTree() = 0;
        virtual void                Show() const = 0;
};

class simpleBVH : public BaseBoundingTree
{
    private:
        BoundingTreeNode* BVHHead;

        Intersection        getLastNodeIntersection(BoundingTreeNode* node, const Ray& ray);
        BoundingTreeNode*   build(std::vector<OBJECT::Object* > objects);        
    public:
        simpleBVH();
        void                buildBVH(std::vector<OBJECT::Object *> objects) override;
        Intersection        getIntersectionObject(const Ray& ray) override;
        BoundingTreeNode*   getBoundingTree() override;
        void                Show() const override;
        ~simpleBVH();
};


class BoundingTreeFactory
{
    private:
        static BoundingTreeFactory* instance;

        BoundingTreeFactory();
        BoundingTreeFactory(const BoundingTreeFactory& ) = delete;
        BoundingTreeFactory&            operator = (const BoundingTreeFactory& ) = delete;
    public:
        enum type { BVH };

        static BoundingTreeFactory&     Instance();
        BaseBoundingTree*               getBoundingTree(type t);    
};