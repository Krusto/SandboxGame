#pragma once
#include <cstdint>

namespace Engine
{
    constexpr size_t CHUNK_SIZE = 32;
    constexpr size_t CHUNK_SIZE_SQUARE = CHUNK_SIZE * CHUNK_SIZE;
    constexpr size_t CHUNK_SIZE_CUBIC = CHUNK_SIZE_SQUARE * CHUNK_SIZE;

    constexpr size_t CHUNK_SIZE_PADDED = 34;
    constexpr size_t CHUNK_SIZE_SQUARE_PADDED = CHUNK_SIZE_PADDED * CHUNK_SIZE_PADDED;
    constexpr size_t CHUNK_SIZE_CUBIC_PADDED = CHUNK_SIZE_SQUARE_PADDED * CHUNK_SIZE_PADDED;

    constexpr size_t BLOCKS_PER_ROW = 32;
    constexpr size_t BLOCKS_PER_ROW_SQUARE = BLOCKS_PER_ROW * BLOCKS_PER_ROW;

    constexpr size_t CHUNK_SIZE_COMPRESSED = 32;
    constexpr size_t CHUNK_SIZE_SQUARE_COMPRESSED = CHUNK_SIZE_COMPRESSED * CHUNK_SIZE_COMPRESSED;

    // constexpr size_t CHUNK_SIZE = 62;
    // constexpr size_t CHUNK_SIZE_SQUARE = CHUNK_SIZE * CHUNK_SIZE;
    // constexpr size_t CHUNK_SIZE_CUBIC = CHUNK_SIZE_SQUARE * CHUNK_SIZE;

    // constexpr size_t CHUNK_SIZE_PADDED = CHUNK_SIZE + 2;
    // constexpr size_t CHUNK_SIZE_SQUARE_PADDED = CHUNK_SIZE_PADDED * CHUNK_SIZE_PADDED;
    // constexpr size_t CHUNK_SIZE_CUBIC_PADDED = CHUNK_SIZE_SQUARE_PADDED * CHUNK_SIZE_PADDED;

    // constexpr size_t BLOCKS_PER_ROW = 62;
    // constexpr size_t BLOCKS_PER_ROW_SQUARE = BLOCKS_PER_ROW * BLOCKS_PER_ROW;

    // constexpr size_t CHUNK_SIZE_COMPRESSED = 62;
    // constexpr size_t CHUNK_SIZE_SQUARE_COMPRESSED = CHUNK_SIZE_COMPRESSED * CHUNK_SIZE_COMPRESSED;
}// namespace Engine