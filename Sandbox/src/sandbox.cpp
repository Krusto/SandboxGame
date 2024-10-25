
#define GLFW_INCLUDE_NONE
#include <Engine.hpp>
#include "SandboxLayer.hpp"
using namespace Engine;

class Sandbox: public Engine::Application
{
public:
    explicit Sandbox(const ApplicationSpec& spec) { LayerStack::ConstructAndPushLayer<SandboxLayer>(spec); }

    ~Sandbox() = default;
};

int main()
{
    auto spec = Engine::ApplicationSpec{"Sandbox", "./", Engine::Version{1, 0, 0}, 1280, 720};
    auto* sandbox = new Sandbox(spec);
    sandbox->Init(spec);
    sandbox->Run();

    sandbox->Destroy();

    delete sandbox;

    return 0;
}
