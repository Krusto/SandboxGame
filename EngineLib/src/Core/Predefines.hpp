#pragma once

#ifdef LINK_SHARED
#ifdef EngineLib_EXPORTS
    #if defined(_MSC_VER)
    #define EXPORT_ENGINE __declspec(dllexport)
    #define EXPORT_REF __declspec(dllexport)
    #elif defined(__GNUC__)
    #define EXPORT_ENGINE __attribute__ ((visibility ("default")))
    #define EXPORT_REF __attribute__ ((visibility ("default")))
    #endif
#else
#ifdef Renderer_EXPORTS
#define EXPORT_REF
#else
#if defined(_MSC_VER)
#define EXPORT_REF __declspec(dllimport)
#elif defined(__GNUC__)
#define EXPORT_REF 
#endif
#endif
#if defined(_MSC_VER)
#define EXPORT_ENGINE __declspec(dllimport)
#elif defined(__GNUC__)
#define EXPORT_ENGINE 
#endif
#endif
#else
#define EXPORT_REF
#define EXPORT_ENGINE
#endif