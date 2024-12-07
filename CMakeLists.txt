cmake_minimum_required(VERSION 3.20)

project(SandboxGame)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared library" FORCE)
set(FASTNOISE2_NOISETOOL OFF CACHE BOOL "Build noise tool" FORCE)
set(FASTNOISE2_TESTS OFF CACHE BOOL "Build noise tests" FORCE)

set(CMAKE_CXX_STANDARD 23)
message("Compiler Version: ${CMAKE_CXX_COMPILER_VERSION}")

message("${PROJECT_NAME}: Adding yaml-cpp...")
add_subdirectory("${CMAKE_SOURCE_DIR}/Vendor/yaml-cpp")


message("${PROJECT_NAME}: Adding glfw...")
add_subdirectory("${CMAKE_SOURCE_DIR}/Vendor/glfw-3.4")

message("${PROJECT_NAME}: Adding FastNoise2...")
add_subdirectory("${CMAKE_SOURCE_DIR}/Vendor/FastNoise")

message("${PROJECT_NAME}: Adding source files")
file(GLOB_RECURSE GLAD_SOURCE_FILES ./Vendor/glad/src/*.c)
file(GLOB_RECURSE GLAD_HEADER_FILES ./Vendor/glad/src/*.h)

file(GLOB_RECURSE IMGUI_SOURCE_FILES imgui_impl_opengl3.cpp imgui_impl_glfw.cpp imgui.cpp imgui_draw.cpp imgui_tables.cpp imgui_widgets.cpp )
file(GLOB_RECURSE IMGUI_HEADER_FILES imgui_impl_opengl3.h imgui_impl_glfw.h   imgui.h imconfig.h imgui_internal.h imstb_rectpack.h imstb_textedit.h imstb_truetype.h)

file(GLOB_RECURSE SANDBOX_SOURCE_FILES ./Sandbox/src/*.cpp)
file(GLOB_RECURSE SANDBOX_HEADER_FILES ./Sandbox/src/*.h)

file(GLOB_RECURSE ENGINE_SOURCE_FILES ./EngineLib/src/Core/*.cpp ./EngineLib/src/Layer/*.cpp ./EngineLib/src/Scene/*.cpp ./EngineLib/src/Util/*.cpp)
file(GLOB_RECURSE ENGINE_HEADER_FILES ./EngineLib/src/Core/*.hpp ./EngineLib/src/Layer/*.hpp ./EngineLib/src/Scene/*.hpp ./EngineLib/src/Util/*.hpp
./EngineLib/src/Core/*.h ./EngineLib/src/Layer/*.h ./EngineLib/src/Scene/*.h ./EngineLib/src/Util/*.h
)

file(GLOB_RECURSE UTILS_SOURCE_FILES ./EngineLib/src/Util/*.cpp)
file(GLOB_RECURSE UTILS_HEADER_FILES ./EngineLib/src/Util/*.hpp ./EngineLib/src/Util/*.h)

file(GLOB_RECURSE RENDERER_SOURCE_FILES ./EngineLib/src/Renderer/*.cpp )
file(GLOB_RECURSE RENDERER_HEADER_FILES ./EngineLib/src/Renderer/*.hpp ./EngineLib/src/Renderer/*.h)

set(ENGINE_APPLICATION_SOURCE_FILES ./EngineLib/src/Application/Application.cpp)
set(ENGINE_APPLICATION_HEADER_FILES ./EngineLib/src/Application/Application.hpp)

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
add_library(imgui STATIC ${IMGUI_SOURCE_FILES} ${IMGUI_HEADER_FILES})
message("${PROJECT_NAME}: Creating glad lib")
add_library(glad STATIC ${GLAD_SOURCE_FILES} ${GLAD_HEADER_FILES})

add_library(EngineUtils STATIC ${UTILS_SOURCE_FILES} ${UTILS_HEADER_FILES})

add_library(Renderer SHARED ${RENDERER_SOURCE_FILES} ${RENDERER_HEADER_FILES})

add_library(EngineLib SHARED ${ENGINE_SOURCE_FILES} ${ENGINE_HEADER_FILES})

add_library(EngineEntry SHARED ${ENGINE_APPLICATION_SOURCE_FILES} ${ENGINE_APPLICATION_HEADER_FILES})

add_library(CUtils INTERFACE)
target_include_directories(CUtils INTERFACE ${CUTILS_INCLDUE_DIR})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(EngineLib PRIVATE -static-libstdc++)
    target_compile_options(EngineLib PRIVATE -Wall)
    target_compile_options(EngineLib PRIVATE -pedantic)
    target_compile_options(CUtils INTERFACE -Wno-fPermissive)

endif()

if(MSVC)
    target_compile_options(EngineLib PRIVATE /MP)
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
    FASTNOISE_STATIC_LIB 
    ENGINE_BUILD_DLL
    RENDERER_BUILD_DLL
    $<$<CONFIG:Debug>:ENGINE_ENABLE_VERBOSE_LOG>
    $<$<CONFIG:Debug>:ENGINE_ENABLE_MEMORY_DEBUG_LOG>
    $<$<CONFIG:Release>:ENGINE_ENABLE_MEMORY_DEBUG_LOG>
    $<$<CONFIG:Release>:_RELEASE>
    $<$<CONFIG:RelWithDebInfo>:_DEBUG>
    $<$<CONFIG:MinSizeRel>:_RELEASE>
)

target_compile_definitions(Renderer
    PUBLIC
    FASTNOISE_STATIC_LIB 
    RENDERER_BUILD_DLL
    $<$<CONFIG:Debug>:ENGINE_ENABLE_VERBOSE_LOG>
    $<$<CONFIG:Debug>:ENGINE_ENABLE_MEMORY_DEBUG_LOG>
    $<$<CONFIG:Release>:ENGINE_ENABLE_MEMORY_DEBUG_LOG>
    $<$<CONFIG:Release>:_RELEASE>
    $<$<CONFIG:RelWithDebInfo>:_DEBUG>
    $<$<CONFIG:MinSizeRel>:_RELEASE>
)

target_compile_definitions(EngineEntry
    PUBLIC
    FASTNOISE_STATIC_LIB 
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
target_include_directories(Renderer PRIVATE "./EngineLib/src/Core")
target_include_directories(Renderer PRIVATE ${INCLUDE_DIRS})

target_include_directories(EngineEntry PRIVATE "./EngineLib/src")
target_include_directories(EngineEntry PUBLIC ${INCLUDE_DIRS})

add_library(EngineInterfaceLibrary INTERFACE)

message("${PROJECT_NAME}: Linking libraries")

target_link_libraries(imgui PRIVATE glfw glad)

target_link_libraries(EngineInterfaceLibrary INTERFACE EngineUtils glfw imgui glad CUtils FastNoise )

target_link_libraries(Renderer PRIVATE EngineInterfaceLibrary)
target_link_libraries(EngineLib PRIVATE Renderer EngineInterfaceLibrary)
target_link_libraries(EngineEntry PRIVATE EngineLib Renderer EngineInterfaceLibrary)

add_executable(Sandbox ${SANDBOX_HEADER_FILES} ${SANDBOX_SOURCE_FILES})
target_include_directories(Sandbox PRIVATE "${CMAKE_SOURCE_DIR}/EngineLib/src")
target_include_directories(Sandbox PRIVATE "${CMAKE_SOURCE_DIR}/Sandbox/src")
target_link_libraries(Sandbox EngineEntry Renderer EngineLib)

add_executable(ShaderReflectionTest ./ShaderReflectionTest/main.cpp )
target_link_libraries(ShaderReflectionTest EngineEntry Renderer EngineLib)
target_include_directories(ShaderReflectionTest PRIVATE ./EngineLib/src/Renderer/Reflection)
target_include_directories(ShaderReflectionTest PRIVATE ./EngineLib/src/)
