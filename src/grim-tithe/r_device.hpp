#pragma once

#include <vulkan/vulkan.h>

#include "r_context.hpp"

namespace gt::renderer
{
    void createDevice(VulkanContext& context);
    void destroyDevice(const VulkanContext& c_context);
    void waitForGpuOperations(const VulkanContext& c_context);
} // namespace gt::renderer