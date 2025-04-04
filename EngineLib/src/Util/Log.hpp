#pragma once
#include <string>

#include <CLog.h>

#include "Predefines.hpp"
#ifdef _WIN32
#include <Windows.h>
#else
#endif
#ifdef __cplusplus

namespace Engine
{
    extern "C" {
#endif
    EXPORT_UTILS void LoggerCreate(void);
    EXPORT_UTILS void LoggerDestroy(void);
    EXPORT_UTILS void LogMessage(LogLevel level, const char* message, ...);
    EXPORT_UTILS void LoggerAttachFileHandler(const char* filename, LogPolicy policy);
    EXPORT_UTILS void LoggerAttachTerminalHandler(LogPolicy policy);
    EXPORT_UTILS void LoggerAttachHandler(void (*handler)(LogEvent*, void*), void* param, LogPolicy policy);

#ifdef __cplusplus
    }
}// namespace Engine
#ifdef LOG_DEBUG
#undef LOG_DEBUG
#endif
#ifdef LOG_INFO
#undef LOG_INFO
#endif
#ifdef LOG_WARNING
#undef LOG_WARNING
#endif
#ifdef LOG_ERROR
#undef LOG_ERROR
#endif
#ifdef _MSC_VER
#define LOG_DEBUG(...) Engine::LogMessage(LogLevel::DEBUG_LEVEL, __VA_ARGS__)
#define LOG_INFO(...) Engine::LogMessage(LogLevel::INFO_LEVEL, __VA_ARGS__)
#define LOG_WARNING(...) Engine::LogMessage(LogLevel::WARNING_LEVEL, __VA_ARGS__)
#define LOG_ERROR(...) Engine::LogMessage(LogLevel::ERROR_LEVEL, __VA_ARGS__)
#else
#define LOG_DEBUG(format, ...) Engine::LogMessage(LogLevel::DEBUG_LEVEL, format __VA_OPT__(, ) __VA_ARGS__)
#define LOG_INFO(format, ...) Engine::LogMessage(LogLevel::INFO_LEVEL, format __VA_OPT__(, ) __VA_ARGS__)
#define LOG_WARNING(format, ...) Engine::LogMessage(LogLevel::WARNING_LEVEL, format __VA_OPT__(, ) __VA_ARGS__)
#define LOG_ERROR(format, ...) Engine::LogMessage(LogLevel::ERROR_LEVEL, format __VA_OPT__(, ) __VA_ARGS__)
#endif
#else
#ifdef _MSC_VER
#define LOG_DEBUG(...) LogMessage(LogLevel::DEBUG_LEVEL, __VA_ARGS__)
#define LOG_INFO(...) LogMessage(LogLevel::INFO_LEVEL, __VA_ARGS__)
#define LOG_WARNING(...) LogMessage(LogLevel::WARNING_LEVEL, __VA_ARGS__)
#define LOG_ERROR(...) LogMessage(LogLevel::ERROR_LEVEL, __VA_ARGS__)
#else
#define LOG_DEBUG(format, ...) LogMessage(LogLevel::DEBUG_LEVEL, format __VA_OPT__(, ) __VA_ARGS__)
#define LOG_INFO(format, ...) LogMessage(LogLevel::INFO_LEVEL, format __VA_OPT__(, ) __VA_ARGS__)
#define LOG_WARNING(format, ...) LogMessage(LogLevel::WARNING_LEVEL, format __VA_OPT__(, ) __VA_ARGS__)
#define LOG_ERROR(format, ...) LogMessage(LogLevel::ERROR_LEVEL, format __VA_OPT__(, ) __VA_ARGS__)
#endif
#endif
