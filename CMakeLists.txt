cmake_minimum_required(VERSION 3.20)

project(SandboxGame)

set(CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/Vendor/glfw-3.4/src")
set(BUILD_ENGINE_SHARED ON CACHE BOOL "Build engine shared library" FORCE)
set(BUILD_SHARED_LIBS ON CACHE BOOL "Build engine shared library" FORCE)
set(FASTNOISE2_NOISETOOL OFF CACHE BOOL "Build noise tool" FORCE)
set(FASTNOISE2_TESTS OFF CACHE BOOL "Build noise tests" FORCE)
set(GLFW_LIBRARY_TYPE "SHARED" CACHE STRING "Library type override for GLFW (SHARED, STATIC, OBJECT, or empty to follow BUILD_SHARED_LIBS)")
set(YAML_BUILD_SHARED_LIBS ON CACHE BOOL "Build yaml-cpp shared library" FORCE)

if(BUILD_SHARED_LIBS)
    message("${PROJECT_NAME}: Build engine shared library")
    set(BUILD_LIB_FLAG "SHARED")
else()
    message("${PROJECT_NAME}: Build engine static library")
    set(BUILD_LIB_FLAG "STATIC")
endif()


set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_$<CONFIG> ${CMAKE_SOURCE_DIR}/build/$<CONFIG>/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_$<CONFIG> ${CMAKE_SOURCE_DIR}/build/$<CONFIG>/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_$<CONFIG> ${CMAKE_SOURCE_DIR}/build/$<CONFIG>/bin)

set(CMAKE_CXX_STANDARD 23)
message("Compiler Version: ${CMAKE_CXX_COMPILER_VERSION}")

message("${PROJECT_NAME}: Adding yaml-cpp...")
add_subdirectory("${CMAKE_SOURCE_DIR}/Vendor/yaml-cpp")
find_package(Threads REQUIRED)

message("${PROJECT_NAME}: Adding glfw...")
include(CMakeDependentOption)
cmake_dependent_option(GLFW_BUILD_WIN32 "Build support for Win32" ON "WIN32" OFF)
cmake_dependent_option(GLFW_BUILD_COCOA "Build support for Cocoa" ON "APPLE" OFF)
cmake_dependent_option(GLFW_BUILD_X11 "Build support for X11" ON "UNIX;NOT APPLE" OFF)
cmake_dependent_option(GLFW_BUILD_WAYLAND "Build support for Wayland" ON "UNIX;NOT APPLE" OFF)
cmake_dependent_option(USE_MSVC_RUNTIME_LIBRARY_DLL "Use MSVC runtime library DLL" ON
                       "MSVC" OFF)
if (GLFW_BUILD_WIN32)
    message(STATUS "Including Win32 support")
endif()
if (GLFW_BUILD_COCOA)
    message(STATUS "Including Cocoa support")
endif()
if (GLFW_BUILD_WAYLAND)
    message(STATUS "Including Wayland support")
endif()
if (GLFW_BUILD_X11)
    message(STATUS "Including X11 support")
endif()
if (MSVC AND NOT USE_MSVC_RUNTIME_LIBRARY_DLL)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()
set(GLFW_SOURCE_DIR "${CMAKE_SOURCE_DIR}/Vendor/glfw-3.4/src")

add_library(glfw SHARED
                 "${GLFW_SOURCE_DIR}/../include/GLFW/glfw3.h"
                 "${GLFW_SOURCE_DIR}/../include/GLFW/glfw3native.h"
                 "${GLFW_SOURCE_DIR}/internal.h "
                 "${GLFW_SOURCE_DIR}/platform.h "
                 "${GLFW_SOURCE_DIR}/mappings.h"
                 "${GLFW_SOURCE_DIR}/context.c "
                 "${GLFW_SOURCE_DIR}/init.c "
                 "${GLFW_SOURCE_DIR}/input.c"
                 "${GLFW_SOURCE_DIR}/monitor.c"
                 "${GLFW_SOURCE_DIR}/platform.c"
                 "${GLFW_SOURCE_DIR}/vulkan.c"
                 "${GLFW_SOURCE_DIR}/window.c"
                 "${GLFW_SOURCE_DIR}/egl_context.c "
                 "${GLFW_SOURCE_DIR}/osmesa_context.c"
                 "${GLFW_SOURCE_DIR}/null_platform.h"
                 "${GLFW_SOURCE_DIR}/null_joystick.h"
                 "${GLFW_SOURCE_DIR}/null_init.c"
                 "${GLFW_SOURCE_DIR}/null_monitor.c"
                 "${GLFW_SOURCE_DIR}/null_window.c"
                 "${GLFW_SOURCE_DIR}/null_joystick.c")
