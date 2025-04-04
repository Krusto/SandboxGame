#ifndef CFILESYSTEM_H
#define CFILESYSTEM_H

#ifdef _WIN32
#include <Windows.h>
#else
#include <sys/stat.h>
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
#else
    static bool FileExistsLinux(const char* path);
#endif

    inline static bool FileExists(const char* path)
    {
#ifdef _WIN32
        return FileExistsWin32(path);
#else
    return FileExistsLinux(path);
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
#else 
    inline static bool FileExistsLinux(const char* path)
    {
        struct stat buffer;
        return (stat(path, &buffer) == 0);
    }
#endif

#ifdef __cplusplus
}
#endif
#endif
