#pragma once
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <string>
#include <fstream>
#include "engine/core/vector_defination.h"
#include "engine/core/ray.h"


namespace resource
{
    class Material;

    class Vertex
    {
        public:

            float data[8]; 
            
            Vertex();
            Vertex(core::Vector3f& p);
            Vertex(core::Vector3f& p, core::Vector3f& n);
            Vertex(core::Vector3f& p, core::Vector3f& n, core::Vector2f& t);
            
            const core::Vector3f    GetPosition() const;
            const core::Vector3f    GetNormal() const;
            const core::Vector2f    GetTexCoord() const;

            void                    SetPosition(const core::Vector3f& p);
            void                    SetNormal(const core::Vector3f& n);
            void                    SetTexCoord(const core::Vector2f& t);

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
                core::Vector3f  wi;
                core::Vector3f  n;
                Material*       m;
            };
            
            struct ShaderOutput
            {
                core::Vector3f wo = core::Vector3f(0.0f);
                core::Vector3f BRDF = core::Vector3f(0.0f);
                float pdf = 0;    
                ShaderOutput() = default;
            };
            std::function<core::Vector3f(const core::Vector3f& wi, const core::Vector3f& n)>                            Sample;
            std::function<float(const core::Vector3f& wi, const core::Vector3f& wo, const core::Vector3f& n)>           Pdf;
            std::function<core::Vector3f(const core::Vector3f& wi, const core::Vector3f& wo, const core::Vector3f& n, const Material* m)>  GetBRDF;
            std::function<ShaderOutput(ShaderInput&)>                                                                   shader_;
    };


    class Material
    {
        public:
            bool            self_illuminating_;
            int             illum_;//Illumination
            
            float           Ns_;//Specular Exponent
            float           Ni_;//Optical Density
            float           d_;//Dissolve
            
            core::Vector3f  illuminate_;//illuminate Color
            core::Vector3f  Ka_;//Ambient Color
            core::Vector3f  Kd_;//Diffuse Color
            core::Vector3f  Ks_;//Specular Color
            core::Vector3f  Ke_;//emissive coeficient
            
            std::string     materialName_;//name
            std::string     map_Ka_;//Ambient Texture Map
            std::string     map_Kd_;//Diffuse Texture Map
            std::string     map_Ks_;//Specular Texture Map
            std::string     map_Ns_;//Specular HightLight map
            std::string     map_d_;//Alpha Texture Map;
            std::string     map_bump_;//Bump Map;

            Shader*          shader_;

            Material();
            Material(const std::string& Name);
            Material(const std::string& Name, const core::Vector3f& ka, const core::Vector3f& kd, const core::Vector3f& ks, const core::Vector3f ke, float ns, float ni, float dd, int i);
            Material(const Material& m);
            Material(const Material* m);

            void Clear();

            void Show() const;
    };

    class Mesh
    {
        private:
            unsigned int            id_;
        public:
            std::string             meshName_;
            std::string             materialName_;
            std::vector<Vertex>     vertices_;
            Material*               material_;

            Mesh();
            Mesh(const std::string& mN);
            
            void                SetUniqueID(unsigned int ID);
            const unsigned int  GetUniqueID();
            void                BingMesh(const std::string& mN);
            void                Show() const;
    };

    struct Transform
    {
        core::Vector3f transform_;
        core::Vector3f rotate_;
        core::Vector3f scale_;
    };
    
    class Obj
    {  
        private:
            void InitMeshMaterial();
            bool MaterialFileLoder(const std::string& path);
            void Rotate(const core::Vector3f& rota);
            void Transform(const core::Vector3f& trans);
            void Scale(const core::Vector3f& scale);
        public:
            std::string             path_;
            std::vector<Mesh>       meshes_;
            std::vector<Material>   lodeMaterials_;
            Obj();
            Obj(const std::string& path);
            bool ObjFileLoder(const std::string& path);
            void ResetMaterial(Material* m);
            void Show() const;
    };

    

}






