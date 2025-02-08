cmake_minimum_required(VERSION 3.20)
set (CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")

project(SandboxGame)

set(BUILD_SHARED_LIBS ON CACHE BOOL "Build engine shared library" FORCE)
set(FASTNOISE2_NOISETOOL OFF CACHE BOOL "Build noise tool" FORCE)
set(FASTNOISE2_TESTS OFF CACHE BOOL "Build noise tests" FORCE)
set(YAML_BUILD_SHARED_LIBS ON CACHE BOOL "Build yaml-cpp shared library" FORCE)

if(BUILD_SHARED_LIBS)
message(STATUS "${PROJECT_NAME}: Build engine shared library")
set(BUILD_LIB_FLAG "SHARED")
else()
message(STATUS "${PROJECT_NAME}: Build engine static library")
set(BUILD_LIB_FLAG "STATIC")
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_STANDARD 23)
message("Compiler Version: ${CMAKE_CXX_COMPILER_VERSION}")

message(STATUS "${PROJECT_NAME}: Finding VulkanSDK...")
find_package(Vulkan REQUIRED)

message(STATUS "${PROJECT_NAME}: Finding python...")
find_package(Python COMPONENTS Interpreter REQUIRED)
message(STATUS "Found python executable: ${Python_EXECUTABLE}")

message(STATUS "${PROJECT_NAME}: Adding yaml-cpp...")
add_subdirectory("${CMAKE_SOURCE_DIR}/Vendor/yaml-cpp")

message(STATUS "${PROJECT_NAME}: Adding glfw...")
add_subdirectory("${CMAKE_SOURCE_DIR}/Vendor/glfw-3.4")

message(STATUS "${PROJECT_NAME}: Adding FastNoise2...")
add_subdirectory("${CMAKE_SOURCE_DIR}/Vendor/FastNoise")

