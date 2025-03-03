#include <set>
#include <string>
#include <vector>

#include "globals.hpp"
#include "utilities.hpp"
#include "vk-device.hpp"

namespace gt::vk
{
    using namespace gt::globals;

    static QueueFamilyIndices
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

        return getQueueFamilies(c_device, c_context.surface).isComplete() && checkDeviceExtensions(c_device, c_context);
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
} // namespace gt::vk