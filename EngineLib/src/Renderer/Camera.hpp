#pragma once
#include <cstdint>
#include <string_view>

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>

#include <Core/Ref.hpp>
#include <Renderer/Viewport.hpp>
#include <Renderer/RendererCommand.hpp>
#include <Renderer/Shader.hpp>
#include <Renderer/Predefines.hpp>

namespace Engine
{
    struct CameraSpec {
    public:
        CameraSpec() = default;

        CameraSpec(ViewportSize size, float FOV, float Near = 0.1f, float Far = 1000.0f)
        {
            this->viewport = size;
            this->fov = FOV;
            this->Near = Near;
            this->Far = Far;
            this->AspectRatio = (float) size.width / (float) size.height;
        }

        ViewportSize viewport{0, 0};
        float fov{};
        float Near = 0.1f;
        float Far = 1000.0f;
        float AspectRatio = 1.0f;
    };

    class Shader;

    class EXPORT_RENDERER Camera: public RefCounted
    {
    public:
        Camera() = default;
        ~Camera() = default;


    public:
        void Init(CameraSpec spec);

        void ChangeViewport(uint32_t width, uint32_t height);

        void Update(double dt, double rotationSpeed, double moveSpeed);

        const auto& GetView() const { return this->m_View; }

        auto& GetView() { return this->m_View; }

        const auto& GetProjection() const { return this->m_Projection; }

        auto& GetProjection() { return this->m_Projection; }

        void SetPosition(glm::vec3 position) { this->m_Position = position; }

        void SetRotation(glm::vec3 rotation) { this->m_Rotation = rotation; }

        const glm::vec3& GetPosition() const { return this->m_Position; }

        glm::vec3& GetPosition() { return this->m_Position; }

        const glm::vec3& GetRotation() const { return this->m_Rotation; }

        glm::vec3& GetRotation() { return this->m_Rotation; }

        void Move(glm::vec3 dir);

        void Rotate(glm::vec3 rot);

        void Upload(Shader* shader, std::string_view uProjection = "camera.projection",
                    std::string_view uView = "camera.view", std::string_view uPosition = "camera.position") const;

        glm::vec3 GetVectorUP() { return m_VectorUP; }

        glm::vec3 GetVectorForward() { return m_VectorForward; }

        CameraSpec& GetSpec() { return m_CameraSpec; }

        const CameraSpec& GetSpec() const { return m_CameraSpec; }

        void ProcessMouseMovement(float x, float y, float sensitivity = 1, bool constrainPitch = true);

        void ProcessKeyboardInput(int action, int key, bool spacePressed, bool shiftPressed);

        void ProcessMouseScroll(float y, double speed = 1);

        RendererCommand UploadCommand(Shader* shader) const;

    private:
        glm::vec3 m_VectorUP{};
        glm::vec3 m_VectorForward{};
        glm::vec3 m_Position{};
        glm::vec3 m_Rotation{};

        glm::mat4 m_Projection{};
        glm::mat4 m_View{};

        CameraSpec m_CameraSpec{};

        bool startedEvent{};
        bool startetScrollEvent{};
        double m_ScrollValue{0.0f};

        double m_MoveDownUp{};
        glm::vec3 m_MoveVector{};
        glm::vec3 m_RotateVector{};

        glm::vec2 m_LastMousePos{0.0f, 0.0f};
    };
}// namespace Engine