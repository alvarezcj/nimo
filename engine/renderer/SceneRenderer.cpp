#include "SceneRenderer.h"
#include "glad/glad.h"
#include "core/Application.h"

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
    m_environmentMap = nimo::AssetManager::Get<EnvironmentMap>("Environment/old_room_4k.hdr");
    //Tone mapping shaderm_backgroundPass
    m_hdrToneMappingPass = nimo::AssetManager::Get<Shader>("Shaders/hdr_tone_mapping.nshader");
    //Bloom
    m_hdrBrightFilterPass = nimo::AssetManager::Get<Shader>("Shaders/hdr_bright_filter.nshader");
    m_hdrBloomDownsamplePass = nimo::AssetManager::Get<Shader>("Shaders/hdr_bloom_downsample.nshader");
    m_hdrBloomUpsamplePass = nimo::AssetManager::Get<Shader>("Shaders/hdr_bloom_upsample.nshader");
    //2D
    m_shader2d = nimo::AssetManager::Get<Shader>("Shaders/unlit_texture.nshader");
    m_shaderText = AssetManager::Get<Shader>("Shaders/text.nshader");

    //White texture in memory
    unsigned int whitePixel = 0xFFFFFFFF;
    m_white = std::make_shared<Texture>(1, 1, &whitePixel);
    //Black texture in memory
    unsigned int blackPixel = 0x00000000;
    m_black = std::make_shared<Texture>(1, 1, &blackPixel);

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

    std::vector<QuadVertex> m_vertices ={
        {{1.0f,  1.0f, 0.0f}, {1.0f, 1.0f}},
        {{1.0f,  -1.0f, 0.0f}, {1.0f, 0.0f}},
        {{-1.0f,  -1.0f, 0.0f}, {0.0f, 0.0f}},
        {{-1.0f,  1.0f, 0.0f}, {0.0f, 1.0f}},
    };
    m_vao = new VertexArray();
    m_vbo = new VertexBuffer(
        {
            {"position", ShaderDataType::Float3},
            {"uv", ShaderDataType::Float2},
        },
        m_vertices.data(), sizeof(QuadVertex) * m_vertices.size()
    );
    m_ibo = new IndexBuffer(indices.data(), indices.size());
    m_vao->bind();
    m_ibo->bind();
    m_vbo->bind();
    m_vbo->applyLayout();


    
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
    m_vaoText->bind();
    m_iboText->bind();
    m_vboText->bind();
    m_vboText->applyLayout();
}
nimo::SceneRenderer::~SceneRenderer()
{
    delete m_vao;
    delete m_vbo;
    delete m_ibo;
}

