#pragma once
#include <string>
#include <cstdint>

namespace Engine
{
    class ShaderResourceDeclaration
    {
    public:
        ShaderResourceDeclaration() = default;

        explicit ShaderResourceDeclaration(std::string_view name, uint32_t resourceRegister, uint32_t count)
            : m_Name(name), m_Register(resourceRegister), m_Count(count)
        {}

        ~ShaderResourceDeclaration() = default;

    public:
        virtual const std::string& GetName() const { return m_Name; }

        virtual uint32_t GetRegister() const { return m_Register; }

        virtual uint32_t GetCount() const { return m_Count; }

    private:
        std::string m_Name;
        uint32_t m_Register = 0;
        uint32_t m_Count = 0;
    };
}// namespace Engine