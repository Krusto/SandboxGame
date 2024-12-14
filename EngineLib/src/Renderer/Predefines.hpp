#pragma once

#ifdef LINK_SHARED
#ifdef Renderer_EXPORTS
#define EXPORT_RENDERER __declspec(dllexport)
#else
#define EXPORT_RENDERER __declspec(dllimport)
#endif
#else
#define EXPORT_RENDERER
#endif