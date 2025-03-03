#include <algorithm>
#include <limits> 

#include "utilities.hpp"
#include "vk-swapChain.hpp"
#include "vk-utilities.hpp"

namespace gt::vk
{
    static VkSurfaceFormatKHR
        getSwapFormat(const std::vector<VkSurfaceFormatKHR>& c_formats)
    {
        for (const auto& c_format : c_formats)
        {
            if (c_format.format == VK_FORMAT_B8G8R8A8_SRGB && c_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            {
                return c_format;
            }
        }

        return c_formats[0];
    }

    static VkPresentModeKHR
        getSwapPresent(const std::vector<VkPresentModeKHR>& c_modes)
    {
        for (const auto &c_mode : c_modes)
        {
            // triple buffering
            if (c_mode == VK_PRESENT_MODE_MAILBOX_KHR)
            {
                return c_mode;
            }
        }

        return VK_PRESENT_MODE_FIFO_KHR; 
    }

    static VkExtent2D
        getSwapExtent(const VkSurfaceCapabilitiesKHR& c_capabilites, 
            int32_t frameBufferWidth, int32_t frameBufferHeight)
    {
        if (c_capabilites.currentExtent.width != std::numeric_limits<uint32_t>::max())
        {
            return c_capabilites.currentExtent;
        }
        else
        {
            VkExtent2D extent = { static_cast<uint32_t>(frameBufferWidth), static_cast<uint32_t>(frameBufferHeight) };

            extent.width =
                std::clamp(extent.width, c_capabilites.minImageExtent.width, c_capabilites.maxImageExtent.width);
            extent.height =
                std::clamp(extent.height, c_capabilites.minImageExtent.height, c_capabilites.maxImageExtent.height);

            return extent;
        }
    }

    void
        createSwapChain(VulkanContext& context, 
            int32_t frameBufferWidth, int32_t frameBufferHeight)
    {
        SwapChainDetails details = getSwapChainDetails(context.physicalDevice, context);

        VkSurfaceFormatKHR format    =  getSwapFormat(details.formats);
        VkPresentModeKHR   mode      =  getSwapPresent(details.modes);
        VkExtent2D         extent    =  getSwapExtent(details.capabilites, frameBufferWidth, frameBufferHeight);

        uint32_t imageCount = details.capabilites.minImageCount + 1;

        if (details.capabilites.maxImageCount > 0 && imageCount > details.capabilites.minImageCount)
        {
            imageCount = details.capabilites.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType   = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = context.surface;
        createInfo.minImageCount    = imageCount;
        createInfo.imageFormat      = format.format;
        createInfo.imageColorSpace  = format.colorSpace;
        createInfo.imageExtent      = extent;
        createInfo.imageArrayLayers = 1;

        // if post processing is going to be used, this will need to be changed
        createInfo.imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        QueueFamilyIndices indices = getQueueFamilies(context.physicalDevice, context.surface);
        uint32_t           families[] = {indices.graphicsFamily, indices.presentFamily};

        if (indices.graphicsFamily != indices.presentFamily)
        {
            createInfo.imageSharingMode      = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices   = families;
        }
        else
        {
            createInfo.imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE;
        }

        createInfo.preTransform = details.capabilites.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode    = mode;
        createInfo.clipped        = VK_TRUE;

        createInfo.oldSwapchain   = context.swapChain;

        gtAssert(vkCreateSwapchainKHR(context.device, &createInfo, nullptr, &context.swapChain) == VK_SUCCESS);

        vkGetSwapchainImagesKHR(context.device, context.swapChain, &imageCount, nullptr);
        context.swapChainImages.resize(imageCount);

        vkGetSwapchainImagesKHR(context.device, context.swapChain, &imageCount, context.swapChainImages.data());

        context.format = format.format;
        context.extent = extent;
    }

    void 
        destroySwapChain(const VulkanContext& c_context)
    {
        vkDestroySwapchainKHR(c_context.device, c_context.swapChain, nullptr);
    }

    void
        createImageViews(VulkanContext& context)
    {
        context.swapChainImageViews.resize(context.swapChainImages.size());

        for (size_t i = 0; i < context.swapChainImages.size(); ++i)
        {
            VkImageViewCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.image = context.swapChainImages[i];
            createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            createInfo.format   = context.format;

            createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

            createInfo.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
            createInfo.subresourceRange.baseMipLevel   = 0;
            createInfo.subresourceRange.levelCount     = 1;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount     = 1;

            gtAssert(vkCreateImageView(context.device, &createInfo, nullptr, &context.swapChainImageViews[i]) == VK_SUCCESS);
        }
    }

    void
        destroyImageViews(const VulkanContext& c_context)
    {
        for (const auto& c_imageView : c_context.swapChainImageViews)
        {
            vkDestroyImageView(c_context.device, c_imageView, nullptr);
        }
    }
}