#pragma once

#include <Core/Log.hpp>
#include <unordered_map>
#include <cstdint>
#include <mutex>
#include <shared_mutex>
#include <future>
#include <vector>

#include <functional>
#include <source_location>

namespace Engine
{
#ifdef _DEBUG

    struct PointerMetaData {
        PointerMetaData(std::source_location location = std::source_location::current()) : location(location) {}

        std::source_location location;
        void* ptr{};
        size_t size{};
        size_t len{};
        bool isArray{};
    };

    class Allocator
    {
    public:
        inline static std::vector<Engine::PointerMetaData> s_Allocations;
        inline static size_t s_AllocatedMemorySize;
        inline static std::recursive_mutex s_Mutex{};

        template <typename T, typename... Args>
        static T* _Allocate(const Engine::PointerMetaData metadata, Args... args)
        {
            std::unique_lock<std::recursive_mutex> lock(Engine::Allocator::s_Mutex);
            T* ptr;

            if (metadata.isArray == true) 
            { 
                ptr = new T[metadata.len * metadata.size];
            }
            else
            {
                ptr = new T(std::forward<Args>(args)...);
            }
            Engine::PointerMetaData newmetadata = metadata;
            newmetadata.size = metadata.size * metadata.len;
            newmetadata.ptr = ptr;
            Engine::Allocator::s_Allocations.push_back(newmetadata);
            Engine::Allocator::s_AllocatedMemorySize += newmetadata.size;

#ifdef ENGINE_ENABLE_MEMORY_DEBUG_LOG
            LOG_INFO("Allocated: %s:%i Size: %i Address: %p\n", newmetadata.location.file_name(),
                     newmetadata.location.line(), newmetadata.size, newmetadata.ptr);
#endif
            return ptr;
        }

        template <typename T>
        static void _Deallocate(Engine::PointerMetaData metadata)
        {
            std::unique_lock<std::recursive_mutex> lock(Engine::Allocator::s_Mutex);
            bool found = false;
            uint32_t index = 0;
            for (auto& allocation: Engine::Allocator::s_Allocations)
            {
                if (allocation.ptr == metadata.ptr)
                {
                    found = true;
                    if (allocation.isArray) { delete[] (T) metadata.ptr; }
                    else { delete (T) metadata.ptr; }
                    Engine::Allocator::s_AllocatedMemorySize -= allocation.size;

#ifdef ENGINE_ENABLE_MEMORY_DEBUG_LOG
                    LOG_INFO("Deallocated: %s:%i Size: %i Address: %p\n", allocation.location.file_name(),
                             allocation.location.line(), allocation.size, allocation.ptr);
#endif
                    break;
                }
                index++;
            }
            if (found) { Engine::Allocator::s_Allocations.erase(Engine::Allocator::s_Allocations.begin() + index); }
        }

        template <typename T>
        static void _AddToAllocatedMemory(Engine::PointerMetaData metadata)
        {
            if (metadata.ptr)
            {
                std::unique_lock<std::recursive_mutex> lock(Engine::Allocator::s_Mutex);
                bool found = false;
                uint32_t index = 0;
                for (auto& allocation: Engine::Allocator::s_Allocations)
                {
                    if (allocation.ptr == metadata.ptr)
                    {
                        metadata = allocation;
                        found = true;
                        break;
                    }
                    index++;
                }
                if (!found)
                {
#ifdef ENGINE_ENABLE_MEMORY_DEBUG_LOG
                    LOG_INFO("Added memory to allocator: %s:%i Size: %i Address: %p\n", metadata.location.file_name(),
                             metadata.location.line(), metadata.size, metadata.ptr);
#endif
                    Engine::Allocator::s_Allocations.emplace_back(metadata);
                }
            }
        }

        template <typename T>
        static bool _IsLive(Engine::PointerMetaData metadata)
        {
            for (auto& allocation: Engine::Allocator::s_Allocations)
            {
                if (allocation.ptr == metadata.ptr) { return true; }
            }
            return false;
        }

