#include "Mesh.h"
#include "glad/glad.h"

nimo::Mesh::Mesh(std::shared_ptr<MeshSource> source)
    : m_source(source)
    , m_vao()
    , m_vbo(
        {
            {"position", ShaderDataType::Float3},
            {"uv", ShaderDataType::Float2}
        },
        m_source->vertices.data(), sizeof(Vertex) * m_source->vertices.size()
    )
    , m_ibo(m_source->indices.data(), m_source->indices.size())
{
    m_vao.bind();
    m_ibo.bind();
    m_vbo.bind();
    m_vbo.applyLayout();
}
nimo::Mesh::~Mesh()
{

}
void nimo::Mesh::draw()
{
    m_vao.bind();
    glDrawElements(GL_TRIANGLES, m_ibo.count(), GL_UNSIGNED_INT, 0);
}