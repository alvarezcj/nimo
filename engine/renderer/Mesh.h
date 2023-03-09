#pragma once
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "assets/MeshSource.h"
#include <memory>

namespace nimo{
    class Mesh{
    public:
        Mesh(std::shared_ptr<MeshSource> source);
        ~Mesh();
        void draw();
        std::shared_ptr<MeshSource> m_source;
        VertexArray m_vao;
        VertexBuffer m_vbo;
        IndexBuffer m_ibo;
    };
};