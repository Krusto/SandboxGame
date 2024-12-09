#pragma once

#ifdef EngineUtils_EXPORTS
#define EXPORT_UTILS __declspec(dllexport)
#else
#define EXPORT_UTILS __declspec(dllimport)
#endif