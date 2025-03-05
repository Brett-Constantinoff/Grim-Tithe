#pragma once

#include <cassert>
#include <string>
#include <vector>

namespace gt::misc
{

#ifdef NDEBUG
#define gtAssert(condition) ((void) 0)
#else
#define gtAssert(condition) assert(condition)
#endif

    void readFile(std::vector<char> &data, const std::string &c_fileName);

} // namespace gt::misc
