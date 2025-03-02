#include "globals.hpp"

namespace gt::globals
{
    uint32_t    g_width  = 800;
    uint32_t    g_height = 600;
    const char *g_title  = "Grim Tithe";

#ifdef NODEBUG
    const bool g_enableValidation = false;
#else
    const bool g_enableValidation = true;
#endif
} // namespace gt::globals
