#include "Object.h"
#include "MATH.h"
#include "Scene.h"
#include "Tool.h"
#include <fstream>
#include <string>

#define EPSILON 0.0001

namespace OBJECT
{

    Vertex::Vertex()
    {
        this->setPosition(MATH::Vector3f(0.0));
        this->setNormal(MATH::Vector3f(0.0));
        this->setTexCoord(MATH::Vector2f(0.0));
    }

    Vertex::Vertex(MATH::Vector3f& p)
    {
        this->setPosition(p);
        this->setNormal(MATH::Vector3f(0.0));
        this->setTexCoord(MATH::Vector2f(0.0));
    }

    Vertex::Vertex(MATH::Vector3f& p, MATH::Vector3f& n) 
    {
        this->setPosition(p);
        this->setNormal(n);
        this->setTexCoord(MATH::Vector2f(0.0));
    }

    Vertex::Vertex(MATH::Vector3f& p, MATH::Vector3f& n, MATH::Vector2f& t) 
    {
        this->setPosition(p);
        this->setNormal(n);
        this->setTexCoord(t);
    }

    const MATH::Vector3f Vertex::getPosition() const
    {
        return MATH::Vector3f(this->data[0], this->data[1], this->data[2]);
    }

    const MATH::Vector3f Vertex::getNormal() const
    {
        return MATH::Vector3f(this->data[3], this->data[4], this->data[5]);
    }

    const MATH::Vector2f Vertex::getTexCoord() const
    {
        return MATH::Vector2f(this->data[6], this->data[7]);
    }

    void Vertex::setPosition(const MATH::Vector3f& p)
    {   
        for (int i = 0; i < 3; i++)
            this->data[i] = p[i];
    }

    void Vertex::setNormal(const MATH::Vector3f& n)
    {
        for (int i = 0; i < 3; i++)
            this->data[i + 3] = n[i];
    }

    void Vertex::setTexCoord(const MATH::Vector2f& t)
    {
        for (int i = 0; i < 2; i++)
            this->data[i + 6] = t[i];
    }

    std::ostream& operator << (std::ostream& os, const Vertex& v)
    {
        std::cout << (v.getPosition()) << "\t" << (v.getNormal()) << "\t" << (v.getTexCoord()) << "\n";
        return os;
    }


    Material::Material() { }

    Material::Material(const std::string& Name) : 
                        materialName(Name), self_illuminating(false), illuminate(0.0), Ka(0.0), Kd(0.0), Ks(0.0), Ke(0.0), Ns(0.0), Ni(0.0), d(0.0), illum(0) { }

    Material::Material(const std::string& Name, const MATH::Vector3f& ka, const MATH::Vector3f& kd, const MATH::Vector3f& ks, const MATH::Vector3f ke, float ns, float ni, float dd, int i) : 
                        materialName(Name), self_illuminating(false), illuminate(0.0), Ka(ka), Kd(kd), Ks(ks), Ke(ke), Ns(ns), Ni(ni), d(dd), illum(i) { }

    Material::Material(const Material& m) : 
                        materialName(m.materialName), self_illuminating(false), illuminate(0.0), Ka(m.Ka), Kd(m.Kd), Ks(m.Ks), Ke(m.Ke), Ns(m.Ns), Ni(m.Ni), d(m.d), illum(m.illum) { }

    Material::Material(const Material* m) : 
                        materialName(m->materialName), self_illuminating(false), illuminate(0.0), Ka(m->Ka), Kd(m->Kd), Ks(m->Ks), Ke(m->Ke), Ns(m->Ns), Ni(m->Ni), d(m->d), illum(m->illum) { }

    void Material::clear()
    {
        this->illum = 0;
        this->Ns = 0.0;
        this->Ni = 0.0;
        this->d = 0.0;

        this->Ka = MATH::Vector3f(0.0);
        this->Kd = MATH::Vector3f(0.0);
        this->Ks = MATH::Vector3f(0.0);
        this->Ke = MATH::Vector3f(0.0);

        this->materialName.clear();
        this->map_Ka.clear();
        this->map_Kd.clear();
        this->map_Ks.clear();
        this->map_Ns.clear();
        this->map_d.clear();
        this->map_bump.clear();
    }

