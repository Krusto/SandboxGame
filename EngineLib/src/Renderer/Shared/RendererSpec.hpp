#pragma once
#include "Viewport.hpp"
#include <vector>

struct RendererSpec {
    ViewportSize SurfaceSize;
    const char* extensions[20];
};
