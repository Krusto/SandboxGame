#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "Log.hpp"
#ifdef _WIN32
#include <Windows.h>
#else
#include <pthread.h>
#endif

#ifdef __cplusplus
namespace Engine
{
#endif
#ifndef _WIN32
#define CRITICAL_SECTION pthread_mutex_t
#define InitializeCriticalSection pthread_mutex_create
#define DeleteCriticalSection pthread_mutex_destroy
#endif
    static LoggerT* globalLoggerInstance = NULL;
    static CRITICAL_SECTION* cs = NULL;
    static CRITICAL_SECTION* cs_flush = NULL;

    inline static CRITICAL_SECTION* GetCriticalSection() { return cs; }

    inline static CRITICAL_SECTION* GetCriticalSectionFlushing() { return cs_flush; }

    inline static void LockLoggerResource(CRITICAL_SECTION* section_mutex)
    {
#ifdef _WIN32
        EnterCriticalSection(section_mutex);
#else
    pthread_mutex_lock(section_mutex);
#endif
    }

    inline static void UnlockLoggerResource(CRITICAL_SECTION* section_mutex)
    {
#ifdef _WIN32
        LeaveCriticalSection(section_mutex);
#else
    pthread_mutex_unlock(section_mutex);
#endif
    }

    static void FlushLogger()
    {
        LockLoggerResource(GetCriticalSection());
        {

            LoggerDataT* instance = &LoggerGetInstance()->data;
            LockLoggerResource(GetCriticalSectionFlushing());
            {
                if (instance->sink == LOG_SINK_STDOUT)
                {
#ifdef _WIN32
                    size_t flushed_byte_count{};
                    WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), instance->buffer, instance->offset,
                              (LPDWORD) &flushed_byte_count, 0);
                    instance->offset = 0;
#else
                int flushed_byte_count = fwrite(instance->buffer, 1, instance->offset, stdout);
                if (flushed_byte_count) { instance->offset -= instance->offset - flushed_byte_count }
                else { fprintf(stderr, "Can not flush to stdout!\n"); }
#endif
                }
                else if (instance->sink == LOG_SINK_BUFFER)
                {
                    memcpy_s(instance->flushBuffer, instance->length, instance->buffer, instance->length);
                    instance->offset = 0;
                }
            }
            UnlockLoggerResource(GetCriticalSectionFlushing());
        }
        UnlockLoggerResource(GetCriticalSection());
    }

    static void Log(const char* format, ...)
    {
        LockLoggerResource(GetCriticalSection());
        LoggerT* logger = LoggerGetInstance();
        if (logger == NULL) return;
        if (logger->Log == NULL) return;
        if (logger->Flush == NULL) return;
        LoggerDataT* instance = &LoggerGetInstance()->data;
        va_list args;
        va_start(args, format);
        int length = vsnprintf(nullptr, 0, format, args);
        if (instance->offset + length >= instance->length)
        {
            FlushLogger();
            Log(format, args);
        }

        int written_chars = vsnprintf_s(instance->buffer + instance->offset, instance->length - instance->offset,
                                        instance->length - instance->offset, format, args);
        va_end(args);

        if (written_chars > 0) { instance->offset += written_chars; }
        UnlockLoggerResource(GetCriticalSection());
    }

    static int GetMessageLength()
    {
        int length = -1;
        LockLoggerResource(GetCriticalSection());
        LoggerT* logger = LoggerGetInstance();
        if (logger == NULL) return length;
        length = logger->data.offset;
        UnlockLoggerResource(GetCriticalSection());
        return length;
    }

    void LockLoggerBuffer() { LockLoggerResource(GetCriticalSection()); }

    void UnlockLoggerBuffer() { UnlockLoggerResource(GetCriticalSection()); }

    LoggerT* LoggerGetInstance()
    {
        LockLoggerResource(GetCriticalSection());
        if (globalLoggerInstance == NULL)
        {
            globalLoggerInstance = (LoggerT*) malloc(sizeof(LoggerT));
            if (globalLoggerInstance == NULL) return NULL;

            globalLoggerInstance->data.length = 1024 * 16;
            globalLoggerInstance->data.buffer = (char*) malloc(globalLoggerInstance->data.length);
            globalLoggerInstance->data.offset = 0;
            globalLoggerInstance->data.sink = LogSinkEnumT::LOG_SINK_STDOUT;
            globalLoggerInstance->Log = Log;
            globalLoggerInstance->Flush = FlushLogger;
            globalLoggerInstance->GetMessageLength = GetMessageLength;
        }
        UnlockLoggerResource(GetCriticalSection());
        return globalLoggerInstance;
    }

    void LoggerDestroy()
    {
        LoggerT* logger = LoggerGetInstance();
        if (logger)
        {
            if (logger->data.buffer && logger->data.sink == LogSinkEnumT::LOG_SINK_STDOUT) free(logger->data.buffer);
            free(logger);
            logger = nullptr;
            DeleteCriticalSection(GetCriticalSection());
            DeleteCriticalSection(GetCriticalSectionFlushing());
            free(cs);
            free(cs_flush);
        }
    }

    void LoggerCreate()
    {
        if (cs == NULL) { cs = (CRITICAL_SECTION*) malloc(sizeof(CRITICAL_SECTION)); }
        if (cs_flush == NULL) { cs_flush = (CRITICAL_SECTION*) malloc(sizeof(CRITICAL_SECTION)); }

        if (cs != NULL) { InitializeCriticalSection(cs); }
        if (cs_flush != NULL) { InitializeCriticalSection(cs_flush); }
    }

    void LoggerChangeDstToSTDOUT()
    {
        LockLoggerResource(GetCriticalSection());
        LoggerDataT* instance = &LoggerGetInstance()->data;
        instance->sink = LOG_SINK_STDOUT;
        instance->flushBuffer = nullptr;
        UnlockLoggerResource(GetCriticalSection());
    }

    void LoggerChangeDstToBuffer(char* buffer, size_t length)
    {
        LockLoggerResource(GetCriticalSection());
        LoggerDataT* instance = &LoggerGetInstance()->data;
        instance->sink = LOG_SINK_BUFFER;
        instance->flushBuffer = buffer;
        if (instance->length != length)
        {
            LoggerGetInstance()->Flush();
            if (instance->buffer) free(instance->buffer);
            instance->buffer = (char*) malloc(length);
        }
        instance->length = length;
        UnlockLoggerResource(GetCriticalSection());
    }
#ifdef __cplusplus
}// namespace Engine
#endif