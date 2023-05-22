#pragma once

#include <memory>
#include "json.hpp"

namespace nimo
{
    struct IAssetSettings{};
    template<typename T>
    struct AssetSettings : public IAssetSettings{};

    class Texture;
    template<> struct AssetSettings<Texture> : public IAssetSettings{
        bool generateMipMaps = true;
    };
    class Shader;
    template<> struct AssetSettings<Shader> : public IAssetSettings{
    };
    class Material;
    template<> struct AssetSettings<Material> : public IAssetSettings{
    };
    class Mesh;
    template<> struct AssetSettings<Mesh> : public IAssetSettings{
    };
    class EnvironmentMap;
    template<> struct AssetSettings<EnvironmentMap> : public IAssetSettings{
    };
    class Script;
    template<> struct AssetSettings<Script> : public IAssetSettings{
    };
    class Prefab;
    template<> struct AssetSettings<Prefab> : public IAssetSettings{
    };
    class AudioSource;
    template<> struct AssetSettings<AudioSource> : public IAssetSettings{
    };
    class Font;
    template<> struct AssetSettings<Font> : public IAssetSettings{
    };
    class Scene;
    template<> struct AssetSettings<Scene> : public IAssetSettings{
    };
} // namespace nimo
