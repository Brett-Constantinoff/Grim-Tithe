#pragma once

#include <vulkan/vulkan.h>

#include "vk-context.hpp"

namespace gt::vk
{
    struct QueueFamilyIndices
    {
        int32_t graphicsFamily = -1;
        int32_t presentFamily  = -1;

        bool
            isComplete() const
        {
            return graphicsFamily >= 0 && presentFamily >= 0;
        }
    };

    void createDevice(VulkanContext& context);
    void destroyDevice(const VulkanContext& c_context);
} // namespace gt::vk