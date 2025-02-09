
#define GLFW_INCLUDE_NONE

#include <Engine.hpp>
#include <Layer/TestLayer.hpp>
#include <Layer/GUILayer.hpp>


using namespace Engine;

class Test: public Engine::Application
{
public:
    Test(const ApplicationSpec& spec)
    {
        Engine::LayerStack::ConstructAndPushLayer<GUILayer>(spec);
        Engine::LayerStack::ConstructAndPushLayer<TestLayer>(spec);
    }

    ~Test() {}
};

int main()
{
    auto spec = Engine::ApplicationSpec{"Test", "E:/Projects/SandboxGame", Engine::Version{1, 0, 0}, 1280, 720};
    {
        Test test(spec);
        test.Init(spec);
        test.Run();

        test.Destroy();
    }
    Engine::Allocator::AnalyzeMemory();
    Engine::Allocator::DeallocateAll();
    Engine::Allocator::AnalyzeMemory();
    return 0;
}
