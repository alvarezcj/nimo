#include "SceneSerializer.h"
#include "json.hpp"
#include "Components.h"
#include <fstream>
#include "project/Project.h"

// nimo::SceneSerializer::SceneSerializer(const std::shared_ptr<Scene>& scene)
//     : m_scene(scene)
// {}
// void nimo::SceneSerializer::Serialize(const std::filesystem::path& filepath)
// {
//     nlohmann::ordered_json j;
//     nlohmann::ordered_json jentities = nlohmann::json::array();
//     j["Name"] = m_scene->name;
//     j["GUID"] = m_scene->id.str();
//     int i = 0;
//     m_scene->m_registry.each([&](entt::entity id)
//     {
//         nlohmann::ordered_json jentity;
//         Entity e(id, m_scene->m_registry);
//         if(e.HasComponent<IDComponent>())
//         {
//             jentity["GUID"] = e.GetComponent<IDComponent>().Id.str();
//         }
//         if(e.HasComponent<LabelComponent>())
//         {
//             jentity["Label"] = e.GetComponent<LabelComponent>().Label;
//         }
//         if(e.HasComponent<TransformComponent>())
//         {
//             TransformComponent t = e.GetComponent<TransformComponent>();
//             nlohmann::ordered_json jtransform{
//                 {"Translation", {t.Translation.x, t.Translation.y, t.Translation.z}},
//                 {"Rotation", {t.Rotation.x, t.Rotation.y, t.Rotation.z}},
//                 {"Scale", {t.Scale.x, t.Scale.y, t.Scale.z}}
//             };
//             jentity["Transform"] = jtransform;
//         }
//         if(e.HasComponent<CameraComponent>())
//         {
//             CameraComponent c = e.GetComponent<CameraComponent>();
//             nlohmann::ordered_json jcamera{
//                 {"FOV", c.FOV},
//                 {"ClippingPlanes", {{"Near", c.ClippingPlanes.Near},{"Far", c.ClippingPlanes.Far}}}
//             };
//             c.Projection == CameraComponent::Projection::Perspective ? jcamera["Projection"] = "Perspective" : jcamera["Projection"] = "Orthographic";
//             jentity["Camera"] = jcamera;
//         }
//         if(e.HasComponent<MeshComponent>())
//         {
//             MeshComponent m = e.GetComponent<MeshComponent>();
//             nlohmann::ordered_json jmesh{
//                 {"Source", m.source->id.str()}
//             };
//             jentity["Mesh"] = jmesh;
//         }
//         if(e.HasComponent<MeshRendererComponent>())
//         {
//             MeshRendererComponent m = e.GetComponent<MeshRendererComponent>();
//             nlohmann::ordered_json jmesh{
//                 {"Material", m.material->id.str()}
//             };
//             jentity["MeshRenderer"] = jmesh;
//         }

