#pragma once
#include <array>
#include "VoxelVertex.hpp"

namespace Engine
{
    inline static constexpr std::array<VoxelVertex, 4> TopBlockFaceVertices = {
            VoxelVertex{glm::vec3(0, 1, 1), 0, glm::vec2{0, 1}}, VoxelVertex{glm::vec3(1, 1, 1), 0, glm::vec2{0, 0}},
            VoxelVertex{glm::vec3(1, 1, 0), 0, glm::vec2{1, 0}}, VoxelVertex{glm::vec3(0, 1, 0), 0, glm::vec2{1, 1}}};

    inline static constexpr std::array<VoxelVertex, 4> BottomBlockFaceVertices = {
            VoxelVertex{glm::vec3(0, 0, 0), 1, glm::vec2{1, 1}}, VoxelVertex{glm::vec3(1, 0, 0), 1, glm::vec2{1, 0}},
            VoxelVertex{glm::vec3(1, 0, 1), 1, glm::vec2{0, 0}}, VoxelVertex{glm::vec3(0, 0, 1), 1, glm::vec2{0, 1}}};

    inline static constexpr std::array<VoxelVertex, 4> LeftBlockFaceVertices = {
            VoxelVertex{glm::vec3(0, 1, 0), 2, glm::vec2{0, 1}}, VoxelVertex{glm::vec3(0, 0, 0), 2, glm::vec2{0, 0}},
            VoxelVertex{glm::vec3(0, 0, 1), 2, glm::vec2{1, 0}}, VoxelVertex{glm::vec3(0, 1, 1), 2, glm::vec2{1, 1}}};

    inline static constexpr std::array<VoxelVertex, 4> RightBlockFaceVertices = {
            VoxelVertex{glm::vec3(1, 1, 0), 3, glm::vec2{1, 1}}, VoxelVertex{glm::vec3(1, 0, 0), 3, glm::vec2{1, 0}},
            VoxelVertex{glm::vec3(1, 0, 1), 3, glm::vec2{0, 0}}, VoxelVertex{glm::vec3(1, 1, 1), 3, glm::vec2{0, 1}}};

    inline static constexpr std::array<VoxelVertex, 4> FrontBlockFaceVertices = {
            VoxelVertex{glm::vec3(1, 0, 1), 4, glm::vec2{1, 1}}, VoxelVertex{glm::vec3(1, 1, 1), 4, glm::vec2{1, 0}},
            VoxelVertex{glm::vec3(0, 1, 1), 4, glm::vec2{0, 0}}, VoxelVertex{glm::vec3(0, 0, 1), 4, glm::vec2{0, 1}}};

    inline static constexpr std::array<VoxelVertex, 4> BackBlockFaceVertices = {
            VoxelVertex{glm::vec3(0, 1, 0), 5, glm::vec2{0, 1}}, VoxelVertex{glm::vec3(1, 1, 0), 5, glm::vec2{0, 0}},
            VoxelVertex{glm::vec3(1, 0, 0), 5, glm::vec2{1, 0}}, VoxelVertex{glm::vec3(0, 0, 0), 5, glm::vec2{1, 1}}};

    inline static const std::array<VoxelVertex, 4> BlockFaceVertices[6] = {
            LeftBlockFaceVertices,   RightBlockFaceVertices, TopBlockFaceVertices,
            BottomBlockFaceVertices, FrontBlockFaceVertices, BackBlockFaceVertices};
}// namespace Engine