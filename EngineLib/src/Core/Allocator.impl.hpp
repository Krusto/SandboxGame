#pragma once
#include <algorithm>
#include <Core/Allocator.hpp>
#include <Core/Log.hpp>
#include <mutex>
#include <shared_mutex>
#include <source_location>

#include "Allocator.hpp"

namespace Engine
{
    namespace Allocator
    {
        using mutex_type = std::shared_timed_mutex;
        using read_only_lock = std::shared_lock<mutex_type>;
        using updatable_lock = std::unique_lock<mutex_type>;

        inline static mutex_type s_AllocatorMutex{};

#ifdef _DEBUG
        inline static void LOG_ALLOC_LOCATION(size_t size,
                                              std::source_location location = std::source_location::current())
        {
            LOG("Allocated %ziB\n     FILE: %s(%d:%d) `%s`: \n", size, location.file_name(), location.line(),
                location.column(), location.function_name());
        }

        inline static void LOG_DEALLOC_LOCATION(size_t size,
                                                std::source_location location = std::source_location::current())
        {
            LOG("Deallocated %ziB\n     FILE: %s(%d:%d) `%s`: \n", size, location.file_name(), location.line(),
                location.column(), location.function_name());
        }

        inline static void LOG_ALLOC_ARRAY_LOCATION(size_t size,
                                                    std::source_location location = std::source_location::current())
        {
            LOG("Allocated araray %ziB\n     FILE: %s(%d:%d) `%s`: \n", size, location.file_name(), location.line(),
                location.column(), location.function_name());
        }

        inline static void LOG_DEALLOC_ARRAY_LOCATION(size_t size,
                                                      std::source_location location = std::source_location::current())
        {
            LOG("Deallocated array %ziB\n     FILE: %s(%d:%d) `%s`: \n", size, location.file_name(), location.line(),
                location.column(), location.function_name());
        }

#define Allocate(T, varName, ...)                                                                                      \
    T* varName;                                                                                                        \
    do {                                                                                                               \
        Engine::Allocator::LOG_ALLOC_LOCATION(sizeof(T));                                                              \
        varName = Engine::Allocator::_Allocate<T>(__VA_ARGS__);                                                        \
    } while (0);

#define Deallocate(varName)                                                                                            \
    do {                                                                                                               \
        Engine::Allocator::LOG_DEALLOC_LOCATION(Engine::Allocator::_Deallocate<decltype(varName)>(varName));           \
        varName = nullptr;                                                                                             \
    } while (0);

#define AllocateArray(T, varName, size)                                                                                \
    do {                                                                                                               \
        Engine::Allocator::LOG_ALLOC_ARRAY_LOCATION(sizeof(T) * size);                                                 \
        varName = Engine::Allocator::_AllocateArray<T>(size);                                                          \
    } while (0);

#define DeallocateArray(varName)                                                                                       \
    do {                                                                                                               \
        Engine::Allocator::LOG_DEALLOC_ARRAY_LOCATION(                                                                 \
                Engine::Allocator::_DeallocateArray<decltype(varName)>(varName));                                      \
        varName = nullptr;                                                                                             \
    } while (0);
#else
#endif

        template <typename T, typename... Args>
        T* _Allocate(Args&&... args)
        {

            updatable_lock lock(s_AllocatorMutex);
            auto ptr = new T(std::forward<Args>(args)...);

            s_AllocatedMemorySize += sizeof(T);
            s_AllocatedMemory.try_emplace(ptr, sizeof(T));
            return ptr;
        }

        template <typename T>
        inline T* _AllocateArray(size_t size)
        {
            updatable_lock lock(s_AllocatorMutex);
            auto ptr = new T[size];

            s_AllocatedMemorySize += size * sizeof(T);
            s_AllocatedMemory.try_emplace(ptr, size * sizeof(T));
            return ptr;
        }

        template <typename T, size_t size>
        void AddToAllocatedMemory(T* instance)
        {
            if (instance)
            {
                updatable_lock lock(s_AllocatorMutex);
                auto [ignore, inserted] = s_AllocatedMemory.try_emplace(instance, size);
                if (inserted) { s_AllocatedMemorySize += size; }
            }
        }

        template <typename T>
        inline static size_t _Deallocate(T instance)
        {
            updatable_lock lock(s_AllocatorMutex);
            if (s_AllocatedMemory.find(instance) != s_AllocatedMemory.end())
            {
                size_t size = s_AllocatedMemory[instance];
                s_AllocatedMemorySize -= size;
                delete instance;

                s_AllocatedMemory.erase(instance);
                return size;
            }
            return 0;
        }

        template <typename T>
        inline static size_t _DeallocateArray(T instance)
        {

            updatable_lock lock(s_AllocatorMutex);
            if (s_AllocatedMemory.find(instance) != s_AllocatedMemory.end())
            {
                size_t size = s_AllocatedMemory[instance];
                s_AllocatedMemorySize -= size;
                delete[] instance;

                s_AllocatedMemory.erase(instance);
                return size;
            }
            return 0;
        }

        template <typename T>
        bool IsLive(T* instance)
        {
            updatable_lock lock(s_AllocatorMutex);
            return s_AllocatedMemory.find(instance) != s_AllocatedMemory.end();
        }

        template <typename T>
        size_t Copy(T* destination, T* source, size_t size)
        {
            if (nullptr == destination) { return 0; }
            if (nullptr == source) { return 0; }

            std::for_each_n(destination, size, [source](T& value) mutable {
                value = *source;
                ++source;
            });

            return size;
        }
    }// namespace Allocator
}// namespace Engine