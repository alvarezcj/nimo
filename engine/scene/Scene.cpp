#include "Scene.h"
#include "Components.h"

nimo::Scene::Scene(const std::string& name)
    : name(name)
{

}
nimo::Scene::~Scene()
{

}
void nimo::Scene::Update()
{
    m_registry.view<IDComponent, MeshComponent, TransformComponent, MeshRendererComponent>().each([&](IDComponent& id, MeshComponent& m, TransformComponent& t, MeshRendererComponent& r) {
        if(!r.material || !r.material->shader || !r.mesh) return;
        r.material->shader->use();
        r.material->Setup();
        r.material->shader->set("transform", GetWorldSpaceTransformMatrix(GetEntity(id.Id)));
        m_registry.view<CameraComponent, TransformComponent>().each([&](CameraComponent& cam, TransformComponent& camTransform)
        {
            glm::mat4 projection;
            if(cam.Projection == CameraComponent::Projection::Perspective)
                projection = glm::perspective(glm::radians(cam.FOV), (float)1920 / (float)1080, cam.ClippingPlanes.Near, cam.ClippingPlanes.Far);
            else
                projection = glm::ortho(-10.0f * 0.5f, 10.0f * 0.5f, -10.0f * 0.5f *9.0f/16.0f, 10.0f * 0.5f*9.0f/16.0f, 0.1f, 100.0f);
            r.material->shader->set("view", glm::toMat4(glm::quat(camTransform.Rotation)) * glm::translate(glm::mat4(1.0f), {-camTransform.Translation.x, -camTransform.Translation.y, camTransform.Translation.z}) );
            r.material->shader->set("projection", projection);
        });
        r.mesh->draw();
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

