
#define GLFW_INCLUDE_NONE

#include <Engine.hpp>
#include <Layer/TerrainGenerationToolLayer.hpp>
#include <Layer/GUILayer.hpp>


using namespace Engine;

class TerrainGenerationTool: public Engine::Application
{
public:
    TerrainGenerationTool(const ApplicationSpec& spec)
    {
        Engine::LayerStack::ConstructAndPushLayer<GUILayer>(spec);
        Engine::LayerStack::ConstructAndPushLayer<TerrainGenerationToolLayer>(spec);
    }

    ~TerrainGenerationTool() {}
};

int main()
{
    auto spec = Engine::ApplicationSpec{"TerrainGenerationTool", "E:/Projects/SandboxGame", Engine::Version{1, 0, 0},
                                        1280, 720};
    {
        TerrainGenerationTool test(spec);
        test.Init(spec);
        test.Run();

        test.Destroy();
    }
    Engine::Allocator::AnalyzeMemory();
    Engine::Allocator::DeallocateAll();
    Engine::Allocator::AnalyzeMemory();
    return 0;
}
