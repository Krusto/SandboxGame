#include "Camera.hpp"

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#include <Renderer/Shader.hpp>
#include <Core/Log.hpp>

namespace Engine
{

    void Camera::Init(CameraSpec spec)
    {
        m_CameraSpec = spec;
        m_VectorUP = {0, 1, 0};
        m_VectorForward = {0, 0, -1};
        Update();
    }

    void Camera::ChangeViewport(uint32_t width, uint32_t height)
    {
        m_CameraSpec.viewport = {width, height};
        m_CameraSpec.AspectRatio = (float) width / (float) height;
    }

    void Camera::Update()
    {
        if (startedEvent) { m_Position += m_MoveVector; }

        m_View = glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.x), {1, 0, 0});
        m_View *= glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.y), {0, 1, 0});
        m_View *= glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.z), {0, 0, 1});
        m_View *= glm::lookAt(m_Position, m_Position + m_VectorForward, m_VectorUP);

        m_Projection = glm::perspective(glm::radians(m_CameraSpec.fov), m_CameraSpec.AspectRatio, m_CameraSpec.Near,
                                        m_CameraSpec.Far);
    }

    void Camera::Move(glm::vec3 dir)
    {
        m_Position += dir;
        Update();
    }

    void Camera::Rotate(glm::vec3 rot)
    {
        m_Rotation += rot;
        Update();
    }

    void Camera::Upload(Shader* shader, std::string_view uProjection, std::string_view uView,
                        std::string_view uPosition)
    {
        Update();
        shader->Bind();
        shader->SetUniform(std::string(uProjection), m_Projection);
        shader->SetUniform(std::string(uView), m_View);
        shader->SetUniform(std::string(uPosition), m_Position);
    }

    void Camera::ProcessMouseMovement(float x, float y, float dt, float sensitivity, bool constrainPitch)
    {
        if (m_FirstMouse)
        {
            m_LastMousePos = {x, y};
            m_FirstMouse = false;
        }

        float xoffset = x - m_LastMousePos.x;
        float yoffset = m_LastMousePos.y - y;

        xoffset *= sensitivity * dt;
        yoffset *= sensitivity * dt;

        m_LastMousePos = {x, y};

        m_Rotation.y += xoffset;
        m_Rotation.x -= yoffset;

        if (constrainPitch)
        {
            if (m_Rotation.x > 89.0f) m_Rotation.x = 89.0f;
            if (m_Rotation.x < -89.0f) m_Rotation.x = -89.0f;
        }

        Update();
    }

    void Camera::ProcessKeyboardInput(int action, int key, float dt, float speed)
    {
        if (action == GLFW_PRESS)
        {
            switch (key)
            {
                case GLFW_KEY_W:
                    m_MoveVector = {-speed * dt * cos(glm::radians(GetRotation().y + 90.0f)), 0,
                                    -speed * dt * sin(glm::radians(GetRotation().y + 90.0f))};
                    break;
                case GLFW_KEY_S:
                    m_MoveVector = {speed * dt * cos(glm::radians(GetRotation().y + 90.0f)), 0,
                                    speed * dt * sin(glm::radians(GetRotation().y + 90.0f))};
                    break;
                case GLFW_KEY_D:
                    m_MoveVector = {speed * dt * cos(glm::radians(GetRotation().y)), 0,
                                    speed * dt * sin(glm::radians(GetRotation().y))};
                    break;
                case GLFW_KEY_A:
                    m_MoveVector = {-speed * dt * cos(glm::radians(GetRotation().y)), 0,
                                    -speed * dt * sin(glm::radians(GetRotation().y))};
                case GLFW_KEY_SPACE:
                    m_MoveVector = {0, speed * dt, 0};
                    break;
                case GLFW_KEY_LEFT_SHIFT:
                    m_MoveVector = {0, -speed * dt, 0};
                    break;
                default:
                    m_MoveVector = {};
            }
            if (!startedEvent) startedEvent = true;
        }
        else if (action == GLFW_RELEASE)
        {
            if (startedEvent) startedEvent = false;
        }
        Update();
    }

}// namespace Engine