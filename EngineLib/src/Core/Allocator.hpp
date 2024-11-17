#pragma once
#include <unordered_map>
#include <cstdint>
#include <Core/Log.hpp>

namespace Engine
{

    namespace Allocator
    {
        template <typename T, typename... Args>
        static T* _Allocate(Args&&... args);

        template <typename T>
        static T* _AllocateArray(const size_t size);

        template <typename T, size_t size = sizeof(T)>
        static void AddToAllocatedMemory(T* instance = nullptr);

        template <typename T>
        static size_t _Deallocate(T instance);

        template <typename T>
        static size_t _DeallocateArray(T instance);

        template <typename T>
        static bool IsLive(T* instance);

        template <typename T>
        static size_t Copy(T* destination, T* source, size_t size);

        inline static size_t s_AllocatedMemorySize{};
        inline static std::unordered_map<void*, size_t> s_AllocatedMemory{};

        inline static void AnalyzeMemory()
        {
            if (s_AllocatedMemorySize > 0)
            {
                LOG_INFO("Allocated memory on heap: %ziB\n", s_AllocatedMemorySize);

                for (auto& [ptr, size]: s_AllocatedMemory) { LOG_INFO("  %p: %ziB\n", ptr, size); }
            }
        }
    }// namespace Allocator

}// namespace Engine

#include <Core/Allocator.impl.hpp>