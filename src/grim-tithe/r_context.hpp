#pragma once

#include <vector>
#include <vulkan/vulkan.h>

namespace gt::renderer
{
    struct VulkanContext
    {
        // misc
        const static int32_t c_framesInFlight = 3;

        // instance
        VkInstance               instance = VK_NULL_HANDLE;
        VkDebugUtilsMessengerEXT debugMessenger{};

        // validation
        std::vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"};

#ifdef NODEBUG
        bool enableValidation = false;
#else
        bool enableValidation = true;
#endif

        // device
        VkDevice                  device           = VK_NULL_HANDLE;
        VkPhysicalDevice          physicalDevice   = VK_NULL_HANDLE;
        VkQueue                   graphicsQueue    = VK_NULL_HANDLE;
        VkQueue                   presentQueue     = VK_NULL_HANDLE;
        std::vector<const char *> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

        // surface
        VkSurfaceKHR surface = VK_NULL_HANDLE;

        // swap chain
        VkSwapchainKHR             oldSwapChain = VK_NULL_HANDLE;
        VkSwapchainKHR             swapChain    = VK_NULL_HANDLE;
        VkFormat                   format{};
        VkExtent2D                 extent{};
        std::vector<VkImage>       swapChainImages{};
        std::vector<VkImageView>   swapChainImageViews{};
        std::vector<VkFramebuffer> framebuffers{};

        // graphics pipeline
        VkRenderPass     renderPass{};
        VkPipelineLayout layout{};
        VkPipeline       pipeline{};

        // compute pipeline
        VkPipelineLayout      computePipelineLayout{};
        VkDescriptorSetLayout computeDescriptorSetLayout{};
        VkPipeline            computePipeline{};

        // commands
        VkCommandPool   commandPool{};
        VkCommandBuffer commandBuffers[c_framesInFlight]{};

        // sync
        VkSemaphore imageAvailableSemaphore[c_framesInFlight]{};
        VkSemaphore renderFinishedSemaphore[c_framesInFlight]{};
        VkFence     inFlightFence[c_framesInFlight]{};
    };
} // namespace gt::renderer