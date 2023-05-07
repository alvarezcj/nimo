#pragma once

#include <memory>
#include "spdlog/spdlog.h"

namespace nimo
{
    class Log{
    public:
        enum class Level : unsigned char {
            Debug = 0,
            Info,
            Warn,
            Error,
            Critical,
        };
        static void Initialize();
        static void Cleanup();
	    template<typename... Args>
        static void Print(Level level, Args&&... args);
    private:
        static std::shared_ptr<spdlog::logger> engineLogger;
    };
} // namespace nimo

// Logging macros
#define NIMO_DEBUG(...)  ::nimo::Log::Print(::nimo::Log::Level::Debug, __VA_ARGS__)
#define NIMO_INFO(...)   ::nimo::Log::Print(::nimo::Log::Level::Info, __VA_ARGS__)
#define NIMO_WARN(...)   ::nimo::Log::Print(::nimo::Log::Level::Warn, __VA_ARGS__)
#define NIMO_ERROR(...)  ::nimo::Log::Print(::nimo::Log::Level::Error, __VA_ARGS__)
#define NIMO_CRITICAL(...)  ::nimo::Log::Print(::nimo::Log::Level::Critical, __VA_ARGS__)

template<typename... Args>
void nimo::Log::Print(Level level, Args&&... args)
{
    if (!engineLogger) return;
    switch (level)
    {
    case Level::Debug:
        engineLogger->trace(fmt::format(std::forward<Args>(args)...));
        break;
    case Level::Info:
        engineLogger->info(fmt::format(std::forward<Args>(args)...));
        break;
    case Level::Warn:
        engineLogger->warn(fmt::format(std::forward<Args>(args)...));
        break;
    case Level::Error:
        engineLogger->error(fmt::format(std::forward<Args>(args)...));
        break;
    case Level::Critical:
        engineLogger->critical(fmt::format(std::forward<Args>(args)...));
        break;
    }
}
