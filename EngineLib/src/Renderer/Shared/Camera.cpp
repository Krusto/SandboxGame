#include "Camera.hpp"

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#include <Renderer/Shared/APISpecific/Shader.hpp>
#include <Core/Log.hpp>

namespace Engine
{

    void Camera::Init(CameraSpec spec)
    {
        m_CameraSpec = spec;
        m_VectorUP = {0, 1, 0};
        m_VectorForward = {-1, 0, 0};
    }

    void Camera::ChangeViewport(uint32_t width, uint32_t height)
    {
        m_CameraSpec.viewport = {width, height};
        m_CameraSpec.AspectRatio = (float) width / (float) height;
    }

    void Camera::Update(double dt, double rotationSpeed, double moveSpeed)
    {
        if (m_ScrollValue != 0.0f)
        {
            m_CameraSpec.fov -= m_ScrollValue * dt;
            if (m_CameraSpec.fov > 90) { m_CameraSpec.fov = 90.0f; }
            if (m_CameraSpec.fov < 1.0f) { m_CameraSpec.fov = 1.0f; }
            m_ScrollValue = 0.0f;
        }

        if (m_RotateVector != glm::vec3{})
        {
            double speed = 1 / rotationSpeed;
            glm::vec3 vel = {m_RotateVector.x * speed * dt, m_RotateVector.y * speed * dt,
                             m_RotateVector.z * speed * dt};

            Rotate(vel);

            // LOG_INFO("Rotation: %f %f %f\n", m_Rotation.x, m_Rotation.y, m_Rotation.z);
            m_RotateVector = {};

            if (m_Rotation.x > 89.0f) m_Rotation.x = 89.0f;
            if (m_Rotation.x < -89.0f) m_Rotation.x = -89.0f;

            if (m_Rotation.y > 360.0f) m_Rotation.y -= 360.0f;
            if (m_Rotation.y < -360.0f) m_Rotation.y += 360.0f;
        }
        if (m_MoveVector != glm::vec3{} || m_MoveDownUp)
        {
            float distance =
                    m_MoveVector.x * m_MoveVector.x + m_MoveDownUp * m_MoveDownUp + m_MoveVector.z * m_MoveVector.z;
            double speed = 1 / moveSpeed;

            distance = speed / sqrt(distance);
            m_MoveVector.x *= distance;
            m_MoveDownUp *= distance;
            m_MoveVector.z *= distance;

            double s = glm::sin(glm::radians(m_Rotation.y));
            double c = glm::cos(glm::radians(m_Rotation.y));
            double mX = m_MoveVector.x * c - m_MoveVector.z * s;
            double mZ = m_MoveVector.z * c + m_MoveVector.x * s;

            m_Position.x += mX * dt;
            m_Position.y += m_MoveDownUp * dt;
            m_Position.z += mZ * dt;

            LOG_INFO("Position: %.2f %.2f %.2f", m_Position.x, m_Position.y, m_Position.z);
        }

        m_View = glm::mat4(1.0);
        m_View = glm::rotate(m_View, glm::radians(m_Rotation.x), glm::vec3(1, 0, 0));
        m_View = glm::rotate(m_View, glm::radians(m_Rotation.y), glm::vec3(0, 1, 0));
        m_View = glm::rotate(m_View, glm::radians(m_Rotation.z), glm::vec3(0, 0, 1));
        m_View *= glm::lookAt(m_Position, m_Position + m_VectorForward, m_VectorUP);

        m_Projection = glm::perspective(glm::radians(m_CameraSpec.fov), m_CameraSpec.AspectRatio, m_CameraSpec.Near,
                                        m_CameraSpec.Far);
    }

    void Camera::Move(glm::vec3 dir) { m_Position += dir; }

    void Camera::Rotate(glm::vec3 rot) { m_Rotation += rot; }

    void Camera::Upload(Shader* shader, std::string_view uProjection, std::string_view uView,
                        std::string_view uPosition) const
    {
        shader->SetUniform(std::string(uProjection), m_Projection);
        shader->SetUniform(std::string(uView), m_View);
        shader->SetUniform(std::string(uPosition), m_Position);
    }

    void Camera::ProcessMouseMovement(float x, float y, float sensitivity, bool constrainPitch)
    {
        glm::vec3 offset = {(y - m_LastMousePos.y), (x - m_LastMousePos.x), 0.0f};

        m_LastMousePos = {x, y};

        m_RotateVector = offset;
    }

    void Camera::ProcessKeyboardInput(int action, int key, bool spacePressed, bool shiftPressed)
    {
        if (spacePressed) { m_MoveDownUp = 1; }
        else if (shiftPressed) { m_MoveDownUp = -1; }

        if (action == GLFW_PRESS)
        {
            switch (key)
            {
                case GLFW_KEY_W:
                    m_MoveVector = glm::vec3{-1, 0, 0};
                    break;
                case GLFW_KEY_S:
                    m_MoveVector = glm::vec3{1, 0, 0};
                    break;
                case GLFW_KEY_D:
                    m_MoveVector = glm::vec3{0, 0, -1};
                    break;
                case GLFW_KEY_A:
                    m_MoveVector = glm::vec3{0, 0, 1};
                    break;
            }
        }
        else if (action == GLFW_RELEASE)
        {
            m_MoveVector = {};
            m_MoveDownUp = {};
        }
    }

    void Camera::ProcessMouseScroll(float y, double speed) { m_ScrollValue = y * speed; }

    RendererCommand Camera::UploadCommand(Shader* shader) const
    {
        return RendererCommand([=]() { Upload(shader); });
    }

}// namespace Engine