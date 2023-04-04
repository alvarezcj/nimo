#include "EnvironmentMap.h"
#include <glad/glad.h>
#include "core/Log.h"
#include "stb_image.h"

nimo::EnvironmentMap::EnvironmentMap(const std::string& path)
{
    NIMO_DEBUG("nimo::EnvironmentMap::EnvironmentMap({})",path);
    stbi_set_flip_vertically_on_load(true);
    int components;
    float* data = stbi_loadf(path.c_str(), (int*)&m_width, (int*)&m_height, &components, 0);
    if(data)
    {
        glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
        glTextureStorage2D(m_id, 1, GL_RGB16F, m_width, m_height);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTextureSubImage2D(m_id, 0, 0, 0, m_width, m_height, GL_RGB, GL_UNSIGNED_BYTE, data);

        stbi_image_free(data);
    }
    else
    {
        NIMO_ERROR("Could not load {} as environment map!", path);
    }
}

nimo::EnvironmentMap::~EnvironmentMap()
{
    NIMO_DEBUG("nimo::EnvironmentMap::~EnvironmentMap");
    glDeleteTextures(1, &m_id);
}

void nimo::EnvironmentMap::bind(unsigned int slot)
{
    glBindTextureUnit(slot, m_id);
}
