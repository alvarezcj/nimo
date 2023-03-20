#pragma once
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include <memory>
#include "assets/Asset.h"
#include "glm/glm.hpp"

namespace nimo{
    struct Vertex{
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 uv;
    };
    class Mesh : public Asset{
    public:
        Mesh(const std::string& file);
        ~Mesh();
        AssetType Type() const { return AssetType::Mesh; }
        static AssetType StaticType(){return AssetType::Mesh;}
        void draw();
        VertexArray* m_vao;
        VertexBuffer* m_vbo;
        IndexBuffer* m_ibo;
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
    };
};