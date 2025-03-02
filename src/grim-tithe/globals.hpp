#pragma once

#include <cstdint>
#include <vector>

namespace gt::globals
{
    extern uint32_t                        g_width;
    extern uint32_t                        g_height;
    extern const char                     *g_title;
    extern const bool                      g_enableValidation;
    extern const std::vector<const char *> g_validationLayers;
} // namespace gt::globals
