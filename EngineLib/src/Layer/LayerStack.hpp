#pragma once
#include "Layer.hpp"
#include <unordered_map>
#include <utility>
#include <vector>

namespace Engine
{

    class LayerStack
    {
    public:
        LayerStack() = default;

        static void PushLayer(std::weak_ptr<Layer> layer)
        {
            if (layer.expired()) { return; }

            auto layerPtr = layer.lock();
            if (!LayerStack::ContainsLayer(layerPtr->GetName()))
            {
                LayerStack::m_Layers.push_back(layerPtr);
                layerPtr->OnAttach();
            }
        }

        template <typename T, typename... Args>
        static void ConstructAndPushLayer(Args&&... args)
        {
            LayerStack::PushLayer(std::shared_ptr<Layer>(new T(std::forward<Args>(args)...)));
        }

        static void PopLayer(std::string name)
        {
            if (LayerStack::ContainsLayer(name))
            {
                LayerStack::m_Layers[FindLayerIndex(name)]->OnDetach();

                LayerStack::m_Layers.erase(LayerStack::m_Layers.begin() + FindLayerIndex(name));
            }
        }

        static void PopLayers()
        {
            for (uint32_t index = 0; index < m_Layers.size(); ++index)
            {
                auto& layer = m_Layers[index];
                layer->OnDetach();
            }
        }

        static void InitLayers(std::shared_ptr<Window> window);

        static void DestroyLayers()
        {
            for (auto& layer: m_Layers) { layer->Destroy(); }
            m_Layers.clear();
        }

        static std::shared_ptr<Layer> GetLayer(std::string name) { return LayerStack::m_Layers[FindLayerIndex(name)]; }

        static auto end() { return LayerStack::m_Layers.end(); }

        static auto begin() { return LayerStack::m_Layers.begin(); }

        static auto& data() { return LayerStack::m_Layers; }

    private:
        inline static std::vector<std::shared_ptr<Layer>> m_Layers;

        static uint32_t FindLayerIndex(std::string_view name)
        {

            for (uint32_t i = 0; i < m_Layers.size(); i++)
            {
                if (m_Layers[i]->GetName() == name) return i;
            }
            return LayerStack::m_Layers.size();
        }

        static bool ContainsLayer(std::string_view name)
        {
            if (FindLayerIndex(name) != LayerStack::m_Layers.size()) { return true; }
            return false;
        }
    };
}// namespace Engine