#include "GLSLReflection.hpp"
#include <Core/Log.hpp>
#include <sstream>

namespace Engine
{

    auto ShaderDaraTypeFromString(std::string_view type) -> ShaderUniformType
    {
        if (type == "float") { return ShaderUniformType::Float; }
        if (type == "vec2") { return ShaderUniformType::Vec2; }
        if (type == "vec3") { return ShaderUniformType::Vec3; }
        if (type == "vec4") { return ShaderUniformType::Vec4; }
        if (type == "mat4") { return ShaderUniformType::Mat4; }
        return ShaderUniformType::None;
    };

    void GLSLReflection::Init(std::string_view source) { m_ShaderSource = std::string(source); }

    std::vector<ShaderUniformDeclaration> GLSLReflection::GetUniforms()
    {
        std::vector<ShaderUniformDeclaration> uniforms;

        std::string token;
        LEXER_TYPE lexerType = LEXER_TYPE::NONE;
        while ((lexerType = GetNext(token)) != LEXER_TYPE::END)
        {
            if (lexerType == LEXER_TYPE::LITERAL && token == "layout") { LOG("Potential uniform!\n"); }
        }
        return uniforms;
    }

    void GLSLReflection::SkipSpaceOrNewLine()
    {
        for (int i = m_LexerPivot; i < m_ShaderSource.length(); i++)
        {
            char current = m_ShaderSource[i];
            switch (current)
            {
                case ' ':
                case '\n':
                case '\t':
                case '\r':
                    m_LexerPivot = i + 1;
                    break;
                default:
                    return;
            }
        }
        return;
    }

    GLSLReflection::LEXER_TYPE GLSLReflection::GetNext(std::string& token)
    {
        SkipSpaceOrNewLine();
        token = {};
        LEXER_TYPE currentType{};
        LEXER_TYPE lastType = LEXER_TYPE::NONE;
        for (int i = m_LexerPivot; i < m_ShaderSource.length(); i++)
        {
            char current = m_ShaderSource[i];
            switch (current)
            {
                case ' ':
                case '\n':
                case '\t':
                case '\r':
                    m_LexerPivot = i + 1;
                    i = m_ShaderSource.length();
                    lastType = currentType;
                    currentType = LEXER_TYPE::SPACE;
                    break;
                case ';':
                    m_LexerPivot = i + 1;
                    lastType = currentType;
                    currentType = LEXER_TYPE::SEMICOLON;
                    break;
                case '(':
                    m_LexerPivot = i + 1;
                    lastType = currentType;
                    currentType = LEXER_TYPE::OPEN_BRACKET;
                    break;
                case ')':
                    m_LexerPivot = i + 1;
                    lastType = currentType;
                    currentType = LEXER_TYPE::CLOSE_BRACKET;
                    break;
                case ',':
                    m_LexerPivot = i + 1;
                    lastType = currentType;
                    currentType = LEXER_TYPE::COMMA;
                    break;
                case '=':
                    m_LexerPivot = i + 1;
                    lastType = currentType;
                    currentType = LEXER_TYPE::EQUAL;
                    break;
                default:
                    m_LexerPivot = i + 1;
                    token += current;
                    lastType = currentType;
                    currentType = LEXER_TYPE::LITERAL;
                    break;
            }

            if (currentType != lastType && lastType != LEXER_TYPE::NONE) { return lastType; }
        }
        return LEXER_TYPE::END;
    }
}// namespace Engine