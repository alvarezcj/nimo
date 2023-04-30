#include "Scene.h"
#include "Components.h"
#include "renderer/Shader.h"
#include "glad/glad.h"
#include "project/Project.h"
#include "scripting/ScriptManager.h"

// const unsigned int NR_LIGHTS = 32;
// std::vector<glm::vec3> lightPositions;
// std::vector<glm::vec3> lightColors;
nimo::Scene::Scene(const std::string& name)
    : name(name)
{
    // FrameBuffer::Details gBufferDetails;
    // gBufferDetails.width = 1920;
    // gBufferDetails.height = 1080;
    // gBufferDetails.clearColorOnBind = true;
    // gBufferDetails.clearDepthOnBind = true;
    // gBufferDetails.colorAttachments.push_back({GL_RGBA32F, GL_RGBA, GL_FLOAT});
    // gBufferDetails.colorAttachments.push_back({GL_RGBA32F, GL_RGBA, GL_FLOAT});
    // gBufferDetails.colorAttachments.push_back({GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE});
    // gBuffer = std::make_shared<FrameBuffer>(gBufferDetails);

    // srand(13);
    // for (unsigned int i = 0; i < NR_LIGHTS; i++)
    // {
    //     // calculate slightly random offsets
    //     float xPos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 3.0);
    //     float yPos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 4.0);
    //     float zPos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 3.0);
    //     lightPositions.push_back(glm::vec3(xPos, yPos, zPos));
    //     // also calculate random color
    //     float rColor = static_cast<float>(((rand() % 100) / 200.0f) + 0.5); // between 0.5 and 1.)
    //     float gColor = static_cast<float>(((rand() % 100) / 200.0f) + 0.5); // between 0.5 and 1.)
    //     float bColor = static_cast<float>(((rand() % 100) / 200.0f) + 0.5); // between 0.5 and 1.)
    //     lightColors.push_back(glm::vec3(rColor, gColor, bColor));
    // }
}
nimo::Scene::~Scene()
{

}

void nimo::Scene::Update(float deltaTime)
{
    m_registry.view<ScriptComponent>().each([&](ScriptComponent& script)
    {
        for(const auto& instance : script.instances)
        {
            ScriptManager::OnUpdate(instance, deltaTime);
        }
    });
}
nimo::Entity nimo::Scene::CreateEntity(const std::string& name)
{
    auto id = m_registry.create();
    auto e = Entity(id, m_registry);
    e.AddComponent<IDComponent>().Id = GUID::Create();
    if(name == "")
        e.AddComponent<LabelComponent>().Label = e.GetComponent<IDComponent>().Id.str();
    else
        e.AddComponent<LabelComponent>().Label = name;
    e.AddComponent<FamilyComponent>();
    e.AddComponent<TransformComponent>();
    m_entities[e.GetComponent<IDComponent>().Id] = id;
    return e;
}
nimo::Entity nimo::Scene::CreateEntityWithID(GUID desiredId)
{
    auto id = m_registry.create();
    auto e = Entity(id, m_registry);
    e.AddComponent<IDComponent>().Id = desiredId;
    if(name == "")
        e.AddComponent<LabelComponent>().Label = e.GetComponent<IDComponent>().Id.str();
    else
        e.AddComponent<LabelComponent>().Label = name;
    e.AddComponent<FamilyComponent>();
    e.AddComponent<TransformComponent>();
    m_entities[e.GetComponent<IDComponent>().Id] = id;
    return e;
}

nimo::Entity nimo::Scene::CreateEntityWithParent(Entity parent, const std::string& name)
{
    auto e = CreateEntity(name);
    e.GetComponent<FamilyComponent>().Parent = parent.GetComponent<IDComponent>().Id;
    parent.GetComponent<FamilyComponent>().Children.push_back(e.GetComponent<IDComponent>().Id);
    return e;
}

void nimo::Scene::DestroyEntity(Entity entity)
{
    if(entity.Parent().valid())
    {
        auto parent = GetEntity(entity.Parent());
        parent.GetComponent<FamilyComponent>().Children.erase(
            std::remove(parent.GetComponent<FamilyComponent>().Children.begin(), parent.GetComponent<FamilyComponent>().Children.end(), entity.ID()),
             parent.GetComponent<FamilyComponent>().Children.end());
    }
    m_entities.erase(entity.ID());
    for(auto child : entity.Children())
    {
        DestroyEntity(GetEntity(child));
    }
    m_registry.destroy(entity.m_handle);
}

void nimo::Scene::ForEachEntity(std::function<void(Entity&)> action)
{
    m_registry.each([&](entt::entity e)
    {
        Entity ent(e, m_registry);
        action(ent);
    });
}

glm::mat4 nimo::Scene::GetWorldSpaceTransformMatrix(Entity entity)
{
    glm::mat4 transform(1.0f);

    if (entity.GetComponent<FamilyComponent>().Parent.valid())
    {
        Entity parent = GetEntity(entity.GetComponent<FamilyComponent>().Parent);
        if (parent.GetComponent<IDComponent>().Id.valid())
            transform = GetWorldSpaceTransformMatrix(parent);
    }

    return transform * entity.GetComponent<TransformComponent>().GetTransform();
}

