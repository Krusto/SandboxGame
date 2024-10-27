#pragma once
#include <cstdlib>
#include <iostream>
#include <memory>
#include <stdio.h>

namespace Engine
{
    class Logger
    {
    public:
        Logger() { Init(); }

        ~Logger() { Destroy(); };

        void Init();
        void Destroy();

        static void Create();

        static Logger* GetInstance();
#ifdef LWLOG
        std::shared_ptr<lwlog::logger<lwlog::default_log_policy, lwlog::default_storage_policy,
                                      lwlog::single_threaded_policy, lwlog::sinks::stdout_sink>>
                console;
#endif
    private:
        static std::shared_ptr<Logger> s_Logger;
    };

    class mem
    {
    public:
        inline static int memsize = 0;
    };

}// namespace Engine


#ifdef LWLOG
#define LOG(...)                                                                                                       \
    Engine::Logger::GetInstance()->console->info(__VA_ARGS__);                                                         \
    std::flush(std::cout);
#define LOG_INFO(...)                                                                                                  \
    Engine::Logger::GetInstance()->console->info(__VA_ARGS__);                                                         \
    std::flush(std::cout);
#define LOG_ERROR(...)                                                                                                 \
    Engine::Logger::GetInstance()->console->error(__VA_ARGS__);                                                        \
    std::flush(std::cout);
#define LOG_DEBUG(...)                                                                                                 \
    Engine::Logger::GetInstance()->console->debug(__VA_ARGS__);                                                        \
    std::flush(std::cout);
#define LOG_WARNING(...)                                                                                               \
    Engine::Logger::GetInstance()->console->warning(__VA_ARGS__);                                                      \
    std::flush(std::cout);
#define LOG_CRITICAL(...)                                                                                              \
    Engine::Logger::GetInstance()->console->critical(__VA_ARGS__);                                                     \
    std::flush(std::cout);
#else

template <typename... Args>
inline static void LOG(const char* format, Args... args)
{
    printf_s(format, args...);
}

template <typename... Args>
inline static void LOG_INFO(const char* format, Args... args)
{
    LOG(format, args...);
}

template <typename... Args>
inline static void LOG_DEBUG(const char* format, Args... args)
{
    LOG(format, args...);
}

template <typename... Args>
inline static void LOG_ERROR(const char* format, Args... args)
{
    LOG(format, args...);
}

template <typename... Args>
inline static void LOG_WARNING(const char* format, Args... args)
{
    LOG(format, args...);
}


#endif