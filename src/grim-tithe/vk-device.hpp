#pragma once

#include <vulkan/vulkan.h>

#include "vk-context.hpp"

namespace gt::vk
{
    void createDevice(VulkanContext& context);
    void destroyDevice(const VulkanContext& c_context);
    void waitForGpuOperations(const VulkanContext& c_context);
} // namespace gt::vk