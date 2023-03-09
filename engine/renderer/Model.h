#pragma once

#include "core/types.h"
#include "Mesh.h"
#include <string>
#include <memory>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "core/defines.h"

namespace nimo {
    class JAR_API Model{
    public:
        Model(const std::string& path);
        ~Model();
        void Draw(const Shader& sh);
    
    private:
        void LoadModel(const std::string& path);
        void ProcessNode(aiNode* node, const aiScene* scene);
        std::shared_ptr<Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene);
        std::vector<std::shared_ptr<Mesh>> m_meshes;
        std::string m_directory;
    };
};