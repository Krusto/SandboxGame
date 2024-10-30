#pragma once
#include <functional>

class RendererCommand
{
public:
    RendererCommand() = default;

    RendererCommand(std::function<void(void)> command) : m_Command(command) {}

    ~RendererCommand() = default;

public:
    void Execute()
    {
        if (m_Command) { m_Command(); }
    }

    static RendererCommand Create(std::function<void(void)> command)
    {
        RendererCommand result;
        result.m_Command = command;
        return result;
    }

private:
    std::function<void(void)> m_Command;
};