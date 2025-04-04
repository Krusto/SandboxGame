#pragma once
#include <Core/Types.hpp>
using std::size_t;

namespace Engine
{
    constexprstd::size_t CHUNK_SIZE = 32;
    constexprstd::size_t CHUNK_SIZE_SQUARE = CHUNK_SIZE * CHUNK_SIZE;
    constexprstd::size_t CHUNK_SIZE_CUBIC = CHUNK_SIZE_SQUARE * CHUNK_SIZE;

    constexprstd::size_t CHUNK_SIZE_PADDED = 34;
    constexprstd::size_t CHUNK_SIZE_SQUARE_PADDED = CHUNK_SIZE_PADDED * CHUNK_SIZE_PADDED;
    constexprstd::size_t CHUNK_SIZE_CUBIC_PADDED = CHUNK_SIZE_SQUARE_PADDED * CHUNK_SIZE_PADDED;

    constexprstd::size_t BLOCKS_PER_ROW = 32;
    constexprstd::size_t BLOCKS_PER_ROW_SQUARE = BLOCKS_PER_ROW * BLOCKS_PER_ROW;

    constexprstd::size_t CHUNK_SIZE_COMPRESSED = 32;
    constexprstd::size_t CHUNK_SIZE_SQUARE_COMPRESSED = CHUNK_SIZE_COMPRESSED * CHUNK_SIZE_COMPRESSED;

    // constexprstd::size_t CHUNK_SIZE = 62;
    // constexprstd::size_t CHUNK_SIZE_SQUARE = CHUNK_SIZE * CHUNK_SIZE;
    // constexprstd::size_t CHUNK_SIZE_CUBIC = CHUNK_SIZE_SQUARE * CHUNK_SIZE;

    // constexprstd::size_t CHUNK_SIZE_PADDED = CHUNK_SIZE + 2;
    // constexprstd::size_t CHUNK_SIZE_SQUARE_PADDED = CHUNK_SIZE_PADDED * CHUNK_SIZE_PADDED;
    // constexprstd::size_t CHUNK_SIZE_CUBIC_PADDED = CHUNK_SIZE_SQUARE_PADDED * CHUNK_SIZE_PADDED;

    // constexprstd::size_t BLOCKS_PER_ROW = 62;
    // constexprstd::size_t BLOCKS_PER_ROW_SQUARE = BLOCKS_PER_ROW * BLOCKS_PER_ROW;

    // constexprstd::size_t CHUNK_SIZE_COMPRESSED = 62;
    // constexprstd::size_t CHUNK_SIZE_SQUARE_COMPRESSED = CHUNK_SIZE_COMPRESSED * CHUNK_SIZE_COMPRESSED;
}// namespace Engine