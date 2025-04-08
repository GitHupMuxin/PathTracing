#include <vector>
#include <functional>
#include "engine/scene/bounding_tree.h"

namespace scene
{


    BoundingTreeNode::BoundingTreeNode()
    {
        box_.pMax_ = box_.pMin_ = core::Vector3f(0.0f);
        left_ = nullptr;
        right_ = nullptr;
        object_ = nullptr;
    }

    Intersection SimpleBVH::GetLastNodeIntersection(BoundingTreeNode* node, const core::Ray& ray)
    {
        Intersection intersection;
        if (node == nullptr || !node->box_.GetIntersection(ray))
        {
            return intersection;
        }
        if (node->left_ == nullptr && node->right_ == nullptr)
        {
            intersection = node->object_->GetIntersetion(ray);
        }
        else
        {
            Intersection leftIntersection, rightIntersection;
            if (node->left_ != nullptr)
                leftIntersection = this->GetLastNodeIntersection(node->left_, ray);
            if (node->right_ != nullptr)
                rightIntersection = this->GetLastNodeIntersection(node->right_, ray);
            intersection = leftIntersection.distance < rightIntersection.distance ? leftIntersection : rightIntersection;
        }  
        return intersection;
    }  

    BoundingTreeNode* SimpleBVH::Build(std::vector<Object* > objects)
    {   
        BoundingTreeNode* node = new BoundingTreeNode();
        if (objects.size() == 1)
        {
            node->object_ = objects[0];
            node->box_ = objects[0]->GetBoundingBox();
        }
        else if (objects.size() == 2) 
        {  
            BoundingTreeNode* left = this->Build(std::vector{objects[0]});
            BoundingTreeNode* right = this->Build(std::vector{objects[1]});
            node->left_ = left;
            node->right_ = right;
            node->box_ = BoundingBox::Union(node->left_->box_, node->right_->box_);
        }
        else
        {
            core::Vector3f pMax = core::Vector3f(std::numeric_limits<float>::lowest()), pMin = core::Vector3f(std::numeric_limits<float>::max());
            auto findCenter = [&](core::Vector3f v) -> void {
                pMax.x = std::max(pMax.x, v.x);
                pMax.y = std::max(pMax.y, v.y);
                pMax.z = std::max(pMax.z, v.z);
                pMin.x = std::min(pMin.x, v.x);
                pMin.y = std::min(pMin.y, v.y);
                pMin.z = std::min(pMin.z, v.z);
            };
            for (auto& object : objects)
                findCenter(object->GetBoundingBox().Centroid());
        
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
                std::sort(objects.begin(), objects.end(), [](Object* obj1, Object* obj2) -> bool {
                    return obj1->GetBoundingBox().Centroid().x < obj2->GetBoundingBox().Centroid().x;
                });
                break;
                
            case 1:
                std::sort(objects.begin(), objects.end(), [](Object* obj1, Object* obj2) -> bool {
                    return obj1->GetBoundingBox().Centroid().y < obj2->GetBoundingBox().Centroid().y;
                });
                break;

            case 2:
                std::sort(objects.begin(), objects.end(), [](Object* obj1, Object* obj2) -> bool {
                    return obj1->GetBoundingBox().Centroid().z < obj2->GetBoundingBox().Centroid().z;
                });
                break;

            default:
                break;
            }


            int halfSize = objects.size() / 2;
            std::vector<Object* > leftObjects(objects.begin(), objects.begin() + halfSize);
            std::vector<Object* > rightObjects(objects.begin() + halfSize, objects.end());

            BoundingTreeNode* left = this->Build(leftObjects);
            BoundingTreeNode* right = this->Build(rightObjects);

            node->left_ = left;
            node->right_ = right;

            node->box_ = BoundingBox::Union(node->left_->box_, node->right_->box_);
        
        }

    return node;
}

void SimpleBVH::BuildBVH(std::vector<Object* > objects)
{
    this->BVHHead_ = Build(objects);   
}


Intersection SimpleBVH::GetIntersectionObject(const core::Ray& ray)
{
    Intersection intersection;

    intersection = this->GetLastNodeIntersection(this->BVHHead_, ray);

    return intersection;
}


BoundingTreeNode* SimpleBVH::GetBoundingTree()
{
    return this->BVHHead_;
}

void SimpleBVH::Show() const
{    
    std::function<void(BoundingTreeNode* node)> show = [&](BoundingTreeNode* node) -> void {
        if (node == nullptr)
            return ;
        if (node->left_ != nullptr)
            show(node->left_);
        if (node->right_ != nullptr)
            show(node->right_);
        std::cout << "node max point :" << node->box_.pMax_ << "   min point :" << node->box_.pMin_ << "\n";
    };

    show(this->BVHHead_);
}

SimpleBVH::SimpleBVH() 
{ 
    BVHHead_ = nullptr;
}

SimpleBVH::~SimpleBVH()
{
    std::function<void(BoundingTreeNode* node)> func = [&](BoundingTreeNode* node) -> void{
        if (node == nullptr)
            return;
        if (node->left_ != nullptr)
            func(node->left_);
        if (node->right_ != nullptr)
            func(node->right_);
        delete node;
    };
}













    BoundingTreeFactory* BoundingTreeFactory::instance_ = new BoundingTreeFactory();

    BoundingTreeFactory::BoundingTreeFactory() { }

    BoundingTreeFactory& BoundingTreeFactory::Instance()
    {
        return *instance_;
    }

    BaseBoundingTree* BoundingTreeFactory::GetBoundingTree(BoundingTreeFactory::TypeOfBoundingTree t)
    {
        BaseBoundingTree* result = nullptr;
        switch (t)
        {
        case BVH:
            result = new SimpleBVH();   
            break;

        default:
            break;
        }
        return result;
    }


}