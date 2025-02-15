#ifndef CFILESYSTEM_H
#define CFILESYSTEM_H

#ifdef _WIN32
#include <Windows.h>
#else
#error Add file lib include for Linux
#endif

#include <stdio.h>

#ifdef __cplusplus
#include <filesystem>
namespace Engine
{
#else
#endif

    static bool FileExists(const char* path);

#ifdef _WIN32
    static bool FileExistsWin32(const char* path);
#endif

    inline static bool FileExists(const char* path)
    {
#ifdef _WIN32
        return FileExistsWin32(path);
#else
#error Add FileExists for Linux
#endif
    }


#ifdef _WIN32
    inline static bool FileExistsWin32(const char* path)
    {
        FILE* fp = nullptr;
        fopen_s(&fp, path, "r");
        if (fp == nullptr) { return false; }
        fclose(fp);
        return true;
    }
#endif

#ifdef __cplusplus
}
#endif
#endif
