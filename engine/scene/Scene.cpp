#include "Scene.h"
#include "Components.h"
#include "renderer/Shader.h"
#include "glad/glad.h"
#include "project/Project.h"

const unsigned int NR_LIGHTS = 32;
std::vector<glm::vec3> lightPositions;
std::vector<glm::vec3> lightColors;
nimo::Scene::Scene(const std::string& name)
    : name(name)
{
    FrameBuffer::Details gBufferDetails;
    gBufferDetails.width = 1920;
    gBufferDetails.height = 1080;
    gBufferDetails.clearColorOnBind = true;
    gBufferDetails.clearDepthOnBind = true;
    gBufferDetails.colorAttachments.push_back({GL_RGBA16F, GL_RGBA, GL_FLOAT});
    gBufferDetails.colorAttachments.push_back({GL_RGBA16F, GL_RGBA, GL_FLOAT});
    gBufferDetails.colorAttachments.push_back({GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE});
    gBuffer = std::make_shared<FrameBuffer>(gBufferDetails);

    srand(13);
    for (unsigned int i = 0; i < NR_LIGHTS; i++)
    {
        // calculate slightly random offsets
        float xPos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 3.0);
        float yPos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 4.0);
        float zPos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 3.0);
        lightPositions.push_back(glm::vec3(xPos, yPos, zPos));
        // also calculate random color
        float rColor = static_cast<float>(((rand() % 100) / 200.0f) + 0.5); // between 0.5 and 1.)
        float gColor = static_cast<float>(((rand() % 100) / 200.0f) + 0.5); // between 0.5 and 1.)
        float bColor = static_cast<float>(((rand() % 100) / 200.0f) + 0.5); // between 0.5 and 1.)
        lightColors.push_back(glm::vec3(rColor, gColor, bColor));
    }
}
nimo::Scene::~Scene()
{

}
unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
    if (quadVAO == 0)
    {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

void nimo::Scene::Update(std::shared_ptr<FrameBuffer> target)
{
    // Render scene into gbuffer
    gBuffer->bind();
    m_registry.view<IDComponent, MeshComponent, TransformComponent, MeshRendererComponent>().each([&](IDComponent& id, MeshComponent& m, TransformComponent& t, MeshRendererComponent& r) {
        if(!r.material || !r.material->shader || !m.source) return;
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
        m.source->draw();
    });
    target->bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    auto shaderLightingPass = nimo::AssetManager::Get<Shader>("Shaders/deferred_shading.nshader");
    shaderLightingPass->use();
    shaderLightingPass->set("gPosition", 0);
    shaderLightingPass->set("gNormal", 1);
    shaderLightingPass->set("gAlbedoSpec", 2);
    gBuffer->BindColorTexture(0,0);
    gBuffer->BindColorTexture(1,1);
    gBuffer->BindColorTexture(2,2);
    for (unsigned int i = 0; i < lightPositions.size(); i++)
    {
        shaderLightingPass->set("lights[" + std::to_string(i) + "].Position", lightPositions[i]);
        shaderLightingPass->set("lights[" + std::to_string(i) + "].Color", lightColors[i]);
        // update attenuation parameters and calculate radius
        const float constant = 1.0f; // note that we don't send this to the shader, we assume it is always 1.0 (in our case)
        const float linear = 0.7f;
        const float quadratic = 1.8f;
        shaderLightingPass->set("lights[" + std::to_string(i) + "].Linear", linear);
        shaderLightingPass->set("lights[" + std::to_string(i) + "].Quadratic", quadratic);
        // then calculate radius of light volume/sphere
        const float maxBrightness = std::fmaxf(std::fmaxf(lightColors[i].r, lightColors[i].g), lightColors[i].b);
        float radius = (-linear + std::sqrt(linear * linear - 4 * quadratic * (constant - (256.0f / 5.0f) * maxBrightness))) / (2.0f * quadratic);
        shaderLightingPass->set("lights[" + std::to_string(i) + "].Radius", radius);
    }
    m_registry.view<CameraComponent, TransformComponent>().each([&](CameraComponent& cam, TransformComponent& camTransform)
    {
        glm::mat4 projection;
        if(cam.Projection == CameraComponent::Projection::Perspective)
            projection = glm::perspective(glm::radians(cam.FOV), (float)1920 / (float)1080, cam.ClippingPlanes.Near, cam.ClippingPlanes.Far);
        else
            projection = glm::ortho(-10.0f * 0.5f, 10.0f * 0.5f, -10.0f * 0.5f *9.0f/16.0f, 10.0f * 0.5f*9.0f/16.0f, 0.1f, 100.0f);
        nimo::AssetManager::Get<Shader>("Shaders/deferred_shading.nshader")->set("viewPos", glm::vec3(-camTransform.Translation.x, -camTransform.Translation.y, camTransform.Translation.z));
    });
    renderQuad();

    // glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer->m_id);
    // glBindFramebuffer(GL_DRAW_FRAMEBUFFER, drawFboId);
    // // blit to default framebuffer. Note that this may or may not work as the internal formats of both the FBO and default framebuffer have to match.
    // // the internal formats are implementation defined. This works on all of my systems, but if it doesn't on yours you'll likely have to write to the 		
    // // depth buffer in another shader stage (or somehow see to match the default framebuffer's internal format with the FBO's internal format).
    // glBlitFramebuffer(0, 0, 1920, 1080, 0, 0, 1920, 1080, GL_COLOR_BUFFER_BIT, GL_NEAREST);
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

