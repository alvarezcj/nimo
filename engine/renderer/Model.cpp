#include "Model.h"
#include "core/AssetManager.h"
#include "core/ServiceLocator.h"
#include "core/Log.h"
#include "Material.h"

nimo::Model::Model(const std::string& path)
{
    LoadModel(path);
}
nimo::Model::~Model(){}

void nimo::Model::Draw(const Shader& sh)
{
    for (size_t i = 0; i < m_meshes.size(); ++i)
        m_meshes[i]->Draw(sh);
}

void nimo::Model::LoadModel(const std::string& path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    // check for errors
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        NIMO_ERROR("ERROR::ASSIMP:: {}",importer.GetErrorString());
        return;
    }
    // retrieve the directory path of the filepath
    m_directory = path.substr(0, path.find_last_of('\\'));

    // process ASSIMP's root node recursively
     ProcessNode(scene->mRootNode, scene);
}
// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
void nimo::Model::ProcessNode(aiNode* node, const aiScene* scene)
{
    // process each mesh located at the current node
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        // the node object only contains indices to index the actual objects in the scene. 
        // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        m_meshes.push_back(ProcessMesh(mesh, scene));
    }
    // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], scene);
    }
}
std::shared_ptr<nimo::Mesh> nimo::Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices(mesh->mNumVertices);
    std::vector<u32> indices;
    std::vector<std::shared_ptr<Texture>> textures;
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
        // positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.pos = vector;
        // normals
        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.normal = vector;
            // tangent
            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.tangent = vector;
            // bitangent
            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            vertex.bitangent = vector;
        }
        // texture coordinates
        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.tex_coords = vec;
        }
        else
            vertex.tex_coords = glm::vec2(0.0f, 0.0f);

        vertices[i] = vertex;
    }
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    nimo::ServiceLocator::get<nimo::AssetManager>()->bind<Material>(material->GetName().C_Str());
    auto jarMat = nimo::ServiceLocator::get<nimo::AssetManager>()->get<Material>(material->GetName().C_Str());
    
    //for (unsigned int i = 0; i < material->mNumProperties; i++)
    //{
    //    // std::cout << material->mProperties[i]->mKey.C_Str() << std::endl;
    //}
    aiColor4D diffuse;
    aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuse);
    jarMat->m_diffuse_color = { diffuse.r , diffuse.g , diffuse.b };
    aiColor4D ambient;
    aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &ambient);
    jarMat->m_ambient_color = { ambient.r , ambient.g , ambient.b };
    aiColor4D specular;
    aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &specular);
    jarMat->m_specular_color = { specular.r , specular.g , specular.b };
    float shininess;
    aiGetMaterialFloat(material, AI_MATKEY_SHININESS, &shininess);
    jarMat->m_shininess = shininess;
    float refraction;
    aiGetMaterialFloat(material, AI_MATKEY_REFRACTI, &refraction);
    jarMat->m_refraction_index = refraction;
    float dissolve;
    aiGetMaterialFloat(material, AI_MATKEY_OPACITY, &dissolve);
    jarMat->m_dissolve = dissolve;

    aiString str;
    static u8 white_texture_data[4] = {255,255,255,255};
    ServiceLocator::get<nimo::AssetManager>()->bind<Texture>("white", 1,1, white_texture_data);
    if (!material->GetTexture(aiTextureType_DIFFUSE, 0, &str))
    {
        ServiceLocator::get<nimo::AssetManager>()->bind<Texture>(m_directory + "\\" + str.C_Str(), m_directory + "\\" + str.C_Str());
        jarMat->m_diffuse_map = ServiceLocator::get<nimo::AssetManager>()->get<Texture>(m_directory + "\\" + str.C_Str());
    }
    else jarMat->m_diffuse_map = ServiceLocator::get<nimo::AssetManager>()->get<Texture>("white");
    if (!material->GetTexture(aiTextureType_SPECULAR, 0, &str))
    {
        ServiceLocator::get<nimo::AssetManager>()->bind<Texture>(m_directory + "\\" + str.C_Str(), m_directory + "\\" + str.C_Str());
        jarMat->m_specular_map = ServiceLocator::get<nimo::AssetManager>()->get<Texture>(m_directory + "\\" + str.C_Str());
    }
    else jarMat->m_specular_map = ServiceLocator::get<nimo::AssetManager>()->get<Texture>("white");
    if (!material->GetTexture(aiTextureType_AMBIENT, 0, &str))
    {
        ServiceLocator::get<nimo::AssetManager>()->bind<Texture>(m_directory + "\\" + str.C_Str(), m_directory + "\\" + str.C_Str());
        jarMat->m_ambient_map = ServiceLocator::get<nimo::AssetManager>()->get<Texture>(m_directory + "\\" + str.C_Str());
    }
    else jarMat->m_ambient_map = ServiceLocator::get<nimo::AssetManager>()->get<Texture>("white");
    if (!material->GetTexture(aiTextureType_NORMALS, 0, &str))
    {
        ServiceLocator::get<nimo::AssetManager>()->bind<Texture>(m_directory + "\\" + str.C_Str(), m_directory + "\\" + str.C_Str());
        jarMat->m_bump_map = ServiceLocator::get<nimo::AssetManager>()->get<Texture>(m_directory + "\\" + str.C_Str());
    }
    else jarMat->m_bump_map = ServiceLocator::get<nimo::AssetManager>()->get<Texture>("white");

    //for (unsigned int i = 0; i < material->GetTextureCount(aiTextureType_DIFFUSE); i++)
    //{
    //    aiString str;
    //    material->GetTexture(aiTextureType_DIFFUSE, i, &str);
    //    ServiceLocator::get<nimo::AssetManager>->bind<Texture>(m_directory + "\\" + str.C_Str(), m_directory + "\\" + str.C_Str());
    //    //textures.push_back(std::make_shared<Texture>(m_directory + "\\" + str.C_Str()));
    //}
    return std::make_shared<Mesh>(vertices, indices, jarMat);
}
