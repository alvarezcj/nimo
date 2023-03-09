#include "glad/glad.h"

#include "VertexArray.h"


nimo::VertexArray::VertexArray()
{
    glGenVertexArrays(1, &m_id);
}

nimo::VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &m_id);
}

void nimo::VertexArray::bind()
{
    glBindVertexArray(m_id);
}

void nimo::VertexArray::unbind()
{
    glBindVertexArray(0);
}