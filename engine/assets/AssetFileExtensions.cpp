#include "AssetFileExtensions.h"

std::unordered_map<std::string, nimo::AssetType> nimo::AssetFileExtensions::extensions = 
{
    //Scene
    {".nscene", AssetType::Scene},
    //Material
    {".nmat", AssetType::Material},
    //Shader
    {".nshader", AssetType::Shader},
    //Texture
    { ".png", AssetType::Texture },
    { ".jpg", AssetType::Texture },
    { ".jpeg", AssetType::Texture },
    //Mesh
    {".obj", AssetType::Mesh},
    {".fbx", AssetType::Mesh},
};


nimo::AssetType nimo::AssetFileExtensions::GetTypeFromExtension(std::string_view extension)
{
    return extensions[extension.data()];
}