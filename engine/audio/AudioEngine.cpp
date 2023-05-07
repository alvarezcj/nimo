#include "AudioEngine.h"
#include "core/Log.h"
ma_engine* nimo::AudioEngine::engine = nullptr;
void nimo::AudioEngine::Initialize()
{
    ma_result result;
    engine = (ma_engine*)malloc(sizeof(ma_engine));
    result = ma_engine_init(NULL, engine);
    if (result != MA_SUCCESS) {
        NIMO_CRITICAL("Failed to initialize audio engine.");
    }
    ma_engine_set_volume(engine, 0.1f);
}
void nimo::AudioEngine::Cleanup()
{
    ma_engine_uninit(engine);
    free(engine);
}