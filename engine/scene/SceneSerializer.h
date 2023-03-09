#pragma once

#include "Scene.h"
#include <memory>
#include <filesystem>
#include "assets/AssetSerializer.h"

namespace nimo {

    template<> struct AssetSerializer<Scene>{
        void Serialize(const AssetMetadata& metadata, const std::shared_ptr<nimo::Scene>& asset);
        std::shared_ptr<nimo::Scene> Deserialize(const nimo::AssetMetadata& metadata);
    };
	// class SceneSerializer
	// {
	// public:
	// 	SceneSerializer(const std::shared_ptr<Scene>& scene);
	// 	void Serialize(const std::filesystem::path& filepath);
	// 	bool Deserialize(const std::filesystem::path& filepath);

	// private:
	// 	std::shared_ptr<Scene> m_scene;
	// };

}
