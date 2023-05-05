#include "SceneSerializer.h"
#include "json.hpp"
#include "Components.h"
#include <fstream>
#include "project/Project.h"
#include "scripting/ScriptManager.h"
#include "SceneManager.h"
#include "Prefab.h"
#include "renderer/Texture.h"
#include "renderer/Mesh.h"
#include "renderer/Material.h"
#include "renderer/EnvironmentMap.h"

void nimo::AssetSerializer<nimo::Scene>::Serialize(const AssetMetadata& metadata, const std::shared_ptr<nimo::Scene>& asset)
{
    nlohmann::ordered_json j;
    nlohmann::ordered_json jentities = nlohmann::ordered_json::array();
    j["Name"] = asset->name;
    j["GUID"] = asset->id.str();

    asset->m_registry.each([&](entt::entity id)
    {
        Entity e(id, asset->m_registry);
        if(!e.GetComponent<FamilyComponent>().Parent.valid())
        {
            nlohmann::ordered_json jentity = SerializeEntity(asset, e);
            jentities.push_back(jentity);
        }
    });
    j["Entities"] = jentities;

    std::filesystem::path p = ".";
    if(nimo::Project::GetActiveProject())
        p = nimo::Project::GetActiveProject()->GetAssetsFolderPath();
    std::ofstream ofs(p/metadata.filepath);
    ofs << j;   
}
std::shared_ptr<nimo::Scene> nimo::AssetSerializer<nimo::Scene>::Deserialize(const nimo::AssetMetadata& metadata)
{
    std::filesystem::path p = ".";
    if(nimo::Project::GetActiveProject())
        p = nimo::Project::GetActiveProject()->GetAssetsFolderPath();
    std::ifstream ifs(p/metadata.filepath);
    nlohmann::ordered_json j;
    j << ifs;

    std::shared_ptr<Scene> scene = SceneManager::CreateScene(metadata.filepath.stem().string());

    scene->id = GUID(std::string(j["GUID"]));
    for(auto entity : j["Entities"])
    {
        DeserializeEntity(scene, entity, GUID(std::string(entity["GUID"])));
    }
    return scene;
}