    void Material::Show() const
    {
        std::cout << "material name :" << materialName << "\n"
                  << "illumination(illum) :" << illum << "\n"
                  << "soecykar exponent(Ns) :" << Ns << "\n"
                  << "optical density(Ni) :" << Ni << "\n"
                  << "dissolve(d) :" << d << "\n"
                  << "ambient color(Ka) :" << Ka << "\n"
                  << "diiffuse color(Kd) :" << Kd << "\n"
                  << "specular color(Ks) :" << Ks << "\n"
                  << "emissive ceficient(Ke) :" << Ke << "\n"
                  << "ambient texture map(map_Ka) :" << map_Ka << "\n"
                  << "diffuse texture map(map_Kd) :" << map_Kd << "\n"
                  << "specular texture map(map_Ks) :" << map_Ks << "\n"
                  << "specular hightlight map(map_Ns) :" << map_Ns << "\n"
                  << "alpha texture map(map_d) :" << map_d << "\n"
                  << "bump map(map_bump) :" << map_bump << std::endl;
                  std::cout << std::endl;
    } 
 

    Mesh::Mesh() : meshName("noname"), vertices({}), material(nullptr) { }

    Mesh::Mesh(const std::string& mN) : meshName(mN) { }

    void Mesh::setUniqueID(unsigned int ID)
    {
        this->id = ID;
    }

    const unsigned int Mesh::getUniqueID()
    {
        return this->id;
    }

    void Mesh::bingMesh(const std::string& mN)
    {
        meshName = mN;
    }

    void OBJ::initMeshMaterial()
    {
        for (int i = 0; i < this->meshes.size(); i++)
        {
            if (this->meshes[i].materialName == "noname")
                continue;
            else 
            {
                for (int j = 0; j < this->lodeMaterials.size(); j++)
                {
                    if (this->meshes[i].materialName == this->lodeMaterials[j].materialName)
                        this->meshes[i].material = &(this->lodeMaterials[j]);
                }
            }
        }
    }

    bool OBJ::materialFileLoder(const std::string& path)
    {
        if (path.substr(path.size() - 4, 4) != ".mtl")
        {
            std::cout << "wrong file input class OBJ function bool materialFileLoder(const std::string& path)\n";
            return false;
        }

        std::ifstream file(path);

        if (!file.is_open())
        {
            std::cout << "file cann't open file path " << path << "class OBJ function bool materialFileLoder(const std::string& path)\n";
            return false;
        }

        Material tempMaterial;
        std::string curLine = "";        
        bool firstOne = true;
        while (std::getline(file, curLine))
        {
            std::vector<std::string> input = Tool::stringTool::stringSplit(curLine, ' ');

            if (input.empty())
                continue;

            if (input[0] == "newmtl")
            {
                if (firstOne)
                {
                    firstOne = false;
                    tempMaterial.materialName = input.back();
                }
                else 
                {
                    this->lodeMaterials.emplace_back(tempMaterial);
                    tempMaterial.clear();
                    tempMaterial.materialName = input.back();
                }
            }
            if (input[0] == "Ka")
                for (int i = 0; i < 3; i++)
                    tempMaterial.Ka.data[i] = std::stof(input[i + 1]);

            if (input[0] == "Kd")
                for (int i = 0; i < 3; i++)
                    tempMaterial.Kd.data[i] = std::stof(input[i + 1]);

            if (input[0] == "Ks")
                for (int i = 0; i < 3; i++)
                    tempMaterial.Ks.data[i] = std::stof(input[i + 1]);

            if (input[0] == "Ns")
                tempMaterial.Ns = std::stof(input[1]);

            if (input[0] == "Ni")
                tempMaterial.Ni = std::stof(input[1]);

            if (input[0] == "d")
                tempMaterial.d = std::stof(input[1]);

            if (input[0] == "illum")
                tempMaterial.illum = std::atoi(input[1].c_str());

            if (input[0] == "map_Ka")
                tempMaterial.map_Ka = input[1];

            if (input[0] == "map_Kd")
                tempMaterial.map_Kd = input[1];

            if (input[0] == "map_Ks")
                tempMaterial.map_Ks = input[1];

            if (input[0] == "map_Ns")
                tempMaterial.map_Ns = input[1];

            if (input[0] == "map_d")
                tempMaterial.map_d = input[1];

            if (input[0] == "map_Bump" || input[0] == "map_bump" || input[0] == "bump")
                tempMaterial.map_bump = input[1];
        }

        //last one
        this->lodeMaterials.emplace_back(tempMaterial);

        return true;
    }

    void Mesh::Show() const
    {
        std::cout << "mesh name :" << meshName << "\n"
                  << "vertices size :" << vertices.size() << "\n";
        if(this->material != nullptr)
            this->material->Show();
        else 
            std::cout << "no material" << std::endl;
        std::cout << std::endl;
    }

    OBJ::OBJ() { }

