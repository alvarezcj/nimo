#pragma once
#include <glm/glm.hpp>

namespace nimo{
class FrameBuffer{
public:
    enum class BlendMode{
		None = 0,
		OneZero,
		SrcAlphaOneMinusSrcAlpha,
		Additive,
		Zero_SrcColor
    };
    struct Details{
		uint32_t width = 0;
		uint32_t height = 0;
		glm::vec4 clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
		float depthClearValue = 1.0f;
		bool clearColorOnBind = true;
		bool clearDepthOnBind = true;
        BlendMode blendMode = BlendMode::SrcAlphaOneMinusSrcAlpha;
    };
    FrameBuffer(const Details& details);
    ~FrameBuffer();
    void bind();
    static void unbind();
    void BindColorTexture(unsigned int slot);
    unsigned int GetColorAttachmentId() {return m_textureAttachment;}

private:
    unsigned int m_id;
    unsigned int m_textureAttachment;
    unsigned int m_renderBuffer;
    Details m_details;
};
};