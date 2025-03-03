#include <set>
#include <string>
#include <vector>

#include "globals.hpp"
#include "utilities.hpp"
#include "vk-device.hpp"
#include "vk-utilities.hpp"

namespace gt::vk
{
    static bool
        checkDeviceExtensions(const VkPhysicalDevice &c_device, const VulkanContext &c_context)
    {
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(c_device, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(c_device, nullptr, &extensionCount, availableExtensions.data());

        std::set<std::string> requiredExtensions(c_context.deviceExtensions.begin(), c_context.deviceExtensions.end());

        for (const auto &c_extension : availableExtensions)
        {
            requiredExtensions.erase(c_extension.extensionName);
        }

        return requiredExtensions.empty();
    }

    static bool
        isGpuGood(const VkPhysicalDevice& c_device, const VulkanContext& c_context)
    {
        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(c_device, &properties);

        bool extensionsSupported = checkDeviceExtensions(c_device, c_context);

        bool swapChainGood = false;
        if (extensionsSupported)
        {
            SwapChainDetails details = getSwapChainDetails(c_device, c_context);
            swapChainGood            = !details.formats.empty() && !details.modes.empty();
        }

        return getQueueFamilies(c_device, c_context.surface).isComplete() && extensionsSupported && swapChainGood;
    }

    static void
        pickGpu(VulkanContext &context)
    {
        uint32_t gpuCount = 0;
        vkEnumeratePhysicalDevices(context.instance, &gpuCount, nullptr);

        gtAssert(gpuCount != 0);

        std::vector<VkPhysicalDevice> gpus{};
        gpus.resize(gpuCount);

        vkEnumeratePhysicalDevices(context.instance, &gpuCount, gpus.data());

        for (const auto &gpu : gpus)
        {
            if (isGpuGood(gpu, context))
            {
                context.physicalDevice = gpu;
                break;
            }
        }

        gtAssert(context.physicalDevice != VK_NULL_HANDLE);
    }

    void
        createDevice(VulkanContext &context)
    {
        pickGpu(context);

        QueueFamilyIndices indices = getQueueFamilies(context.physicalDevice, context.surface);

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<int32_t> uniqueQueueFamilies = {indices.graphicsFamily, indices.presentFamily};

        float queuePriority = 1.0f;
        for (uint32_t queueFamily : uniqueQueueFamilies)
        {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount       = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkPhysicalDeviceFeatures deviceFeatures{};

        VkDeviceCreateInfo deviceCreateInfo{};
        deviceCreateInfo.sType                = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        deviceCreateInfo.pQueueCreateInfos    = queueCreateInfos.data();
        deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        deviceCreateInfo.pEnabledFeatures     = &deviceFeatures;
        deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(context.deviceExtensions.size());
        deviceCreateInfo.ppEnabledExtensionNames = context.deviceExtensions.data();

        if (context.enableValidation)
        {
            deviceCreateInfo.enabledLayerCount   = static_cast<uint32_t>(context.validationLayers.size());
            deviceCreateInfo.ppEnabledLayerNames = context.validationLayers.data();
        }
        else
        {
            deviceCreateInfo.enabledLayerCount = 0;
        }

        gtAssert(vkCreateDevice(context.physicalDevice, &deviceCreateInfo, nullptr, &context.device) == VK_SUCCESS);

        vkGetDeviceQueue(context.device, indices.graphicsFamily, 0, &context.graphicsQueue);
        vkGetDeviceQueue(context.device, indices.presentFamily, 0, &context.presentQueue);
    }

    void
        destroyDevice(const VulkanContext& c_context)
    {
        vkDestroyDevice(c_context.device, nullptr);
    }

    void
        waitForGpuOperations(const VulkanContext& c_context)
    {
        vkDeviceWaitIdle(c_context.device);
    }
} // namespace gt::vk