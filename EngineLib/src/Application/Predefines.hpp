#pragma once

#ifdef EngineEntry_EXPORTS
#define EXPORT_ENGINE_ENTRY __declspec(dllexport)
#else
#define EXPORT_ENGINE_ENTRY __declspec(dllimport)
#endif