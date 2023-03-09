#include "MeshSource.h"

#include <iostream>

#include "assimp/Importer.hpp"
#include "assimp/scene.h"      
#include "assimp/postprocess.h"

nimo::MeshSource::MeshSource(const std::string& file)
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
            vertices.push_back(vertex);
        }
        for(int j = 0; j< scene->mMeshes[i]->mNumFaces; ++j)
        {
            aiFace face = scene->mMeshes[i]->mFaces[j];
            // retrieve all indices of the face and store them in the indices vector
            for (unsigned int k = 0; k < face.mNumIndices; k++)
                indices.push_back(face.mIndices[k]);
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
}

nimo::MeshSource::~MeshSource()
{
}