#include "FrameBuffer.h"
#include "glad/glad.h"

nimo::FrameBuffer::FrameBuffer(const FrameBuffer::Details& details)
    : m_details(details)
{
    glGenFramebuffers(1, &m_id);
    glBindFramebuffer(GL_FRAMEBUFFER, m_id);

    // generate texture
    glGenTextures(1, &m_textureAttachment);
    glBindTexture(GL_TEXTURE_2D, m_textureAttachment);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_details.width, m_details.height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    // attach it to currently bound framebuffer object
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureAttachment, 0);

    glGenRenderbuffers(1, &m_renderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_renderBuffer); 
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_details.width, m_details.height);  
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_renderBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
nimo::FrameBuffer::~FrameBuffer()
{
    glDeleteFramebuffers(1, &m_id);  
    glDeleteTextures(1, &m_textureAttachment);
    glDeleteRenderbuffers(1, &m_renderBuffer);
}
void nimo::FrameBuffer::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_id);
	glViewport(0, 0, m_details.width, m_details.height);
    if(m_details.clearColorOnBind)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(m_details.clearColor.x, m_details.clearColor.y, m_details.clearColor.z, m_details.clearColor.w); 
    }
    if(m_details.clearDepthOnBind)
    {
        glClear(GL_DEPTH_BUFFER_BIT);
        glClearDepthf(m_details.depthClearValue); 
    }
}
void nimo::FrameBuffer::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0); 
}
void nimo::FrameBuffer::BindColorTexture(unsigned int slot)
{
    glBindTextureUnit(slot, m_textureAttachment);
}