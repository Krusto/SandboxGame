#include "Log.hpp"
// #define CLOG_IMPLEMENT
#include <CLog.h>

#ifdef __cplusplus
namespace Engine
{
#endif

     void LoggerCreate(void) { CLogCreate(); }

     void LoggerDestroy(void) { CLogDestroy(); }

     
     void LogMessage(LogLevel level, const char* message, ...)
    {
        va_list list;
        va_start(list, message);
        CLogVMessage(level, message, list);
        va_end(list);
    }

     void LoggerAttachFileHandler(const char* filename, LogPolicy policy)
    {
        CLogAttachFileHandler(filename, policy);
    }

     void LoggerAttachTerminalHandler(LogPolicy policy) { CLogAttachTerminalHandler(policy); }

     void LoggerAttachHandler(void (*handler)(LogEvent*, void*), void* param, LogPolicy policy)
    {
        CLogAttachHandler(handler, param, policy);
    }

#ifdef __cplusplus
}// namespace Engine
#endif