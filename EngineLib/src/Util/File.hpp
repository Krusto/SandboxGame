//
// Created by Stoyanov, Krusto (K.S.) on 6/11/2022.
//
#include <vector>
#include <string>
#include <cstdint>

namespace Engine
{
    enum class FileFormat
    {
        Regular,
        Binary
    };

    class File
    {
    public:
        File() = default;
        File(const char* path, FileFormat format);

    public:
        void Init(const char* path, FileFormat format);
        std::vector<uint8_t> ReadBinaryData();
        std::string ReadTextData();

    public:
        enum class Type
        {
            None = 0,
            Directory,
            Generic
        };

    private:
        bool m_Opened{};
        FileFormat m_FileFormat;
        std::string m_Path{};
    };
}// namespace Engine