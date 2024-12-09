#pragma once

#ifdef Renderer_EXPORTS
#define EXPORT_RENDERER __declspec(dllexport)
#else
#define EXPORT_RENDERER __declspec(dllimport)
#endif