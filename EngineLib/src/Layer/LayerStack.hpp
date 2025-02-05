#pragma once
#include "Layer.hpp"
#include <Core/Predefines.hpp>
#include <Core/Core.hpp>
#include <Core/Ref.hpp>
#include <unordered_map>
#include <utility>
#include <vector>

namespace Engine
{

    class EXPORT_ENGINE LayerStack
    {
    public:
        LayerStack() = default;

        static void InitLayers(Window* window,const char* working_directory);

        static void PushLayer(Layer* layer);

        template <typename T, typename... Args>
        static void ConstructAndPushLayer(Args&&... args);

        static void PopLayer(std::string name);

        static void PopLayers();

        static void DestroyLayers();

        static Layer* GetLayer(std::string name);

        static auto end();

        static auto begin();

        static std::vector<Layer*>& data();

    private:
        static uint32_t FindLayerIndex(std::string_view name);

        static bool ContainsLayer(std::string_view name);
    };
}// namespace Engine

#include "LayerStack.impl"
