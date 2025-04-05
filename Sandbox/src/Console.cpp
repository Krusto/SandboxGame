#include "Console.hpp"
#include <Util/Log.hpp>
#include <imgui.h>
#include <mutex>

inline static std::mutex mEventsMutex;
inline static std::vector<LogEvent> m_LogEvents;

void Console::FormatEvent(LogEvent* event)
{
    const char* level;
    switch (event->log_level)
    {
        case LogLevel::DEBUG_LEVEL:
            level = "[DEBUG]";
            break;
        case LogLevel::INFO_LEVEL:
            level = "[INFO]";
            break;
        case LogLevel::WARNING_LEVEL:
            level = "[WARNING]";
            break;
        case LogLevel::ERROR_LEVEL:
            level = "[ERROR]";
            break;
        default:
            level = "[UNKNOWN]";
            break;
    }
    char temp[256];
    snprintf(temp, sizeof(event->message), "%s %s", level, event->message);
    memcpy(event->message, temp, sizeof(temp));
}

void Console::Log(LogEvent* event, void* param)
{
    FormatEvent(event);
    std::lock_guard<std::mutex> lock(mEventsMutex);
    m_LogEvents.push_back(*event);
    if (m_LogEvents.size() > 100) { m_LogEvents.erase(m_LogEvents.begin()); }
}

void Console::Create() { Engine::LoggerAttachHandler(Console::Log, nullptr, LogPolicy::APPEND_POLICY); }

void Console::Draw()
{
    std::lock_guard<std::mutex> lock(mEventsMutex);
    ImGui::Begin("Console");
    // for (auto& event: m_LogEvents) { ImGui::Text(event.message); }
    for (auto i = m_LogEvents.rbegin(); i < m_LogEvents.rend(); ++i)
    {
        ImGui::TextUnformatted(i->message, i->message + strlen(i->message));
    }
    ImGui::End();
}