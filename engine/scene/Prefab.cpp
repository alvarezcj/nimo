#include "Prefab.h"
#include <fstream>
#include "SceneSerializer.h"

nimo::Prefab::Prefab()
{}

nimo::Prefab::Prefab(const std::string& filepath)
    : filepath(filepath)
{
    std::ifstream ifs(filepath);
    source << ifs;
}

nimo::GUID nimo::Prefab::Create(std::shared_ptr<nimo::Scene> where)
{
    auto ser = AssetSerializer<Scene>();
    return ser.DeserializeEntity(where, source, GUID());
}

void nimo::Prefab::SetData(std::shared_ptr<nimo::Scene> where, Entity e)
{
    auto ser = AssetSerializer<Scene>();
    source = ser.SerializeEntity(where, e);
}
