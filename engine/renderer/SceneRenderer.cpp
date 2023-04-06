#include "SceneRenderer.h"
#include "glad/glad.h"

unsigned int cubeVAO2 = 0;
unsigned int cubeVBO2 = 0;
void renderCube2()
{
    // initialize (if necessary)
    if (cubeVAO2 == 0)
    {
        float vertices[] = {
            // back face
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
            -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
            // front face
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
            -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
            // left face
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            // right face
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
            // bottom face
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
            -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
            // top face
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
             1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
             1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
            -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
        };
        glGenVertexArrays(1, &cubeVAO2);
        glGenBuffers(1, &cubeVBO2);
        // fill buffer
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO2);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // link vertex attributes
        glBindVertexArray(cubeVAO2);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    // render Cube
    glBindVertexArray(cubeVAO2);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}
nimo::SceneRenderer::SceneRenderer()
{
    // GBuffer
    FrameBuffer::Details gBufferDetails;
    gBufferDetails.width = 1920;
    gBufferDetails.height = 1080;
    gBufferDetails.clearColorOnBind = true;
    gBufferDetails.clearDepthOnBind = true;
    gBufferDetails.colorAttachments.push_back({GL_RGBA16F, GL_RGBA, GL_FLOAT});
    gBufferDetails.colorAttachments.push_back({GL_RGBA16F, GL_RGBA, GL_FLOAT});
    gBufferDetails.colorAttachments.push_back({GL_RGBA16F, GL_RGBA, GL_FLOAT});
    gBufferDetails.colorAttachments.push_back({GL_RGBA16F, GL_RGBA, GL_FLOAT});
    m_gBuffer = std::make_shared<FrameBuffer>(gBufferDetails);
    // HDR color buffer
    FrameBuffer::Details hdrColorBufferDetails;
    hdrColorBufferDetails.width = 1920;
    hdrColorBufferDetails.height = 1080;
    hdrColorBufferDetails.clearColorOnBind = true;
    hdrColorBufferDetails.clearDepthOnBind = true;
    hdrColorBufferDetails.colorAttachments.push_back({GL_RGBA16F, GL_RGB, GL_FLOAT});
    m_hdrColorBuffer = std::make_shared<FrameBuffer>(hdrColorBufferDetails);

    //Lighting shader
    m_shaderLightingPass = nimo::AssetManager::Get<Shader>("Shaders/deferred_shading_pbr.nshader");
    //Cubemap background shader
    m_backgroundPass = nimo::AssetManager::Get<Shader>("Shaders/background.nshader");
    m_environmentMap = nimo::AssetManager::Get<EnvironmentMap>("EnvMaps/hilly_terrain_01_puresky_4k.hdr");
    //Tone mapping shaderm_backgroundPass
    m_hdrToneMappingPass = nimo::AssetManager::Get<Shader>("Shaders/hdr_tone_mapping.nshader");

    // Full screen Quad mesh
    std::vector<Vertex> vertices ={
        {{1.0f,  1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}},
        {{1.0f,  -1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}},
        {{-1.0f,  -1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}},
        {{-1.0f,  1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}}
    };
    std::vector<unsigned int> indices ={
        0,1,3,
        1,2,3
    };
    m_quadMesh = std::make_shared<Mesh>(vertices, indices);
}
nimo::SceneRenderer::~SceneRenderer()
{}

