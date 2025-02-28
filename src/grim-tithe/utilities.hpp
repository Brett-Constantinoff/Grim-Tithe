#pragma once

#include <cassert>

namespace gt::utilities
{

#ifdef NDEBUG
#define gtAssert(condition) ((void) 0)
#else
#define gtAssert(condition) assert(condition)
#endif

} // namespace gt::utilities
