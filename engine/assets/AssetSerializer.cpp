#include "AssetSerializer.h"

#include "core/Log.h"
#include "renderer/Texture.h"
#include "renderer/Shader.h"
#include "renderer/Material.h"
#include "renderer/Mesh.h"
#include "project/Project.h"

#include <fstream>

//Texture
void nimo::AssetSerializer<nimo::Texture>::Serialize(const AssetMetadata& metadata, const std::shared_ptr<nimo::Texture>& asset){}
std::shared_ptr<nimo::Texture> nimo::AssetSerializer<nimo::Texture>::Deserialize(const nimo::AssetMetadata& metadata)
{ 
    std::filesystem::path p = ".";
    if(nimo::Project::GetActiveProject())
        p = nimo::Project::GetActiveProject()->GetAssetsFolderPath();
    return std::make_shared<Texture>((p/metadata.filepath).string());
}

//Shader
void nimo::AssetSerializer<nimo::Shader>::Serialize(const AssetMetadata& metadata, const std::shared_ptr<nimo::Shader>& asset)
{
    std::filesystem::path p = ".";
    if(nimo::Project::GetActiveProject())
        p = nimo::Project::GetActiveProject()->GetAssetsFolderPath();
    std::ofstream ofs(p/metadata.filepath);

    ofs << "@VERTEX";
    ofs << asset->GetVertexCode();
    ofs << "@VERTEXEND";
    ofs << "@FRAGMENT";
    ofs << asset->GetFragmentCode();
    ofs << "@FRAGMENTEND";
}
std::shared_ptr<nimo::Shader> nimo::AssetSerializer<nimo::Shader>::Deserialize(const nimo::AssetMetadata& metadata)
{     
    std::filesystem::path p = ".";
    if(nimo::Project::GetActiveProject())
        p = nimo::Project::GetActiveProject()->GetAssetsFolderPath();
    return std::make_shared<Shader>((p/metadata.filepath).string());
}

//Material
void nimo::AssetSerializer<nimo::Material>::Serialize(const AssetMetadata& metadata, const std::shared_ptr<nimo::Material>& asset)
{
    nlohmann::json j;
    j["shader"] = asset->shader->id.str();
    nlohmann::json jprops = nlohmann::json::array();
    for(auto p : asset->properties)
    {
        switch(p->type)
        {
            case ShaderUniformDataType::Float2:
            {
                auto pv2 = static_cast<MaterialProperty<glm::vec2>*>(p);
                jprops.push_back({
                    {"name", pv2->name},
                    {"type", "vector2"},
                    {"value", {((glm::vec2*)pv2->GetDataPtr())->x, ((glm::vec2*)pv2->GetDataPtr())->y}}
                });
            }
            break;
            case ShaderUniformDataType::Float4:
            {
                auto pv2 = static_cast<MaterialProperty<glm::vec4>*>(p);
                jprops.push_back({
                    {"name", pv2->name},
                    {"type", "vector4"},
                    {"value", {((glm::vec4*)pv2->GetDataPtr())->x, ((glm::vec4*)pv2->GetDataPtr())->y, ((glm::vec4*)pv2->GetDataPtr())->z, ((glm::vec4*)pv2->GetDataPtr())->w}}
                });
            }
            break;
            case ShaderUniformDataType::Sampler2D:
            {
                auto pt = static_cast<MaterialPropertyTexture*>(p);
                jprops.push_back({
                    {"name", pt->name},
                    {"type", "texture"},
                    {"value", pt->GetValue()},
                    {"asset", pt->GetTexture()->id.str()}
                });
            }
            break;
        }
    }
    j["properties"] = jprops;
    std::filesystem::path p = ".";
    if(nimo::Project::GetActiveProject())
        p = nimo::Project::GetActiveProject()->GetAssetsFolderPath();
    std::ofstream ofs(p/metadata.filepath);
    ofs << j.dump(4);
}
std::shared_ptr<nimo::Material> nimo::AssetSerializer<nimo::Material>::Deserialize(const nimo::AssetMetadata& metadata)
{
    std::filesystem::path p = ".";
    if(nimo::Project::GetActiveProject())
        p = nimo::Project::GetActiveProject()->GetAssetsFolderPath();
    return std::make_shared<Material>((p/metadata.filepath).string());
}

//Mesh
void nimo::AssetSerializer<nimo::Mesh>::Serialize(const AssetMetadata& metadata, const std::shared_ptr<nimo::Mesh>& asset){}
std::shared_ptr<nimo::Mesh> nimo::AssetSerializer<nimo::Mesh>::Deserialize(const nimo::AssetMetadata& metadata)
{
    std::filesystem::path p = ".";
    if(nimo::Project::GetActiveProject())
        p = nimo::Project::GetActiveProject()->GetAssetsFolderPath();
    return std::make_shared<Mesh>((p/metadata.filepath).string());
}
