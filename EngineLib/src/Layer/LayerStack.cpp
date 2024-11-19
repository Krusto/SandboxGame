#include "LayerStack.hpp"
#include <Renderer/Window.hpp>

namespace Engine
{
    void LayerStack::InitLayers(Window* window)
    {
        for (auto layer: m_Layers) { layer->Init(window); }
    }

    void LayerStack::PushLayer(Layer* layer)
    {
        if (!LayerStack::ContainsLayer(layer->GetName()))
        {
            LayerStack::m_Layers.push_back(layer);
            layer->OnAttach();
        }
    }

    void LayerStack::PopLayer(std::string name)
    {
        if (LayerStack::ContainsLayer(name))
        {
            LayerStack::m_Layers[FindLayerIndex(name)]->OnDetach();

            LayerStack::m_Layers.erase(LayerStack::m_Layers.begin() + FindLayerIndex(name));
        }
    }

    void LayerStack::PopLayers()
    {
        for (uint32_t index = 0; index < m_Layers.size(); ++index) { m_Layers[index]->OnDetach(); }
    }

    void LayerStack::DestroyLayers()
    {
        for (auto& layer: m_Layers) { layer->Destroy(); }
        for (auto& layer: m_Layers) { Deallocate(layer); }

        m_Layers.clear();
    }

    Layer* LayerStack::GetLayer(std::string name) { return LayerStack::m_Layers[FindLayerIndex(name)]; }

    auto LayerStack::end() { return LayerStack::m_Layers.end(); }

    auto LayerStack::begin() { return LayerStack::m_Layers.begin(); }

    std::vector<Layer*>& LayerStack::data() { return LayerStack::m_Layers; }

    uint32_t LayerStack::FindLayerIndex(std::string_view name)
    {

        for (uint32_t i = 0; i < m_Layers.size(); i++)
        {
            if (m_Layers[i]->GetName() == name) return i;
        }
        return LayerStack::m_Layers.size();
    }

    bool LayerStack::ContainsLayer(std::string_view name)
    {
        if (FindLayerIndex(name) != LayerStack::m_Layers.size()) { return true; }
        return false;
    }

}// namespace Engine