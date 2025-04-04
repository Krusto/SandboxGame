#pragma once
#include <Core/Types.hpp>
using std::size_t;

namespace Engine
{
    constexpr std::size_t CHUNK_SIZE = 32;
    constexpr std::size_t CHUNK_SIZE_SQUARE = CHUNK_SIZE * CHUNK_SIZE;
    constexpr std::size_t CHUNK_SIZE_CUBIC = CHUNK_SIZE_SQUARE * CHUNK_SIZE;

    constexpr std::size_t CHUNK_SIZE_PADDED = 34;
    constexpr std::size_t CHUNK_SIZE_SQUARE_PADDED = CHUNK_SIZE_PADDED * CHUNK_SIZE_PADDED;
    constexpr std::size_t CHUNK_SIZE_CUBIC_PADDED = CHUNK_SIZE_SQUARE_PADDED * CHUNK_SIZE_PADDED;

    constexpr std::size_t BLOCKS_PER_ROW = 32;
    constexpr std::size_t BLOCKS_PER_ROW_SQUARE = BLOCKS_PER_ROW * BLOCKS_PER_ROW;

    constexpr std::size_t CHUNK_SIZE_COMPRESSED = 32;
    constexpr std::size_t CHUNK_SIZE_SQUARE_COMPRESSED = CHUNK_SIZE_COMPRESSED * CHUNK_SIZE_COMPRESSED;

    // constexpr std::size_t CHUNK_SIZE = 62;
    // constexpr std::size_t CHUNK_SIZE_SQUARE = CHUNK_SIZE * CHUNK_SIZE;
    // constexpr std::size_t CHUNK_SIZE_CUBIC = CHUNK_SIZE_SQUARE * CHUNK_SIZE;

    // constexpr std::size_t CHUNK_SIZE_PADDED = CHUNK_SIZE + 2;
    // constexpr std::size_t CHUNK_SIZE_SQUARE_PADDED = CHUNK_SIZE_PADDED * CHUNK_SIZE_PADDED;
    // constexpr std::size_t CHUNK_SIZE_CUBIC_PADDED = CHUNK_SIZE_SQUARE_PADDED * CHUNK_SIZE_PADDED;

    // constexpr std::size_t BLOCKS_PER_ROW = 62;
    // constexpr std::size_t BLOCKS_PER_ROW_SQUARE = BLOCKS_PER_ROW * BLOCKS_PER_ROW;

    // constexpr std::size_t CHUNK_SIZE_COMPRESSED = 62;
    // constexpr std::size_t CHUNK_SIZE_SQUARE_COMPRESSED = CHUNK_SIZE_COMPRESSED * CHUNK_SIZE_COMPRESSED;
}// namespace Engine