#include "AudioSource.h"
#include "AudioEngine.h"

nimo::AudioSource::AudioSource(const std::string& filepath)
    : filepath(filepath)
{
    ma_sound_init_from_file(AudioEngine::GetEngine(), filepath.c_str(), MA_SOUND_FLAG_DECODE, NULL, NULL, &sound);
}
nimo::AudioSource::~AudioSource()
{
    ma_sound_uninit(&sound);
}
