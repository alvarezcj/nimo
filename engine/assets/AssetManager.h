#pragma once

#include "AssetIndex.h"
#include "AssetSerializer.h"
#include "core/Log.h"
#include <memory>
#include <typeindex>
#include <typeinfo>

namespace nimo
{
class AssetManager
{
public:
	static void Initialize();
	static void Cleanup();
	
	template<typename T>
	static void CreateNewAsset(const std::string& filename)
	{
		static_assert(std::is_base_of<Asset, T>::value, "T must derive from Asset class");
		AssetMetadata metadata = index.get(filename);
		if(!metadata.id.valid()){
			metadata.id = GUID::Create();
			metadata.filepath = filename;
			metadata.type = T::StaticType();
			index[metadata.id] = metadata;
		}
		AssetSerializer<T> assetSerializer;
		std::shared_ptr<T> asset = assetSerializer.Deserialize(metadata);
		asset->id = metadata.id;
		m_loadedAssets[typeid(T)][asset->id] = asset;
		NIMO_INFO("Created and loaded asset {} with GUID: {}", filename, asset->id.str());
	}

	template<typename T>
	static void CreateAssetFromMemory(const std::string& filename, const std::shared_ptr<T>& asset)
	{
		static_assert(std::is_base_of<Asset, T>::value, "T must derive from Asset class");
		AssetMetadata metadata = index.get(filename);
		if(!metadata.id.valid()){
			metadata.id = GUID::Create();
			metadata.filepath = filename;
			metadata.type = T::StaticType();
			index[metadata.id] = metadata;
		}
		AssetSerializer<T> assetSerializer;
		assetSerializer.Serialize(metadata, asset);
		asset->id = metadata.id;
		m_loadedAssets[typeid(T)][asset->id] = asset;
	}
	
	template <typename T>
	static void Serialize(AssetId id)
	{
		static_assert(std::is_base_of<Asset, T>::value, "T must derive from Asset class");
		AssetMetadata metadata = index.get(id);
		NIMO_INFO("Serializing {} {}", AssetTypeToString(metadata.type), metadata.filepath.string());
		AssetSerializer<T> assetSerializer;
		assetSerializer.Serialize(metadata, std::static_pointer_cast<T, Asset>(m_loadedAssets[typeid(T)][id]));
	}

	template <typename T>
	static std::shared_ptr<T> Get(AssetId id)
	{
		static_assert(std::is_base_of<Asset, T>::value, "T must derive from Asset class");
		auto it = m_loadedAssets[typeid(T)].find(id);
		if(it != m_loadedAssets[typeid(T)].end())
		{
			// Loaded
			return std::static_pointer_cast<T, Asset>(m_loadedAssets[typeid(T)][id]);
		}
		// Not loaded
		NIMO_WARN("NOT found loaded asset {}", id.str());
		AssetSerializer<T> assetSerializer;
		std::shared_ptr<T> asset = assetSerializer.Deserialize(index[id]);
		asset->id = index[id].id;
		m_loadedAssets[typeid(T)][index[id].id] = asset;
		return asset;
	}

	template<typename T>
	static std::shared_ptr<T> Get(const std::string& filepath)
	{
		static_assert(std::is_base_of<Asset, T>::value, "T must derive from Asset class");
		if(index.get(std::filesystem::path(filepath)).id.valid())
			return Get<T>(index.get(std::filesystem::path(filepath)).id);
		else
			return nullptr;
	}

	template<typename T>
	static std::vector<std::shared_ptr<T>> GetAllLoaded()
	{
		static_assert(std::is_base_of<Asset, T>::value, "T must derive from Asset class");
		std::vector<std::shared_ptr<T>> res;
		for(auto [id, asset] : m_loadedAssets[typeid(T)])
		{
			if(asset)
				res.push_back(std::static_pointer_cast<T, Asset>(asset));
		}
		return res;
	}

	template<typename T>
	static std::vector<AssetMetadata> GetAllExisting()
	{
		static_assert(std::is_base_of<Asset, T>::value, "T must derive from Asset class");
		std::vector<AssetMetadata> res;
		for(auto [id, metadata] : index)
		{
			if(metadata.type == T::StaticType())
				res.push_back(metadata);
		}
		return res;
	}

	static const AssetMetadata& GetMetadata(const std::filesystem::path& path);
	static const AssetMetadata& GetMetadata(AssetId id);
	static const AssetMetadata& GetMetadata(const std::shared_ptr<Asset>& asset) { return GetMetadata(asset->id); }

	static void LoadAssetIndex(const std::string& filename);
	static void WriteIndex();
	static std::filesystem::path GetRelativePath(const std::filesystem::path& filepath);
	static void UpdatePath(AssetId id, const std::filesystem::path& newPath);

private:
	static AssetMetadata& GetMetadataRef(AssetId handle);

	static AssetIndex index;
	static std::unordered_map<std::type_index, std::unordered_map<AssetId, std::shared_ptr<Asset>>> m_loadedAssets;
};
} // namespace nimo
