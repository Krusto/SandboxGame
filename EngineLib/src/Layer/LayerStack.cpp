#include "LayerStack.hpp"
#include <Renderer/Window.hpp>

namespace Engine
{
    void LayerStack::InitLayers(std::shared_ptr<Window> window)
    {
        for (auto layer: m_Layers) { layer->Init(window); }
    }

}// namespace Engine