#include "GLSLReflection.hpp"
#include <Core/Log.hpp>
#include <sstream>

namespace Engine
{

    void GLSLReflection::Init(std::string_view source)
    {
        m_ShaderSource = std::string(source);
        Tokenize();
        ParseUniformBlocks();
        CalculateUniformOffsets();

        m_Tokens.clear();
        m_Tokens.shrink_to_fit();
        m_LexerPivot = 0;
    }

    std::vector<ShaderUniformBlockLayout> GLSLReflection::GetUniformBlocksLayout() const { return m_UniformBlocks; }

    void GLSLReflection::ParseUniformBlocks()
    {
        TOKEN_TYPE tokenSequence[] = {TOKEN_TYPE::CLOSE_BRACKET, TOKEN_TYPE::UNIFORM};

        size_t tokenSequenceCount = sizeof(tokenSequence) / sizeof(TOKEN_TYPE);

        size_t uniformStart = 0;
        do {
            uniformStart = FindTokenSequence(tokenSequence, tokenSequenceCount, uniformStart);

            if (uniformStart != m_Tokens.size())
            {
                size_t negativeOffset = 0;

                do {
                    uniformStart -= 1;
                    negativeOffset++;
                } while (m_Tokens[uniformStart].first != TOKEN_TYPE::LAYOUT && uniformStart > 0);

                ShaderUniformBlockLayout layout;
                layout.Binding = std::stoul(m_Tokens[uniformStart + 6].second);
                layout.Name = m_Tokens[uniformStart + negativeOffset + 2].second;
                layout.Layout = GetUniformBlockMemoryLayout(uniformStart + 2);
                m_UniformBlocks.push_back(layout);

                size_t uniformLocation = uniformStart + negativeOffset + 4;

                do {
                    ShaderUniformDeclaration declaration;
                    declaration.Type = ShaderDataType::ShaderDataTypeFromString(m_Tokens[uniformLocation].second);
                    declaration.Name = m_Tokens[uniformLocation + 1].second;
                    m_UniformBlocks.back().Uniforms.push_back(declaration);

                    uniformLocation += 2;

                    while (m_Tokens[uniformLocation].first == TOKEN_TYPE::SEMICOLON) { uniformLocation++; }
                } while (m_Tokens[uniformLocation].first != TOKEN_TYPE::CLOSE_BRACE);

                uniformStart += negativeOffset + 2;
            }
        } while (uniformStart < m_Tokens.size());
    }

    void GLSLReflection::CalculateUniformOffsets()
    {
        for (auto& block: m_UniformBlocks)
        {
            size_t offset = 0;
            for (auto& uniform: block.Uniforms)
            {
                uniform.Offset = offset;
                offset += ShaderDataType::Size(uniform.Type);
            }
        }
    }

    void GLSLReflection::Tokenize()
    {
        std::string tokenValue;
        TOKEN_TYPE tokenType;
        while ((tokenType = GetNextToken(tokenValue, m_LexerPivot)) != TOKEN_TYPE::END)
        {
            m_Tokens.emplace_back(tokenType, tokenValue);
        }
    }

    void GLSLReflection::SkipSpaceOrNewLine(size_t& start) const
    {
        for (; start < m_ShaderSource.length(); start++)
        {
            char current = m_ShaderSource[start];
            switch (current)
            {
                case ' ':
                case '\n':
                case '\t':
                case '\r':
                    break;
                default:
                    return;
            }
        }
        return;
    }