message(STATUS "${PROJECT_NAME}: Adding source files")
file(GLOB_RECURSE GLAD_SOURCE_FILES ./Vendor/glad/src/*.c)
file(GLOB_RECURSE GLAD_HEADER_FILES ./Vendor/glad/src/*.h)

file(GLOB_RECURSE IMGUI_SOURCE_FILES imgui_impl_opengl3.cpp imgui_impl_glfw.cpp imgui.cpp imgui_draw.cpp imgui_tables.cpp imgui_widgets.cpp )
file(GLOB_RECURSE IMGUI_HEADER_FILES imgui_impl_opengl3.h imgui_impl_glfw.h   imgui.h imconfig.h imgui_internal.h imstb_rectpack.h imstb_textedit.h imstb_truetype.h)

file(GLOB_RECURSE SANDBOX_SOURCE_FILES ${CMAKE_SOURCE_DIR}/Sandbox/src/*.cpp)
file(GLOB_RECURSE SANDBOX_HEADER_FILES ${CMAKE_SOURCE_DIR}/Sandbox/src/*.h)

file(GLOB_RECURSE ENGINE_SOURCE_FILES ./EngineLib/src/Core/*.cpp ./EngineLib/src/Layer/*.cpp ./EngineLib/src/Window/*.cpp)
file(GLOB_RECURSE ENGINE_HEADER_FILES ./EngineLib/src/Core/*.hpp ./EngineLib/src/Layer/*.hpp ./EngineLib/src/Window/*.hpp
./EngineLib/src/Core/*.h ./EngineLib/src/Layer/*.h)

file(GLOB_RECURSE SANDBOX_WORLD_SOURCE_FILES ./EngineLib/src/Scene/World/*.cpp)
file(GLOB_RECURSE SANDBOX_WORLD_HEADER_FILES ./EngineLib/src/Scene/World/*.hpp)

file(GLOB_RECURSE UTILS_SOURCE_FILES ./EngineLib/src/Util/*.cpp)
file(GLOB_RECURSE UTILS_HEADER_FILES ./EngineLib/src/Util/*.hpp ./EngineLib/src/Util/*.h)

file(GLOB_RECURSE RENDERER_OPENGL_SOURCE_FILES ./EngineLib/src/Renderer/OpenGL/*.cpp ./EngineLib/src/Core/Ref.cpp)
file(GLOB_RECURSE RENDERER_OPENGL_HEADER_FILES ./EngineLib/src/Renderer/OpenGL/*.hpp)

file(GLOB_RECURSE RENDERER_VULKAN_SOURCE_FILES ./EngineLib/src/Renderer/Vulkan/*.cpp ./EngineLib/src/Core/Ref.cpp)
file(GLOB_RECURSE RENDERER_VULKAN_HEADER_FILES ./EngineLib/src/Renderer/Vulkan/*.hpp )

file(GLOB_RECURSE RENDERER_SOURCE_FILES ./EngineLib/src/Renderer/Shared/*.cpp ./EngineLib/src/Core/Ref.cpp)
file(GLOB_RECURSE RENDERER_HEADER_FILES ./EngineLib/src/Renderer/Shared/*.hpp ./EngineLib/src/Shared/*.h)

list(APPEND RENDERER_SOURCE_FILES ./EngineLib/src/Renderer/Renderer.cpp)
list(APPEND RENDERER_HEADER_FILES ./EngineLib/src/Renderer/Renderer.hpp
    ./EngineLib/src/Renderer/Loader.hpp
    ./EngineLib/src/Renderer/Predefines.hpp)

set(ENGINE_APPLICATION_SOURCE_FILES ./EngineLib/src/Application/Application.cpp )
set(ENGINE_APPLICATION_HEADER_FILES ./EngineLib/src/Application/Application.hpp )

add_compile_definitions(GLM_ENABLE_EXPERIMENTAL)

set(GLAD_INCLDUE_DIR           "./Vendor/glad/include")
set(GLM_INCLDUE_DIR            "./Vendor/glm")
set(STB_INCLDUE_DIR            "./Vendor/stb")
set(IMGUI_INCLUDE_DIR          "./Vendor/imgui")
set(IMGUI_BACKENDS_INCLUDE_DIR "./Vendor/imgui/backends")
set(CUTILS_INCLDUE_DIR         "./Vendor/CUtils/include")
set(PERLIN_INCLUDE_DIR         "./Vendor/PerlinNoise")
set(FASTNOISE_INCLUDE_DIR         "./Vendor/FastNoise/include")
set(LIBRARY_LOADER_INCLUDE_DIR "./Vendor/LibraryLoader/include")

message(STATUS "${PROJECT_NAME}: Creating imgui lib")
add_library(imgui ${BUILD_LIB_FLAG} ${IMGUI_SOURCE_FILES} ${IMGUI_HEADER_FILES})
message(STATUS "${PROJECT_NAME}: Creating glad lib")
add_library(glad STATIC ${GLAD_SOURCE_FILES} ${GLAD_HEADER_FILES})

add_library(EngineUtils ${BUILD_LIB_FLAG} ${UTILS_SOURCE_FILES} ${UTILS_HEADER_FILES})

add_library(Renderer STATIC ${RENDERER_SOURCE_FILES} ${RENDERER_HEADER_FILES})

add_library(RendererOpenGL ${BUILD_LIB_FLAG} ${RENDERER_OPENGL_SOURCE_FILES} ${RENDERER_OPENGL_HEADER_FILES})
add_library(RendererVulkan ${BUILD_LIB_FLAG} ${RENDERER_VULKAN_SOURCE_FILES} ${RENDERER_VULKAN_HEADER_FILES})

add_library(EngineLib STATIC ${ENGINE_SOURCE_FILES} ${ENGINE_HEADER_FILES})

add_library(EngineEntry STATIC ${ENGINE_APPLICATION_SOURCE_FILES} ${ENGINE_APPLICATION_HEADER_FILES})

add_library(SandboxWorld STATIC ${SANDBOX_WORLD_SOURCE_FILES} ${SANDBOX_WORLD_HEADER_FILES})

add_library(CUtils INTERFACE)
target_include_directories(CUtils INTERFACE ${CUTILS_INCLDUE_DIR})

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(EngineLib PUBLIC -static-libstdc++)
    target_compile_options(EngineLib PUBLIC -Wall)
    target_compile_options(EngineLib PUBLIC -pedantic)
    target_compile_options(CUtils INTERFACE -Wno-fPermissive)
endif()

if(MSVC AND ${BUILD_LIB_FLAG} STREQUAL "SHARED")
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()
if (WIN32)
    target_compile_definitions(EngineLib PUBLIC GLFW_EXPOSE_NATIVE_WIN32)
else ()
    find_package(X11 REQUIRED)
    link_libraries(${X11_LIBRARIES})
    target_compile_definitions(EngineLib PUBLIC _LINUX)
    target_compile_definitions(EngineLib PUBLIC GLFW_EXPOSE_NATIVE_X11)
endif ()

target_compile_definitions(EngineUtils
    PUBLIC
    $<$<STREQUAL:${BUILD_LIB_FLAG},SHARED>:LINK_SHARED>
    $<$<STREQUAL:${BUILD_LIB_FLAG},SHARED>:EngineUtils_EXPORTS>
    $<$<CONFIG:Debug>:ENGINE_ENABLE_VERBOSE_LOG>
    $<$<CONFIG:Debug>:ENGINE_ENABLE_MEMORY_DEBUG_LOG>
    $<$<CONFIG:Release>:ENGINE_ENABLE_MEMORY_DEBUG_LOG>
    $<$<CONFIG:Release>:_RELEASE>
    $<$<CONFIG:RelWithDebInfo>:_DEBUG>
    $<$<CONFIG:MinSizeRel>:_RELEASE>
)

target_compile_definitions(EngineLib
    PUBLIC
    $<$<STREQUAL:${BUILD_LIB_FLAG},SHARED>:LINK_SHARED>
    $<$<STREQUAL:${BUILD_LIB_FLAG},SHARED>:EngineLib_EXPORTS>
    $<$<CONFIG:Debug>:ENGINE_ENABLE_VERBOSE_LOG>
    $<$<CONFIG:Debug>:ENGINE_ENABLE_MEMORY_DEBUG_LOG>
    $<$<CONFIG:Release>:ENGINE_ENABLE_MEMORY_DEBUG_LOG>
    $<$<CONFIG:Release>:_RELEASE>
    $<$<CONFIG:RelWithDebInfo>:_DEBUG>
    $<$<CONFIG:MinSizeRel>:_RELEASE>
)

target_compile_definitions(glad
    PUBLIC
    $<$<STREQUAL:${BUILD_LIB_FLAG},SHARED>:LINK_SHARED>
    $<$<STREQUAL:${BUILD_LIB_FLAG},SHARED>:GLAD_GLAPI_EXPORT_BUILD>
    $<$<STREQUAL:${BUILD_LIB_FLAG},SHARED>:GLAD_GLAPI_EXPORT>
)
target_compile_definitions(Renderer
    PUBLIC
    $<$<CONFIG:Debug>:ENGINE_ENABLE_VERBOSE_LOG>
    $<$<CONFIG:Debug>:ENGINE_ENABLE_MEMORY_DEBUG_LOG>
    $<$<CONFIG:Release>:ENGINE_ENABLE_MEMORY_DEBUG_LOG>
    $<$<CONFIG:Release>:_RELEASE>
    $<$<CONFIG:RelWithDebInfo>:_DEBUG>
    $<$<CONFIG:MinSizeRel>:_RELEASE>
)
target_compile_definitions(RendererOpenGL
    PUBLIC
    $<$<STREQUAL:${BUILD_LIB_FLAG},SHARED>:LINK_SHARED>
    $<$<STREQUAL:${BUILD_LIB_FLAG},SHARED>:Renderer_EXPORTS>
    $<$<STREQUAL:${BUILD_LIB_FLAG},SHARED>:GLAD_GLAPI_EXPORT>
    $<$<CONFIG:Debug>:ENGINE_ENABLE_VERBOSE_LOG>
    $<$<CONFIG:Debug>:ENGINE_ENABLE_MEMORY_DEBUG_LOG>
    $<$<CONFIG:Release>:ENGINE_ENABLE_MEMORY_DEBUG_LOG>
    $<$<CONFIG:Release>:_RELEASE>
    $<$<CONFIG:RelWithDebInfo>:_DEBUG>
    $<$<CONFIG:MinSizeRel>:_RELEASE>
)
target_compile_definitions(RendererVulkan
    PUBLIC
    $<$<STREQUAL:${BUILD_LIB_FLAG},SHARED>:LINK_SHARED>
    $<$<STREQUAL:${BUILD_LIB_FLAG},SHARED>:Renderer_EXPORTS>
    $<$<STREQUAL:${BUILD_LIB_FLAG},SHARED>:GLAD_GLAPI_EXPORT>
    $<$<CONFIG:Debug>:ENGINE_ENABLE_VERBOSE_LOG>
    $<$<CONFIG:Debug>:ENGINE_ENABLE_MEMORY_DEBUG_LOG>
    $<$<CONFIG:Release>:ENGINE_ENABLE_MEMORY_DEBUG_LOG>
    $<$<CONFIG:Release>:_RELEASE>
    $<$<CONFIG:RelWithDebInfo>:_DEBUG>
    $<$<CONFIG:MinSizeRel>:_RELEASE>
)
target_compile_definitions(EngineEntry
    PUBLIC
    $<$<STREQUAL:${BUILD_LIB_FLAG},SHARED>:LINK_SHARED>
    $<$<STREQUAL:${BUILD_LIB_FLAG},SHARED>:EngineEntry_EXPORTS>
    $<$<CONFIG:Debug>:ENGINE_ENABLE_VERBOSE_LOG>
    $<$<CONFIG:Debug>:ENGINE_ENABLE_MEMORY_DEBUG_LOG>
    $<$<CONFIG:Release>:ENGINE_ENABLE_MEMORY_DEBUG_LOG>
    $<$<CONFIG:Release>:_RELEASE>
    $<$<CONFIG:RelWithDebInfo>:_DEBUG>
    $<$<CONFIG:MinSizeRel>:_RELEASE>
)

target_compile_definitions(SandboxWorld
    PUBLIC
    $<$<STREQUAL:${BUILD_LIB_FLAG},SHARED>:LINK_SHARED>
    $<$<CONFIG:Debug>:ENGINE_ENABLE_VERBOSE_LOG>
    $<$<CONFIG:Debug>:ENGINE_ENABLE_MEMORY_DEBUG_LOG>
    $<$<CONFIG:Release>:ENGINE_ENABLE_MEMORY_DEBUG_LOG>
    $<$<CONFIG:Release>:_RELEASE>
    $<$<CONFIG:RelWithDebInfo>:_DEBUG>
    $<$<CONFIG:MinSizeRel>:_RELEASE>
)

target_include_directories(glad PUBLIC ${GLAD_INCLDUE_DIR})

target_include_directories(imgui PUBLIC ${IMGUI_INCLUDE_DIR})
target_include_directories(imgui PUBLIC ${IMGUI_BACKENDS_INCLUDE_DIR})
target_include_directories(imgui PUBLIC ${GLFW_INCLDUE_DIR})

set(INCLUDE_DIRS 
${GLAD_INCLDUE_DIR}
 ${STB_INCLDUE_DIR}
 ${IMGUI_INCLUDE_DIR}
 ${IMGUI_BACKENDS_INCLUDE_DIR}
 ${GLFW_INCLDUE_DIR}
 ${GLM_INCLDUE_DIR}
 ${CUTILS_INCLDUE_DIR}
 ${PERLIN_INCLUDE_DIR}
 ${FASTNOISE_INCLUDE_DIR}
 ${LIBRARY_LOADER_INCLUDE_DIR}
 )

target_include_directories(EngineUtils PUBLIC "./EngineLib/src")
target_include_directories(EngineUtils PUBLIC ${INCLUDE_DIRS})

target_include_directories(EngineLib PUBLIC "./EngineLib/src")
target_include_directories(EngineLib PUBLIC ${INCLUDE_DIRS})

target_include_directories(RendererOpenGL PUBLIC "./EngineLib/src")
target_include_directories(RendererOpenGL PUBLIC ${INCLUDE_DIRS})

target_include_directories(RendererVulkan PUBLIC "./EngineLib/src")
target_include_directories(RendererVulkan PUBLIC ${INCLUDE_DIRS})

target_include_directories(Renderer PUBLIC "./EngineLib/src")
target_include_directories(Renderer PUBLIC ${INCLUDE_DIRS})

target_include_directories(EngineEntry PUBLIC "./EngineLib/src")
target_include_directories(EngineEntry PUBLIC ${INCLUDE_DIRS})

target_include_directories(SandboxWorld PUBLIC "./EngineLib/src")
target_include_directories(SandboxWorld PUBLIC ${INCLUDE_DIRS})

add_library(EngineInterfaceLibrary INTERFACE)

message(STATUS "${PROJECT_NAME}: Linking libraries")

target_link_libraries(imgui PUBLIC glfw glad)

target_link_libraries(EngineInterfaceLibrary INTERFACE EngineUtils glfw imgui glad CUtils FastNoise )

target_link_libraries(RendererOpenGL PUBLIC glad imgui EngineInterfaceLibrary)
target_link_libraries(RendererVulkan PUBLIC imgui EngineInterfaceLibrary)
target_link_libraries(Renderer PUBLIC imgui EngineInterfaceLibrary)

add_dependencies(Renderer RendererOpenGL)

target_link_libraries(EngineLib PUBLIC Renderer EngineInterfaceLibrary)

target_link_libraries(EngineEntry PUBLIC EngineLib EngineInterfaceLibrary)

target_link_libraries(SandboxWorld PUBLIC EngineEntry EngineLib EngineInterfaceLibrary)

add_executable(Sandbox ${SANDBOX_HEADER_FILES} ${SANDBOX_SOURCE_FILES})
target_compile_definitions(Sandbox
    PUBLIC
    $<$<STREQUAL:${BUILD_LIB_FLAG},SHARED>:LINK_SHARED>
)

target_include_directories(Sandbox PUBLIC "${CMAKE_SOURCE_DIR}/EngineLib/src")
target_include_directories(Sandbox PUBLIC "${CMAKE_SOURCE_DIR}/Sandbox/src")
target_link_libraries(Sandbox SandboxWorld)

add_executable(ShaderReflectionTest ./ShaderReflectionTest/main.cpp )
target_link_libraries(ShaderReflectionTest EngineInterfaceLibrary )
target_include_directories(ShaderReflectionTest PUBLIC ./EngineLib/src/Reflection)
target_include_directories(ShaderReflectionTest PUBLIC ./EngineLib/src/)
target_include_directories(ShaderReflectionTest PUBLIC  ${INCLUDE_DIRS})

# if(BUILD_ENGINE_SHARED EQUAL "ON")
#     target_compile_definitions(EngineUtils PUBLIC ENGINE_UTILS_BUILD_DLL)
#     target_compile_definitions(EngineLib PUBLIC ENGINE_BUILD_DLL FASTNOISE_SHARED_LIB)
#     target_compile_definitions(Renderer PUBLIC ENGINE_RENDERER_BUILD_DLL FASTNOISE_SHARED_LIB)
#     target_compile_definitions(EngineEntry PUBLIC ENGINE_ENTRY_BUILD_DLL FASTNOISE_SHARED_LIB)
# endif()
