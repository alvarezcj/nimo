#pragma once
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include <memory>
#include <vector>
#include "assets/Asset.h"
#include "glm/glm.hpp"

namespace nimo{
    struct Vertex{
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 uv;
        glm::vec3 tangent;
        glm::vec3 bitangent;
    };
    struct QuadVertex{
        glm::vec3 position;
        glm::vec2 uv;
    };
    struct Submesh{
        Submesh();
        ~Submesh();
        void Submit();
        void Draw();
        VertexArray* m_vao = nullptr;
        VertexBuffer* m_vbo = nullptr;
        IndexBuffer* m_ibo = nullptr;
        std::vector<Vertex> m_vertices;
        std::vector<unsigned int> m_indices;
        std::string m_name;
    };
    class Mesh : public Asset{
    public:
        Mesh(const std::string& file, bool mergeMeshesByMaterial = true);
        Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
        ~Mesh();
        AssetType Type() const { return AssetType::Mesh; }
        static AssetType StaticType(){return AssetType::Mesh;}
        void draw(unsigned int i = 0);
        std::vector<std::shared_ptr<Submesh>> submeshes;
    private:
    };
};