#include "Console.hpp"
#include <Util/Log.hpp>
#include <imgui.h>
#include <mutex>

inline static std::mutex mEventsMutex;
inline static std::vector<LogEvent> m_LogEvents;

void Log(LogEvent* event, void* param)
{
    std::lock_guard<std::mutex> lock(mEventsMutex);
    m_LogEvents.push_back(*event);
    if (m_LogEvents.size() > 30) { m_LogEvents.erase(m_LogEvents.begin()); }
}

void Console::Create() { Engine::LoggerAttachHandler(Log, nullptr, LogPolicy::APPEND_POLICY); }

void Console::Draw()
{
    std::lock_guard<std::mutex> lock(mEventsMutex);
    ImGui::Begin("Console");
    for (auto& event: m_LogEvents) { ImGui::Text(event.message); }
    ImGui::End();
}