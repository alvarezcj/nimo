#pragma once
#include "scene/Scene.h"
#include "EnvironmentMap.h"

namespace nimo
{
class SceneRenderer
{
public:
    SceneRenderer();
    ~SceneRenderer();
    void SetScene(std::shared_ptr<Scene> scene);
    void Render(std::shared_ptr<FrameBuffer> target = {});
public:
    std::shared_ptr<Scene> m_scene;
    std::shared_ptr<FrameBuffer> m_gBuffer;
    std::shared_ptr<FrameBuffer> m_hdrColorBuffer;
    std::shared_ptr<FrameBuffer> m_hdrBrightnessBuffer;
    std::shared_ptr<FrameBuffer> m_hdrBloomDownsample1Buffer;
    std::shared_ptr<FrameBuffer> m_hdrBloomDownsample2Buffer;
    std::shared_ptr<FrameBuffer> m_hdrBloomDownsample3Buffer;
    std::shared_ptr<FrameBuffer> m_hdrBloomDownsample4Buffer;
    std::shared_ptr<FrameBuffer> m_hdrBloomDownsample5Buffer;
    std::shared_ptr<FrameBuffer> m_hdrBloomDownsample6Buffer;
    std::shared_ptr<FrameBuffer> m_hdrBloomUpsample1Buffer;
    std::shared_ptr<FrameBuffer> m_hdrBloomUpsample2Buffer;
    std::shared_ptr<FrameBuffer> m_hdrBloomUpsample3Buffer;
    std::shared_ptr<FrameBuffer> m_hdrBloomUpsample4Buffer;
    std::shared_ptr<FrameBuffer> m_hdrBloomUpsample5Buffer;
    std::shared_ptr<FrameBuffer> m_hdrBloomUpsample6Buffer;
    std::shared_ptr<FrameBuffer> m_hdrFinalBloomBuffer;
    std::shared_ptr<Shader> m_shaderLightingPass;
    std::shared_ptr<Shader> m_backgroundPass;
    std::shared_ptr<Shader> m_hdrToneMappingPass;
    std::shared_ptr<Shader> m_hdrBrightFilterPass;
    std::shared_ptr<Shader> m_hdrBloomDownsamplePass;
    std::shared_ptr<Shader> m_hdrBloomUpsamplePass;
    std::shared_ptr<EnvironmentMap> m_environmentMap;
    std::shared_ptr<Mesh> m_quadMesh;
};  
} // namespace nimo
