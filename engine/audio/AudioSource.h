#pragma once

#include <string>
#include "assets/Asset.h"
#include "miniaudio.h"

namespace nimo{
class AudioSource : public Asset
{
public:
    AudioSource(const std::string& filepath);
    ~AudioSource();
    inline ma_sound* GetSound() {return &sound;}
    std::string filepath;

    static AssetType StaticType(){return AssetType::Audio;}
private:
    ma_sound sound;
};
}