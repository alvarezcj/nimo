#pragma once 
#include "miniaudio.h"
namespace nimo
{
class AudioEngine
{
public:
	static void Initialize();
	static void Cleanup();
    inline static ma_engine* GetEngine(){return engine;}
private:
    static ma_engine* engine;
};
} // namespace nimo
