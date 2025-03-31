#pragma once

#ifdef LINK_SHARED
#ifdef EngineEntry_EXPORTS
#if defined(_MSC_VER)
#define EXPORT_ENGINE_ENTRY __declspec(dllexport)
#elif defined(__GNUC__)
#define EXPORT_ENGINE_ENTRY __attribute__ ((visibility ("default")))
#endif
#else
#if defined(_MSC_VER)
#define EXPORT_ENGINE_ENTRY __declspec(dllimport)
#elif defined(__GNUC__)
#define EXPORT_ENGINE_ENTRY 
#endif
#endif
#else
#define EXPORT_ENGINE_ENTRY
#endif