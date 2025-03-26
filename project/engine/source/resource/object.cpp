#include <fstream>
#include <string>
#include "engine/resource/object.h"
#include "engine/resource/resource_utils.h"


#define EPSILON 0.0001

namespace resource
{

    Vertex::Vertex()
    {
        this->SetPosition(core::Vector3f(0.0));
        this->SetNormal(core::Vector3f(0.0));
        this->SetTexCoord(core::Vector2f(0.0));
    }

    Vertex::Vertex(core::Vector3f& p)
    {
        this->SetPosition(p);
        this->SetNormal(core::Vector3f(0.0));
        this->SetTexCoord(core::Vector2f(0.0));
    }

    Vertex::Vertex(core::Vector3f& p, core::Vector3f& n) 
    {
        this->SetPosition(p);
        this->SetNormal(n);
        this->SetTexCoord(core::Vector2f(0.0));
    }

    Vertex::Vertex(core::Vector3f& p, core::Vector3f& n, core::Vector2f& t) 
    {
        this->SetPosition(p);
        this->SetNormal(n);
        this->SetTexCoord(t);
    }

    const core::Vector3f Vertex::GetPosition() const
    {
        return core::Vector3f(this->data[0], this->data[1], this->data[2]);
    }

    const core::Vector3f Vertex::GetNormal() const
    {
        return core::Vector3f(this->data[3], this->data[4], this->data[5]);
    }

    const core::Vector2f Vertex::GetTexCoord() const
    {
        return core::Vector2f(this->data[6], this->data[7]);
    }

    void Vertex::SetPosition(const core::Vector3f& p)
    {   
        for (int i = 0; i < 3; i++)
            this->data[i] = p[i];
    }

    void Vertex::SetNormal(const core::Vector3f& n)
    {
        for (int i = 0; i < 3; i++)
            this->data[i + 3] = n[i];
    }

    void Vertex::SetTexCoord(const core::Vector2f& t)
    {
        for (int i = 0; i < 2; i++)
            this->data[i + 6] = t[i];
    }

    std::ostream& operator << (std::ostream& os, const Vertex& v)
    {
        std::cout << (v.GetPosition()) << "\t" << (v.GetNormal()) << "\t" << (v.GetTexCoord()) << "\n";
        return os;
    }


    Material::Material() { }

    Material::Material(const std::string& Name) : 
                        materialName_(Name), self_illuminating_(false), illuminate_(0.0), Ka_(0.0), Kd_(0.0), Ks_(0.0), Ke_(0.0), Ns_(0.0), Ni_(0.0), d_(0.0), illum_(0) { }

    Material::Material(const std::string& Name, const core::Vector3f& ka, const core::Vector3f& kd, const core::Vector3f& ks, const core::Vector3f ke, float ns, float ni, float dd, int i) : 
                        materialName_(Name), self_illuminating_(false), illuminate_(0.0), Ka_(ka), Kd_(kd), Ks_(ks), Ke_(ke), Ns_(ns), Ni_(ni), d_(dd), illum_(i) { }

    Material::Material(const Material& m) : 
                        materialName_(m.materialName_), self_illuminating_(m.self_illuminating_), illuminate_(m.illuminate_), Ka_(m.Ka_), Kd_(m.Kd_), Ks_(m.Ks_), Ke_(m.Ke_), Ns_(m.Ns_), Ni_(m.Ni_), d_(m.d_), illum_(m.illum_) { }

    Material::Material(const Material* m) : 
                        materialName_(m->materialName_), self_illuminating_(m->self_illuminating_), illuminate_(m->illuminate_), Ka_(m->Ka_), Kd_(m->Kd_), Ks_(m->Ks_), Ke_(m->Ke_), Ns_(m->Ns_), Ni_(m->Ni_), d_(m->d_), illum_(m->illum_) { }

    void Material::Clear()
    {
        this->illum_ = 0;
        this->Ns_ = 0.0;
        this->Ni_ = 0.0;
        this->d_ = 0.0;

        this->Ka_ = core::Vector3f(0.0);
        this->Kd_ = core::Vector3f(0.0);
        this->Ks_ = core::Vector3f(0.0);
        this->Ke_ = core::Vector3f(0.0);

        this->materialName_.clear();
        this->map_Ka_.clear();
        this->map_Kd_.clear();
        this->map_Ks_.clear();
        this->map_Ns_.clear();
        this->map_d_.clear();
        this->map_bump_.clear();
    }

