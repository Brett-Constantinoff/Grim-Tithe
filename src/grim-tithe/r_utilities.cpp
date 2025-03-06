#include "m_utilities.hpp"

#include "r_utilities.hpp"

namespace gt::renderer
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
            if ((queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) 
                && (queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT))
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

    void
        createSyncObjects(VulkanContext& context)
    {
        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for (size_t i = 0; i < context.c_framesInFlight; ++i)
        {
            gtAssert(vkCreateSemaphore(context.device, &semaphoreInfo, nullptr, &context.imageAvailableSemaphore[i]) ==
                         VK_SUCCESS &&
                     vkCreateSemaphore(context.device, &semaphoreInfo, nullptr, &context.renderFinishedSemaphore[i]) ==
                         VK_SUCCESS &&
                     vkCreateFence(context.device, &fenceInfo, nullptr, &context.inFlightFence[i]) == VK_SUCCESS);
        }
    }

    void
        destroySyncObjects(const VulkanContext& c_context)
    {
        for (size_t i = 0; i < c_context.c_framesInFlight; ++i)
        {
            vkDestroySemaphore(c_context.device, c_context.imageAvailableSemaphore[i], nullptr);
            vkDestroySemaphore(c_context.device, c_context.renderFinishedSemaphore[i], nullptr);
            vkDestroyFence(c_context.device, c_context.inFlightFence[i], nullptr);
        }
    }

} // namespace gt::renderer