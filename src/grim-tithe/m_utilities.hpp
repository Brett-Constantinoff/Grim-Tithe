#pragma once

#include <cassert>
#include <vector>

namespace gt::misc
{

#ifdef NDEBUG
#define gtAssert(condition) ((void) 0)
#else
#define gtAssert(condition) assert(condition)
#endif

    void readFile(std::vector<char> &data, const char *c_filePath);
    void concatString(char *c_res, const char *c_other, const size_t c_bufferSize);
    bool compareString(const char *c_str1, const char *c_str2, const size_t c_size);

} // namespace gt::misc
