#pragma once
#include <memory>
#include "glm/glm.hpp"
#include "FrameBuffer.h"

namespace nimo{
    class Renderer{
        public:
            static void BeginFrame(std::shared_ptr<FrameBuffer> target = {});
            static void EndFrame();
            static void BeginRenderPass(const glm::mat4& view, const glm::mat4& projection);
            static void EndRenderPass();
    };
}