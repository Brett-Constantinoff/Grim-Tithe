#pragma once

#include <vector>
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

    struct SwapChainDetails
    {
        VkSurfaceCapabilitiesKHR        capabilites{};
        std::vector<VkSurfaceFormatKHR> formats{};
        std::vector<VkPresentModeKHR>   modes{};
    };

    [[nodiscard]] QueueFamilyIndices getQueueFamilies(const VkPhysicalDevice &c_device, const VkSurfaceKHR &c_surface);
    [[nodiscard]] SwapChainDetails   getSwapChainDetails(const VkPhysicalDevice &c_device, const VulkanContext &c_context);
}