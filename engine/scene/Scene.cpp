#include "Scene.h"
#include "Components.h"

nimo::Scene::Scene(const std::string& name)
    : name(name)
{

}
nimo::Scene::~Scene()
{

}
void nimo::Scene::Update(const Shader& shader)
{
    auto view1 = m_registry.view<CameraComponent, TransformComponent>();
    view1.each([&](CameraComponent& cam, TransformComponent& t) {
        glm::mat4 projection;
        if(cam.Projection == CameraComponent::Projection::Perspective)
            projection = glm::perspective(glm::radians(cam.FOV), (float)1920 / (float)1080, cam.ClippingPlanes.Near, cam.ClippingPlanes.Far);
        else
            projection = glm::ortho(-10.0f * 0.5f, 10.0f * 0.5f, -10.0f * 0.5f *9.0f/16.0f, 10.0f * 0.5f*9.0f/16.0f, 0.1f, 100.0f);
        
        shader.set("view", glm::toMat4(glm::quat(t.Rotation)) * glm::translate(glm::mat4(1.0f), {-t.Translation.x, -t.Translation.y, t.Translation.z}) );
        shader.set("projection", projection);
    });
    auto view3 = m_registry.view<MeshComponent, TransformComponent, MeshRendererComponent>();
    view3.each([&](MeshComponent& m, TransformComponent& t, MeshRendererComponent& r) {
        shader.set("transform", t.GetTransform());
        r.material->Setup();
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