    void Material::Show() const
    {
        std::cout << "material name :" << materialName_ << "\n"
                  << "illumination(illum) :" << illum_ << "\n"
                  << "soecykar exponent(Ns) :" << Ns_ << "\n"
                  << "optical density(Ni) :" << Ni_ << "\n"
                  << "dissolve(d) :" << d_ << "\n"
                  << "ambient color(Ka) :" << Ka_ << "\n"
                  << "diiffuse color(Kd) :" << Kd_ << "\n"
                  << "specular color(Ks) :" << Ks_ << "\n"
                  << "emissive ceficient(Ke) :" << Ke_ << "\n"
                  << "ambient texture map(map_Ka) :" << map_Ka_ << "\n"
                  << "diffuse texture map(map_Kd) :" << map_Kd_ << "\n"
                  << "specular texture map(map_Ks) :" << map_Ks_ << "\n"
                  << "specular hightlight map(map_Ns) :" << map_Ns_ << "\n"
                  << "alpha texture map(map_d) :" << map_d_ << "\n"
                  << "bump map(map_bump) :" << map_bump_ << std::endl;
                  std::cout << std::endl;
    } 
 

    Mesh::Mesh() : meshName_("noname"), vertices_({}), material_(nullptr) { }

    Mesh::Mesh(const std::string& mN) : meshName_(mN) { }

    void Mesh::SetUniqueID(unsigned int ID)
    {
        this->id_ = ID;
    }

    const unsigned int Mesh::GetUniqueID()
    {
        return this->id_;
    }

    void Mesh::BingMesh(const std::string& mN)
    {
        meshName_ = mN;
    }

    void Obj::InitMeshMaterial()
    {
        for (int i = 0; i < this->meshes_.size(); i++)
        {
            if (this->meshes_[i].materialName_ == "noname")
                continue;
            else 
            {
                for (int j = 0; j < this->lodeMaterials_.size(); j++)
                {
                    if (this->meshes_[i].materialName_ == this->lodeMaterials_[j].materialName_)
                        this->meshes_[i].material_ = &(this->lodeMaterials_[j]);
                }
            }
        }
    }

    bool Obj::MaterialFileLoder(const std::string& path)
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
            std::vector<std::string> input = ResourceUtils::StringSplit(curLine, ' ');

            if (input.empty())
                continue;

            if (input[0] == "newmtl")
            {
                if (firstOne)
                {
                    firstOne = false;
                    tempMaterial.materialName_ = input.back();
                }
                else 
                {
                    this->lodeMaterials_.emplace_back(tempMaterial);
                    tempMaterial.Clear();
                    tempMaterial.materialName_ = input.back();
                }
            }
            if (input[0] == "Ka")
                for (int i = 0; i < 3; i++)
                    tempMaterial.Ka_.data[i] = std::stof(input[i + 1]);

            if (input[0] == "Kd")
                for (int i = 0; i < 3; i++)
                    tempMaterial.Kd_.data[i] = std::stof(input[i + 1]);

            if (input[0] == "Ks")
                for (int i = 0; i < 3; i++)
                    tempMaterial.Ks_.data[i] = std::stof(input[i + 1]);

            if (input[0] == "Ns")
                tempMaterial.Ns_ = std::stof(input[1]);

            if (input[0] == "Ni")
                tempMaterial.Ni_ = std::stof(input[1]);

            if (input[0] == "d")
                tempMaterial.d_ = std::stof(input[1]);

            if (input[0] == "illum")
                tempMaterial.illum_ = std::atoi(input[1].c_str());

            if (input[0] == "map_Ka")
                tempMaterial.map_Ka_ = input[1];

            if (input[0] == "map_Kd")
                tempMaterial.map_Kd_ = input[1];

            if (input[0] == "map_Ks")
                tempMaterial.map_Ks_ = input[1];

            if (input[0] == "map_Ns")
                tempMaterial.map_Ns_ = input[1];

            if (input[0] == "map_d")
                tempMaterial.map_d_ = input[1];

