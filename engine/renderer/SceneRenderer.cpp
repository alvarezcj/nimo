#include "SceneRenderer.h"
#include "glad/glad.h"
#include "core/Application.h"
#include "glm/gtc/matrix_inverse.hpp"
#include <glm/gtx/matrix_decompose.hpp>
#include "Renderer.h"

struct TextVertex
{
    glm::vec4 vertex;
};

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
    // Directional Light buffer
    FrameBuffer::Details directionalLightBufferDetails;
    directionalLightBufferDetails.width = 4096;
    directionalLightBufferDetails.height = 4096;
    directionalLightBufferDetails.clearColorOnBind = true;
    directionalLightBufferDetails.colorAttachments.push_back({GL_RGB16F, GL_RGB, GL_FLOAT});
    m_directionalLightDepthBuffer = std::make_shared<FrameBuffer>(directionalLightBufferDetails);
    // GBuffer
    FrameBuffer::Details gBufferDetails;
    gBufferDetails.width = 1920;
    gBufferDetails.height = 1080;
    gBufferDetails.clearColorOnBind = true;
    gBufferDetails.clearDepthOnBind = true;
    gBufferDetails.colorAttachments.push_back({GL_RGB16F, GL_RGB, GL_FLOAT});
    gBufferDetails.colorAttachments.push_back({GL_RGB16F, GL_RGB, GL_FLOAT});
    gBufferDetails.colorAttachments.push_back({GL_RGB16F, GL_RGB, GL_FLOAT});
    gBufferDetails.colorAttachments.push_back({GL_RGB16F, GL_RGB, GL_FLOAT});
    m_gBuffer = std::make_shared<FrameBuffer>(gBufferDetails);
    // HDR color buffer
    FrameBuffer::Details hdrColorBufferDetails;
    hdrColorBufferDetails.width = 1920;
    hdrColorBufferDetails.height = 1080;
    hdrColorBufferDetails.clearColorOnBind = true;
    hdrColorBufferDetails.clearDepthOnBind = true;
    hdrColorBufferDetails.colorAttachments.push_back({GL_RGBA16F, GL_RGB, GL_FLOAT});
    m_hdrColorBuffer = std::make_shared<FrameBuffer>(hdrColorBufferDetails);
    // HDR brightness buffer
    FrameBuffer::Details hdrBrightnessBufferDetails;
    hdrBrightnessBufferDetails.width = 960;
    hdrBrightnessBufferDetails.height = 540;
    hdrBrightnessBufferDetails.clearColorOnBind = true;
    hdrBrightnessBufferDetails.clearDepthOnBind = true;
    hdrBrightnessBufferDetails.colorAttachments.push_back({GL_RGBA16F, GL_RGB, GL_FLOAT});
    m_hdrBrightnessBuffer = std::make_shared<FrameBuffer>(hdrBrightnessBufferDetails);
    // HDR bloom buffers
    FrameBuffer::Details hdrBloomBufferDetails;
    hdrBloomBufferDetails.clearColorOnBind = true;
    hdrBloomBufferDetails.clearDepthOnBind = true;
    hdrBloomBufferDetails.colorAttachments.push_back({GL_RGBA16F, GL_RGB, GL_FLOAT});
    hdrBloomBufferDetails.width = 1920;
    hdrBloomBufferDetails.height = 1080;
    m_hdrFinalBloomBuffer = std::make_shared<FrameBuffer>(hdrBloomBufferDetails);
    hdrBloomBufferDetails.width = 960;
    hdrBloomBufferDetails.height = 540;
    m_hdrBloomUpsample1Buffer = std::make_shared<FrameBuffer>(hdrBloomBufferDetails);
    hdrBloomBufferDetails.width = 480;
    hdrBloomBufferDetails.height = 270;
    m_hdrBloomDownsample1Buffer = std::make_shared<FrameBuffer>(hdrBloomBufferDetails);
    m_hdrBloomUpsample2Buffer = std::make_shared<FrameBuffer>(hdrBloomBufferDetails);
    hdrBloomBufferDetails.width = 240;
    hdrBloomBufferDetails.height = 135;
    m_hdrBloomDownsample2Buffer = std::make_shared<FrameBuffer>(hdrBloomBufferDetails);
    m_hdrBloomUpsample3Buffer = std::make_shared<FrameBuffer>(hdrBloomBufferDetails);
    hdrBloomBufferDetails.width = 120;
    hdrBloomBufferDetails.height = 67;
    m_hdrBloomDownsample3Buffer = std::make_shared<FrameBuffer>(hdrBloomBufferDetails);
    m_hdrBloomUpsample4Buffer = std::make_shared<FrameBuffer>(hdrBloomBufferDetails);
    hdrBloomBufferDetails.width = 60;
    hdrBloomBufferDetails.height = 33;
    m_hdrBloomDownsample4Buffer = std::make_shared<FrameBuffer>(hdrBloomBufferDetails);
    m_hdrBloomUpsample5Buffer = std::make_shared<FrameBuffer>(hdrBloomBufferDetails);
    hdrBloomBufferDetails.width = 30;
    hdrBloomBufferDetails.height = 16;
    m_hdrBloomDownsample5Buffer = std::make_shared<FrameBuffer>(hdrBloomBufferDetails);
    m_hdrBloomUpsample6Buffer = std::make_shared<FrameBuffer>(hdrBloomBufferDetails);
    hdrBloomBufferDetails.width = 15;
    hdrBloomBufferDetails.height = 8;
    m_hdrBloomDownsample6Buffer = std::make_shared<FrameBuffer>(hdrBloomBufferDetails);

    //Lighting shader
    m_shaderLightingPass = nimo::AssetManager::Get<Shader>("Shaders/deferred_shading_pbr.nshader");
    //Cubemap background shader
    m_backgroundPass = nimo::AssetManager::Get<Shader>("Shaders/background.nshader");
    //Tone mapping shaderm_backgroundPass
    m_hdrToneMappingPass = nimo::AssetManager::Get<Shader>("Shaders/hdr_tone_mapping.nshader");
    //Bloom
    m_hdrBrightFilterPass = nimo::AssetManager::Get<Shader>("Shaders/hdr_bright_filter.nshader");
    m_hdrBloomDownsamplePass = nimo::AssetManager::Get<Shader>("Shaders/hdr_bloom_downsample.nshader");
    m_hdrBloomUpsamplePass = nimo::AssetManager::Get<Shader>("Shaders/hdr_bloom_upsample.nshader");
    //2D
    m_shader2d = nimo::AssetManager::Get<Shader>("Shaders/unlit_texture.nshader");
    m_shaderText = AssetManager::Get<Shader>("Shaders/text.nshader");

    m_shaderDepth = nimo::AssetManager::Get<Shader>("Shaders/depth.nshader");

    //White texture in memory
    unsigned int whitePixel = 0xFFFFFFFF;
    m_white = std::make_shared<Texture>(1, 1, &whitePixel);
    //Black texture in memory
    unsigned int blackPixel = 0x00000000;
    m_black = std::make_shared<Texture>(1, 1, &blackPixel);

    std::vector<QuadVertex> m_vertices ={
        {{1.0f,  1.0f, 0.0f}, {1.0f, 1.0f}},
        {{1.0f,  -1.0f, 0.0f}, {1.0f, 0.0f}},
        {{-1.0f,  -1.0f, 0.0f}, {0.0f, 0.0f}},
        {{-1.0f,  1.0f, 0.0f}, {0.0f, 1.0f}},
    };
    std::vector<unsigned int> indices ={
        0,1,3,
        1,2,3
    };

    std::vector<TextVertex> m_textvertices ={
        {{1.0f,  1.0f, 1.0f, 1.0f}},
        {{1.0f,  -1.0f, 1.0f, 0.0f}},
        {{-1.0f,  -1.0f, 0.0f, 0.0f}},
        {{-1.0f,  1.0f, 0.0f, 1.0f}},
    };
    m_vaoText = new VertexArray();
    m_vboText = new VertexBuffer(
        {
            {"vertex", ShaderDataType::Float4}
        },
        m_textvertices.data(), sizeof(TextVertex) * m_textvertices.size()
    );
    m_iboText = new IndexBuffer(indices.data(), indices.size());
    m_vaoText->Bind();
    m_iboText->Bind();
    m_vboText->Bind();
    m_vboText->ApplyLayout();
}
nimo::SceneRenderer::~SceneRenderer()
{
    delete m_vaoText;
    delete m_vboText;
    delete m_iboText;
}

