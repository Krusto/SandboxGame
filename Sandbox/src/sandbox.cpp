
#define GLFW_INCLUDE_NONE

#include <Engine.hpp>
#include <Layer/SandboxLayer.hpp>
#include <Layer/GUILayer.hpp>

using namespace Engine;

class Sandbox: public Engine::Application
{
public:
    Sandbox(const ApplicationSpec& spec)
    {
        Engine::LayerStack::ConstructAndPushLayer<GUILayer>(spec);
        Engine::LayerStack::ConstructAndPushLayer<SandboxLayer>(spec);
    }

    ~Sandbox() {}
};

int main()
{
    auto spec = Engine::ApplicationSpec{"Sandbox", "E:/Projects/SandboxGame", Engine::Version{1, 0, 0}, 1280, 720};
    {
        Sandbox sandbox(spec);
        sandbox.Init(spec);
        sandbox.Run();

        sandbox.Destroy();
    }
    Engine::Allocator::AnalyzeMemory();
    Engine::Allocator::DeallocateAll();
    Engine::Allocator::AnalyzeMemory();
    return 0;
}
