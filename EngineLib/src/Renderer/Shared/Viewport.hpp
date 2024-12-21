#pragma once
#include <cstdint>

struct ViewportSize {
    uint32_t width{};
    uint32_t height{};

    ViewportSize& operator=(ViewportSize&) = default;
    ViewportSize& operator=(const ViewportSize&) = default;
    bool operator==(const ViewportSize& other) {
        return (other.width == width) && (other.height == height);
    }
    bool operator!=(const ViewportSize& other) {
        return !((*this) == other);
    }
};