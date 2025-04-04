//
// Created by Stoyanov, Krusto (K.S.) on 6/3/2022.
//
#pragma once

#include <Engine.hpp>
#include <Core/Ref.hpp>
#include <Application/Predefines.hpp>

namespace Engine
{

    class EXPORT_ENGINE_ENTRY Application
    {
    public:
        Application() = default;
        ~Application() = default;

        void Run();
        void Init(const ApplicationSpec& spec);

        uint32_t GetWidth() { return m_Window->GetSpec()->width; }

        uint32_t GetWidth() const { return m_Window->GetSpec()->width; }

        uint32_t GetHeight() { return m_Window->GetSpec()->height; }

        uint32_t GetHeight() const { return m_Window->GetSpec()->height; }

        auto& Spec() { return m_ApplicationSpec; }

        static Application& Get();

        Window* WindowHandle() { return m_Window; }

        void Destroy();

    public:
        static std::filesystem::path GetExectuablePath() { return std::filesystem::current_path(); }

    protected:
        Window* m_Window{};
        ApplicationSpec m_ApplicationSpec{};
    };

}// namespace Engine
