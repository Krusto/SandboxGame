#pragma once

#include <Core/Log.hpp>
#include <unordered_map>
#include <cstdint>
#include <mutex>
#include <shared_mutex>
#include <future>
#include <vector>
#include <memory>

#include <functional>
#include <source_location>
#include <algorithm>

#include <cstdlib>
#include <type_traits>

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

            if (metadata.isArray == true) { ptr = new T[metadata.len]; }
            else { ptr = new T(std::forward<Args>(args)...); }
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
                std::construct_at();
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
        Engine::PointerMetaData metaData(location);                                                                    \
        metaData.size = sizeof(type);                                                                                  \
        metaData.isArray = false;                                                                                      \
        metaData.len = 1;                                                                                              \
        return Engine::Allocator::_Allocate<type>(metaData, __VA_ARGS__);                                              \
    })
#define AllocateArray(type, ssize)                                                                                     \
    std::invoke([&, location = std::source_location::current()]() -> type* {                                           \
        Engine::PointerMetaData metaData(location);                                                                    \
        metaData.size = sizeof(type);                                                                                  \
        metaData.isArray = true;                                                                                       \
        metaData.len = ssize;                                                                                          \
        return Engine::Allocator::_Allocate<type>(metaData);                                                           \
    })
#define Deallocate(pptr)                                                                                               \
    std::invoke([&, location = std::source_location::current()]() -> void {                                            \
        Engine::PointerMetaData metaData(location);                                                                    \
        metaData.ptr = pptr;                                                                                           \
        metaData.isArray = false;                                                                                      \
        Engine::Allocator::_Deallocate<decltype(pptr)>(metaData);                                                      \
    })
#define DeallocateArray(pptr)                                                                                          \
    std::invoke([&, location = std::source_location::current()]() -> void {                                            \
        Engine::PointerMetaData metaData(location);                                                                    \
        metaData.ptr = pptr;                                                                                           \
        metaData.isArray = true;                                                                                       \
        Engine::Allocator::_Deallocate<decltype(pptr)>(metaData);                                                      \
    })
#define AddToAllocatedMemory(pptr, type)                                                                               \
    std::invoke([&, location = std::source_location::current()]() -> void {                                            \
        Engine::PointerMetaData metaData(location);                                                                    \
        metaData.ptr = pptr;                                                                                           \
        metaData.size = sizeof(type);                                                                                  \
        Engine::Allocator::_AddToAllocatedMemory<type>(metaData);                                                      \
    })
#define AllocatorIsLive(pptr, type)                                                                                    \
    std::invoke([&, location = std::source_location::current()]() -> bool {                                            \
        Engine::PointerMetaData metaData(location);                                                                    \
        metaData.ptr = pptr;                                                                                           \
        return Engine::Allocator::_IsLive<type>(metaData);                                                             \
    })
