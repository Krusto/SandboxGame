#include "LayerStack.hpp"
#include <Window/Window.hpp>
#include <Renderer/Loader.hpp>

namespace Engine
{
    inline static std::vector<Layer*> s_Layers;

    void LayerStack::InitLayers(Window* window)
    {
        Loader::Load();
        for (auto layer: s_Layers) { layer->Init(window); }
    }

    void LayerStack::PushLayer(Layer* layer)
    {
        if (!LayerStack::ContainsLayer(layer->GetName()))
        {
            s_Layers.push_back(layer);
            layer->OnAttach();
        }
    }

    void LayerStack::PopLayer(std::string name)
    {
        if (LayerStack::ContainsLayer(name))
        {
            s_Layers[FindLayerIndex(name)]->OnDetach();

            s_Layers.erase(s_Layers.begin() + FindLayerIndex(name));
        }
    }

    void LayerStack::PopLayers()
    {
        for (uint32_t index = 0; index < s_Layers.size(); ++index) { s_Layers[index]->OnDetach(); }
    }

    void LayerStack::DestroyLayers()
    {
        for (auto& layer: s_Layers) { layer->Destroy(); }
        for (auto& layer: s_Layers) { Engine::Allocator::Deallocate(layer); }

        s_Layers.clear();
    }

    Layer* LayerStack::GetLayer(std::string name) { return s_Layers[FindLayerIndex(name)]; }

    auto LayerStack::end() { return s_Layers.end(); }

    auto LayerStack::begin() { return s_Layers.begin(); }

    std::vector<Layer*>& LayerStack::data() { return s_Layers; }

    uint32_t LayerStack::FindLayerIndex(std::string_view name)
    {

        for (uint32_t i = 0; i < s_Layers.size(); i++)
        {
            if (s_Layers[i]->GetName() == name) return i;
        }
        return s_Layers.size();
    }

    bool LayerStack::ContainsLayer(std::string_view name)
    {
        if (FindLayerIndex(name) != s_Layers.size()) { return true; }
        return false;
    }

}// namespace Engine