#pragma once

#include <vector>
#include <vulkan/vulkan.h>

#include "r_context.hpp"

namespace gt::renderer
{
    void createInstance(VulkanContext &context, const std::vector<const char *> &c_extensions);
    void destroyInstance(const VulkanContext &c_context);
} // namespace gt::renderer