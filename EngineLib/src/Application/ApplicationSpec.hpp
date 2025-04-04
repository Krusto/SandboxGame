#pragma once
#include <string_view>
#include <string>
#include <filesystem>
#include <Core/Types.hpp>
#include <Util/Version.hpp>

namespace Engine
{
    struct ApplicationSpec {
        std::string_view ApplicationName;
        std::filesystem::path WorkingDirectory;
        Engine::Version AppVersion;
        uint32_t width;
        uint32_t height;
    };
}// namespace Engine