#pragma once
#include <string>
#include <vector>
#include "glm/glm.hpp"
#include "assets/Asset.h"

namespace nimo
{
    struct Vertex{
        glm::vec3 position;
        glm::vec2 uv;
    };
    class MeshSource : public Asset
    {
    public:
        MeshSource(const std::string& file);
        ~MeshSource();
        AssetType Type() const { return AssetType::Mesh; }
        static AssetType StaticType(){return AssetType::Mesh;}

        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        static AssetType GetType(){ return AssetType::Mesh;}
    };
};