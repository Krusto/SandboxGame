#pragma once
#include <cstdint>
#include "OpenGLImage.hpp"

namespace Engine
{

    struct FramebufferData {
        uint32_t width{};
        uint32_t height{};
        uint32_t fbo{};
        Image color_attachment{};
        Image depth_attachment{};
    };
}// namespace Engine