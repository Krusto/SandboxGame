#pragma once

#ifdef LINK_SHARED
#ifdef EngineUtils_EXPORTS
#define EXPORT_UTILS __declspec(dllexport)
#else
#define EXPORT_UTILS __declspec(dllimport)
#endif
#else
#define EXPORT_UTILS
#endif
