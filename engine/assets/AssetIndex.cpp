#include "AssetIndex.h"

nimo::AssetMetadata& nimo::AssetIndex::operator[](const AssetId id)
{
    return db[id];
}

const nimo::AssetMetadata& nimo::AssetIndex::get(const AssetId id) const
{
    return db.at(id);
}

const nimo::AssetMetadata& nimo::AssetIndex::get(const std::filesystem::path& path) const
{
    for (auto& [id, metadata] : db)
    {
        if (metadata.filepath == path)
            return metadata;
    }
    return AssetMetadata();
}

bool nimo::AssetIndex::contains(const AssetId id) const
{
    return db.find(id) != db.end();
}

size_t nimo::AssetIndex::remove(const AssetId id)
{
    return db.erase(id);
}

void nimo::AssetIndex::clear()
{
    db.clear();
}