#include "MATH.h"
#include "BoundingTree.h"
#include "Object.h"
#include <functional>
#include <vector>

BoundingTreeNode::BoundingTreeNode()
{
    box.pMax = box.pMin = MATH::Vector3f(0.0f);
    left = nullptr;
    right = nullptr;
    object = nullptr;
}

Intersection simpleBVH::getLastNodeIntersection(BoundingTreeNode* node, const Ray& ray)
{
    Intersection intersection;
    if (node == nullptr || !node->box.getIntersection(ray))
    {
        return intersection;
    }
    if (node->left == nullptr && node->right == nullptr)
    {
        intersection = node->object->getIntersetion(ray);
    }
    else
    {
        Intersection leftIntersection, rightIntersection;
        if (node->left != nullptr)
            leftIntersection = this->getLastNodeIntersection(node->left, ray);
        if (node->right != nullptr)
            rightIntersection = this->getLastNodeIntersection(node->right, ray);
        intersection = leftIntersection.distance < rightIntersection.distance ? leftIntersection : rightIntersection;
    }  
    return intersection;
}  

BoundingTreeNode* simpleBVH::build(std::vector<OBJECT::Object* > objects)
{
    BoundingTreeNode* node = new BoundingTreeNode();
    if (objects.size() == 1)
    {
        node->object = objects[0];
        node->box = objects[0]->getBoundingBox();
    }
    else if (objects.size() == 2) 
    {  
        BoundingTreeNode* left = this->build(std::vector{objects[0]});
        BoundingTreeNode* right = this->build(std::vector{objects[1]});
        node->left = left;
        node->right = right;
        node->box = BoundingBox::Union(node->left->box, node->right->box);
    }
    else
    {
        MATH::Vector3f pMax = MATH::Vector3f(std::numeric_limits<float>::lowest()), pMin = MATH::Vector3f(std::numeric_limits<float>::max());
        auto findCenter = [&](MATH::Vector3f v) -> void {
            pMax.x = std::max(pMax.x, v.x);
            pMax.y = std::max(pMax.y, v.y);
            pMax.z = std::max(pMax.z, v.z);
            pMin.x = std::min(pMin.x, v.x);
            pMin.y = std::min(pMin.y, v.y);
            pMin.z = std::min(pMin.z, v.z);
        };
        for (auto& object : objects)
            findCenter(object->getBoundingBox().Centroid());
        
        int sortCace = 0;
        float destance_x = pMax.x - pMin.x,
              destance_y = pMax.y - pMin.y,
              destance_z = pMax.z - pMin.z;
        if (destance_x > destance_y && destance_x > destance_z)
            sortCace = 0;
        else if (destance_y > destance_z)
            sortCace = 1;
        else 
            sortCace = 2;


        switch (sortCace)
        {
        case 0:
            std::sort(objects.begin(), objects.end(), [](OBJECT::Object* obj1, OBJECT::Object* obj2) -> bool {
                return obj1->getBoundingBox().Centroid().x < obj2->getBoundingBox().Centroid().x;
            });
            break;
                
        case 1:
            std::sort(objects.begin(), objects.end(), [](OBJECT::Object* obj1, OBJECT::Object* obj2) -> bool {
                return obj1->getBoundingBox().Centroid().y < obj2->getBoundingBox().Centroid().y;
            });
            break;

        case 2:
            std::sort(objects.begin(), objects.end(), [](OBJECT::Object* obj1, OBJECT::Object* obj2) -> bool {
                return obj1->getBoundingBox().Centroid().z < obj2->getBoundingBox().Centroid().z;
            });
            break;

        default:
            break;
        }


        int halfSize = objects.size() / 2;
        std::vector<OBJECT::Object* > leftObjects(objects.begin(), objects.begin() + halfSize);
        std::vector<OBJECT::Object* > rightObjects(objects.begin() + halfSize, objects.end());

        BoundingTreeNode* left = this->build(leftObjects);
        BoundingTreeNode* right = this->build(rightObjects);

        node->left = left;
        node->right = right;

        node->box = BoundingBox::Union(node->left->box, node->right->box);
        
    }

    return node;
}

void simpleBVH::buildBVH(std::vector<OBJECT::Object* > objects)
{
    this->BVHHead = build(objects);   
}


Intersection simpleBVH::getIntersectionObject(const Ray& ray)
{
    Intersection intersection;

    intersection = this->getLastNodeIntersection(this->BVHHead, ray);

    return intersection;
}


BoundingTreeNode* simpleBVH::getBoundingTree()
{
    return this->BVHHead;
}

void simpleBVH::Show() const
{    
    std::function<void(BoundingTreeNode* node)> show = [&](BoundingTreeNode* node) -> void {
        if (node == nullptr)
            return ;
        if (node->left != nullptr)
            show(node->left);
        if (node->right != nullptr)
            show(node->right);
        std::cout << "node max point :" << node->box.pMax << "   min point :" << node->box.pMin << "\n";
    };

    show(this->BVHHead);
}

simpleBVH::simpleBVH() 
{ 
    BVHHead = nullptr;
}

simpleBVH::~simpleBVH()
{
    std::function<void(BoundingTreeNode* node)> func = [&](BoundingTreeNode* node) -> void{
        if (node == nullptr)
            return;
        if (node->left != nullptr)
            func(node->left);
        if (node->right != nullptr)
            func(node->right);
        delete node;
    };
}













BoundingTreeFactory* BoundingTreeFactory::instance = new BoundingTreeFactory();

BoundingTreeFactory::BoundingTreeFactory() { }

BoundingTreeFactory& BoundingTreeFactory::Instance()
{
    return *instance;
}

BaseBoundingTree* BoundingTreeFactory::getBoundingTree(BoundingTreeFactory::type t)
{
    BaseBoundingTree* result = nullptr;
    switch (t)
    {
    case BVH:
        result = new simpleBVH();   
        break;

    default:
        break;
    }
    return result;
}
