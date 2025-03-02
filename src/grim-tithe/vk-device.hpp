#pragma once

#include <vulkan/vulkan.h>

namespace gt::vk
{
    struct QueueFamilyIndices
    {
        int32_t graphicsFamily = -1;

        bool
            isComplete() const
        {
            return graphicsFamily >= 0;
        }
    };

    [[nodiscard]] VkDevice getDevice(const VkInstance &instance);
    void                   destroyDevice(const VkDevice &device);
} // namespace gt::vk