#else

    class Allocator
    {
    public:
        Allocator() = delete;
        Allocator(Allocator&&) = delete;
        Allocator(Allocator&) = delete;
        ~Allocator() = delete;

    public:
        static void Init() noexcept
        {
            std::unique_lock<std::recursive_mutex> lock(Allocator::s_Mutex);
            s_Allocations.reserve(30);
        }

        template <typename T, size_t TypeSize = sizeof(T), typename l = std::enable_if_t<!std::is_pointer<T>::value>,
                  typename... Args>
        static constexpr T* Allocate(Args&&... args) noexcept
        {
            return _AllocateArray<T, TypeSize, Args...>(1, std::forward<Args>(args)...);
        }

        template <typename T, size_t TypeSize = sizeof(T), typename l = std::enable_if_t<std::is_pointer<T>::value>,
                  typename... Args>
        static constexpr T Allocate(Args&&... args) noexcept
        {
            return _AllocateArray<std::remove_pointer<T>::type, TypeSize, Args...>(1, std::forward<Args>(args)...);
        }

        template <typename T, size_t TypeSize = sizeof(T), typename l = std::enable_if_t<!std::is_pointer<T>::value>,
                  typename... Args>
        static constexpr T* AllocateArray(size_t count, Args&&... args) noexcept
        {
            if (count <= 0) return {};
            return _AllocateArray<T, TypeSize, Args...>(count, std::forward<Args...>(args)...);
        }

        template <typename T, size_t TypeSize = sizeof(T), typename l = std::enable_if_t<std::is_pointer<T>::value>,
                  typename... Args>
        static constexpr T AllocateArray(size_t count, Args&&... args) noexcept
        {
            if (count <= 0) return {};
            return _AllocateArray<std::remove_pointer<T>::type, TypeSize, Args...>(count,
                                                                                   std::forward<Args...>(args)...);
        }

        template <typename T, typename l = std::enable_if_t<!std::is_pointer<T>::value>>
        static constexpr void Deallocate(T* ptr) noexcept
        {
            return _Deallocate(ptr);
        }

        template <typename T, typename l = std::enable_if_t<std::is_pointer<T>::value>>
        static constexpr void Deallocate(T ptr) noexcept
        {
            return _Deallocate<std::remove_pointer<T>::type>(ptr);
        }

        template <typename T, typename l = std::enable_if_t<std::is_pointer<T>::value>>
        static constexpr void AddToAllocatedMemory(T ptr) noexcept
        {
            _AddToAllocatedMemory<std::remove_pointer<T>::type>(ptr);
        }

        template <typename T, typename l = std::enable_if_t<!std::is_pointer<T>::value>>
        static constexpr void AddToAllocatedMemory(T* ptr) noexcept
        {
            _AddToAllocatedMemory<T>(ptr);
        }

        template <typename T, typename l = std::enable_if_t<std::is_pointer<T>::value>>
        static constexpr bool IsLive(T ptr) noexcept
        {
            return _IsLive<std::remove_pointer<T>>(ptr);
        }

        template <typename T, typename l = std::enable_if_t<!std::is_pointer<T>::value>>
        static constexpr bool IsLive(T* ptr) noexcept
        {
            return _IsLive<T>(ptr);
        }

        static void AnalyzeMemory()
        {
            printf("============================================\n");
            size_t allocationCount = 0;
            for (auto& [ptr, metadata]: s_Allocations)
            {
                if (metadata.count > 1)
                {
                    allocationCount += metadata.count;
                    printf("    Buffer - Allocated size: %ziB Length: %zi Element size: %zi Address: %p\n",
                           metadata.size * metadata.count, metadata.count, metadata.size, ptr);
                }
                else
                {
                    allocationCount++;
                    printf("    Element- Allocated size: %ziB Address: %p\n", metadata.size, ptr);
                }
            }
            std::unique_lock<std::recursive_mutex> lock(Allocator::s_Mutex);
            printf("Allocated Memory: %ziB\n", s_AllocatedMemorySize);
            printf("Number of allocations: %zi\n", allocationCount);
            printf("============================================\n");
        }

    private:
        template <typename T, typename l = std::enable_if_t<!std::is_pointer<T>::value>>
        static constexpr void _Deallocate(T* ptr) noexcept
        {
            std::unique_lock<std::recursive_mutex> lock(Allocator::s_Mutex);
            if (s_Allocations.contains(ptr))
            {
                size_t elementSize = s_Allocations[ptr].size;
                size_t count = s_Allocations[ptr].count;
                for (size_t i = 0; i < count; i++)
                {
                    char* newptr = (char*) ptr;
                    newptr += i * elementSize;
                    if (std::invoke(s_Allocations[ptr].destructor, newptr)) { s_AllocatedMemorySize -= elementSize; }
                }
                s_Allocations.erase(ptr);
                std::free(ptr);
            }
        }

        template <typename T>
        static bool _Destructor(void* ptr) noexcept
        {
            if (ptr)
            {
                ((T*) ptr)->~T();

                return true;
            }
            return false;
        }

        template <typename T, size_t TypeSize = sizeof(T), typename... Args>
        static constexpr T* _AllocateArray(const size_t count, Args&&... args) noexcept
        {
            T* ptr{};
            ptr = (T*) std::malloc(count * TypeSize);
            for (size_t i = 0; i < count; i++)
            {
                char* newptr = (char*) ptr;
                newptr += i * TypeSize;

                std::construct_at((T*) newptr, std::forward<Args>(args)...);
            }

            std::unique_lock<std::recursive_mutex> lock(Allocator::s_Mutex);
            s_AllocatedMemorySize += TypeSize * count;
            s_Allocations.emplace(
                    (void*) ptr, PointerMetaData{.size = TypeSize, .count = count, .destructor = &_Destructor<T>});
            return ptr;
        }

        template <typename T, size_t TypeSize = sizeof(T), typename... Args>
        static constexpr T* _Allocate(Args&&... args) noexcept
        {
            T* ptr{};
            ptr = (T*) std::malloc(TypeSize);
            std::construct_at((T*) ptr, std::forward<Args>(args)...);

            std::unique_lock<std::recursive_mutex> lock(Allocator::s_Mutex);
            s_AllocatedMemorySize += TypeSize;
            s_Allocations.emplace(
                    {(void*) ptr, PointerMetaData{.size = TypeSize, .count = 1, .destructor = &_Destructor<T>}});
            return ptr;
        }

        template <typename T>
        static constexpr void _AddToAllocatedMemory(T* ptr) noexcept
        {
            std::unique_lock<std::recursive_mutex> lock(Allocator::s_Mutex);
            if (!s_Allocations.contains((void*) ptr))
            {
                s_AllocatedMemorySize += sizeof(T);
                s_Allocations.emplace(
                        (void*) ptr, PointerMetaData{.size = sizeof(T), .count = 1, .destructor = &_Destructor<T>});
            }
        }

        template <typename T>
        static constexpr bool _IsLive(T* ptr) noexcept
        {
            std::unique_lock<std::recursive_mutex> lock(Allocator::s_Mutex);
            if (s_Allocations.contains((void*) ptr)) { return true; }
            return false;
        }

    public:
        struct Lambda {
            template <typename Tret, typename T>
            static Tret lambda_ptr_exec(void* data) noexcept
            {
                return (Tret) (*(T*) fn<T>())(data);
            }

            template <typename Tret = void, typename Tfp = Tret (*)(void*), typename T>
            static Tfp ptr(T t) noexcept
            {
                fn<T>(&t);
                return (Tfp) lambda_ptr_exec<Tret, T>;
            }

            template <typename T>
            static void* fn(void* new_fn = nullptr) noexcept
            {
                static void* fn;
                if (new_fn != nullptr) fn = new_fn;
                return fn;
            }
        };

        struct PointerMetaData {
            size_t size;
            size_t count;
            bool (*destructor)(void*);
        };


    public:
        inline static std::recursive_mutex s_Mutex{};
        inline static std::unordered_map<void*, PointerMetaData> s_Allocations;
        inline static size_t s_AllocatedMemorySize{};
    };

#endif

}// namespace Engine

#include <Core/Allocator.impl.hpp>