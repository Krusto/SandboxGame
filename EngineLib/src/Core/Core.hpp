#pragma once

#ifdef GCC
#define SANDBOX_ENGINE_API __attribute__((visibility("default")))
#else
#ifdef ENGINE_BUILD_DLL
#ifndef SANDBOX_ENGINE_API
#define SANDBOX_ENGINE_API __declspec(dllexport)
#else
#define SANDBOX_ENGINE_API __declspec(dllimport)
#endif
#endif
#endif

#include <Core/Log.hpp>
#include <Core/Ref.hpp>
#include <Core/Allocator.hpp>