void nimo::SceneRenderer::SetScene(std::shared_ptr<Scene> scene)
{
    m_scene = scene;
}
void nimo::SceneRenderer::Render(std::shared_ptr<FrameBuffer> target)
{
    totalFrameTimer.Reset();
    geometryFrameTimer.Reset();
    Entity camera(*m_scene->m_registry.view<CameraComponent>().begin(), m_scene->m_registry);
    auto camTransform = camera.GetComponent<TransformComponent>();
    auto cam = camera.GetComponent<CameraComponent>();
    glm::mat4 projection = glm::perspectiveFov(glm::radians(cam.FOV), (float)Application::Instance().GetWindow().GetWidth() , (float)Application::Instance().GetWindow().GetHeight(), cam.ClippingPlanes.Near, cam.ClippingPlanes.Far);
    glm::mat4 projectionOrtho = glm::ortho(
        -(float)Application::Instance().GetWindow().GetWidth() * 0.5f,
        (float)Application::Instance().GetWindow().GetWidth() * 0.5f,
        -(float)Application::Instance().GetWindow().GetHeight() * 0.5f, 
        (float)Application::Instance().GetWindow().GetHeight() * 0.5f, 
        -0.1f, cam.ClippingPlanes.Far);
    glm::mat4 viewMatrix = camTransform.GetView();
    auto viewPosition = glm::vec3(camTransform.Translation.x, camTransform.Translation.y, camTransform.Translation.z);
    // Render scene into gbuffer
    m_gBuffer->bind();
    m_scene->m_registry.view<ActiveComponent, IDComponent, MeshComponent, MeshRendererComponent>().each([&](ActiveComponent& active, IDComponent& id, MeshComponent& m, MeshRendererComponent& r) {
        if(!active.active) return;
        if(!r.material || !r.material->shader || !m.source) return;
        r.material->shader->use();
        r.material->Setup();
        r.material->shader->set("viewPos", viewPosition);
        r.material->shader->set("transform", m_scene->GetWorldSpaceTransformMatrix(m_scene->GetEntity(id.Id)));
        r.material->shader->set("view", viewMatrix);
        r.material->shader->set("projection", projection);
        m.source->draw(m.submeshIndex);
    });
    geometryFrameTimer.Stop();

    lightingFrameTimer.Reset();
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
    m_scene->m_registry.view<ActiveComponent, PointLightComponent, TransformComponent>().each([&](ActiveComponent active,PointLightComponent& light, TransformComponent& lightTransform)
    {
        if(!active.active) return;
        m_shaderLightingPass->set("lights[" + std::to_string(currentLights) + "].Position", lightTransform.Translation);
        m_shaderLightingPass->set("lights[" + std::to_string(currentLights) + "].Color", light.Color);
        m_shaderLightingPass->set("lights[" + std::to_string(currentLights) + "].Intensity", light.Intensity);
        static const float constant = 1.0f; // note that we don't send this to the shader, we assume it is always 1.0 (in our case)
        static const float linear = 0.7f;
        static const float quadratic = 1.8f;
        m_shaderLightingPass->set("lights[" + std::to_string(currentLights) + "].Linear", linear);
        m_shaderLightingPass->set("lights[" + std::to_string(currentLights) + "].Quadratic", quadratic);
        // then calculate radius of light volume/sphere
        const float maxBrightness = std::fmaxf(std::fmaxf(light.Color.r, light.Color.g), light.Color.b) * light.Intensity;
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
    lightingFrameTimer.Stop();

    // Background pass
    glDepthFunc(GL_LEQUAL);
    m_gBuffer->CopyDepthTo(m_hdrColorBuffer);
    m_backgroundPass->use();
    m_backgroundPass->set("view", viewMatrix);
    m_backgroundPass->set("projection", projection);
    m_backgroundPass->set("environmentMap", 0);
    m_environmentMap->bind(0);
    renderCube2();
    glDepthFunc(GL_LESS);

    bloomFrameTimer.Reset();
    // Bloom
    // Get bright pixels in buffer
    m_hdrBrightnessBuffer->bind(); //960x520
    m_hdrBrightFilterPass->use();
    m_hdrBrightFilterPass->set("hdrBuffer", 0);
    m_hdrColorBuffer->BindColorTexture(0,0);
    m_quadMesh->draw();
    // Downsample
    m_hdrBloomDownsample1Buffer->bind(); //480x270
    m_hdrBloomDownsamplePass->use();
    m_hdrBloomDownsamplePass->set("hdrBuffer", 0);
    m_hdrBrightnessBuffer->BindColorTexture(0,0); //960x540
    m_hdrBloomDownsamplePass->set("textureResolution", glm::vec2(960.0f, 540.0f));
    m_quadMesh->draw();
    m_hdrBloomDownsample2Buffer->bind(); //240x135
    m_hdrBloomDownsample1Buffer->BindColorTexture(0,0); //480x270
    m_hdrBloomDownsamplePass->set("textureResolution", glm::vec2(480.0f, 270.0f)); 
    m_quadMesh->draw();
    m_hdrBloomDownsample3Buffer->bind(); //120x67
    m_hdrBloomDownsample2Buffer->BindColorTexture(0,0); //240x135
    m_hdrBloomDownsamplePass->set("textureResolution", glm::vec2(240.0f, 135.0f));
    m_quadMesh->draw();
    m_hdrBloomDownsample4Buffer->bind(); //60x33
    m_hdrBloomDownsample3Buffer->BindColorTexture(0,0); //120x67
    m_hdrBloomDownsamplePass->set("textureResolution", glm::vec2(120.0f, 67.0f)); 
    m_quadMesh->draw();
    m_hdrBloomDownsample5Buffer->bind(); //30x16
    m_hdrBloomDownsample4Buffer->BindColorTexture(0,0); //60x33
    m_hdrBloomDownsamplePass->set("textureResolution", glm::vec2(60.0f, 33.0f)); 
    m_quadMesh->draw();
    m_hdrBloomDownsample6Buffer->bind(); //15x8
    m_hdrBloomDownsample5Buffer->BindColorTexture(0,0); //30x16
    m_hdrBloomDownsamplePass->set("textureResolution", glm::vec2(30.0f, 16.0f)); 
    m_quadMesh->draw();
    //Upsample
    m_hdrBloomUpsample6Buffer->bind(); //30x16
    m_hdrBloomUpsamplePass->use();
    m_hdrBloomUpsamplePass->set("textureBig", 0);
    m_hdrBloomDownsample5Buffer->BindColorTexture(0,0); //30x16
    m_hdrBloomUpsamplePass->set("textureSmall", 1);
    m_hdrBloomDownsample6Buffer->BindColorTexture(0,1); //15x8
    m_hdrBloomDownsamplePass->set("textureResolution", glm::vec2(15.0f, 8.0f)); 
    m_quadMesh->draw();
    m_hdrBloomUpsample5Buffer->bind(); //60x33
    m_hdrBloomUpsamplePass->use();
    m_hdrBloomUpsamplePass->set("textureBig", 0);
    m_hdrBloomDownsample4Buffer->BindColorTexture(0,0); //60x33
    m_hdrBloomUpsamplePass->set("textureSmall", 1);
    m_hdrBloomUpsample6Buffer->BindColorTexture(0,1); //30x16
    m_hdrBloomDownsamplePass->set("textureResolution", glm::vec2(30.0f, 16.0f)); 
    m_quadMesh->draw();
    m_hdrBloomUpsample4Buffer->bind(); //120x67
    m_hdrBloomUpsamplePass->use();
    m_hdrBloomUpsamplePass->set("textureBig", 0);
    m_hdrBloomDownsample3Buffer->BindColorTexture(0,0); //120x67
    m_hdrBloomUpsamplePass->set("textureSmall", 1);
    m_hdrBloomUpsample5Buffer->BindColorTexture(0,1); //60x33
    m_hdrBloomDownsamplePass->set("textureResolution", glm::vec2(60.0f, 33.0f)); 
    m_quadMesh->draw();
    m_hdrBloomUpsample3Buffer->bind(); //240x135
    m_hdrBloomUpsamplePass->use();
    m_hdrBloomUpsamplePass->set("textureBig", 0);
    m_hdrBloomDownsample2Buffer->BindColorTexture(0,0); //240x135
    m_hdrBloomUpsamplePass->set("textureSmall", 1);
    m_hdrBloomUpsample4Buffer->BindColorTexture(0,1); //120x67
    m_hdrBloomDownsamplePass->set("textureResolution", glm::vec2(120.0f, 67.0f)); 
    m_quadMesh->draw();
    m_hdrBloomUpsample2Buffer->bind(); //480x270
    m_hdrBloomUpsamplePass->use();
    m_hdrBloomUpsamplePass->set("textureBig", 0);
    m_hdrBloomDownsample1Buffer->BindColorTexture(0,0); //480x270
    m_hdrBloomUpsamplePass->set("textureSmall", 1);
    m_hdrBloomUpsample3Buffer->BindColorTexture(0,1); //240x135
    m_hdrBloomDownsamplePass->set("textureResolution", glm::vec2(240.0f, 135.0f)); 
    m_quadMesh->draw();
    m_hdrBloomUpsample1Buffer->bind(); //960x540
    m_hdrBloomUpsamplePass->use();
    m_hdrBloomUpsamplePass->set("textureBig", 0);
    m_hdrBrightnessBuffer->BindColorTexture(0,0); //960x540
    m_hdrBloomUpsamplePass->set("textureSmall", 1);
    m_hdrBloomUpsample2Buffer->BindColorTexture(0,1); //480x270
    m_hdrBloomDownsamplePass->set("textureResolution", glm::vec2(480.0f, 270.0f)); 
    m_quadMesh->draw();

    m_hdrFinalBloomBuffer->bind(); //1920x1080
    m_hdrBloomUpsamplePass->use();
    m_hdrBloomUpsamplePass->set("textureBig", 0);
    m_hdrColorBuffer->BindColorTexture(0,0); //1920x1080
    m_hdrBloomUpsamplePass->set("textureSmall", 1);
    m_hdrBloomUpsample1Buffer->BindColorTexture(0,1); //960x540
    m_hdrBloomDownsamplePass->set("textureResolution", glm::vec2(960.0f, 540.0f)); 
    m_quadMesh->draw();
    bloomFrameTimer.Stop();

    // HDR tone mapping pass
    if(target)
        target->bind();
    else
    {
        FrameBuffer::unbind();
        glViewport(0, 0, Application::Instance().GetWindow().GetWidth(), Application::Instance().GetWindow().GetHeight());
    }
    m_hdrToneMappingPass->use();
    m_hdrToneMappingPass->set("hdrBuffer", 0);
    m_hdrFinalBloomBuffer->BindColorTexture(0,0);
    m_quadMesh->draw();

    geometry2DFrameTimer.Reset();
    glDepthMask(GL_FALSE);  // disable writes to Z-Buffer
    glDisable(GL_DEPTH_TEST);  // disable depth-testing
    glEnable(GL_BLEND); // enable blend
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    m_shader2d->use();
    m_shader2d->set("view", viewMatrix);
    m_shader2d->set("projection", projectionOrtho);
    m_shader2d->set("mainTexture", 0);
    m_scene->m_registry.view<ActiveComponent, IDComponent, SpriteRendererComponent>().each([&](ActiveComponent& active, IDComponent& id, SpriteRendererComponent& r) {
        if(!active.active) return;
        m_shader2d->set("transform", m_scene->GetWorldSpaceTransformMatrix(m_scene->GetEntity(id.Id)));
        if(!r.texture)
        {
            m_white->bind(0);
        }
        else
        {
            r.texture->bind(0);
        }
        m_shader2d->set("color", r.Color);
        m_shader2d->set("tiling", r.tiling);
        m_shader2d->set("offset", r.offset);
        m_vao->bind();
        glDrawElements(GL_TRIANGLES, m_ibo->count(), GL_UNSIGNED_INT, 0);
    });
    m_shaderText->use();
    m_shaderText->set("projection", projectionOrtho);
    m_shaderText->set("text", 0);
    m_scene->m_registry.view<ActiveComponent, IDComponent, TransformComponent, TextRendererComponent>().each([&](ActiveComponent active, IDComponent& id, TransformComponent& t, TextRendererComponent& r) {
        if(!active.active) return;
        if(!r.font) return;
        float x = .0f;
        float y = .0f;
        m_shaderText->set("transform", m_scene->GetWorldSpaceTransformMatrix(m_scene->GetEntity(id.Id)));
        m_shaderText->set("color", r.Color);
        for(std::string::const_iterator c = r.text.begin(); c != r.text.end(); ++c)
        {
            auto& glyphIt = r.font->m_glyphs.find(*c);
            if(glyphIt == r.font->m_glyphs.end()) continue;
            auto& glyph = glyphIt->second;
            float xpos = x + (float)glyph.bearing.x * t.Scale.x;
            float ypos = y - ((float)glyph.size.y - glyph.bearing.y) * t.Scale.y;
            float w = (float)glyph.size.x * t.Scale.x;
            float h = (float)glyph.size.y * t.Scale.y;
            std::vector<TextVertex> textvertices ={
                {{xpos + w,  ypos, 1.0f, 1.0f}},
                {{xpos + w,  ypos + h, 1.0f, 0.0f}},
                {{xpos,  ypos + h, 0.0f, 0.0f}},
                {{xpos,  ypos, 0.0f, 1.0f}},
            };
            glyph.texture->bind(0);
            m_vaoText->bind();
            m_vboText->bind();
            m_vboText->setData(textvertices.data(), sizeof(TextVertex) * textvertices.size());
            m_vboText->unbind();
            glDrawElements(GL_TRIANGLES, m_iboText->count(), GL_UNSIGNED_INT, 0);
            x += ((glyph.advance >> 6) + r.characterSpacing) * t.Scale.x;
        }
    });

    glDepthMask(GL_TRUE);  // disable writes to Z-Buffer
    glEnable(GL_DEPTH_TEST);  // disable depth-testing
    glDisable(GL_BLEND);  // disable blend
    geometry2DFrameTimer.Stop();
    totalFrameTimer.Stop();
    m_scene = {};
}