            if (input[0] == "map_Bump" || input[0] == "map_bump" || input[0] == "bump")
                tempMaterial.map_bump_ = input[1];
        }

        //last one
        this->lodeMaterials_.emplace_back(tempMaterial);

        return true;
    }

    void Mesh::Show() const
    {
        std::cout << "mesh name :" << meshName_ << "\n"
                  << "vertices size :" << vertices_.size() << "\n";
        if(this->material_ != nullptr)
            this->material_->Show();
        else 
            std::cout << "no material" << std::endl;
        std::cout << std::endl;
    }

    Obj::Obj() { }

    Obj::Obj(const std::string& path)
    {
        this->ObjFileLoder(path);
    }

    bool Obj::ObjFileLoder(const std::string& path)
    {
        this->path_ = path;
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

        std::vector<core::Vector3f>     positions;
        std::vector<core::Vector3f>     normals;
        std::vector<core::Vector2f>     texCoords;
        std::vector<Vertex>             verties;
        std::string meshName = "noname";
        std::string materialName = "noname";
        std::string curLine = "";
        Mesh tempMesh;
        tempMesh.meshName_ = "noname";
        tempMesh.materialName_ = "noname";

        while (std::getline(file, curLine))
        {
            std::vector<std::string> input = ResourceUtils::StringSplit(curLine, ' ');
            if (input.empty())
                continue;

            if (input[0] == "o" || input[0] == "g")
            {
                if (verties.empty())
                    meshName = input[1];
                else 
                {
                    tempMesh.vertices_ = verties;
                    this->meshes_.emplace_back(tempMesh);
                    tempMesh.meshName_ = "noname";
                    tempMesh.vertices_.clear();
                    tempMesh.materialName_ = "noname"; 
                }
            }

            if (input[0] == "v")
            {                   
                core::Vector3f p(0.0);
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
                core::Vector3f vn(0.0);
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
                core::Vector2f vt(0.0);
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
                    std::vector<std::string> vertiesInformation = ResourceUtils::StringSplit(input[i + 1], '/');
                    type = vertiesInformation.size();
                    if (type > 1 && vertiesInformation[1] == "")
                        type++;
                    int index = 0;
                    core::Vector2f tcoord = core::Vector2f(0.0);
                    switch (type)
                    {
                    case 1:
                        index = std::atoi(vertiesInformation[0].c_str()) - 1;
                        ver.SetPosition(positions[index]);
                        break;

                    case 2:
                        index = std::atoi(vertiesInformation[0].c_str()) - 1;
                        ver.SetPosition(positions[index]);

                        index = std::atoi(vertiesInformation[1].c_str()) - 1;
                        ver.SetTexCoord(texCoords[index]);
                        break;

                    case 3:
                        index = std::atoi(vertiesInformation[0].c_str()) - 1;
                        ver.SetPosition(positions[index]);

                        index = std::atoi(vertiesInformation[1].c_str()) - 1;
                        ver.SetTexCoord(texCoords[index]);

                        index = std::atoi(vertiesInformation[2].c_str()) - 1;
                        ver.SetNormal(normals[index]);
                        break;

                    case 4:
                        index = std::atoi(vertiesInformation[0].c_str()) - 1;
                        ver.SetPosition(positions[index]);
                        
                        ver.SetTexCoord(tcoord);

                        index = std::atoi(vertiesInformation[2].c_str()) - 1;
                        ver.SetNormal(normals[index]);
                        break;
                    default:
                        break;
                    }
                    verties.emplace_back(ver);
                }
            }

            if (input[0] == "usemtl")
            {
                tempMesh.materialName_ = input.back();
            }

            if (input[0] ==  "mtllib")
            {
                this->MaterialFileLoder(input.back());
            }
        }

        //last mesh
        if (!verties.empty())
        {
            tempMesh.vertices_ = verties;
            this->meshes_.emplace_back(tempMesh);
        }

        //seting material
        this->InitMeshMaterial();

        return true;
    }

    void Obj::ResetMaterial(Material* m)
    {
        for (auto& it : this->meshes_)
            it.material_ = m;
    }

    void Obj::Show() const
    {
        std::cout << "object path :" << path_ << "\n"
                  << "mesh size :" << meshes_.size() << "\n"
                  << "materials size :" << lodeMaterials_.size() << "\n\n";
        for (int i = 0; i < meshes_.size(); i++)
        {
            std::cout << i + 1 <<".. ";
            meshes_[i].Show();
        }        

        for (int i = 0; i < lodeMaterials_.size(); i++)
        {
            std::cout << i + 1 <<"... ";
            lodeMaterials_[i].Show();
        }

    }

}