    GLSLReflection::TOKEN_TYPE GLSLReflection::GetNextToken(std::string& token, size_t& start) const
    {
        SkipSpaceOrNewLine(start);
        token = {};
        TOKEN_TYPE currentType{};
        TOKEN_TYPE lastType = TOKEN_TYPE::NONE;
        for (; start < m_ShaderSource.length(); start++)
        {
            lastType = currentType;
            currentType = GetTokenType(m_ShaderSource, start);
            if (currentType == lastType || lastType == TOKEN_TYPE::NONE) { token += m_ShaderSource[start]; }
            if (currentType != lastType && lastType != TOKEN_TYPE::NONE)
            {
                if (token == "in") { return TOKEN_TYPE::INPUT; }
                else if (token == "out") { return TOKEN_TYPE::OUTPUT; }
                else if (token == "uniform") { return TOKEN_TYPE::UNIFORM; }
                else if (token == "layout") { return TOKEN_TYPE::LAYOUT; }
                else if (token == "shared") { return TOKEN_TYPE::SHARED; }
                else if (token == "packed") { return TOKEN_TYPE::PACKED; }
                else if (token == "std140") { return TOKEN_TYPE::STD140; }
                else if (token == "std430") { return TOKEN_TYPE::STD430; }
                else if (IsTokenNumber(token)) { return TOKEN_TYPE::NUMBER; }

                return lastType;
            }
        }
        return TOKEN_TYPE::END;
    }

    GLSLReflection::TOKEN_TYPE GLSLReflection::GetTokenType(const std::string& source, size_t start) const
    {
        for (; start < source.length(); start++)
        {
            char current = source[start];
            switch (current)
            {
                case ' ':
                case '\n':
                case '\t':
                case '\r':
                    return TOKEN_TYPE::SPACE;
                case ';':
                    return TOKEN_TYPE::SEMICOLON;
                case '(':
                    return TOKEN_TYPE::OPEN_BRACKET;
                case ')':
                    return TOKEN_TYPE::CLOSE_BRACKET;
                case '{':
                    return TOKEN_TYPE::OPEN_BRACE;
                case '}':
                    return TOKEN_TYPE::CLOSE_BRACE;
                case ',':
                    return TOKEN_TYPE::COMMA;
                case '=':
                    return TOKEN_TYPE::EQUAL;
                case ':':
                    return TOKEN_TYPE::COLON;
                default:
                    return TOKEN_TYPE::LITERAL;
            }
            return TOKEN_TYPE::END;
        }
        return TOKEN_TYPE::END;
    }

    bool GLSLReflection::IsTokenNumber(const std::string& token) const
    {
        for (auto& c: token)
        {
            switch (c)
            {
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                case '.':
                case 'f':
                    continue;
                default:
                    return false;
            }
        }

        return token.length() > 0;
    }

    size_t GLSLReflection::FindTokenSequence(const GLSLReflection::TOKEN_TYPE* tokens, size_t count, size_t start) const
    {
        if (start >= m_Tokens.size()) { return m_Tokens.size(); }
        if (m_Tokens.size() <= count) { return m_Tokens.size(); }
        if (count == 0) { return m_Tokens.size(); }

        for (size_t i = start; i <= m_Tokens.size() - count; i++)
        {
            size_t j = 0;
            for (j = 0; j < count; j++)
            {
                TOKEN_TYPE currentType = m_Tokens[i + j].first;
                auto& currentValue = m_Tokens[i + j].second;
                if (currentType != tokens[j]) { break; }
            }
            if (j == count) { return i; }
        };
        return m_Tokens.size();
    }

    ShaderBlockMemoryLayout GLSLReflection::GetUniformBlockMemoryLayout(size_t start) const
    {
        auto token_type = m_Tokens[start].first;

        if (token_type == TOKEN_TYPE::PACKED) { return ShaderBlockMemoryLayout::Packed; }
        else if (token_type == TOKEN_TYPE::SHARED) { return ShaderBlockMemoryLayout::Shared; }
        else if (token_type == TOKEN_TYPE::STD140) { return ShaderBlockMemoryLayout::Std140; }
        else if (token_type == TOKEN_TYPE::STD430) { return ShaderBlockMemoryLayout::Std430; }

        return ShaderBlockMemoryLayout::None;
    }
}// namespace Engine