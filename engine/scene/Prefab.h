#pragma once

#pragma once

#include <string>
#include "assets/Asset.h"

namespace nimo{
class Prefab : public Asset
{
public:
    Prefab(const std::string& filepath);
    std::string filepath;

    static AssetType StaticType(){return AssetType::Prefab;}
private:
};
}