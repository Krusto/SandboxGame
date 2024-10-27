
#include <unordered_map>
#include <mutex>
#include "Ref.hpp"

static std::unordered_map<void*, size_t> s_LiveReferences;
static std::mutex s_LiveReferenceMutex;

namespace RefUtils
{

    void AddToLiveReferences(void* instance, size_t size)
    {
        std::scoped_lock<std::mutex> lock(s_LiveReferenceMutex);
        s_LiveReferences.emplace(instance, size);
    }

    void RemoveFromLiveReferences(void* instance)
    {
        std::scoped_lock<std::mutex> lock(s_LiveReferenceMutex);
        s_LiveReferences.erase(instance);
    }

    size_t GetSizeFromLiveReferences(void* instance)
    {
        std::scoped_lock<std::mutex> lock(s_LiveReferenceMutex);
        if (s_LiveReferences.contains(instance)) return s_LiveReferences.at(instance);
        return 0;
    }

    bool IsLive(void* instance) { return s_LiveReferences.find(instance) != s_LiveReferences.end(); }
}// namespace RefUtils
