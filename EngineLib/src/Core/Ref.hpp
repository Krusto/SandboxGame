#pragma once

#include <mutex>
#include <atomic>

#include <Core/Log.hpp>
#ifdef __GNUC__
#include <cxxabi.h>// GCC / Clang only
#endif
class RefCounted
{
public:
    void IncRefCount() const
    {
        m_RefCount++;
        LOG_INFO("IncRef %u\n", GetRefCount());
    }

    void DecRefCount() const
    {
        m_RefCount--;
        LOG_INFO("DecRef %i\n", GetRefCount());
    }

    uint32_t GetRefCount() const { return m_RefCount.load(); }

private:
    mutable std::atomic<uint32_t> m_RefCount = 0;
};

namespace RefUtils
{
    void AddToLiveReferences(void* instance, size_t size);
    void RemoveFromLiveReferences(void* instance);
    size_t GetSizeFromLiveReferences(void* instance);
    bool IsLive(void* instance);
}// namespace RefUtils

inline static std::mutex m{};

template <typename T>
class Ref
{
public:
    Ref() : m_Instance(nullptr) {}

    Ref(std::nullptr_t) : m_Instance(nullptr) {}

    Ref(T* instance) : m_Instance(instance)
    {
        static_assert(std::is_base_of<RefCounted, T>::value, "Class is not RefCounted!");
        std::lock_guard<std::mutex> lock(m);
        LOG_INFO("Allocated %iB\n", sizeof(T));
        IncRef();
    }

    template <typename T2>
    Ref(const Ref<T2>& other)
    {
        m_Instance = (T*) other.m_Instance;
        IncRef();
    }

    template <typename T2>
    Ref(Ref<T2>&& other)
    {
        m_Instance = (T*) other.m_Instance;
        other.m_Instance = nullptr;
    }

    static Ref<T> CopyWithoutIncrement(const Ref<T>& other)
    {
        Ref<T> result = nullptr;
        result->m_Instance = other.m_Instance;
        return result;
    }

    ~Ref() { DecRef(); }

    Ref(const Ref<T>& other) : m_Instance(other.m_Instance) { IncRef(); }

    Ref& operator=(std::nullptr_t)
    {
        DecRef();
        m_Instance = nullptr;
        return *this;
    }

    Ref& operator=(const Ref<T>& other)
    {
        other.IncRef();
        DecRef();

        m_Instance = other.m_Instance;
        return *this;
    }

    template <typename T2>
    Ref& operator=(const Ref<T2>& other)
    {
        other.IncRef();
        DecRef();

        m_Instance = other.m_Instance;
        return *this;
    }

    template <typename T2>
    Ref& operator=(Ref<T2>&& other)
    {
        DecRef();

        m_Instance = other.m_Instance;
        other.m_Instance = nullptr;
        return *this;
    }

    operator bool() { return m_Instance != nullptr; }

    operator bool() const { return m_Instance != nullptr; }

    T* operator->() { return m_Instance; }

    const T* operator->() const { return m_Instance; }

    T& operator*() { return *m_Instance; }

    const T& operator*() const { return *m_Instance; }

    T* Raw() { return m_Instance; }

    const T* Raw() const { return m_Instance; }

    void Reset(T* instance = nullptr)
    {
        DecRef();
        m_Instance = instance;
    }

    template <typename T2>
    Ref<T2> As() const
    {
        return Ref<T2>(*this);
    }

    template <typename... Args, auto size = sizeof(T)>
    static Ref<T> Create(Args&&... args)
    {
        Engine::mem::memsize += size;

        return Ref<T>(new T(std::forward<Args>(args)...));
    }

    bool operator==(const Ref<T>& other) const { return m_Instance == other.m_Instance; }

    bool operator!=(const Ref<T>& other) const { return !(*this == other); }

    bool EqualsObject(const Ref<T>& other)
    {
        if (!m_Instance || !other.m_Instance) return false;

        return *m_Instance == *other.m_Instance;
    }

    operator T*() { return m_Instance; }

    operator T*() const { return m_Instance; }

private:
    template <auto size = sizeof(T)>
    void IncRef() const
    {
        if (m_Instance)
        {
            m_Instance->IncRefCount();
            RefUtils::AddToLiveReferences((void*) m_Instance, size);
        }
    }

    void DecRef() const
    {
        if (m_Instance)
        {
            m_Instance->DecRefCount();
            if (m_Instance->GetRefCount() == 0)
            {
                std::lock_guard<std::mutex> lock(m);
                delete m_Instance;
                Engine::mem::memsize -= RefUtils::GetSizeFromLiveReferences((void*) m_Instance);

                LOG_INFO("Deallocated %iB\n", RefUtils::GetSizeFromLiveReferences((void*) m_Instance));
                RefUtils::RemoveFromLiveReferences((void*) m_Instance);
                m_Instance = nullptr;
            }
        }
    }

    template <class T2>
    friend class Ref;
    mutable T* m_Instance;
};

template <typename T>
class WeakRef
{
public:
    WeakRef() = default;

    WeakRef(Ref<T> ref) { m_Instance = ref.Raw(); }

    WeakRef(T* instance) { m_Instance = instance; }

    bool IsValid() const { return m_Instance ? RefUtils::IsLive(m_Instance) : false; }

    operator bool() const { return IsValid(); }

    Ref<T> Lock() const
    {
        if (IsValid()) { return Ref<T>(m_Instance); }
        return Ref<T>();
    }

private:
    T* m_Instance = nullptr;
};
