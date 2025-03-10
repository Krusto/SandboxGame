#pragma once

#ifdef LINK_SHARED
#ifdef EngineLib_EXPORTS
#define EXPORT_ENGINE __declspec(dllexport)
#define EXPORT_REF __declspec(dllexport)
#else
#ifdef Renderer_EXPORTS
#define EXPORT_REF
#else
#define EXPORT_REF __declspec(dllimport)
#endif
#define EXPORT_ENGINE __declspec(dllimport)
#endif
#else
#define EXPORT_REF
#define EXPORT_ENGINE
#endif