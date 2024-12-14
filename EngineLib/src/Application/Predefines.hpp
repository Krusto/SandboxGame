#pragma once

#ifdef LINK_SHARED
#ifdef EngineEntry_EXPORTS
#define EXPORT_ENGINE_ENTRY __declspec(dllexport)
#else
#define EXPORT_ENGINE_ENTRY __declspec(dllimport)
#endif
#else
#define EXPORT_ENGINE_ENTRY
#endif