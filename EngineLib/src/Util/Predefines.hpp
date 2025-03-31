#pragma once

#ifdef LINK_SHARED
#if defined(_MSC_VER)
#ifdef EngineUtils_EXPORTS
#define EXPORT_UTILS __declspec(dllexport)
#else
#define EXPORT_UTILS __declspec(dllimport)
#endif
#elif defined(__GNUC__)
#ifdef EngineUtils_EXPORTS
#define EXPORT_UTILS __attribute__ ((visibility ("default")))
#else
#define EXPORT_UTILS 
#endif
#endif

#else
#define EXPORT_UTILS
#endif
