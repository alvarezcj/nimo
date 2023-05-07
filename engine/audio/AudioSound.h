#pragma once

#include <string>
#include <memory>
#include "miniaudio.h"
#include "AudioSource.h"

namespace nimo{
class AudioSound
{
public:
    AudioSound(std::shared_ptr<AudioSource> source);
    ~AudioSound();

private:
    ma_sound sound;
};
}