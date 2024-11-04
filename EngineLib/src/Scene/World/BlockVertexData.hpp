#pragma once
#include <array>
#include <Renderer/Vertex.hpp>
inline static const std::array<Engine::Vertex, 4> TopBlockFaceVertices = {
        Engine::Vertex{glm::vec3(0, 1, 1), glm::vec3{0, 1, 0}, glm::vec2{0, 1}},
        Engine::Vertex{glm::vec3(1, 1, 1), glm::vec3{0, 1, 0}, glm::vec2{0, 0}},
        Engine::Vertex{glm::vec3(1, 1, 0), glm::vec3{0, 1, 0}, glm::vec2{1, 0}},
        Engine::Vertex{glm::vec3(0, 1, 0), glm::vec3{0, 1, 0}, glm::vec2{1, 1}}};

inline static const std::array<Engine::Vertex, 4> BottomBlockFaceVertices = {
        Engine::Vertex{glm::vec3(0, 0, 0), glm::vec3{0, 0, 0}, glm::vec2{1, 1}},
        Engine::Vertex{glm::vec3(1, 0, 0), glm::vec3{0, 0, 0}, glm::vec2{1, 0}},
        Engine::Vertex{glm::vec3(1, 0, 1), glm::vec3{0, 0, 0}, glm::vec2{0, 0}},
        Engine::Vertex{glm::vec3(0, 0, 1), glm::vec3{0, 0, 0}, glm::vec2{0, 1}}};

inline static const std::array<Engine::Vertex, 4> LeftBlockFaceVertices = {
        Engine::Vertex{glm::vec3(0, 1, 0), glm::vec3{0, 0, 0}, glm::vec2{0, 1}},
        Engine::Vertex{glm::vec3(0, 0, 0), glm::vec3{0, 0, 0}, glm::vec2{0, 0}},
        Engine::Vertex{glm::vec3(0, 0, 1), glm::vec3{0, 0, 0}, glm::vec2{1, 0}},
        Engine::Vertex{glm::vec3(0, 1, 1), glm::vec3{0, 0, 0}, glm::vec2{1, 1}}};

inline static const std::array<Engine::Vertex, 4> RightBlockFaceVertices = {
        Engine::Vertex{glm::vec3(1, 1, 0), glm::vec3{1, 0, 0}, glm::vec2{1, 1}},
        Engine::Vertex{glm::vec3(1, 0, 0), glm::vec3{1, 0, 0}, glm::vec2{1, 0}},
        Engine::Vertex{glm::vec3(1, 0, 1), glm::vec3{1, 0, 0}, glm::vec2{0, 0}},
        Engine::Vertex{glm::vec3(1, 1, 1), glm::vec3{1, 0, 0}, glm::vec2{0, 1}}};

inline static const std::array<Engine::Vertex, 4> FrontBlockFaceVertices = {
        Engine::Vertex{glm::vec3(1, 0, 1), glm::vec3{0, 0, 0}, glm::vec2{1, 1}},
        Engine::Vertex{glm::vec3(1, 1, 1), glm::vec3{0, 0, 0}, glm::vec2{1, 0}},
        Engine::Vertex{glm::vec3(0, 1, 1), glm::vec3{0, 0, 0}, glm::vec2{0, 0}},
        Engine::Vertex{glm::vec3(0, 0, 1), glm::vec3{0, 0, 0}, glm::vec2{0, 1}}};

inline static const std::array<Engine::Vertex, 4> BackBlockFaceVertices = {
        Engine::Vertex{glm::vec3(0, 1, 0), glm::vec3{0, 0, 1}, glm::vec2{0, 1}},
        Engine::Vertex{glm::vec3(1, 1, 0), glm::vec3{0, 0, 1}, glm::vec2{0, 0}},
        Engine::Vertex{glm::vec3(1, 0, 0), glm::vec3{0, 0, 1}, glm::vec2{1, 0}},
        Engine::Vertex{glm::vec3(0, 0, 0), glm::vec3{0, 0, 1}, glm::vec2{1, 1}}};

inline static const std::array<Engine::Vertex, 4> BlockFaceVertices[6] = {
        LeftBlockFaceVertices,RightBlockFaceVertices,   TopBlockFaceVertices,
        BottomBlockFaceVertices, FrontBlockFaceVertices, BackBlockFaceVertices};