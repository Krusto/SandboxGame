#pragma once
#include <string>
#include "Predefines.hpp"
#ifdef _WIN32
#include <Windows.h>
#else
#endif
#ifdef __cplusplus
namespace Engine
{
#endif
    typedef enum
    {
        LOG_SINK_NONE=0,
        LOG_SINK_STDOUT,
        LOG_SINK_FILE,
        LOG_SINK_BUFFER
    }LogSinkEnumT;

    typedef struct {
        char* buffer;
        char* flushBuffer;
        size_t length;
        size_t offset;
        LogSinkEnumT sink;
    } LoggerDataT;

    typedef struct {
        void (*Log)(const char* format, ...);
        void (*Flush)();
        LoggerDataT data;
    } LoggerT;

    EXPORT_UTILS LoggerT* LoggerGetInstance();
    EXPORT_UTILS void LoggerDestroy();
    EXPORT_UTILS void LoggerCreate();
    EXPORT_UTILS void LoggerChangeDstToSTDOUT();
    EXPORT_UTILS void LoggerChangeDstToBuffer(char* buffer, size_t length);


#ifdef __cplusplus
}// namespace Engine
#endif

#ifndef __cplusplus
#define Engine ::LoggerGetInstance()->Log(...) CLog(__VA_ARGS__)

#ifdef ENGINE_ENABLE_VERBOSEEngine::LoggerGetInstance()->Log
#define LOG_INFO(...)                                                                                                  \
    {                                                                                                                  \
        Engine::LoggerGetInstance()->Log("[INFO] ");                                                                         \
        Engine::LoggerGetInstance()->Log(__VA_ARGS__);                                                                       \
    }                                                                 \
    #else
#define LOG_INFO(...)
#endif


#define ENGINE_ENABLE_MEMORY_DEBUG_LOG
#ifdef ENGINE_ENABLE_MEMORY_DEBUG_LOG
#define LOG_DEBUG(...)                                                                                                 \
    {                                                                                                                  \
        Engine::LoggerGetInstance()->Log("%s","[DEBUG]");                                                                        \
        Engine::LoggerGetInstance()->Log(__VA_ARGS__);                                                                       \
    }                                                                   \
    #else
#define LOG_DEBUG(...)
#endif

#ifndef LOG_ERROR
#define LOG_ERROR(...)                                                                                                 \
    {                                                                                                                  \
        Engine::LoggerGetInstance()->Log("%s","[ERROR]");                                                                        \
        Engine::LoggerGetInstance()->Log(__VA_ARGS__);                                                                       \
    }
#endif
#else
    inline static void LOG_INIT() { Engine::LoggerCreate(); }

    inline static void LOG_DESTROY() { Engine::LoggerDestroy(); }

inline static void LOG_FLUSH() { Engine::LoggerGetInstance()->Flush(); }

template <typename... Args>
inline static void LOG_INFO(const char* format, Args... args)
{
    Engine::LoggerGetInstance()->Log("%s", "[INFO]");
    Engine::LoggerGetInstance()->Log(format, std::forward<Args>(args)...);
}


template <typename T>
inline static void LOG_INFO(const T* format)
{
    Engine::LoggerGetInstance()->Log("%s", "[INFO]");
    Engine::LoggerGetInstance()->Log("%s", format);
}

#define ENGINE_ENABLE_DEBUG_LOG

template <typename... Args>
inline static void LOG_DEBUG(const char* format, Args... args)
{
#ifdef ENGINE_ENABLE_DEBUG_LOG
    Engine::LoggerGetInstance()->Log("%s", "[DEBUG]");
    Engine::LoggerGetInstance()->Log(format, std::forward<Args>(args)...);
#endif
}

inline static void LOG_DEBUG(const char* format)
{
#ifdef ENGINE_ENABLE_DEBUG_LOG
    Engine::LoggerGetInstance()->Log("%s", "[DEBUG]");
    Engine::LoggerGetInstance()->Log("%s",format);
#endif
}

template <typename... Args>
inline static void LOG_ERROR(const char* format, Args... args)
{
    Engine::LoggerGetInstance()->Log("%s", "[ERROR]");
    Engine::LoggerGetInstance()->Log(format, std::forward<Args>(args)...);
}

inline static void LOG_ERROR(std::string_view format)
{
    Engine::LoggerGetInstance()->Log("%s", "[ERROR]");
    Engine::LoggerGetInstance()->Log("%s", format.data());
}

inline static void LOG_ERROR(const std::string& format)
{
    Engine::LoggerGetInstance()->Log("%s", "[ERROR]");
    Engine::LoggerGetInstance()->Log("%s", format.c_str());
}

template <typename... Args>
inline static void LOG_WARNING(const char* format, Args... args)
{
    Engine::LoggerGetInstance()->Log("%s", "[WARNING]");
    Engine::LoggerGetInstance()->Log(format, std::forward<Args>(args)...);
}

inline static void LOG_WARNING(const char* format)
{
    Engine::LoggerGetInstance()->Log("%s", "[WARNING]");
    Engine::LoggerGetInstance()->Log("%s",format);
}

template <typename... Args>
inline static void LOG_MEMORY_ALLOC(const char* format, Args... args)
{
#ifdef ENGINE_ENABLE_MEMORY_DEBUG_LOG
    // Engine::LoggerGetInstance()->Log("[MEMORY] ");
    // Engine::LoggerGetInstance()->Log(format, std::forward<Args>(args)...);
#endif
}

inline static void LOG_MEMORY_ALLOC(const char* format)
{
#ifdef ENGINE_ENABLE_MEMORY_DEBUG_LOG
    // Engine::LoggerGetInstance()->Log("[MEMORY] ");
    // Engine::LoggerGetInstance()->Log("%s",format);
#endif
}
#endif
