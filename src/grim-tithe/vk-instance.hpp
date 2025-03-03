#pragma once

#include <vector>
#include <vulkan/vulkan.h>

#include "vk-context.hpp"

namespace gt::vk
{
    void createInstance(VulkanContext &context, const std::vector<const char *> &c_extensions);
    void destroyInstance(const VulkanContext &c_context);
} // namespace gt::vk