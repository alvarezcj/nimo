#pragma once

#include <unordered_map>

#include "AssetMetadata.h"

namespace nimo
{
class AssetIndex
{
public:
    AssetMetadata& operator[](const AssetId handle);
    const AssetMetadata& get(const AssetId handle) const;
    const AssetMetadata& get(const std::filesystem::path& path) const;

    size_t count() const { return db.size(); }
    bool contains(const AssetId handle) const;
    size_t remove(const AssetId handle);
    void clear();

    auto begin() { return db.begin(); }
    auto end() { return db.end(); }
    auto begin() const { return db.cbegin(); }
    auto end() const { return db.cend(); }
private:
    std::unordered_map<AssetId, AssetMetadata> db;
};
} // namespace nimo