if (WIN32)
    target_sources(glfw PRIVATE "${GLFW_SOURCE_DIR}/win32_time.h"
                                "${GLFW_SOURCE_DIR}/win32_thread.h"
                                "${GLFW_SOURCE_DIR}/win32_module.c"
                                "${GLFW_SOURCE_DIR}/win32_time.c"
                                "${GLFW_SOURCE_DIR}/win32_thread.c")
else()
    target_sources(glfw PRIVATE "${GLFW_SOURCE_DIR}/posix_time.h" 
                                "${GLFW_SOURCE_DIR}/posix_thread.h"
                                "${GLFW_SOURCE_DIR}/posix_module.c"
                                "${GLFW_SOURCE_DIR}/posix_time.c" 
                                "${GLFW_SOURCE_DIR}/posix_thread.c")
endif()
if (GLFW_BUILD_WIN32)
target_compile_definitions(glfw PRIVATE _GLFW_WIN32)
target_sources(glfw PRIVATE "${GLFW_SOURCE_DIR}/win32_platform.h" 
                            "${GLFW_SOURCE_DIR}/win32_joystick.h" 
                            "${GLFW_SOURCE_DIR}/win32_init.c"
                            "${GLFW_SOURCE_DIR}/win32_joystick.c" 
                            "${GLFW_SOURCE_DIR}/win32_monitor.c" 
                            "${GLFW_SOURCE_DIR}/win32_window.c"
                            "${GLFW_SOURCE_DIR}/wgl_context.c")
endif()

if (GLFW_BUILD_X11)
target_compile_definitions(glfw PRIVATE _GLFW_X11)
target_sources(glfw PRIVATE "${GLFW_SOURCE_DIR}/x11_platform.h" 
                            "${GLFW_SOURCE_DIR}/xkb_unicode.h"
                            "${GLFW_SOURCE_DIR}/x11_init.c"
                            "${GLFW_SOURCE_DIR}/x11_monitor.c"
                            "${GLFW_SOURCE_DIR}/x11_window.c" 
                            "${GLFW_SOURCE_DIR}/xkb_unicode.c"
                            "${GLFW_SOURCE_DIR}/glx_context.c")
endif()

if (GLFW_BUILD_WAYLAND)
target_compile_definitions(glfw PRIVATE _GLFW_WAYLAND)
target_sources(glfw PRIVATE "${GLFW_SOURCE_DIR}/wl_platform.h" 
                            "${GLFW_SOURCE_DIR}/xkb_unicode.h"
                            "${GLFW_SOURCE_DIR}/wl_init.c"
                            "${GLFW_SOURCE_DIR}/wl_monitor.c"
                            "${GLFW_SOURCE_DIR}/wl_window.c"
                            "${GLFW_SOURCE_DIR}/xkb_unicode.c")
endif()

if (GLFW_BUILD_X11 OR GLFW_BUILD_WAYLAND)
if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_sources(glfw PRIVATE "${GLFW_SOURCE_DIR}/linux_joystick.h" 
                                "${GLFW_SOURCE_DIR}/linux_joystick.c")
endif()
target_sources(glfw PRIVATE "${GLFW_SOURCE_DIR}/posix_poll.h" 
                            "${GLFW_SOURCE_DIR}/posix_poll.c")
endif()

if (UNIX AND GLFW_BUILD_SHARED_LIBRARY)
    set(GLFW_LIB_NAME glfw)
else()
    set(GLFW_LIB_NAME glfw3)
endif()
set(GLFW_LIB_NAME_SUFFIX "")

set_target_properties(glfw PROPERTIES
                      OUTPUT_NAME glfw3
                      C_STANDARD 99
                      C_EXTENSIONS OFF
                      DEFINE_SYMBOL _GLFW_BUILD_DLL
                     )
