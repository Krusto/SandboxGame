#pragma once

#include "./CVector.h"
#include "./CStringView.h"
#include "./CPair.h"

namespace Engine
{

    template <typename T>
    class Vector
    {
    public:
        Vector() { Create(); }

        Vector(std::initializer_list<T> list)
        {
            Create();
            for (auto& item: list) PushBack(item);
        };

        ~Vector() { Destroy(); }

    public:
        inline void Create() { m_Vector = cvector_create_generic(sizeof(T)); }

        inline void Destroy() { cvector_destroy(m_Vector); }

    public:
        inline std::size_t Length() const { return cvector_length(m_Vector); }

        inline void Clear()
        {
            cvector_resize(m_Vector, 0);
            cvector_shrink_to_fit(m_Vector);
        }

        T& Get(size_t index) { return (T*) cvector_get_ptr(m_Vector, index); }

        inline void PushBack(const T& value) { cvector_push_generic(m_Vector, (void*) &value); }

    public:
        CVectorT* Raw() const { return m_Vector; }

    public:
        T& operator[](size_t index) { return Get(index); }

        const T& operator[](size_t index) const { return Get(index); }

    private:
        CVectorT* m_Vector;
    };

    class StringView
    {
    public:
        StringView() = default;
        ~StringView() = default;

    public:
        CStringView m_StringView;
    };

    template <typename T, typename U>
    class Pair
    {
    public:
        Pair() = default;

        Pair(T first, U second) : first(first), second(second) {}

        Pair(const Pair& other) : first(other.first), second(other.second) {}

        ~Pair() = default;

    public:
        T first;
        U second;
    };

}// namespace Engine