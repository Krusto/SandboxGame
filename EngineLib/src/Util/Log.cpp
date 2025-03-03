#pragma once
#include "Log.hpp"
// #define CLOG_IMPLEMENT
#include <CLog.h>

#ifdef __cplusplus
namespace Engine
{
#endif

    static void LoggerCreate(void) { CLogCreate(); }

    static void LoggerDestroy(void) { CLogDestroy(); }

    static void LogMessage(LogLevel level, const char* message, ...)
    {
        va_list list;
        va_start(list, message);
        CLogVMessage(level, message, list);
        va_end(list);
    }

    static void LoggerAttachFileHandler(const char* filename, LogPolicy policy)
    {
        CLogAttachFileHandler(filename, policy);
    }

    static void LoggerAttachTerminalHandler(LogPolicy policy) { CLogAttachTerminalHandler(policy); }

    static void LoggerAttachHandler(void (*handler)(LogEvent*, void*), void* param, LogPolicy policy)
    {
        CLogAttachHandler(handler, param, policy);
    }

#ifdef __cplusplus
}// namespace Engine
#endif