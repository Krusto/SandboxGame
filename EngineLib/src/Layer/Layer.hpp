#pragma once

#include <memory>
#include <string_view>
#include <Core/Ref.hpp>

namespace Engine
{
    class Window;

    class Layer
    {
    public:
        virtual ~Layer() = default;

    public:
        virtual void Init(Ref<Window> window) = 0;

        virtual void OnAttach() = 0;

        virtual void OnUpdate(float dt) = 0;

        virtual void OnDetach() = 0;

        virtual void Destroy() = 0;

        virtual void OnImGuiDraw() = 0;

        virtual void OnMouseMoveEvent(int width, int height) = 0;

        virtual void OnKeyboardEvent(int action, int key) = 0;

        virtual void OnWindowResizeEvent(int width, int height) = 0;

        virtual void OnWindowShouldCloseEvent() = 0;

        virtual void Begin() = 0;

        virtual void End() = 0;

    public:
        std::string_view GetName();

        std::string_view GetName() const;

        bool ShouldExit();

        void SetShouldExit(bool value);

    protected:
        bool m_ShouldExit{};
        std::string_view m_Name{};
    };
}// namespace Engine