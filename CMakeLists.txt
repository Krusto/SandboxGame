cmake_minimum_required(VERSION 3.20)

project(FIKO_ENGINE)

set(CMAKE_CXX_STANDARD 20)
message("Compiler Version: ${CMAKE_CXX_COMPILER_VERSION}")

message("${PROJECT_NAME}: Adding yaml-cpp...")
add_subdirectory("${CMAKE_SOURCE_DIR}/Vendor/yaml-cpp")

message("${PROJECT_NAME}: Adding glfw...")
add_subdirectory("${CMAKE_SOURCE_DIR}/Vendor/glfw-3.4")

message("${PROJECT_NAME}: Adding source files")
file(GLOB_RECURSE GLAD_SOURCE_FILES ./Vendor/glad/src/*.c)
file(GLOB_RECURSE GLAD_HEADER_FILES ./Vendor/glad/src/*.h)

file(GLOB_RECURSE IMGUI_SOURCE_FILES imgui_impl_opengl3.cpp imgui_impl_glfw.cpp imgui.cpp imgui_draw.cpp imgui_tables.cpp imgui_widgets.cpp )
file(GLOB_RECURSE IMGUI_HEADER_FILES imgui_impl_opengl3.h imgui_impl_glfw.h   imgui.h imconfig.h imgui_internal.h imstb_rectpack.h imstb_textedit.h imstb_truetype.h)

file(GLOB_RECURSE SANDBOX_SOURCE_FILES ./Sandbox/src/*.cpp)
file(GLOB_RECURSE SANDBOX_HEADER_FILES ./Sandbox/src/*.h)

file(GLOB_RECURSE ENGINE_SOURCE_FILES ./EngineLib/src/*.cpp)
file(GLOB_RECURSE ENGINE_HEADER_FILES ./EngineLib/src/*.h ./EngineLib/src/*.hpp)

add_compile_definitions(GLM_ENABLE_EXPERIMENTAL)

set(GLAD_INCLDUE_DIR           "./Vendor/glad/include")
set(GLM_INCLDUE_DIR            "./Vendor/glm")
set(STB_INCLDUE_DIR            "./Vendor/stb")
set(IMGUI_INCLUDE_DIR          "./Vendor/imgui")
set(IMGUI_BACKENDS_INCLUDE_DIR "./Vendor/imgui/backends")
set(GLFW_INCLDUE_DIR           "./Vendor/glfw-3.4/include")
set(CUTILS_INCLDUE_DIR         "./Vendor/CUtils/include")

message("${PROJECT_NAME}: Creating imgui lib")
add_library(imgui STATIC ${IMGUI_SOURCE_FILES} ${IMGUI_HEADER_FILES})
message("${PROJECT_NAME}: Creating glad lib")
add_library(glad STATIC ${GLAD_SOURCE_FILES} ${GLAD_HEADER_FILES})

add_library(EngineLib STATIC ${ENGINE_SOURCE_FILES} ${ENGINE_HEADER_FILES})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(EngineLib PRIVATE -static-libstdc++)
    target_compile_options(EngineLib PRIVATE -Wall)
    target_compile_options(EngineLib PRIVATE -pedantic)
endif()
if (WIN32)
    target_compile_definitions(EngineLib PRIVATE GLFW_EXPOSE_NATIVE_WIN32)
    
else ()
    find_package(X11 REQUIRED)
    link_libraries(${X11_LIBRARIES})
    target_compile_definitions(EngineLib PRIVATE _LINUX)
    target_compile_definitions(EngineLib PRIVATE GLFW_EXPOSE_NATIVE_X11)
endif ()

target_include_directories(glad PRIVATE ${GLAD_INCLDUE_DIR})

target_include_directories(imgui PRIVATE ${IMGUI_INCLUDE_DIR})
target_include_directories(imgui PRIVATE ${IMGUI_BACKENDS_INCLUDE_DIR})
target_include_directories(imgui PRIVATE ${GLFW_INCLDUE_DIR})

target_include_directories(EngineLib PRIVATE "./EngineLib/src")
target_include_directories(EngineLib PUBLIC ${GLAD_INCLDUE_DIR})
target_include_directories(EngineLib PUBLIC ${STB_INCLDUE_DIR})
target_include_directories(EngineLib PUBLIC ${IMGUI_INCLUDE_DIR})
target_include_directories(EngineLib PUBLIC ${IMGUI_BACKENDS_INCLUDE_DIR})
target_include_directories(EngineLib PUBLIC ${GLFW_INCLDUE_DIR})
target_include_directories(EngineLib PUBLIC ${GLM_INCLDUE_DIR})
target_include_directories(EngineLib PUBLIC ${CUTILS_INCLDUE_DIR})

add_library(EngineInterfaceLibrary INTERFACE)

add_library(CUtils INTERFACE)
target_include_directories(CUtils INTERFACE ${CUTILS_INCLDUE_DIR})
target_compile_options(CUtils INTERFACE -Wno-fPermissive)

message("${PROJECT_NAME}: Linking libraries")

target_link_libraries(imgui PRIVATE glfw glad)

target_link_libraries(EngineInterfaceLibrary INTERFACE
        # glm
        glfw
        imgui
        glad
        CUtils
        )
target_link_libraries(EngineLib PUBLIC EngineInterfaceLibrary)

add_executable(Sandbox ${SANDBOX_HEADER_FILES} ${SANDBOX_SOURCE_FILES})
target_include_directories(Sandbox PRIVATE "${CMAKE_SOURCE_DIR}/EngineLib/src")
target_include_directories(Sandbox PRIVATE "${CMAKE_SOURCE_DIR}/Sandbox/src")
target_link_libraries(Sandbox EngineLib)
