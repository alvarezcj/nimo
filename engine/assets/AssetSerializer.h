#pragma once

#include "AssetMetadata.h"
#include <memory>

namespace nimo
{
    template<typename T>
    struct AssetSerializer{
        void Serialize(const AssetMetadata& metadata, const std::shared_ptr<T>& asset) = delete;
        std::shared_ptr<T> Deserialize(const AssetMetadata& metadata) = delete;
    };

    class Texture;
    template<> struct AssetSerializer<Texture>{
        void Serialize(const AssetMetadata& metadata, const std::shared_ptr<nimo::Texture>& asset);
        std::shared_ptr<nimo::Texture> Deserialize(const nimo::AssetMetadata& metadata);
    };
    class Shader;
    template<> struct AssetSerializer<Shader>{
        void Serialize(const AssetMetadata& metadata, const std::shared_ptr<nimo::Shader>& asset);
        std::shared_ptr<nimo::Shader> Deserialize(const nimo::AssetMetadata& metadata);
    };
    class Material;
    template<> struct AssetSerializer<Material>{
        void Serialize(const AssetMetadata& metadata, const std::shared_ptr<nimo::Material>& asset);
        std::shared_ptr<nimo::Material> Deserialize(const nimo::AssetMetadata& metadata);
    };
    class Mesh;
    template<> struct AssetSerializer<Mesh>{
        void Serialize(const AssetMetadata& metadata, const std::shared_ptr<nimo::Mesh>& asset);
        std::shared_ptr<nimo::Mesh> Deserialize(const nimo::AssetMetadata& metadata);
    };
} // namespace nimo
