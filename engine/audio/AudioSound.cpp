#include "AudioSound.h"
#include "AudioEngine.h"

nimo::AudioSound::AudioSound(std::shared_ptr<AudioSource> source)
{
    ma_sound_init_copy(AudioEngine::GetEngine(), source->GetSound(), MA_SOUND_FLAG_DECODE, NULL, &sound);
    ma_sound_set_volume(&sound, 0.1f);
    ma_sound_start(&sound);
    // ma_sound_stop(&sound);
    // ma_sound_seek_to_pcm_frame(&sound, 0);
    // ma_sound_set_looping(&sound, 1);
    // ma_sound_is_playing(&sound);
    // ma_sound_is_looping(&sound);
    // ma_sound_set_pitch(&sound, 1.0f);
    // ma_sound_set_pan(&sound, 0.0f);
    // ma_sound_set_fade_in_milliseconds(&sound, 0, 1, 1000);
    // ma_sound_set_fade_in_milliseconds(&sound, -1, 0, 1000);
}
nimo::AudioSound::~AudioSound()
{
    ma_sound_uninit(&sound);
}