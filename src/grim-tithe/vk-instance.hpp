#pragma once

#include <vector>
#include <vulkan/vulkan.h>

namespace gt::vk
{
    [[nodiscard]] VkInstance &createInstance(const std::vector<const char *> &c_extensions);
    void                      destroyInstance(VkInstance &instance);
} // namespace gt::vk