#pragma once
#include "scene/Scene.h"

namespace nimo
{
class SceneRenderer
{
public:
    SceneRenderer();
    ~SceneRenderer();
    void SetScene(std::shared_ptr<Scene> scene);
    void Render(std::shared_ptr<FrameBuffer> target);
private:
    std::shared_ptr<Scene> m_scene;
    std::shared_ptr<FrameBuffer> m_gBuffer;
    std::shared_ptr<Shader> m_shaderLightingPass;
    std::shared_ptr<Mesh> m_quadMesh;
};  
} // namespace nimo
