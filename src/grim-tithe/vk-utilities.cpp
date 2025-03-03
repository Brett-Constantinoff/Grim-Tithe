#include "vk-utilities.hpp"

namespace gt::vk
{
    QueueFamilyIndices
        getQueueFamilies(const VkPhysicalDevice& c_device, const VkSurfaceKHR& c_surface)
    {
        QueueFamilyIndices indices{};

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(c_device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies{};
        queueFamilies.resize(queueFamilyCount);

        vkGetPhysicalDeviceQueueFamilyProperties(c_device, &queueFamilyCount, queueFamilies.data());

        int32_t i = 0;
        for (const auto &queueFamily : queueFamilies)
        {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                indices.graphicsFamily = i;
            }

            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(c_device, i, c_surface, &presentSupport);

            if (presentSupport)
            {
                indices.presentFamily = i;
            }

            if (indices.isComplete())
            {
                break;
            }

            i++;
        }

        return indices;
    }

    SwapChainDetails
        getSwapChainDetails(const VkPhysicalDevice& c_device, const VulkanContext& c_context)
    {
        SwapChainDetails details{};

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(c_device, c_context.surface, &details.capabilites);

        uint32_t formatCount = 0;
        vkGetPhysicalDeviceSurfaceFormatsKHR(c_device, c_context.surface, &formatCount, nullptr);

        if (formatCount != 0)
        {
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(c_device, c_context.surface, &formatCount, details.formats.data());
        }

        uint32_t presentModeCount = 0;
        vkGetPhysicalDeviceSurfacePresentModesKHR(c_device, c_context.surface, &presentModeCount, nullptr);

        if (presentModeCount != 0)
        {
            details.modes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(c_device, c_context.surface, &presentModeCount,
                                                      details.modes.data());
        }

        return details;
    }
}