void nimo::SceneRenderer::SetScene(std::shared_ptr<Scene> scene)
{
    m_scene = scene;
}
void nimo::SceneRenderer::Render(std::shared_ptr<FrameBuffer> target)
{
    Entity camera(*m_scene->m_registry.view<CameraComponent>().begin(), m_scene->m_registry);
    auto camTransform = camera.GetComponent<TransformComponent>();
    auto cam = camera.GetComponent<CameraComponent>();
    glm::mat4 projection;
    if(cam.Projection == CameraComponent::Projection::Perspective)
        projection = glm::perspectiveFov(glm::radians(cam.FOV), (float)1920 , (float)1080, cam.ClippingPlanes.Near, cam.ClippingPlanes.Far);
    else
        projection = glm::ortho(-10.0f * 0.5f, 10.0f * 0.5f, -10.0f * 0.5f *9.0f/16.0f, 10.0f * 0.5f*9.0f/16.0f, 0.1f, 100.0f);
    glm::mat4 viewMatrix = glm::toMat4(glm::quat(camTransform.Rotation)) * glm::translate(glm::mat4(1.0f), {camTransform.Translation.x, camTransform.Translation.y, camTransform.Translation.z});
    auto viewPosition = glm::vec3(camTransform.Translation.x, camTransform.Translation.y, camTransform.Translation.z);
    // Render scene into gbuffer
    m_gBuffer->bind();
    m_scene->m_registry.view<IDComponent, MeshComponent, TransformComponent, MeshRendererComponent>().each([&](IDComponent& id, MeshComponent& m, TransformComponent& t, MeshRendererComponent& r) {
        if(!r.material || !r.material->shader || !m.source) return;
        r.material->shader->use();
        r.material->Setup();
        r.material->shader->set("viewPos", viewPosition);
        r.material->shader->set("transform", m_scene->GetWorldSpaceTransformMatrix(m_scene->GetEntity(id.Id)));
        r.material->shader->set("view", viewMatrix);
        r.material->shader->set("projection", projection);
        m.source->draw();
    });

    // Lighting pass
    m_hdrColorBuffer->bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_shaderLightingPass->use();
    m_shaderLightingPass->set("gPosition", 0);
    m_shaderLightingPass->set("gNormal", 1);
    m_shaderLightingPass->set("gAlbedoSpec", 2);
    m_shaderLightingPass->set("gARM", 3);
    m_gBuffer->BindColorTexture(0,0);
    m_gBuffer->BindColorTexture(1,1);
    m_gBuffer->BindColorTexture(2,2);
    m_gBuffer->BindColorTexture(3,3);
    int currentLights = 0;
    m_scene->m_registry.view<PointLightComponent, TransformComponent>().each([&](PointLightComponent& light, TransformComponent& lightTransform)
    {
        m_shaderLightingPass->set("lights[" + std::to_string(currentLights) + "].Position", lightTransform.Translation);
        m_shaderLightingPass->set("lights[" + std::to_string(currentLights) + "].Color", light.Color);
        m_shaderLightingPass->set("lights[" + std::to_string(currentLights) + "].Intensity", light.Intensity);
        static const float constant = 1.0f; // note that we don't send this to the shader, we assume it is always 1.0 (in our case)
        static const float linear = 0.7f;
        static const float quadratic = 1.8f;
        m_shaderLightingPass->set("lights[" + std::to_string(currentLights) + "].Linear", linear);
        m_shaderLightingPass->set("lights[" + std::to_string(currentLights) + "].Quadratic", quadratic);
        // then calculate radius of light volume/sphere
        const float maxBrightness = std::fmaxf(std::fmaxf(light.Color.r, light.Color.g), light.Color.b);
        float radius = (-linear + std::sqrt(linear * linear - 4 * quadratic * (constant - (256.0f / 5.0f) * maxBrightness))) / (2.0f * quadratic);
        m_shaderLightingPass->set("lights[" + std::to_string(currentLights) + "].Radius", radius);
        m_shaderLightingPass->set("lights[" + std::to_string(currentLights) + "].Active", true);
        currentLights++;
    });
    for(int i = currentLights; i < 32; ++i)
    {
        m_shaderLightingPass->set("lights[" + std::to_string(i) + "].Active", false);
    }
    m_shaderLightingPass->set("viewPos", viewPosition);
    m_quadMesh->draw();

    // Background pass
    glDepthFunc(GL_LEQUAL);
    glBlitNamedFramebuffer(m_gBuffer->m_id, m_hdrColorBuffer->m_id, 0,0,1920,1080, 0,0,1920,1080, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    m_backgroundPass->use();
    m_backgroundPass->set("view", viewMatrix);
    m_backgroundPass->set("projection", projection);
    m_backgroundPass->set("environmentMap", 0);
    m_environmentMap->bind(0);
    renderCube2();
    glDepthFunc(GL_LESS);

    // HDR tone mapping pass
    target->bind();
    m_hdrToneMappingPass->use();
    m_shaderLightingPass->set("hdrBuffer", 0);
    m_hdrColorBuffer->BindColorTexture(0,0);
    m_quadMesh->draw();
}
