#pragma once
#include "Viewport.hpp"
#include <vector>

struct RendererSpec {
    ViewportSize SurfaceSize;
    std::vector<const char*> extensions;
};
