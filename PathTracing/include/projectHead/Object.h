#pragma once
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <string>
#include <fstream>
#include "BoundingBox.h"
#include "MATH.h"
#include "Ray.h"



struct Intersection;

namespace OBJECT
{
    class Material;

    class Vertex
    {
        public:

            float data[8]; 
            
            Vertex();
            Vertex(MATH::Vector3f& p);
            Vertex(MATH::Vector3f& p, MATH::Vector3f& n);
            Vertex(MATH::Vector3f& p, MATH::Vector3f& n, MATH::Vector2f& t);
            
            const MATH::Vector3f    getPosition() const;
            const MATH::Vector3f    getNormal() const;
            const MATH::Vector2f    getTexCoord() const;

            void                    setPosition(const MATH::Vector3f& p);
            void                    setNormal(const MATH::Vector3f& n);
            void                    setTexCoord(const MATH::Vector2f& t);

            friend std::ostream&    operator << (std::ostream& os, const Vertex& v);
    };

    class Texture
    {
        public:
            Texture(std::string path);
    };

    class Shader
    {
        public:
            struct ShaderInput
            {   
                MATH::Vector3f  wi;
                MATH::Vector3f  n;
                Material*       m;
            };
            
            struct ShaderOutput
            {
                MATH::Vector3f wo = MATH::Vector3f(0.0f);
                MATH::Vector3f BRDF = MATH::Vector3f(0.0f);
                float pdf = 0;    
                ShaderOutput() = default;
            };
            std::function<MATH::Vector3f(const MATH::Vector3f& wi, const MATH::Vector3f& n)>                            sample;
            std::function<float(const MATH::Vector3f& wi, const MATH::Vector3f& wo, const MATH::Vector3f& n)>           pdf;
            std::function<MATH::Vector3f(const MATH::Vector3f& wi, const MATH::Vector3f& wo, const MATH::Vector3f& n, const Material* m)>  getBRDF;
            std::function<ShaderOutput(ShaderInput&)>                                                                   shader;
    };


    class Material
    {
        public:
            bool            self_illuminating;
            int             illum;//Illumination
            
            float           Ns;//Specular Exponent
            float           Ni;//Optical Density
            float           d;//Dissolve
            
            MATH::Vector3f  illuminate;//illuminate Color
            MATH::Vector3f  Ka;//Ambient Color
            MATH::Vector3f  Kd;//Diffuse Color
            MATH::Vector3f  Ks;//Specular Color
            MATH::Vector3f  Ke;//emissive coeficient
            
            std::string     materialName;//name
            std::string     map_Ka;//Ambient Texture Map
            std::string     map_Kd;//Diffuse Texture Map
            std::string     map_Ks;//Specular Texture Map
            std::string     map_Ns;//Specular HightLight map
            std::string     map_d;//Alpha Texture Map;
            std::string     map_bump;//Bump Map;

            Shader*          shader;

            Material();
            Material(const std::string& Name);
            Material(const std::string& Name, const MATH::Vector3f& ka, const MATH::Vector3f& kd, const MATH::Vector3f& ks, const MATH::Vector3f ke, float ns, float ni, float dd, int i);
            Material(const Material& m);
            Material(const Material* m);

            void clear();

            void Show() const;
    };

    class Mesh
    {
        private:
            unsigned int            id;
        public:
            std::string             meshName;
            std::string             materialName;
            std::vector<Vertex>     vertices;
            Material*               material;

            Mesh();
            Mesh(const std::string& mN);
            
            void                setUniqueID(unsigned int ID);
            const unsigned int  getUniqueID();
            void                bingMesh(const std::string& mN);
            void                Show() const;
    };

    class Object
    {
        public:
            virtual BoundingBox&        getBoundingBox() = 0;
            virtual Intersection        getIntersetion(const Ray& ray) = 0;           
            virtual float               getArea() = 0;
            virtual const unsigned int  getUniqueID() = 0; 
    };



    struct Transform
    {
        MATH::Vector3f transform;
        MATH::Vector3f rotate;
        MATH::Vector3f scale;
    };
    
    class OBJ 
    {  
        private:
            void initMeshMaterial();
            bool materialFileLoder(const std::string& path);
            void rotate(const MATH::Vector3f& rota);
            void transform(const MATH::Vector3f& trans);
            void scale(const MATH::Vector3f& scale);
        public:
            std::string             path;
            Transform               meshTransform;
            std::vector<Mesh>       meshes;
            std::vector<Material>   lodeMaterials;
            OBJ();
            OBJ(const std::string& path);
            bool objFileLoder(const std::string& path);
            void resetMaterial(Material* m);
            void setTrasform(const Transform& transform);
            void Show() const;
    };

    class Triangle : public Object
    {
        private:
            unsigned int    id;
        public:
            MATH::Vector3f  v0, v1, v2;
            MATH::Vector2f  t0, t1, t2;
            MATH::Vector3f  e1, e2;
            MATH::Vector3f  normal;
            BoundingBox     boundingBox;
            Material*       m;
            float           area;

            Triangle(const Vertex& vv0, const Vertex& vv1, const Vertex& vv2, Material* mm);
            BoundingBox&        getBoundingBox() override;
            Intersection        getIntersetion(const Ray& ray) override;
            float               getArea() override;
            const unsigned int  getUniqueID() override;
            void                setUniqueID(unsigned int ID);
            MATH::Vector3f      getInner() const;
            void                Sample(Intersection& intersection, float& pdf);
    };

}


struct Intersection
{
    bool                happened;
    unsigned int        id;
    MATH::Vector3f      coords;
    MATH::Vector2f      tcoords;
    MATH::Vector3f      normal;
    double              distance;
    OBJECT::Object*     obj;
    OBJECT::Material*   m;

    Intersection();

};