    OBJ::OBJ(const std::string& path)
    {
        this->objFileLoder(path);
    }

    bool OBJ::objFileLoder(const std::string& path)
    {
        this->path = path;
        if (path.substr(path.size() - 4, 4) != ".obj")
        {
            std::cout << "wrong file input class OBJ function bool objFileLoder(const std::string& path)" << "\n"
                      << "path :" << path << std::endl;
            return false;
        }

        std::ifstream file(path);

        if (!file.is_open())
        {
            std::cout << "file cann't open file path " << path << "class OBJ function bool objFileLoder(const std::string& path)\n";
            return false;
        }

        std::vector<MATH::Vector3f>     positions;
        std::vector<MATH::Vector3f>     normals;
        std::vector<MATH::Vector2f>     texCoords;
        std::vector<Vertex>             verties;
        std::string meshName = "noname";
        std::string materialName = "noname";
        std::string curLine = "";
        Mesh tempMesh;
        tempMesh.meshName = "noname";
        tempMesh.materialName = "noname";

        while (std::getline(file, curLine))
        {
            std::vector<std::string> input = Tool::stringTool::stringSplit(curLine, ' ');
            if (input.empty())
                continue;

            if (input[0] == "o" || input[0] == "g")
            {
                if (verties.empty())
                    meshName = input[1];
                else 
                {
                    tempMesh.vertices = verties;
                    this->meshes.emplace_back(tempMesh);
                    tempMesh.meshName = "noname";
                    tempMesh.vertices.clear();
                    tempMesh.materialName = "noname"; 
                }
            }

            if (input[0] == "v")
            {                   
                MATH::Vector3f p(0.0);
                int i = 1;
                while (i < 4)
                {
                    p.data[i - 1] = std::stof(input[i]);
                    i++;
                }
                positions.emplace_back(p);
            }

            if (input[0] == "vn")
            {
                MATH::Vector3f vn(0.0);
                int i = 1;
                while (i < 4)
                {
                    vn.data[i - 1] = std::stof(input[i]);
                    i++;
                }
                normals.emplace_back(vn);
            }

            if (input[0] == "vt")
            {
                MATH::Vector2f vt(0.0);
                int i = 1;
                while (i < 3)
                {
                    vt.data[i - 1] = std::stof(input[i]);
                    i++;
                }
                texCoords.emplace_back(vt);
            }

            if (input[0] == "f")
            {
                Vertex ver;
                for (int i = 0; i < 3; i++)
                {
                    int type = 0; // check 1.v 2.v/vt 3.v/vt/vn 4.v//vn
                    std::vector<std::string> vertiesInformation = Tool::stringTool::stringSplit(input[i + 1], '/');
                    type = vertiesInformation.size();
                    if (type > 1 && vertiesInformation[1] == "")
                        type++;
                    int index = 0;
                    MATH::Vector2f tcoord = MATH::Vector2f(0.0);
                    switch (type)
                    {
                    case 1:
                        index = std::atoi(vertiesInformation[0].c_str()) - 1;
                        ver.setPosition(positions[index]);
                        break;

                    case 2:
                        index = std::atoi(vertiesInformation[0].c_str()) - 1;
                        ver.setPosition(positions[index]);

                        index = std::atoi(vertiesInformation[1].c_str()) - 1;
                        ver.setTexCoord(texCoords[index]);
                        break;

                    case 3:
                        index = std::atoi(vertiesInformation[0].c_str()) - 1;
                        ver.setPosition(positions[index]);

                        index = std::atoi(vertiesInformation[1].c_str()) - 1;
                        ver.setTexCoord(texCoords[index]);

                        index = std::atoi(vertiesInformation[2].c_str()) - 1;
                        ver.setNormal(normals[index]);
                        break;

                    case 4:
                        index = std::atoi(vertiesInformation[0].c_str()) - 1;
                        ver.setPosition(positions[index]);
                        
                        ver.setTexCoord(tcoord);

                        index = std::atoi(vertiesInformation[2].c_str()) - 1;
                        ver.setNormal(normals[index]);
                        break;
                    default:
                        break;
                    }
                    verties.emplace_back(ver);
                }
            }

            if (input[0] == "usemtl")
            {
                tempMesh.materialName = input.back();
            }

            if (input[0] ==  "mtllib")
            {
                this->materialFileLoder(input.back());
            }
        }

        //last mesh
        if (!verties.empty())
        {
            tempMesh.vertices = verties;
            this->meshes.emplace_back(tempMesh);
        }

        //seting material
        this->initMeshMaterial();

        return true;
    }

