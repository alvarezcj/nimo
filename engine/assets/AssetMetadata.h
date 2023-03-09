#pragma once

#include <filesystem>

#include "Asset.h"

namespace nimo
{
class AssetMetadata
{
public:
    AssetId id;
    AssetType type;
    std::filesystem::path filepath;
};
} // namespace nimo

