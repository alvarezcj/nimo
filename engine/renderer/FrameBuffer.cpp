#include "FrameBuffer.h"
#include "glad/glad.h"
#include "core/Log.h"

nimo::FrameBuffer::FrameBuffer(const FrameBuffer::Details& details)
    : m_details(details)
{
    m_textureAttachments.resize(m_details.colorAttachments.size());
    // Create framebuffer
    glGenFramebuffers(1, &m_id);
    glBindFramebuffer(GL_FRAMEBUFFER, m_id);

    // Create color attachments
    for(int i = 0; i < m_details.colorAttachments.size(); i++)
    {
        // generate texture
        glGenTextures(1, &m_textureAttachments[i]);
        glBindTexture(GL_TEXTURE_2D, m_textureAttachments[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, m_details.colorAttachments[i].internalFormat, m_details.width, m_details.height, 0, m_details.colorAttachments[i].format, m_details.colorAttachments[i].type, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
        // attach it to currently bound framebuffer object
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_textureAttachments[i], 0);
    }
    std::vector<unsigned int> attachments;
    for(int i = 0; i < m_details.colorAttachments.size(); i++)
    {
        attachments.push_back(GL_COLOR_ATTACHMENT0 + i);
    }
    glDrawBuffers(m_details.colorAttachments.size(), attachments.data());

    // Create and attach depth buffer (renderbuffer)
    glGenRenderbuffers(1, &m_renderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_renderBuffer); 
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_details.width, m_details.height);  
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_renderBuffer);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        NIMO_ERROR("Framebuffer not completed");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
nimo::FrameBuffer::~FrameBuffer()
{
    glDeleteFramebuffers(1, &m_id);  
    glDeleteTextures(m_textureAttachments.size(), m_textureAttachments.data());
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
void nimo::FrameBuffer::BindColorTexture(int id, unsigned int slot)
{
    //NIMO_DEBUG("Binding color attachment {} with value {} to slot {}", id, m_textureAttachments[id], slot);
    glBindTextureUnit(slot, m_textureAttachments[id]);
}