    void OBJ::resetMaterial(Material* m)
    {
        for (auto& it : this->meshes)
            it.material = m;
    }

    void OBJ::Show() const
    {
        std::cout << "object path :" << path << "\n"
                  << "mesh size :" << meshes.size() << "\n"
                  << "materials size :" << lodeMaterials.size() << "\n\n";
        for (int i = 0; i < meshes.size(); i++)
        {
            std::cout << i + 1 <<".. ";
            meshes[i].Show();
        }        

        for (int i = 0; i < lodeMaterials.size(); i++)
        {
            std::cout << i + 1 <<"... ";
            lodeMaterials[i].Show();
        }

    }

    Triangle::Triangle(const Vertex& vv0, const Vertex& vv1, const Vertex& vv2, Material* mm) : v0(vv0.getPosition()), v1(vv1.getPosition()), v2(vv2.getPosition()),
                                                                t0(vv0.getTexCoord()), t1(vv1.getTexCoord()), t2(vv2.getTexCoord()),
                                                                m(mm)
    {
        this->e1 = (v1 - v0);
        this->e2 = (v2 - v0);
        this->area = MATH::crossProduction(this->e1, this->e2).lenght() * 0.5;
        this->e1 = this->e1.normalize();
        this->e2 = this->e2.normalize();
        this->normal = MATH::crossProduction(this->e1, this->e2).normalize();
        this->boundingBox = BoundingBox(v0, v1, v2);

    }

    BoundingBox& Triangle::getBoundingBox()
    {
        return boundingBox;
    }

    Intersection Triangle::getIntersetion(const Ray& ray)
    {
        Intersection inter;

        if (MATH::dotProduction(ray.dir, this->normal) > 0)
            return inter;

        MATH::Vector3f E1 = this->v1 - this->v0;
        MATH::Vector3f E2 = this->v2 - this->v0;
        MATH::Vector3f S = ray.oir - this->v0;
        MATH::Vector3f S1 = MATH::crossProduction(ray.dir, E2);
        MATH::Vector3f S2 = MATH::crossProduction(S, E1);

        float div = MATH::dotProduction(S1, E1);
        if (div == 0.0f)
        {
            inter.happened = false;
            return inter;    
        }

        div = 1.0 / div;

        float t = MATH::dotProduction(S2, E2) * div;
        float u = MATH::dotProduction(S1, S) * div;
        float v = MATH::dotProduction(S2, ray.dir) * div;

        inter.happened = t >= 0.0f && (u >= 0.0f && u <= 1.0f) && (v >= 0.0f && v <= 1.0f) && ((1.0f - u - v) >= 0.0f && (1.0f - u - v <= 1.0f));
        if (!inter.happened)
            return inter;

        inter.distance = t;
        inter.coords = ray.trace(t);
        inter.tcoords = this->t0 * u + this->t1 * v + this->t2 * (1 - u - v);
        inter.normal = this->normal;
        inter.obj = this;
        inter.m = this->m;
        inter.id = this->id;

        return inter;

    } 



    float Triangle::getArea()
    {
        return this->area;    
    }

    const unsigned int Triangle::getUniqueID()
    {
        return this->id;
    }

    void Triangle::setUniqueID(unsigned int ID)
    {
        this->id = ID;
    }

    MATH::Vector3f Triangle::getInner() const
    {
        float a = (this->v1 - this->v2).lenght();
        float b = (this->v2 - this->v0).lenght();
        float c = (this->v0 - this->v1).lenght();
        float divSum = 1.0f / (a + b + c);

        MATH::Vector3f O = a * this->v0 + b * this->v1 + c * this->v2 * divSum;    

        return O;

    }

    void Triangle::Sample(Intersection& intersection, float& pdf)
    {
        float r1 = Tool::randomQueue::getInstance()->getRandomNum();
        float r2 = Tool::randomQueue::getInstance()->getRandomNum();
        
        r1 = std::sqrt(r1);

        MATH::Vector3f P = this->v0 * (1.0f - r1) + this->v1 * (r1 * (1.0f - r2)) + this->v2 * r1 * r2;
        intersection.coords = P;
        intersection.normal = this->normal;
        pdf = 1.0f / this->area;
    }

}

Intersection::Intersection()
{
    this->happened = false;
    this->coords = MATH::Vector3f(0.0f);
    this->tcoords = MATH::Vector2f(0.0f);
    this->normal = MATH::Vector3f(0.0f);
    this->distance = std::numeric_limits<double>::max();
    this->obj = nullptr;
    this->m = nullptr;
    this->id = 0;
}





