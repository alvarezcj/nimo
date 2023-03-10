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
    m_registry.view<MeshComponent, TransformComponent, MeshRendererComponent>().each([&](MeshComponent& m, TransformComponent& t, MeshRendererComponent& r) {
        r.material->shader->use();
        r.material->Setup();
        r.material->shader->set("transform", t.GetTransform());
        r.mesh->draw();
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
    m_entities[e.GetComponent<IDComponent>().Id] = id;
    return e;
}
void nimo::Scene::DestroyEntity(Entity entity)
{
}

void nimo::Scene::ForEachEntity(std::function<void(Entity&)> action)
{
    m_registry.each([&](entt::entity e)
    {
        Entity ent(e, m_registry);
        action(ent);
    });
}
