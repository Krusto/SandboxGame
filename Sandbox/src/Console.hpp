#pragma once

#include <Engine.hpp>

class Console
{
public:
    Console() = default;
    ~Console() = default;

    void Create();

    void Draw();

private:
    //static void Log(LogEvent* event, void* param);
};