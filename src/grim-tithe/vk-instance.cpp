#include <iostream>
#include <vector>

#include "globals.hpp"
#include "utilities.hpp"
#include "vk-context.hpp"
#include "vk-instance.hpp"

using namespace gt::globals;

namespace gt::vk
{
    static VKAPI_ATTR VkBool32 VKAPI_CALL
        debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT      messageSeverity,
                      VkDebugUtilsMessageTypeFlagsEXT             messageType,
                      const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData)
    {
        if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
        {
            std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
        }

        return VK_FALSE;
    }

    static void
        populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo)
    {
        createInfo                 = {};
        createInfo.sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                     VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                     VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = debugCallback;
    }

    static VkResult
        createDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *c_pCreateInfo,
                                     const VkAllocationCallbacks *c_pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger)
    {
        auto func =
            (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
        if (func != nullptr)
        {
            return func(instance, c_pCreateInfo, c_pAllocator, pDebugMessenger);
        }
        else
        {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }

    static void
        destroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
                                      const VkAllocationCallbacks *c_pAllocator)
    {
        auto func =
            (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
        if (func != nullptr)
        {
            func(instance, debugMessenger, c_pAllocator);
        }
    }

    void
        createInstance(VulkanContext& context, const std::vector<const char *> &c_extensions)
    {
        // check for validation
        uint32_t layerCount{};
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> layers{};
        layers.resize(layerCount);

        vkEnumerateInstanceLayerProperties(&layerCount, layers.data());

        bool validationSupport = true;
        for (const auto &layer : context.validationLayers)
        {
            bool found = false;
            for (const auto &property : layers)
            {
                if (strncmp(layer, property.layerName, 255) == 0)
                {
                    found = true;
                    break;
                }
            }

            if (!found)
            {
                validationSupport = false;
                break;
            }
        }

        gtAssert(context.enableValidation && validationSupport);

        VkApplicationInfo appInfo{};
        appInfo.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName   = g_title;
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName        = "No Engine";
        appInfo.engineVersion      = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion         = VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo{};
        createInfo.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo        = &appInfo;
        createInfo.enabledExtensionCount   = static_cast<uint32_t>(c_extensions.size());
        createInfo.ppEnabledExtensionNames = c_extensions.data();

        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
        if (context.enableValidation)
        {
            createInfo.enabledLayerCount   = static_cast<uint32_t>(context.validationLayers.size());
            createInfo.ppEnabledLayerNames = context.validationLayers.data();

            populateDebugMessengerCreateInfo(debugCreateInfo);
            createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *) &debugCreateInfo;
        }
        else
        {
            createInfo.enabledLayerCount = 0;
            createInfo.pNext             = nullptr;
        }

        gtAssert(vkCreateInstance(&createInfo, nullptr, &context.instance) == VK_SUCCESS);

        // create debug messenger
        if (context.enableValidation)
        {
            VkDebugUtilsMessengerCreateInfoEXT createInfo{};
            populateDebugMessengerCreateInfo(createInfo);

            gtAssert(createDebugUtilsMessengerEXT(context.instance, &createInfo, nullptr, &context.debugMessenger) == VK_SUCCESS);
        }
    }

    void
        destroyInstance(const VulkanContext &c_context)
    {
        if (c_context.enableValidation)
        {
            destroyDebugUtilsMessengerEXT(c_context.instance, c_context.debugMessenger, nullptr);
        };

        vkDestroyInstance(c_context.instance, nullptr);
    }

} // namespace gt::vk