#include "AudioSound.h"
#include "AudioEngine.h"
#include "core/Log.h"

nimo::AudioSound::AudioSound(std::shared_ptr<AudioSource> source)
{
    NIMO_DEBUG("nimo::AudioSound::AudioSound({})", source->id.str());
    ma_sound_init_copy(AudioEngine::GetEngine(), source->GetSound(), MA_SOUND_FLAG_DECODE, NULL, &sound);
    // ma_sound_is_looping(&sound);
    // ma_sound_set_fade_in_milliseconds(&sound, 0, 1, 1000);
    // ma_sound_set_fade_in_milliseconds(&sound, -1, 0, 1000);
}
nimo::AudioSound::~AudioSound()
{
    NIMO_DEBUG("nimo::AudioSound::~AudioSound()");
    ma_sound_uninit(&sound);
}
void nimo::AudioSound::SetPitch(float pitch)
{
    ma_sound_set_pitch(&sound, pitch);
}
void nimo::AudioSound::SetVolume(float volume)
{
    ma_sound_set_volume(&sound, volume);
}
void nimo::AudioSound::SetPan(float pan)
{
    ma_sound_set_pan(&sound, pan);
}
void nimo::AudioSound::SetLoop(bool loop)
{
    ma_sound_set_looping(&sound, loop);
}
void nimo::AudioSound::Play()
{
    ma_sound_seek_to_pcm_frame(&sound, 0);
    ma_sound_start(&sound);
}
void nimo::AudioSound::Stop()
{
    ma_sound_stop(&sound);
}
void nimo::AudioSound::Resume()
{
    ma_sound_start(&sound);
}
bool nimo::AudioSound::IsPlaying()
{
    return ma_sound_is_playing(&sound);
}
