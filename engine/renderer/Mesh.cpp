#include "Mesh.h"
#include "glad/glad.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"      
#include "assimp/postprocess.h"
#include "core/Log.h"

nimo::Mesh::Mesh(const std::string& file)
{
    // Create an instance of the Importer class
    Assimp::Importer importer;
    // And have it read the given file with some example postprocessing
    // Usually - if speed is not the most important aspect for you - you'll 
    // propably to request more postprocessing than we do in this example.
    const aiScene* scene = importer.ReadFile( file, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);
    
    // If the import failed, report it
    if( !scene)
    {
        // std::cout << "Error loading assimp scene for " << file << std::endl;
    }
    else
    {
        // std::cout << "Succesfully loaded assimp scene for " << file << std::endl;
        // std::cout << file << std::endl;
        // std::cout << "\t NumMeshes: " << scene->mNumMeshes << std::endl;
        for(unsigned int i = 0; i < scene->mNumMeshes; ++i)
        {
            // std::cout << "\t\tMesh " << i << std::endl;
            // std::cout << "\t\t NumVertices: " << scene->mMeshes[i]->mNumVertices << std::endl;
            // std::cout << "\t\t NumFaces: " << scene->mMeshes[i]->mNumFaces << std::endl;
            // std::cout << "\t\t NumAnimMeshes: " << scene->mMeshes[i]->mNumAnimMeshes << std::endl;
            // std::cout << "\t\t NumBones: " << scene->mMeshes[i]->mNumBones << std::endl;
            for(int j = 0; j< scene->mMeshes[i]->mNumVertices; ++j)
            {
                Vertex vertex;
                vertex.position = {scene->mMeshes[i]->mVertices[j].x,scene->mMeshes[i]->mVertices[j].y,scene->mMeshes[i]->mVertices[j].z};
                if (scene->mMeshes[i]->HasNormals())
                    vertex.normal = {scene->mMeshes[i]->mNormals[j].x,scene->mMeshes[i]->mNormals[j].y,scene->mMeshes[i]->mNormals[j].z};
                else
                {
                    NIMO_ERROR("Model has no normals");
                    vertex.normal = glm::vec3(0.0f, 0.0f, 0.0f);
                }
                // texture coordinates
                if (scene->mMeshes[i]->mTextureCoords[0]) // does the mesh contain texture coordinates?
                {
                    // std::cout << "Has uvs" << std::endl;
                    glm::vec2 vec;
                    vec.x = scene->mMeshes[i]->mTextureCoords[0][j].x;
                    vec.y = scene->mMeshes[i]->mTextureCoords[0][j].y;
                    vertex.uv = vec;
                }
                else
                    vertex.uv = glm::vec2(0.0f, 0.0f);
                // std::cout << "Vertex num " << i << "[" << vertex.position.x << ", "<< vertex.position.y << ", "<< vertex.position.z << "]["<< vertex.uv.x << ", "<< vertex.uv.y << "]"<< std::endl;
                m_vertices.push_back(vertex);
            }
            for(int j = 0; j< scene->mMeshes[i]->mNumFaces; ++j)
            {
                aiFace face = scene->mMeshes[i]->mFaces[j];
                // retrieve all indices of the face and store them in the indices vector
                for (unsigned int k = 0; k < face.mNumIndices; k++)
                    m_indices.push_back(face.mIndices[k]);
            }
        }
        // std::cout << "\t NumTextures: " << scene->mNumTextures << std::endl;
        // std::cout << "\t NumMaterials: " << scene->mNumMaterials << std::endl;
        for(unsigned int i = 0; i < scene->mNumMaterials; ++i)
        {
            // std::cout << "\t\tMaterial " << i << std::endl;
            // std::cout << "\t\t NumProperties: " << scene->mMaterials[i]->mNumProperties << std::endl;
            for(unsigned int j = 0; j < scene->mMaterials[i]->mNumProperties; ++j)
            {
                // std::cout << "\t\t\tProperty " << i << std::endl;
                // std::cout << "\t\t\t Key: " << scene->mMaterials[i]->mProperties[j]->mKey.C_Str() << std::endl;
            }
        }
        // std::cout << "\t NumAnimations: " << scene->mNumAnimations << std::endl;
        // std::cout << "\t NumCameras: " << scene->mNumCameras << std::endl;
        // std::cout << "\t NumLights: " << scene->mNumLights << std::endl;

        // std::cout << "\tRootNode name: " << scene->mRootNode->mName.C_Str() << std::endl;
        // std::cout << "\tRootNode NumMeshes: " << scene->mRootNode->mNumMeshes << std::endl;
        // std::cout << "\tRootNode NumChildren: " << scene->mRootNode->mNumChildren << std::endl;
        for(unsigned int i = 0; i < scene->mRootNode->mNumChildren; ++i)
        {
            // std::cout << "\t\tRootNode Child " << i << std::endl;
            // std::cout << "\t\tRootNode Child Name " << scene->mRootNode->mChildren[i]->mName.C_Str() << std::endl;
            // std::cout << "\t\tRootNode Child NumMeshes " << scene->mRootNode->mChildren[i]->mNumMeshes << std::endl;
            for(int j = 0; j < scene->mRootNode->mChildren[i]->mNumMeshes; ++j)
            {
                // std::cout << "\t\t\tRootNode Child Mesh Id " << scene->mRootNode->mChildren[i]->mMeshes[j] << std::endl;
            }
        }
        
    }
    m_vao = new VertexArray();
    m_vbo = new VertexBuffer(
        {
            {"position", ShaderDataType::Float3},
            {"normal", ShaderDataType::Float3},
            {"uv", ShaderDataType::Float2}
        },
        m_vertices.data(), sizeof(Vertex) * m_vertices.size()
    );
    m_ibo = new IndexBuffer(m_indices.data(), m_indices.size());
    m_vao->bind();
    m_ibo->bind();
    m_vbo->bind();
    m_vbo->applyLayout();
}
nimo::Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
    : m_vertices(vertices)
    , m_indices(indices)
{
    m_vao = new VertexArray();
    m_vbo = new VertexBuffer(
        {
            {"position", ShaderDataType::Float3},
            {"normal", ShaderDataType::Float3},
            {"uv", ShaderDataType::Float2}
        },
        m_vertices.data(), sizeof(Vertex) * m_vertices.size()
    );
    m_ibo = new IndexBuffer(m_indices.data(), m_indices.size());
    m_vao->bind();
    m_ibo->bind();
    m_vbo->bind();
    m_vbo->applyLayout();
}
nimo::Mesh::~Mesh()
{
    delete m_vao;
    delete m_ibo;
    delete m_vbo;
}
void nimo::Mesh::draw()
{
    m_vao->bind();
    glDrawElements(GL_TRIANGLES, m_ibo->count(), GL_UNSIGNED_INT, 0);
}