target_include_directories(glfw PUBLIC
                      "$<BUILD_INTERFACE:${GLFW_SOURCE_DIR}/../include>")
target_include_directories(glfw PRIVATE
                      "${GLFW_SOURCE_DIR}/")
target_link_libraries(glfw PRIVATE Threads::Threads)

message("${PROJECT_NAME}: Adding FastNoise2...")
add_subdirectory("${CMAKE_SOURCE_DIR}/Vendor/FastNoise")

message("${PROJECT_NAME}: Adding source files")
file(GLOB_RECURSE GLAD_SOURCE_FILES ./Vendor/glad/src/*.c)
file(GLOB_RECURSE GLAD_HEADER_FILES ./Vendor/glad/src/*.h)

file(GLOB_RECURSE IMGUI_SOURCE_FILES imgui_impl_opengl3.cpp imgui_impl_glfw.cpp imgui.cpp imgui_draw.cpp imgui_tables.cpp imgui_widgets.cpp )
file(GLOB_RECURSE IMGUI_HEADER_FILES imgui_impl_opengl3.h imgui_impl_glfw.h   imgui.h imconfig.h imgui_internal.h imstb_rectpack.h imstb_textedit.h imstb_truetype.h)

file(GLOB_RECURSE SANDBOX_SOURCE_FILES ./Sandbox/src/*.cpp)
file(GLOB_RECURSE SANDBOX_HEADER_FILES ./Sandbox/src/*.h)

file(GLOB_RECURSE ENGINE_SOURCE_FILES ./EngineLib/src/Core/*.cpp ./EngineLib/src/Layer/*.cpp)
file(GLOB_RECURSE ENGINE_HEADER_FILES ./EngineLib/src/Core/*.hpp ./EngineLib/src/Layer/*.hpp
./EngineLib/src/Core/*.h ./EngineLib/src/Layer/*.h
)

file(GLOB_RECURSE SANDBOX_WORLD_SOURCE_FILES ./EngineLib/src/Scene/World/*.cpp)
file(GLOB_RECURSE SANDBOX_WORLD_HEADER_FILES ./EngineLib/src/Scene/World/*.hpp)

file(GLOB_RECURSE UTILS_SOURCE_FILES ./EngineLib/src/Util/*.cpp)
file(GLOB_RECURSE UTILS_HEADER_FILES ./EngineLib/src/Util/*.hpp ./EngineLib/src/Util/*.h)

file(GLOB_RECURSE RENDERER_SOURCE_FILES ./EngineLib/src/Renderer/*.cpp ./EngineLib/src/Core/Ref.cpp)
file(GLOB_RECURSE RENDERER_HEADER_FILES ./EngineLib/src/Renderer/*.hpp ./EngineLib/src/Renderer/*.h)

set(ENGINE_APPLICATION_SOURCE_FILES ./EngineLib/src/Application/Application.cpp ./EngineLib/src/Application/Window.cpp)
set(ENGINE_APPLICATION_HEADER_FILES ./EngineLib/src/Application/Application.hpp ./EngineLib/src/Application/Window.hpp)

add_compile_definitions(GLM_ENABLE_EXPERIMENTAL)

set(GLAD_INCLDUE_DIR           "./Vendor/glad/include")
set(GLM_INCLDUE_DIR            "./Vendor/glm")
set(STB_INCLDUE_DIR            "./Vendor/stb")
set(IMGUI_INCLUDE_DIR          "./Vendor/imgui")
set(IMGUI_BACKENDS_INCLUDE_DIR "./Vendor/imgui/backends")
set(GLFW_INCLDUE_DIR           "./Vendor/glfw-3.4/include")
set(CUTILS_INCLDUE_DIR         "./Vendor/CUtils/include")
set(PERLIN_INCLUDE_DIR         "./Vendor/PerlinNoise")
set(FASTNOISE_INCLUDE_DIR         "./Vendor/FastNoise/include")

message("${PROJECT_NAME}: Creating imgui lib")
add_library(imgui SHARED ${IMGUI_SOURCE_FILES} ${IMGUI_HEADER_FILES})
message("${PROJECT_NAME}: Creating glad lib")
add_library(glad SHARED ${GLAD_SOURCE_FILES} ${GLAD_HEADER_FILES})

add_library(EngineUtils SHARED ${UTILS_SOURCE_FILES} ${UTILS_HEADER_FILES})

add_library(Renderer SHARED ${RENDERER_SOURCE_FILES} ${RENDERER_HEADER_FILES})

add_library(EngineLib SHARED ${ENGINE_SOURCE_FILES} ${ENGINE_HEADER_FILES})

add_library(EngineEntry SHARED ${ENGINE_APPLICATION_SOURCE_FILES} ${ENGINE_APPLICATION_HEADER_FILES})

add_library(SandboxWorld STATIC ${SANDBOX_WORLD_SOURCE_FILES} ${SANDBOX_WORLD_HEADER_FILES})

add_library(CUtils INTERFACE)
target_include_directories(CUtils INTERFACE ${CUTILS_INCLDUE_DIR})


if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(EngineLib PRIVATE -static-libstdc++)
    target_compile_options(EngineLib PRIVATE -Wall)
    target_compile_options(EngineLib PRIVATE -pedantic)
    target_compile_options(CUtils INTERFACE -Wno-fPermissive)

endif()

if(MSVC)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()
if (WIN32)
    target_compile_definitions(EngineLib PRIVATE GLFW_EXPOSE_NATIVE_WIN32)
else ()
    find_package(X11 REQUIRED)
    link_libraries(${X11_LIBRARIES})
    target_compile_definitions(EngineLib PRIVATE _LINUX)
    target_compile_definitions(EngineLib PRIVATE GLFW_EXPOSE_NATIVE_X11)
endif ()

target_compile_definitions(EngineUtils
    PUBLIC
    $<$<CONFIG:Debug>:ENGINE_ENABLE_VERBOSE_LOG>
    $<$<CONFIG:Debug>:ENGINE_ENABLE_MEMORY_DEBUG_LOG>
    $<$<CONFIG:Release>:ENGINE_ENABLE_MEMORY_DEBUG_LOG>
    $<$<CONFIG:Release>:_RELEASE>
    $<$<CONFIG:RelWithDebInfo>:_DEBUG>
    $<$<CONFIG:MinSizeRel>:_RELEASE>
)

target_compile_definitions(EngineLib
    PUBLIC
    $<$<CONFIG:Debug>:ENGINE_ENABLE_VERBOSE_LOG>
    $<$<CONFIG:Debug>:ENGINE_ENABLE_MEMORY_DEBUG_LOG>
    $<$<CONFIG:Release>:ENGINE_ENABLE_MEMORY_DEBUG_LOG>
    $<$<CONFIG:Release>:_RELEASE>
    $<$<CONFIG:RelWithDebInfo>:_DEBUG>
    $<$<CONFIG:MinSizeRel>:_RELEASE>
)

target_compile_definitions(glad
    PRIVATE
    GLAD_GLAPI_EXPORT_BUILD
    GLAD_GLAPI_EXPORT 
)
target_compile_definitions(Renderer
    PUBLIC
    GLAD_GLAPI_EXPORT
    $<$<CONFIG:Debug>:ENGINE_ENABLE_VERBOSE_LOG>
    $<$<CONFIG:Debug>:ENGINE_ENABLE_MEMORY_DEBUG_LOG>
    $<$<CONFIG:Release>:ENGINE_ENABLE_MEMORY_DEBUG_LOG>
    $<$<CONFIG:Release>:_RELEASE>
    $<$<CONFIG:RelWithDebInfo>:_DEBUG>
    $<$<CONFIG:MinSizeRel>:_RELEASE>
)

target_compile_definitions(EngineEntry
    PUBLIC
    $<$<CONFIG:Debug>:ENGINE_ENABLE_VERBOSE_LOG>
    $<$<CONFIG:Debug>:ENGINE_ENABLE_MEMORY_DEBUG_LOG>
    $<$<CONFIG:Release>:ENGINE_ENABLE_MEMORY_DEBUG_LOG>
    $<$<CONFIG:Release>:_RELEASE>
    $<$<CONFIG:RelWithDebInfo>:_DEBUG>
    $<$<CONFIG:MinSizeRel>:_RELEASE>
)

target_compile_definitions(SandboxWorld
    PUBLIC
    $<$<CONFIG:Debug>:ENGINE_ENABLE_VERBOSE_LOG>
    $<$<CONFIG:Debug>:ENGINE_ENABLE_MEMORY_DEBUG_LOG>
    $<$<CONFIG:Release>:ENGINE_ENABLE_MEMORY_DEBUG_LOG>
    $<$<CONFIG:Release>:_RELEASE>
    $<$<CONFIG:RelWithDebInfo>:_DEBUG>
    $<$<CONFIG:MinSizeRel>:_RELEASE>
)

target_include_directories(glad PRIVATE ${GLAD_INCLDUE_DIR})

target_include_directories(imgui PRIVATE ${IMGUI_INCLUDE_DIR})
target_include_directories(imgui PRIVATE ${IMGUI_BACKENDS_INCLUDE_DIR})
target_include_directories(imgui PRIVATE ${GLFW_INCLDUE_DIR})

set(INCLUDE_DIRS ${GLAD_INCLDUE_DIR}
 ${STB_INCLDUE_DIR}
 ${IMGUI_INCLUDE_DIR}
 ${IMGUI_BACKENDS_INCLUDE_DIR}
 ${GLFW_INCLDUE_DIR}
 ${GLM_INCLDUE_DIR}
 ${CUTILS_INCLDUE_DIR}
 ${PERLIN_INCLUDE_DIR}
 ${FASTNOISE_INCLUDE_DIR})

target_include_directories(EngineUtils PRIVATE "./EngineLib/src")
target_include_directories(EngineUtils PRIVATE ${INCLUDE_DIRS})

target_include_directories(EngineLib PRIVATE "./EngineLib/src")
target_include_directories(EngineLib PRIVATE ${INCLUDE_DIRS})

target_include_directories(Renderer PRIVATE "./EngineLib/src")
target_include_directories(Renderer PRIVATE ${INCLUDE_DIRS})

target_include_directories(EngineEntry PRIVATE "./EngineLib/src")
target_include_directories(EngineEntry PUBLIC ${INCLUDE_DIRS})

target_include_directories(SandboxWorld PRIVATE "./EngineLib/src")
target_include_directories(SandboxWorld PRIVATE ${INCLUDE_DIRS})

add_library(EngineInterfaceLibrary INTERFACE)

message("${PROJECT_NAME}: Linking libraries")

target_link_libraries(imgui PRIVATE glfw glad)

target_link_libraries(EngineInterfaceLibrary INTERFACE EngineUtils glfw imgui glad CUtils FastNoise )

target_link_libraries(Renderer PRIVATE EngineInterfaceLibrary)
target_link_libraries(EngineLib PRIVATE Renderer EngineInterfaceLibrary)
target_link_libraries(EngineEntry PRIVATE EngineLib Renderer EngineInterfaceLibrary)

target_link_libraries(SandboxWorld PRIVATE EngineEntry EngineLib Renderer EngineInterfaceLibrary)

add_executable(Sandbox ${SANDBOX_HEADER_FILES} ${SANDBOX_SOURCE_FILES})
target_include_directories(Sandbox PRIVATE "${CMAKE_SOURCE_DIR}/EngineLib/src")
target_include_directories(Sandbox PRIVATE "${CMAKE_SOURCE_DIR}/Sandbox/src")
target_link_libraries(Sandbox SandboxWorld EngineEntry Renderer EngineLib EngineInterfaceLibrary)

add_executable(ShaderReflectionTest ./ShaderReflectionTest/main.cpp )
target_link_libraries(ShaderReflectionTest EngineEntry Renderer EngineLib)
target_include_directories(ShaderReflectionTest PRIVATE ./EngineLib/src/Renderer/Reflection)
target_include_directories(ShaderReflectionTest PRIVATE ./EngineLib/src/)

# if(BUILD_ENGINE_SHARED EQUAL "ON")
#     target_compile_definitions(EngineUtils PUBLIC ENGINE_UTILS_BUILD_DLL)
#     target_compile_definitions(EngineLib PUBLIC ENGINE_BUILD_DLL FASTNOISE_SHARED_LIB)
#     target_compile_definitions(Renderer PUBLIC ENGINE_RENDERER_BUILD_DLL FASTNOISE_SHARED_LIB)
#     target_compile_definitions(EngineEntry PUBLIC ENGINE_ENTRY_BUILD_DLL FASTNOISE_SHARED_LIB)
# endif()
