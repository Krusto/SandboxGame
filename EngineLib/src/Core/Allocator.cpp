#include "Allocator.hpp"

namespace Engine
{

    void Allocator::AnalyzeMemory()
    {
        if (s_AllocatedMemorySize > 0)
        {
            LOG_INFO("Allocated memory on heap: %ziB\n", s_AllocatedMemorySize);

            for (auto& [ptr, size]: s_AllocatedMemory) { LOG_INFO("  %p: %ziB\n", ptr, size); }
        }
    }
}// namespace Engine