void nimo::SceneRenderer::SetScene(std::shared_ptr<Scene> scene)
{
    m_scene = scene;
}
void nimo::SceneRenderer::Render(std::shared_ptr<FrameBuffer> target, const CameraComponent& cameraSettings, const TransformComponent& cameraTransform)
{
    m_totalFrameTimer.Reset();
    glViewport(0, 0, target ? target->GetDetails().width : Application::Instance().GetWindow().GetWidth(), target ? target->GetDetails().height : Application::Instance().GetWindow().GetHeight());
    auto camTransform = cameraTransform;
    auto cam = cameraSettings;
    glm::mat4 projection = glm::perspectiveFov(glm::radians(cam.FOV),
                                            target ? (float)target->GetDetails().width : (float)Application::Instance().GetWindow().GetWidth(),
                                            target ? (float)target->GetDetails().height : (float)Application::Instance().GetWindow().GetHeight(),
                                            cam.ClippingPlanes.Near, cam.ClippingPlanes.Far);
    glm::mat4 projectionOrtho = glm::ortho(
        -(target ? (float)target->GetDetails().width : (float)Application::Instance().GetWindow().GetWidth()) * 0.5f,
        (target ? (float)target->GetDetails().width : (float)Application::Instance().GetWindow().GetWidth()) * 0.5f,
        -(target ? (float)target->GetDetails().height : (float)Application::Instance().GetWindow().GetHeight()) * 0.5f, 
        (target ? (float)target->GetDetails().height : (float)Application::Instance().GetWindow().GetHeight()) * 0.5f, 
        -0.1f, cam.ClippingPlanes.Far);
    glm::mat4 viewMatrix = camTransform.GetView();
    auto viewPosition = glm::vec3(camTransform.Translation.x, camTransform.Translation.y, camTransform.Translation.z);

    m_geometryFrameTimer.Reset();
    // Render scene into gbuffer
    glEnable(GL_DEPTH_TEST);  
    glDepthMask(GL_TRUE);  
    m_gBuffer->Bind();
    m_scene->m_registry.view<ActiveComponent, IDComponent, MeshComponent, MeshRendererComponent>().each([&](ActiveComponent& active, IDComponent& id, MeshComponent& m, MeshRendererComponent& r) {
        if(!active.active) return;
        if(!r.material || !r.material->shader || !m.source) return;
        r.material->shader->use();
        r.material->Setup();
        r.material->shader->Set("viewPos", viewPosition);
        r.material->shader->Set("transform", m_scene->GetWorldSpaceTransformMatrix(m_scene->GetEntity(id.Id)));
        r.material->shader->Set("view", viewMatrix);
        r.material->shader->Set("projection", projection);
        Renderer::DrawMesh(*m.source->GetSubmesh(m.submeshIndex));
    });
    m_geometryFrameTimer.Stop();

    m_lightingFrameTimer.Reset();
    // Render scene into directional light depth buffer
    auto directionalLightEntities = m_scene->m_registry.view<DirectionalLightComponent>();
    auto directionalLightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 100.0f);
    if(directionalLightEntities.size())
    {
        Entity directionalLight(*directionalLightEntities.begin(), m_scene->m_registry);
        glCullFace(GL_FRONT);
        m_directionalLightDepthBuffer->Bind();
        m_shaderDepth->use();
        auto directionalLightView = directionalLight.GetComponent<TransformComponent>().GetView();
        m_shaderDepth->Set("projection", directionalLightProjection);
        m_shaderDepth->Set("view", directionalLightView);
        m_scene->m_registry.view<ActiveComponent, IDComponent, MeshComponent>().each([&](ActiveComponent& active, IDComponent& id, MeshComponent& m) {
            if(!active.active) return;
            if(!m.source) return;
            m_shaderDepth->Set("transform", m_scene->GetWorldSpaceTransformMatrix(m_scene->GetEntity(id.Id)));
            Renderer::DrawMesh(*m.source->GetSubmesh(m.submeshIndex));
        });
        glCullFace(GL_BACK);
    }
    // Lighting pass
    m_hdrColorBuffer->Bind();
    m_shaderLightingPass->use();
    m_shaderLightingPass->Set("gPosition", 0);
    m_shaderLightingPass->Set("gNormal", 1);
    m_shaderLightingPass->Set("gAlbedo", 2);
    m_shaderLightingPass->Set("gARM", 3);
    m_shaderLightingPass->Set("gDepth", 4);
    m_shaderLightingPass->Set("InvProjection", glm::inverse(projection));
    m_gBuffer->BindColorTexture(0,0);
    m_gBuffer->BindColorTexture(1,1);
    m_gBuffer->BindColorTexture(2,2);
    m_gBuffer->BindColorTexture(3,3);
    m_gBuffer->BindDepthTexture(4);
    if(directionalLightEntities.size())
    {
        Entity directionalLight(*directionalLightEntities.begin(), m_scene->m_registry);
        auto directionalLightPosition = directionalLight.GetComponent<TransformComponent>().Translation;
        auto directionalLightView = directionalLight.GetComponent<TransformComponent>().GetView();
        m_shaderLightingPass->Set("directionalLightShadowMap", 5);
        m_directionalLightDepthBuffer->BindDepthTexture(5);
        m_shaderLightingPass->Set("directionalLightSpaceMatrix", directionalLightProjection * directionalLightView);
        m_shaderLightingPass->Set("directionalLightPos", directionalLightPosition);
        m_shaderLightingPass->Set("directionalLightColor", directionalLight.GetComponent<DirectionalLightComponent>().Color);
        m_shaderLightingPass->Set("directionalLightIntensity", directionalLight.GetComponent<DirectionalLightComponent>().Intensity);
    }
    int currentLights = 0;
    m_scene->m_registry.view<IDComponent, ActiveComponent, PointLightComponent, TransformComponent>().each([&](IDComponent id, ActiveComponent active,PointLightComponent& light, TransformComponent& lightTransform)
    {
        if(!active.active) return;
        glm::vec3 scale;
        glm::quat rotation;
        glm::vec3 translation;
        glm::vec3 skew;
        glm::vec4 perspective;
        glm::decompose(m_scene->GetWorldSpaceTransformMatrix(m_scene->GetEntity(id.Id)), scale, rotation, translation, skew, perspective);
        m_shaderLightingPass->Set("lights[" + std::to_string(currentLights) + "].Position", translation);
        m_shaderLightingPass->Set("lights[" + std::to_string(currentLights) + "].Color", light.Color);
        m_shaderLightingPass->Set("lights[" + std::to_string(currentLights) + "].Intensity", light.Intensity);
        static const float constant = 1.0f; // note that we don't send this to the shader, we assume it is always 1.0 (in our case)
        static const float linear = 0.7f;
        static const float quadratic = 1.8f;
        m_shaderLightingPass->Set("lights[" + std::to_string(currentLights) + "].Linear", linear);
        m_shaderLightingPass->Set("lights[" + std::to_string(currentLights) + "].Quadratic", quadratic);
        // then calculate radius of light volume/sphere
        const float maxBrightness = std::fmaxf(std::fmaxf(light.Color.r, light.Color.g), light.Color.b) * light.Intensity;
        float radius = (-linear + std::sqrt(linear * linear - 4 * quadratic * (constant - (256.0f / 5.0f) * maxBrightness))) / (2.0f * quadratic);
        m_shaderLightingPass->Set("lights[" + std::to_string(currentLights) + "].Radius", radius);
        m_shaderLightingPass->Set("lights[" + std::to_string(currentLights) + "].Active", true);
        currentLights++;
    });
    for(int i = currentLights; i < 32; ++i)
    {
        m_shaderLightingPass->Set("lights[" + std::to_string(i) + "].Active", false);
    }
    m_shaderLightingPass->Set("viewPos", viewPosition);
    auto skyLightEntities = m_scene->m_registry.view<SkyLightComponent>();
    if(skyLightEntities.size())
    {
        Entity skyLight(*skyLightEntities.begin(), m_scene->m_registry);
        if(skyLight.GetComponent<SkyLightComponent>().environment)
        {
            m_shaderLightingPass->Set("irradianceMap", 8);
            skyLight.GetComponent<SkyLightComponent>().environment->BindIrradiance(8);
        }
    }
    Renderer::DrawFullScreenQuad();
    m_lightingFrameTimer.Stop();

    // Background pass
    glDepthFunc(GL_LEQUAL);
    m_gBuffer->CopyDepthTo(m_hdrColorBuffer);
    m_backgroundPass->use();
    m_backgroundPass->Set("view", viewMatrix);
    m_backgroundPass->Set("projection", projection);
    if(skyLightEntities.size())
    {
        Entity skyLight(*skyLightEntities.begin(), m_scene->m_registry);
        if(skyLight.GetComponent<SkyLightComponent>().environment)
        {
            m_backgroundPass->Set("environmentMap", 0);
            skyLight.GetComponent<SkyLightComponent>().environment->Bind(0);
        }
    }
    renderCube2();
    glDepthFunc(GL_LESS);

    m_bloomFrameTimer.Reset();
    // Bloom
    // Get bright pixels in buffer
    m_hdrBrightnessBuffer->Bind(); //960x520
    m_hdrBrightFilterPass->use();
    m_hdrBrightFilterPass->Set("bloomThreshold", 1.2f);
    m_hdrBrightFilterPass->Set("hdrBuffer", 0);
    m_hdrColorBuffer->BindColorTexture(0,0);
    Renderer::DrawFullScreenQuad();
    // Downsample
    m_hdrBloomDownsample1Buffer->Bind(); //480x270
    m_hdrBloomDownsamplePass->use();
    m_hdrBloomDownsamplePass->Set("hdrBuffer", 0);
    m_hdrBrightnessBuffer->BindColorTexture(0,0); //960x540
    m_hdrBloomDownsamplePass->Set("textureResolution", glm::vec2(960.0f, 540.0f));
    Renderer::DrawFullScreenQuad();
    m_hdrBloomDownsample2Buffer->Bind(); //240x135
    m_hdrBloomDownsample1Buffer->BindColorTexture(0,0); //480x270
    m_hdrBloomDownsamplePass->Set("textureResolution", glm::vec2(480.0f, 270.0f)); 
    Renderer::DrawFullScreenQuad();
    m_hdrBloomDownsample3Buffer->Bind(); //120x67
    m_hdrBloomDownsample2Buffer->BindColorTexture(0,0); //240x135
    m_hdrBloomDownsamplePass->Set("textureResolution", glm::vec2(240.0f, 135.0f));
    Renderer::DrawFullScreenQuad();
    m_hdrBloomDownsample4Buffer->Bind(); //60x33
    m_hdrBloomDownsample3Buffer->BindColorTexture(0,0); //120x67
    m_hdrBloomDownsamplePass->Set("textureResolution", glm::vec2(120.0f, 67.0f)); 
    Renderer::DrawFullScreenQuad();
    m_hdrBloomDownsample5Buffer->Bind(); //30x16
    m_hdrBloomDownsample4Buffer->BindColorTexture(0,0); //60x33
    m_hdrBloomDownsamplePass->Set("textureResolution", glm::vec2(60.0f, 33.0f)); 
    Renderer::DrawFullScreenQuad();
    m_hdrBloomDownsample6Buffer->Bind(); //15x8
    m_hdrBloomDownsample5Buffer->BindColorTexture(0,0); //30x16
    m_hdrBloomDownsamplePass->Set("textureResolution", glm::vec2(30.0f, 16.0f)); 
    Renderer::DrawFullScreenQuad();
    //Upsample
    m_hdrBloomUpsample6Buffer->Bind(); //30x16
    m_hdrBloomUpsamplePass->use();
    m_hdrBloomUpsamplePass->Set("textureBig", 0);
    m_hdrBloomDownsample5Buffer->BindColorTexture(0,0); //30x16
    m_hdrBloomUpsamplePass->Set("textureSmall", 1);
    m_hdrBloomDownsample6Buffer->BindColorTexture(0,1); //15x8
    m_hdrBloomDownsamplePass->Set("textureResolution", glm::vec2(15.0f, 8.0f)); 
    Renderer::DrawFullScreenQuad();
    m_hdrBloomUpsample5Buffer->Bind(); //60x33
    m_hdrBloomUpsamplePass->use();
    m_hdrBloomUpsamplePass->Set("textureBig", 0);
    m_hdrBloomDownsample4Buffer->BindColorTexture(0,0); //60x33
    m_hdrBloomUpsamplePass->Set("textureSmall", 1);
    m_hdrBloomUpsample6Buffer->BindColorTexture(0,1); //30x16
    m_hdrBloomDownsamplePass->Set("textureResolution", glm::vec2(30.0f, 16.0f)); 
    Renderer::DrawFullScreenQuad();
    m_hdrBloomUpsample4Buffer->Bind(); //120x67
    m_hdrBloomUpsamplePass->use();
    m_hdrBloomUpsamplePass->Set("textureBig", 0);
    m_hdrBloomDownsample3Buffer->BindColorTexture(0,0); //120x67
    m_hdrBloomUpsamplePass->Set("textureSmall", 1);
    m_hdrBloomUpsample5Buffer->BindColorTexture(0,1); //60x33
    m_hdrBloomDownsamplePass->Set("textureResolution", glm::vec2(60.0f, 33.0f)); 
    Renderer::DrawFullScreenQuad();
    m_hdrBloomUpsample3Buffer->Bind(); //240x135
    m_hdrBloomUpsamplePass->use();
    m_hdrBloomUpsamplePass->Set("textureBig", 0);
    m_hdrBloomDownsample2Buffer->BindColorTexture(0,0); //240x135
    m_hdrBloomUpsamplePass->Set("textureSmall", 1);
    m_hdrBloomUpsample4Buffer->BindColorTexture(0,1); //120x67
    m_hdrBloomDownsamplePass->Set("textureResolution", glm::vec2(120.0f, 67.0f)); 
    Renderer::DrawFullScreenQuad();
    m_hdrBloomUpsample2Buffer->Bind(); //480x270
    m_hdrBloomUpsamplePass->use();
    m_hdrBloomUpsamplePass->Set("textureBig", 0);
    m_hdrBloomDownsample1Buffer->BindColorTexture(0,0); //480x270
    m_hdrBloomUpsamplePass->Set("textureSmall", 1);
    m_hdrBloomUpsample3Buffer->BindColorTexture(0,1); //240x135
    m_hdrBloomDownsamplePass->Set("textureResolution", glm::vec2(240.0f, 135.0f)); 
    Renderer::DrawFullScreenQuad();
    m_hdrBloomUpsample1Buffer->Bind(); //960x540
    m_hdrBloomUpsamplePass->use();
    m_hdrBloomUpsamplePass->Set("textureBig", 0);
    m_hdrBrightnessBuffer->BindColorTexture(0,0); //960x540
    m_hdrBloomUpsamplePass->Set("textureSmall", 1);
    m_hdrBloomUpsample2Buffer->BindColorTexture(0,1); //480x270
    m_hdrBloomDownsamplePass->Set("textureResolution", glm::vec2(480.0f, 270.0f)); 
    Renderer::DrawFullScreenQuad();

    m_hdrFinalBloomBuffer->Bind(); //1920x1080
    m_hdrBloomUpsamplePass->use();
    m_hdrBloomUpsamplePass->Set("textureBig", 0);
    m_hdrColorBuffer->BindColorTexture(0,0); //1920x1080
    m_hdrBloomUpsamplePass->Set("textureSmall", 1);
    m_hdrBloomUpsample1Buffer->BindColorTexture(0,1); //960x540
    m_hdrBloomDownsamplePass->Set("textureResolution", glm::vec2(960.0f, 540.0f)); 
    Renderer::DrawFullScreenQuad();
    m_bloomFrameTimer.Stop();

    // HDR tone mapping pass
    if(target)
        target->Bind();
    else
    {
        FrameBuffer::Unbind();
        glViewport(0, 0, Application::Instance().GetWindow().GetWidth(), Application::Instance().GetWindow().GetHeight());
    }
    m_hdrToneMappingPass->use();
    m_hdrToneMappingPass->Set("hdrBuffer", 0);
    m_hdrFinalBloomBuffer->BindColorTexture(0,0);
    Renderer::DrawFullScreenQuad();

    m_geometry2DFrameTimer.Reset();
    // glDepthMask(GL_FALSE);  // disable writes to Z-Buffer
    glDisable(GL_DEPTH_TEST);  // disable depth-testing
    glEnable(GL_BLEND); // enable blend
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    m_shader2d->use();
    m_shader2d->Set("view", viewMatrix);
    m_shader2d->Set("projection", projectionOrtho);
    m_shader2d->Set("mainTexture", 0);
    auto draw2DLayer = [&](int layer)
    {
        m_scene->m_registry.view<ActiveComponent, IDComponent, SpriteRendererComponent>().each([&](ActiveComponent& active, IDComponent& id, SpriteRendererComponent& r) {
            if(!active.active) return;
            if(r.layer != layer) return;
            m_shader2d->Set("transform", m_scene->GetWorldSpaceTransformMatrix(m_scene->GetEntity(id.Id)));
            if(!r.texture)
            {
                m_white->bind(0);
            }
            else
            {
                r.texture->bind(0);
            }
            m_shader2d->Set("color", r.Color);
            m_shader2d->Set("tiling", r.tiling);
            m_shader2d->Set("offset", r.offset);
            Renderer::DrawQuad();
        });
    };
    draw2DLayer(0);
    draw2DLayer(1);
    draw2DLayer(2);
    draw2DLayer(3);
    draw2DLayer(4);
    draw2DLayer(5);
    draw2DLayer(6);
    draw2DLayer(7);
    draw2DLayer(8);
    draw2DLayer(9);
    m_shaderText->use();
    m_shaderText->Set("projection", projectionOrtho);
    m_shaderText->Set("text", 0);
    m_scene->m_registry.view<ActiveComponent, IDComponent, TransformComponent, TextRendererComponent>().each([&](ActiveComponent active, IDComponent& id, TransformComponent& t, TextRendererComponent& r) {
        if(!active.active) return;
        if(!r.font) return;
        float x = .0f;
        float y = .0f;
        glm::vec3 scale;
        glm::quat rotation;
        glm::vec3 translation;
        glm::vec3 skew;
        glm::vec4 perspective;
        glm::decompose(m_scene->GetWorldSpaceTransformMatrix(m_scene->GetEntity(id.Id)), scale, rotation, translation, skew, perspective);
        m_shaderText->Set("color", r.Color);
        for(std::string::const_iterator c = r.text.begin(); c != r.text.end(); ++c)
        {
            auto& glyphIt = r.font->m_glyphs.find(*c);
            if(glyphIt == r.font->m_glyphs.end()) continue;
            auto& glyph = glyphIt->second;
            if(*c == '\n')
            {
                y -= (r.font->lineSpacing >> 6) * t.Scale.y;
                x = 0.0f;
                continue;
            }
            float xpos = x + ((float)glyph.bearing.x + (float)glyph.size.x * 0.5f)* t.Scale.x;
            float ypos = y + ((float)glyph.bearing.y) * 0.5f * t.Scale.y - ((float)glyph.size.y - (float)glyph.bearing.y) * 0.5f * t.Scale.y;
            float w = (float)glyph.size.x * t.Scale.x;
            float h = (float)glyph.size.y * t.Scale.y;
            // model = glm::translate(model, glm::vec3(xpos , ypos, .0f));

            TransformComponent t2;
            t2.Translation = translation + glm::vec3(xpos, ypos, 0.0f);
            t2.Scale = scale * glm::vec3((float)glyph.size.x, (float)glyph.size.y, 1.0f);
            m_shaderText->Set("transform", t2.GetTransform());
            glyph.texture->bind(0);
            Renderer::DrawQuad();
            // std::vector<TextVertex> textvertices ={
            //     {{xpos + w,  ypos, 1.0f, 1.0f}},
            //     {{xpos + w,  ypos + h, 1.0f, 0.0f}},
            //     {{xpos,  ypos + h, 0.0f, 0.0f}},
            //     {{xpos,  ypos, 0.0f, 1.0f}},
            // };
            // glyph.texture->bind(0);
            // m_vaoText->Bind();
            // m_vboText->Bind();
            // m_vboText->SetData(textvertices.data(), sizeof(TextVertex) * textvertices.size());
            // m_vboText->Unbind();
            // glDrawElements(GL_TRIANGLES, m_iboText->Count(), GL_UNSIGNED_INT, 0);
            x += ((glyph.advance.x >> 6) + r.characterSpacing) * t.Scale.x;
            if(*c == '\n')
            {
                y -= (r.font->lineSpacing >> 6) * t.Scale.y;
                x = 0.0f;
            }
        }
    });

    glEnable(GL_DEPTH_TEST);  
    // glDepthMask(GL_TRUE);  
    glDisable(GL_BLEND);  
    m_geometry2DFrameTimer.Stop();
    m_totalFrameTimer.Stop();
    m_scene = {};
}
