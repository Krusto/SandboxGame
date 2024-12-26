#pragma once
#include <windows.h>
#include <imagehlp.h>
#include <stdio.h>

inline static const WCHAR* ConvertToWChar(const char* input)
{
    // Get the length of the input string
    int len = MultiByteToWideChar(CP_ACP, 0, input, -1, NULL, 0);

    if (len == 0)
    {
        // Handle error
        return NULL;
    }

    // Allocate memory for the wide character string (including null terminator)
    WCHAR* wideStr = (WCHAR*) malloc(len * sizeof(WCHAR));

    if (wideStr == NULL)
    {
        // Handle memory allocation failure
        return NULL;
    }

    // Convert the input string to wide characters
    MultiByteToWideChar(CP_ACP, 0, input, -1, wideStr, len);

    return wideStr;
}

inline static void ListExports(const char* dllPath)
{
    // Load the DLL and map it to memory
    auto d = ConvertToWChar(dllPath);
    HANDLE hFile = CreateFile(dllPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        printf("Error: Could not open the file %s\n", dllPath);
        return;
    }
    free((void*) d);
    DWORD fileSize = GetFileSize(hFile, NULL);
    if (fileSize == INVALID_FILE_SIZE)
    {
        printf("Error: Could not get the file size.\n");
        CloseHandle(hFile);
        return;
    }

    // Map the DLL file into memory
    HANDLE hMapping = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
    if (hMapping == NULL)
    {
        printf("Error: Could not create file mapping.\n");
        CloseHandle(hFile);
        return;
    }

    LPBYTE fileData = (LPBYTE) MapViewOfFile(hMapping, FILE_MAP_READ, 0, 0, 0);
    if (fileData == NULL)
    {
        printf("Error: Could not map file to memory.\n");
        CloseHandle(hMapping);
        CloseHandle(hFile);
        return;
    }

    // Parse the PE headers
    PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER) fileData;
    if (dosHeader->e_magic != IMAGE_DOS_SIGNATURE)
    {
        printf("Error: Not a valid PE file.\n");
        UnmapViewOfFile(fileData);
        CloseHandle(hMapping);
        CloseHandle(hFile);
        return;
    }

    IMAGE_NT_HEADERS ntHeaders = *(PIMAGE_NT_HEADERS) (fileData + dosHeader->e_lfanew);
    if (ntHeaders.Signature != IMAGE_NT_SIGNATURE)
    {
        printf("Error: Not a valid NT header.\n");
        UnmapViewOfFile(fileData);
        CloseHandle(hMapping);
        CloseHandle(hFile);
        return;
    }


        // Extract sections (for example, the code section)
    printf("\nSections in the DLL:\n");
    PIMAGE_SECTION_HEADER sectionHeader =
            (PIMAGE_SECTION_HEADER) ((PBYTE)&ntHeaders.OptionalHeader + ntHeaders.FileHeader.SizeOfOptionalHeader);

    for (int i = 0; i < ntHeaders.FileHeader.NumberOfSections; i++)
    {
        printf("Section %d: %s\n", i + 1, sectionHeader[i].Name);
        printf("  Virtual Size: %d\n", sectionHeader[i].Misc.VirtualSize);
        printf("  Virtual Address: 0x%08X\n", sectionHeader[i].VirtualAddress);
        printf("  Size of Raw Data: %d\n", sectionHeader[i].SizeOfRawData);
        printf("  Pointer to Raw Data: 0x%08X\n", sectionHeader[i].PointerToRawData);
    }

    // Get the export directory from the PE header
    PIMAGE_EXPORT_DIRECTORY exportDir =
            (PIMAGE_EXPORT_DIRECTORY) ((PBYTE) fileData + sectionHeader->PointerToRawData +
                                       ntHeaders.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT]
                                               .VirtualAddress -
                                       sectionHeader->VirtualAddress);
    if (exportDir == NULL)
    {
        printf("Error: No export directory found.\n");
        UnmapViewOfFile(fileData);
        CloseHandle(hMapping);
        CloseHandle(hFile);
        return;
    }

    // Get pointers to the export functions, names, and ordinals
    DWORD* functions = (DWORD*) (fileData + exportDir->AddressOfFunctions);
    DWORD* names = (DWORD*) (fileData + exportDir->AddressOfNames);
    WORD* ordinals = (WORD*) (fileData + exportDir->AddressOfNameOrdinals);

    printf("Exported functions from %s:\n", dllPath);
    for (DWORD i = 0; i < exportDir->NumberOfNames; i++)
    {
        // Get function name
        char* functionName = (char*) (fileData + names[i]);
        printf("%s\n", functionName);
    }

    // Cleanup
    UnmapViewOfFile(fileData);
    CloseHandle(hMapping);
    CloseHandle(hFile);
}

namespace Engine
{
    class Loader
    {
    public:
        inline static void Load()
        {
            //hLibInstance = LoadLibraryA("RendererOpenGL.dll");

            //if (hLibInstance == NULL) { printf("Error: Could not load library\n"); }

            ListExports("E:\\Projects\\SandboxGame\\build\\bin\\Debug\\RendererOpenGL.dll");

            //FreeLibrary(hLibInstance);
        }

        inline static HINSTANCE hLibInstance{};
    };
}// namespace Engine