#include "AssetManager.h"
#include "json.hpp"
#include <fstream>
#include <iostream>
#include "core/Log.h"
#include "core/FileHandling.h"

#include "project/Project.h"

nimo::AssetIndex nimo::AssetManager::index = nimo::AssetIndex();
std::unordered_map<std::type_index, std::unordered_map<nimo::AssetId, std::shared_ptr<nimo::Asset>>> nimo::AssetManager::m_loadedAssets = {};

void nimo::AssetManager::Initialize()
{
    LoadAssetIndex(Project::GetActiveProject()->GetAssetIndexPath().string());
}
void nimo::AssetManager::Cleanup()
{
    index.clear();
    m_loadedAssets.clear();
}

void nimo::AssetManager::LoadAssetIndex(const std::string& filename)
{
    NIMO_DEBUG("Loading AssetIndex from {}", filename);
    if(FileHandling::Exists(filename)){
        std::ifstream fin(filename);
        nlohmann::json j;
        j << fin;
        for(auto o : j["assets"])
        {
            AssetMetadata metadata;
            metadata.id = GUID(std::string(o["guid"]));
            metadata.filepath = std::string(o["filepath"]);
            metadata.type = AssetTypeFromString(std::string(o["type"]));
            NIMO_DEBUG("\t- {}", o.dump());
            index[metadata.id] = metadata;
        }
    }
    else{
        WriteIndex();
    }
}
void nimo::AssetManager::WriteIndex()
{
    nlohmann::json j;
    nlohmann::json assetsjson = nlohmann::json::array();

    int i = 0;
    for(auto a : index)
    {
        assetsjson[i] = {{"guid",a.second.id.str()}, {"filepath",a.second.filepath.c_str()}, {"type", AssetTypeToString(a.second.type)}};
        i++;
    }
    j["assets"] = assetsjson;
    std::ofstream fout(Project::GetActiveProject()->GetAssetIndexPath());
    fout << j.dump(4);
}

nimo::AssetMetadata nimo::AssetManager::GetMetadata(nimo::AssetId id)
{
    return index[id];
}
