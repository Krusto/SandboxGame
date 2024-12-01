#pragma once
#include <Renderer/ShaderUniform.hpp>

#include <string_view>
#include <string>
#include <vector>

namespace Engine
{
    class GLSLReflection
    {
    public:
        void Init(std::string_view source);
        std::vector<ShaderUniformDeclaration> GetUniforms();

    private:
        std::string m_ShaderSource;
        size_t m_LexerPivot{};

    private:
        enum class LEXER_TYPE
        {
            NONE,
            COMMA,
            SEMICOLON,
            LITERAL,
            OPEN_BRACKET,
            CLOSE_BRACKET,
            EQUAL,
            SPACE,
            END
        };

    private:
        void SkipSpaceOrNewLine();
        LEXER_TYPE GetNext(std::string& token);
    };
}// namespace Engine