#pragma once

#include <vulkan/vulkan.h>

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

    [[nodiscard]] VkDevice getDevice(const VkInstance &c_instance, const VkSurfaceKHR &c_surface);
    void                   destroyDevice(const VkDevice &c_device);
} // namespace gt::vk