nlohmann::ordered_json nimo::AssetSerializer<nimo::Scene>::SerializeEntity(const std::shared_ptr<nimo::Scene>& scene, const Entity& e)
{
    nlohmann::ordered_json jentity;
    if(e.HasComponent<IDComponent>())
    {
        jentity["GUID"] = e.GetComponent<IDComponent>().Id.str();
    }
    if(e.HasComponent<LabelComponent>())
    {
        jentity["Label"] = e.GetComponent<LabelComponent>().Label;
    }
    if(e.HasComponent<ActiveComponent>())
    {
        jentity["Active"] = e.GetComponent<ActiveComponent>().active;
    }
    if(e.HasComponent<FamilyComponent>())
    {
        const FamilyComponent& f = e.GetComponent<FamilyComponent>();
        nlohmann::ordered_json jfamily{
            {"Parent", f.Parent.str()}
        };
        nlohmann::ordered_json jchildren = nlohmann::ordered_json::array();
        for(auto child : f.Children)
        {
            jchildren.push_back(SerializeEntity(scene, scene->GetEntity(child)));
        }
        jfamily["Children"] = jchildren;
        jentity["Family"] = jfamily;
    }
    if(e.HasComponent<TransformComponent>())
    {
        TransformComponent t = e.GetComponent<TransformComponent>();
        nlohmann::ordered_json jtransform{
            {"Translation", {t.Translation.x, t.Translation.y, t.Translation.z}},
            {"Rotation", {t.Rotation.x, t.Rotation.y, t.Rotation.z}},
            {"Scale", {t.Scale.x, t.Scale.y, t.Scale.z}}
        };
        jentity["Transform"] = jtransform;
    }
    if(e.HasComponent<CameraComponent>())
    {
        CameraComponent c = e.GetComponent<CameraComponent>();
        nlohmann::ordered_json jcamera{
            {"FOV", c.FOV},
            {"ClippingPlanes", {{"Near", c.ClippingPlanes.Near},{"Far", c.ClippingPlanes.Far}}}
        };
        c.Projection == CameraComponent::Projection::Perspective ? jcamera["Projection"] = "Perspective" : jcamera["Projection"] = "Orthographic";
        jentity["Camera"] = jcamera;
    }
    if(e.HasComponent<MeshComponent>())
    {
        MeshComponent m = e.GetComponent<MeshComponent>();
        nlohmann::ordered_json jmesh{
            {"Source", m.source->id.str()}
        };
        jentity["Mesh"] = jmesh;
    }
    if(e.HasComponent<MeshRendererComponent>())
    {
        MeshRendererComponent m = e.GetComponent<MeshRendererComponent>();
        nlohmann::ordered_json jmesh{
            {"Material", m.material->id.str()}
        };
        jentity["MeshRenderer"] = jmesh;
    }
    if(e.HasComponent<PointLightComponent>())
    {
        PointLightComponent l = e.GetComponent<PointLightComponent>();
        nlohmann::ordered_json jpointlight{
            {"Color",{l.Color.r,l.Color.g,l.Color.b}},
            {"Intensity",l.Intensity}
        };
        jentity["PointLight"] = jpointlight;
    }
    if(e.HasComponent<ScriptComponent>())
    {
        const ScriptComponent& l = e.GetComponent<ScriptComponent>();
        auto instances = nlohmann::json::array();
        for(auto& instance : l.instances)
        {
            auto instanceJson = nlohmann::json({{"Source", instance.script->id.str()}});
            auto fields = nlohmann::json::array();
            for(auto& field : instance.fields)
            {
                switch (field.second->GetType())
                {
                case nimo::ScriptFieldType::Number:
                    fields.push_back({{"Name",field.first}, {"Value",std::static_pointer_cast<nimo::ScriptFieldNumber>(field.second)->value}, {"Type","Number"}});
                    break;
                case nimo::ScriptFieldType::Boolean:
                    fields.push_back({{"Name",field.first}, {"Value",std::static_pointer_cast<nimo::ScriptFieldBool>(field.second)->value}, {"Type","Boolean"}});
                    break;
                case nimo::ScriptFieldType::String:
                    fields.push_back({{"Name",field.first}, {"Value",std::static_pointer_cast<nimo::ScriptFieldString>(field.second)->value}, {"Type","String"}});
                    break;
                case nimo::ScriptFieldType::Asset:
                    fields.push_back({
                        {"Name",field.first},
                        {"Value",std::static_pointer_cast<nimo::ScriptFieldAsset>(field.second)->value ? std::static_pointer_cast<nimo::ScriptFieldAsset>(field.second)->value->id.str() : ""},
                        {"Type","Asset"},
                        {"AssetType",AssetTypeToString(std::static_pointer_cast<nimo::ScriptFieldAsset>(field.second)->type)}
                    });
                    break;
                default:
                    break;
                }
            }
            instanceJson.push_back({"Fields", fields});
            instances.push_back(instanceJson);
        }
        jentity["Scripts"] = instances;
    }
    return jentity;
}
nimo::GUID nimo::AssetSerializer<nimo::Scene>::DeserializeEntity(const std::shared_ptr<nimo::Scene>& scene, const nlohmann::ordered_json& source, GUID desiredId)
{
    GUID id;
    if(desiredId.valid())
        id = desiredId;
    else
        id = GUID::Create();
    nimo::Entity createdEntity = scene->CreateEntityWithID(id);
    for (auto field : source.items())
    {
        if(field.key() == "Label")
        {
            createdEntity.GetComponent<LabelComponent>().Label = field.value();
        }
        if(field.key() == "Active")
        {
            createdEntity.GetComponent<ActiveComponent>().active = field.value();
        }
        if(field.key() == "Transform")
        {
            auto Translation = glm::vec3((float)field.value()["Translation"][0], (float)field.value()["Translation"][1], (float)field.value()["Translation"][2]);
            auto Rotation = glm::vec3((float)field.value()["Rotation"][0], (float)field.value()["Rotation"][1], (float)field.value()["Rotation"][2]);
            auto Scale = glm::vec3((float)field.value()["Scale"][0], (float)field.value()["Scale"][1], (float)field.value()["Scale"][2]);
            TransformComponent& t = createdEntity.GetComponent<TransformComponent>();
            t.Translation = Translation;
            t.Rotation = Rotation;
            t.Scale = Scale;
        }
        if(field.key() == "Family")
        {
            FamilyComponent& f = createdEntity.GetComponent<FamilyComponent>();
            f.Parent = GUID(std::string(field.value()["Parent"]));
            for(auto child : field.value()["Children"])
            {
                f.Children.push_back(DeserializeEntity(scene, child, GUID(std::string(child["GUID"]))));
            }
        }
        if(field.key() == "Camera")
        {
            CameraComponent& c = createdEntity.AddComponent<CameraComponent>();
            c.FOV = (float)field.value()["FOV"];
            c.ClippingPlanes.Far = (float)field.value()["ClippingPlanes"]["Far"];
            c.ClippingPlanes.Near = (float)field.value()["ClippingPlanes"]["Near"];
            if(field.value()["Projection"] == "Perspective")
            {
                c.Projection = CameraComponent::Projection::Perspective;
            }
            else {
                c.Projection = CameraComponent::Projection::Orthographic;
            }
        }
        if(field.key() == "Mesh")
        {
            MeshComponent& c = createdEntity.AddComponent<MeshComponent>();
            c.source = AssetManager::Get<Mesh>(AssetId((std::string)field.value()["Source"]));
        }
        if(field.key() == "MeshRenderer")
        {
            MeshRendererComponent& c = createdEntity.AddComponent<MeshRendererComponent>();
            c.material = AssetManager::Get<Material>(AssetId((std::string)field.value()["Material"]));
        }
        if(field.key() == "PointLight")
        {
            PointLightComponent& l = createdEntity.AddComponent<PointLightComponent>();
            l.Color = glm::vec3((float)field.value()["Color"][0], (float)field.value()["Color"][1], (float)field.value()["Color"][2]);
            l.Intensity = (float)field.value()["Intensity"];
        }
        if(field.key() == "Scripts")
        {
            ScriptComponent& l = createdEntity.AddComponent<ScriptComponent>();
            for(auto i : field.value())
            {
                auto instance = ScriptManager::CreateInstance(AssetManager::Get<Script>(GUID(std::string(i["Source"]))), createdEntity.ID(), scene);
                for(auto field : i ["Fields"])
                {
                    auto it = instance.fields.find(field["Name"]);
                    auto value = field["Value"];
                    auto type = field["Type"];
                    if(it != instance.fields.end())
                    {
                        if(type == "Number" && it->second->GetType() == ScriptFieldType::Number)
                        {
                            std::static_pointer_cast<nimo::ScriptFieldNumber>(instance.fields[field["Name"]])->value = value;
                        }
                        if(type == "Boolean" && it->second->GetType() == ScriptFieldType::Boolean)
                        {
                            std::static_pointer_cast<nimo::ScriptFieldBool>(instance.fields[field["Name"]])->value = value;
                        }
                        if(type == "String" && it->second->GetType() == ScriptFieldType::String)
                        {
                            std::static_pointer_cast<nimo::ScriptFieldString>(instance.fields[field["Name"]])->value = value;
                        }
                        if(type == "Asset" && it->second->GetType() == ScriptFieldType::Asset)
                        {
                            if(value != "")
                            {
                                switch(AssetManager::GetMetadata(GUID(std::string(value))).type)
                                {
                                case AssetType::Prefab:
                                    std::static_pointer_cast<nimo::ScriptFieldAsset>(instance.fields[field["Name"]])->value = std::static_pointer_cast<nimo::Asset>(AssetManager::Get<Prefab>(GUID(std::string(value))));
                                    std::static_pointer_cast<nimo::ScriptFieldAsset>(instance.fields[field["Name"]])->type = AssetTypeFromString(field["AssetType"]);
                                    break;
                                case AssetType::Texture:
                                    std::static_pointer_cast<nimo::ScriptFieldAsset>(instance.fields[field["Name"]])->value = std::static_pointer_cast<nimo::Asset>(AssetManager::Get<Texture>(GUID(std::string(value))));
                                    std::static_pointer_cast<nimo::ScriptFieldAsset>(instance.fields[field["Name"]])->type = AssetTypeFromString(field["AssetType"]);
                                    break;
                                case AssetType::EnvironmentMap:
                                    std::static_pointer_cast<nimo::ScriptFieldAsset>(instance.fields[field["Name"]])->value = std::static_pointer_cast<nimo::Asset>(AssetManager::Get<EnvironmentMap>(GUID(std::string(value))));
                                    std::static_pointer_cast<nimo::ScriptFieldAsset>(instance.fields[field["Name"]])->type = AssetTypeFromString(field["AssetType"]);
                                    break;
                                case AssetType::Mesh:
                                    std::static_pointer_cast<nimo::ScriptFieldAsset>(instance.fields[field["Name"]])->value = std::static_pointer_cast<nimo::Asset>(AssetManager::Get<Mesh>(GUID(std::string(value))));
                                    std::static_pointer_cast<nimo::ScriptFieldAsset>(instance.fields[field["Name"]])->type = AssetTypeFromString(field["AssetType"]);
                                    break;
                                case AssetType::Material:
                                    std::static_pointer_cast<nimo::ScriptFieldAsset>(instance.fields[field["Name"]])->value = std::static_pointer_cast<nimo::Asset>(AssetManager::Get<Material>(GUID(std::string(value))));
                                    std::static_pointer_cast<nimo::ScriptFieldAsset>(instance.fields[field["Name"]])->type = AssetTypeFromString(field["AssetType"]);
                                    break;
                                case AssetType::Script:
                                    std::static_pointer_cast<nimo::ScriptFieldAsset>(instance.fields[field["Name"]])->value = std::static_pointer_cast<nimo::Asset>(AssetManager::Get<Script>(GUID(std::string(value))));
                                    std::static_pointer_cast<nimo::ScriptFieldAsset>(instance.fields[field["Name"]])->type = AssetTypeFromString(field["AssetType"]);
                                    break;
                                default:
                                    break;
                                }                                
                            }
                        }
                    }
                }
                ScriptManager::ApplyFields(instance);
                l.instances.push_back(instance);
            }
        }
    }
    return createdEntity.GetComponent<IDComponent>().Id;
}