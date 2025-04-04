#pragma once
#include <Renderer/Renderer.hpp>
#include <Renderer/Predefines.hpp>

#include <string_view>
#include <string>
#include <vector>

namespace Engine
{

    class EXPORT_RENDERER GLSLReflection
    {
    public:
        void Init(std::string_view source);

        std::vector<ShaderUniformBlockLayout> GetUniformBlocksLayout() const;

    private:
        enum class TOKEN_TYPE : uint8_t
        {
            NONE,
            COMMA,
            SEMICOLON,
            LITERAL,
            OPEN_BRACKET,
            CLOSE_BRACKET,
            OPEN_BRACE,
            CLOSE_BRACE,
            NUMBER,
            INPUT,
            OUTPUT,
            UNIFORM,
            COLON,
            LAYOUT,
            EQUAL,
            SPACE,
            SHARED,
            PACKED,
            STD140,
            STD430,
            END
        };

    private:
        void Tokenize();
        void ParseUniformBlocks();
        void CalculateUniformOffsets();

        void SkipSpaceOrNewLine(size_t& start) const;
        TOKEN_TYPE GetNextToken(std::string& token, std::size_t& start) const;
        TOKEN_TYPE GetTokenType(const std::string& source, std::size_t start) const;
        bool IsTokenNumber(const std::string& token) const;
        std::size_t FindTokenSequence(const TOKEN_TYPE* tokens, std::size_t count, std::size_t start = 0) const;

        ShaderBlockMemoryLayout GetUniformBlockMemoryLayout(size_t start) const;

    private:
        std::string m_ShaderSource;
        std::size_t m_LexerPivot{};
        std::vector<ShaderUniformBlockLayout> m_UniformBlocks;

        std::vector<std::pair<TOKEN_TYPE, std::string>> m_Tokens{};
    };
}// namespace Engine