#include <filesystem>
#include <fstream>

#include "utilities.hpp"

namespace gt::utilities
{
    void
        readFile(std::vector<char> &data, const std::string &c_fileName)
    {
        const std::string c_exePath     = std::filesystem::current_path().string();
        const std::string c_correctPath = c_exePath + c_fileName;

        std::ifstream file(c_correctPath, std::ios::ate | std::ios::binary);

        gtAssert(file.is_open());

        size_t fileSize = static_cast<size_t>(file.tellg());
        data.resize(fileSize);

        file.seekg(0);
        file.read(data.data(), fileSize);
        file.close();
    }
} // namespace gt::utilities