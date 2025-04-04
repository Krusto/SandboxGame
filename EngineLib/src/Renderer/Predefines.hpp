#pragma once

#ifdef LINK_SHARED
#ifdef Renderer_EXPORTS
#if defined(_MSC_VER)
#define EXPORT_RENDERER extern "C" __declspec(dllexport)
#else
#define EXPORT_RENDERER extern "C" __attribute__((visibility("default")))
#endif
#else
// #define EXPORT_RENDERER __declspec(dllimport)
#endif
#else
#define EXPORT_RENDERER
#endif

#ifndef asTPtr
#define asTPtr(x, ...) ((__VA_ARGS__*) (x))
#define asUnorderedMap(x, key, value) (*static_cast<std::unordered_map<key, value>*>(x))
#endif