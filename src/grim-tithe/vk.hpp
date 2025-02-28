#pragma once

#include <vulkan/vulkan.h>

namespace gt::vk
{
    [[nodiscard]] VkInstance &createInstance(uint32_t extensionCount, const char **c_extensions);
    void                      destroyInstance(VkInstance &instance);
} // namespace gt::vk