        static void AnalyzeMemory()
        {
            std::unique_lock<std::recursive_mutex> lock(Engine::Allocator::s_Mutex);
            LOG_INFO("============================================\n");
            for (auto& allocation: Engine::Allocator::s_Allocations)
            {
                LOG_INFO("Location: %s:%i\n", allocation.location.file_name(), allocation.location.line());
                LOG_INFO("    Size: %i Address: %p\n", allocation.size, allocation.ptr);
            }
            LOG_INFO("Allocated Memory: %zi\n", Engine::Allocator::s_AllocatedMemorySize);
            LOG_INFO("============================================\n");
        }
    };

#define Allocate(type, ...)                                                                                            \
    std::invoke([&, location = std::source_location::current()]() -> type* {                                           \
        Engine::PointerMetaData metaData(location);                                                                            \
        metaData.size = sizeof(type);                                                                                  \
        metaData.isArray = false;                                                                                      \
        metaData.len = 1;                                                                                              \
        return Engine::Allocator::_Allocate<type>(metaData, __VA_ARGS__);                                                 \
    })
#define AllocateArray(type, ssize)                                                                                     \
    std::invoke([&, location = std::source_location::current()]() -> type* {                                           \
        Engine::PointerMetaData metaData(location);                                                                            \
        metaData.size = sizeof(type);                                                                                  \
        metaData.isArray = true;                                                                                       \
        metaData.len = ssize;                                                                                          \
        return Engine::Allocator::_Allocate<type>(metaData);                                                              \
    })
#define Deallocate(pptr)                                                                                               \
    std::invoke([&, location = std::source_location::current()]() -> void {                                            \
        Engine::PointerMetaData metaData(location);                                                                            \
        metaData.ptr = pptr;                                                                                           \
        metaData.isArray = false;                                                                                      \
        Engine::Allocator::_Deallocate<decltype(pptr)>(metaData);                                                              \
    })
#define DeallocateArray(pptr)                                                                                          \
    std::invoke([&, location = std::source_location::current()]() -> void {                                            \
        Engine::PointerMetaData metaData(location);                                                                            \
        metaData.ptr = pptr;                                                                                           \
        metaData.isArray = true;                                                                                       \
        Engine::Allocator::_Deallocate<decltype(pptr)>(metaData);                                                              \
    })
#define AddToAllocatedMemory(pptr, type)                                                                               \
    std::invoke([&, location = std::source_location::current()]() -> void {                                            \
        Engine::PointerMetaData metaData(location);                                                                            \
        metaData.ptr = pptr;                                                                                           \
        metaData.size = sizeof(type);                                                                                  \
        Engine::Allocator::_AddToAllocatedMemory<type>(metaData);                                                              \
    })
#define AllocatorIsLive(pptr, type)                                                                                    \
    std::invoke([&, location = std::source_location::current()]() -> bool {                                            \
        Engine::PointerMetaData metaData(location);                                                                            \
        metaData.ptr = pptr;                                                                                           \
        return Engine::Allocator::_IsLive<type>(metaData);                                                                     \
    })
#else

#define Allocate(type, ...) Allocator::_Allocate<type>(sizeof(type), __VA_ARGS__)
#define AllocateArray(type, size) Allocator::_Allocate<type>(size * sizeof(type))
#define AddToAllocatedMemory(ptr, size) Allocator::_AddToAllocatedMemory<type>(ptr, size)
#define Deallocate(ptr) Allocator::_Deallocate<decltype(ptr)>(ptr, false)
#define DeallocateArray(ptr) Allocator::_Deallocate<decltype(ptr)>(ptr, true)
#define IsLive(ptr, size) Allocator::_IsLive<decltype(ptr)>(ptr, size)

    class Allocator
    {
    public:
        inline static std::vector<std::pair<void*, size_t>> Engine::Allocator::s_Allocations;
        inline static size_t Engine::Allocator::s_AllocatedMemorySize;
        inline static std::recursive_mutex Engine::Allocator::s_Mutex{};

        template <typename T, typename... Args>
        static T* _Allocate(size_t size, Args... args)
        {
            T* ptr;
            ptr = new T(std::forward<Args>(args)...);

            std::unique_lock<std::recursive_mutex> lock(Engine::Allocator::s_Mutex);
            Engine::Allocator::s_Allocations.push_back({(void*) ptr, size});
            Engine::Allocator::s_AllocatedMemorySize += size;

            return ptr;
        }

        template <typename T>
        static void _Deallocate(T ptr, bool isArray = false)
        {
            std::unique_lock<std::recursive_mutex> lock(Engine::Allocator::s_Mutex);
            bool found = false;
            uint32_t index = 0;
            for (auto& allocation: Engine::Allocator::s_Allocations)
            {
                if (allocation.first == ptr)
                {
                    found = true;
                    if (isArray) { delete[] ptr; }
                    else { delete ptr; }
                    Engine::Allocator::s_AllocatedMemorySize -= allocation.second;
                    break;
                }
                index++;
            }
            if (found) { Engine::Allocator::s_Allocations.erase(Engine::Allocator::s_Allocations.begin() + index); }
        }

        template <typename T>
        static void _AddToAllocatedMemory(T* ptr = nullptr, size_t size)
        {
            std::unique_lock<std::recursive_mutex> lock(Engine::Allocator::s_Mutex);
            bool found = false;
            uint32_t index = 0;
            for (auto& allocation: Engine::Allocator::s_Allocations)
            {
                if (allocation.first == ptr)
                {
                    found = true;
                    break;
                }
                index++;
            }
            if (!found) { Engine::Allocator::s_Allocations.push_back({(void*) ptr, size}); }
        }

        template <typename T>
        static bool _IsLive(T ptr, size_t size = sizeof(T))
        {
            for (auto& allocation: Engine::Allocator::s_Allocations)
            {
                if (allocation.ptr == metadata.ptr) { return true; }
            }
            return false;
        }

        static void AnalyzeMemory()
        {
            std::unique_lock<std::recursive_mutex> lock(Engine::Allocator::s_Mutex);
            LOG_INFO("============================================\n");
            for (auto& allocation: Engine::Allocator::s_Allocations)
            {
                LOG_INFO("Size: %i Address: %p\n", allocation.second, allocation.first);
            }
            LOG_INFO("Allocated Memory: %zi\n", Engine::Allocator::s_AllocatedMemorySize);
            LOG_INFO("============================================\n");
        }
    };
#endif

}// namespace Engine

#include <Core/Allocator.impl.hpp>