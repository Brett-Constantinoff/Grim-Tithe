#pragma once

#include <vector>
#include <vulkan/vulkan.h>

namespace gt::vk
{
	struct VulkanContext
	{
		// instance
        VkInstance instance = VK_NULL_HANDLE;
        VkDebugUtilsMessengerEXT debugMessenger{};

        // validation
        std::vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"};

#ifdef NODEBUG
        bool enableValidation = false;
#else
        bool enableValidation = true;
#endif

		// device
        VkDevice         device         = VK_NULL_HANDLE;
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        VkQueue          graphicsQueue  = VK_NULL_HANDLE;
        VkQueue          presentQueue   = VK_NULL_HANDLE;
        std::vector<const char*> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

        // surface
        VkSurfaceKHR surface = VK_NULL_HANDLE;

        // swap chain
        VkSwapchainKHR swapChain = VK_NULL_HANDLE;
        VkFormat format{};
        VkExtent2D           extent{};
        std::vector<VkImage> swapChainImages{};
	};
}