#include <filesystem>
#include <fstream>

#include "m_utilities.hpp"

namespace gt::misc
{
    void
        readFile(std::vector<char> &data, const char *c_filePath)
    {
        std::ifstream file(c_filePath, std::ios::ate | std::ios::binary);

        gtAssert(file.is_open());

        size_t fileSize = static_cast<size_t>(file.tellg());
        data.resize(fileSize);

        file.seekg(0);
        file.read(data.data(), fileSize);
        file.close();
    }

    void
        concatString(char *c_res, const char *c_other, const size_t c_bufferSize)
    {
        std::strncat(c_res, c_other, c_bufferSize - std::strlen(c_res) - 1);
    }

    bool
        compareString(const char *c_str1, const char *c_str2, const size_t c_size)
    {
        return std::strncmp(c_str1, c_str2, c_size) == 0;
    }
} // namespace gt::misc