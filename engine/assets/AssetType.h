#pragma once 

#include <string>

namespace nimo
{
    enum class AssetType : unsigned int
	{
		None = 0,
        Texture,
        Mesh,
        Shader,
        Material,
        Scene,
	};

    inline AssetType AssetTypeFromString(const std::string& str){
        if (str == "None")			return AssetType::None;
        if (str == "Texture")			return AssetType::Texture;
        if (str == "Mesh")			return AssetType::Mesh;
        if (str == "Shader")			return AssetType::Shader;
        if (str == "Material")			return AssetType::Material;
        if (str == "Scene")			return AssetType::Scene;
        return AssetType::None;
    }

    inline const char* AssetTypeToString(AssetType type)
    {
        switch (type)
        {
            case AssetType::None:			return "None"; break;
            case AssetType::Texture:		return "Texture"; break;
            case AssetType::Mesh:		return "Mesh"; break;
            case AssetType::Shader:		return "Shader"; break;
            case AssetType::Material:		return "Material"; break;
            case AssetType::Scene:		return "Scene"; break;
            default:  break;
        }
        return "None";
    }
    
} // namespace nimo
