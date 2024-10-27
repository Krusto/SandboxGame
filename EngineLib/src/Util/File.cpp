//
// Created by Stoyanov, Krusto (K.S.) on 6/11/2022.
//
#include <fstream>

#include "File.hpp"
#include <Core/Log.hpp>
#include <Core/Core.hpp>

namespace Engine
{

    File::File(const char* path, FileFormat format) { Init(path, format); }

    void File::Init(const char* path, FileFormat format)
    {
        m_Path = path;
        m_FileFormat = format;
        m_Opened = false;
    }

    std::vector<uint8_t> Engine::File::ReadBinaryData()
    {
        if (!m_Opened)
        {
            std::fstream file(m_Path, std::ios::in | std::ios::binary);
            if (file.good()) { m_Opened = true; }
            else
            {
                LOG_ERROR("File %s open failed!", m_Path.c_str());
                return {};
            }
            file.seekg(0, std::ios::end);
            size_t length = file.tellg();
            file.seekg(0, std::ios::beg);
            std::vector<uint8_t> buffer(length);
            file.read((char*) buffer.data(), length);
            file.close();
            m_Opened = false;

            return buffer;
        }
        return {};
    }

    std::string Engine::File::ReadTextData()
    {
        if (!m_Opened)
        {
            std::fstream file(m_Path, std::ios::in);
            if (file.good()) { m_Opened = true; }
            else
            {
                LOG_ERROR("File %s open failed!", m_Path.c_str());
                return std::string();
            }
            file.seekg(0, std::ios::end);
            size_t length = file.tellg();
            file.seekg(0, std::ios::beg);
            std::string buffer;
            buffer.resize(length);
            file.read(&buffer[0], length);
            file.close();
            m_Opened = false;
            return buffer;
        }
        return std::string();
    }
}// namespace Engine