//         jentities[i] = jentity;
//         i++;
//     });
//     j["Entities"] = jentities;
//     std::ofstream ofs(filepath);
//     ofs << j;
// }
// bool nimo::SceneSerializer::Deserialize(const std::filesystem::path& filepath)
// {
//     std::ifstream ifs(filepath);
//     nlohmann::ordered_json j;
//     j << ifs;
//     m_scene->id = GUID(std::string(j["GUID"]));
//     for(auto entity : j["Entities"])
//     {
//         auto createdEntity =  m_scene->CreateEntity();
//         for (auto field : entity.items())
//         {
//             if(field.key() == "GUID")
//             {
//                 createdEntity.GetComponent<IDComponent>().Id = GUID(std::string(field.value()));
//             }
//             if(field.key() == "Label")
//             {
//                 createdEntity.GetComponent<LabelComponent>().Label = field.value();
//             }
//             if(field.key() == "Transform")
//             {
//                 auto Translation = glm::vec3((float)field.value()["Translation"][0], (float)field.value()["Translation"][1], (float)field.value()["Translation"][2]);
//                 auto Rotation = glm::vec3((float)field.value()["Rotation"][0], (float)field.value()["Rotation"][1], (float)field.value()["Rotation"][2]);
//                 auto Scale = glm::vec3((float)field.value()["Scale"][0], (float)field.value()["Scale"][1], (float)field.value()["Scale"][2]);
//                 TransformComponent& t = createdEntity.AddComponent<TransformComponent>();
//                 t.Translation = Translation;
//                 t.Rotation = Rotation;
//                 t.Scale = Scale;
//             }
//             if(field.key() == "Camera")
//             {
//                 CameraComponent& c = createdEntity.AddComponent<CameraComponent>();
//                 c.FOV = (float)field.value()["FOV"];
//                 c.ClippingPlanes.Far = (float)field.value()["ClippingPlanes"]["Far"];
//                 c.ClippingPlanes.Near = (float)field.value()["ClippingPlanes"]["Near"];
//                 if(field.value()["Projection"] == "Perspective")
//                 {
//                     c.Projection = CameraComponent::Projection::Perspective;
//                 }
//                 else {
//                     c.Projection = CameraComponent::Projection::Orthographic;
//                 }
//             }
//             if(field.key() == "Mesh")
//             {
//                 MeshComponent& c = createdEntity.AddComponent<MeshComponent>();
//                 c.source = AssetManager::Get<MeshSource>(AssetId((std::string)field.value()["Source"]));
//             }
//             if(field.key() == "MeshRenderer")
//             {
//                 MeshRendererComponent& c = createdEntity.AddComponent<MeshRendererComponent>();
//                 c.material = AssetManager::Get<Material>(AssetId((std::string)field.value()["Material"]));
//             }
//         }
//     }
//     auto view1 = m_scene->m_registry.view<MeshComponent, MeshRendererComponent>();
//     view1.each([&](MeshComponent& mesh, MeshRendererComponent& meshRenderer) {
//         meshRenderer.mesh = std::make_shared<Mesh>(mesh.source);
//     });

//     return true;
// }

void nimo::AssetSerializer<nimo::Scene>::Serialize(const AssetMetadata& metadata, const std::shared_ptr<nimo::Scene>& asset)
{
    nlohmann::ordered_json j;
    nlohmann::ordered_json jentities = nlohmann::json::array();
    j["Name"] = asset->name;
    j["GUID"] = asset->id.str();
    int i = 0;
    asset->m_registry.each([&](entt::entity id)
    {
        nlohmann::ordered_json jentity;
        Entity e(id, asset->m_registry);
        if(e.HasComponent<IDComponent>())
        {
            jentity["GUID"] = e.GetComponent<IDComponent>().Id.str();
        }
        if(e.HasComponent<LabelComponent>())
        {
            jentity["Label"] = e.GetComponent<LabelComponent>().Label;
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

        jentities[i] = jentity;
        i++;
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

    std::shared_ptr<Scene> scene = std::make_shared<Scene>(metadata.filepath.filename().string());

    scene->id = GUID(std::string(j["GUID"]));
    for(auto entity : j["Entities"])
    {
        auto createdEntity =  scene->CreateEntity();
        for (auto field : entity.items())
        {
            if(field.key() == "GUID")
            {
                createdEntity.GetComponent<IDComponent>().Id = GUID(std::string(field.value()));
            }
            if(field.key() == "Label")
            {
                createdEntity.GetComponent<LabelComponent>().Label = field.value();
            }
            if(field.key() == "Transform")
            {
                auto Translation = glm::vec3((float)field.value()["Translation"][0], (float)field.value()["Translation"][1], (float)field.value()["Translation"][2]);
                auto Rotation = glm::vec3((float)field.value()["Rotation"][0], (float)field.value()["Rotation"][1], (float)field.value()["Rotation"][2]);
                auto Scale = glm::vec3((float)field.value()["Scale"][0], (float)field.value()["Scale"][1], (float)field.value()["Scale"][2]);
                TransformComponent& t = createdEntity.AddComponent<TransformComponent>();
                t.Translation = Translation;
                t.Rotation = Rotation;
                t.Scale = Scale;
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
                c.source = AssetManager::Get<MeshSource>(AssetId((std::string)field.value()["Source"]));
            }
            if(field.key() == "MeshRenderer")
            {
                MeshRendererComponent& c = createdEntity.AddComponent<MeshRendererComponent>();
                c.material = AssetManager::Get<Material>(AssetId((std::string)field.value()["Material"]));
            }
        }
    }
    auto view1 = scene->m_registry.view<MeshComponent, MeshRendererComponent>();
    view1.each([&](MeshComponent& mesh, MeshRendererComponent& meshRenderer) {
        meshRenderer.mesh = std::make_shared<Mesh>(mesh.source);
    });
    return scene;
}