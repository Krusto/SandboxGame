#pragma once

#include <memory>
#include <string_view>
#include <Core/Ref.hpp>
#include <Core/Core.hpp>
#include <Core/Predefines.hpp>

namespace Engine
{
    class Window;

    class EXPORT_ENGINE Layer
    {
    public:
        virtual ~Layer() = default;

    public:
        virtual void Init(Window* window) = 0;

        virtual void OnAttach() = 0;

        virtual void OnUpdate(double dt) = 0;

        virtual void OnFixedUpdate(double dt) = 0;

        virtual void OnDetach() = 0;

        virtual void Destroy() = 0;

        virtual void OnImGuiBegin() = 0;

        virtual void OnImGuiDraw() = 0;

        virtual void OnImGuiEnd() = 0;

        virtual void OnMouseMoveEvent(int width, int height) = 0;

        virtual void OnKeyboardEvent(int action, int key) = 0;

        virtual void OnWindowResizeEvent(int width, int height) = 0;

        virtual void OnMouseScrollEvent(double x, double y) = 0;

        virtual void OnWindowShouldCloseEvent() = 0;

        virtual void Begin() = 0;

        virtual void End() = 0;

        virtual std::string_view GetName() = 0;

        virtual std::string_view GetName() const = 0;

        virtual bool ShouldExit() = 0;

        virtual void SetShouldExit(bool value) = 0;
    };
}// namespace Engine
