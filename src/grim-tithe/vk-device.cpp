#include <vector>

#include "globals.hpp"
#include "utilities.hpp"
#include "vk-device.hpp"

namespace gt::vk
{
    using namespace gt::globals;

    static VkPhysicalDevice s_physicalDevice = VK_NULL_HANDLE;
    static VkQueue          s_graphicsQueue  = VK_NULL_HANDLE;

    static QueueFamilyIndices
        getQueueFamilies(VkPhysicalDevice device)
    {
        QueueFamilyIndices indices{};

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies{};
        queueFamilies.resize(queueFamilyCount);

        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        int32_t i = 0;
        for (const auto &queueFamily : queueFamilies)
        {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                indices.graphicsFamily = i;
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
        isGpuGood(VkPhysicalDevice device)
    {
        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(device, &properties);

        VkPhysicalDeviceType type = properties.deviceType;

        // no integrated graphics
        bool isDiscrete = type == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;

        return isDiscrete && getQueueFamilies(device).isComplete();
    }

    static void
        pickGpu(const VkInstance &instance)
    {
        uint32_t gpuCount = 0;
        vkEnumeratePhysicalDevices(instance, &gpuCount, nullptr);

        gtAssert(gpuCount != 0);

        std::vector<VkPhysicalDevice> gpus{};
        gpus.resize(gpuCount);

        vkEnumeratePhysicalDevices(instance, &gpuCount, gpus.data());

        for (const auto &gpu : gpus)
        {
            if (isGpuGood(gpu))
            {
                s_physicalDevice = gpu;
            }
        }

        gtAssert(s_physicalDevice != VK_NULL_HANDLE);
    }

    VkDevice
        getDevice(const VkInstance &instance)
    {
        VkDevice device = VK_NULL_HANDLE;

        pickGpu(instance);

        QueueFamilyIndices indices = getQueueFamilies(s_physicalDevice);

        VkDeviceQueueCreateInfo createInfo{};
        createInfo.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        createInfo.queueFamilyIndex = indices.graphicsFamily;
        createInfo.queueCount       = 1;

        float priority              = 1.0f;
        createInfo.pQueuePriorities = &priority;

        VkPhysicalDeviceFeatures deviceFeatures{};

        VkDeviceCreateInfo deviceCreateInfo{};
        deviceCreateInfo.sType                = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        deviceCreateInfo.pQueueCreateInfos    = &createInfo;
        deviceCreateInfo.queueCreateInfoCount = 1;
        deviceCreateInfo.pEnabledFeatures     = &deviceFeatures;

        if (g_enableValidation)
        {
            deviceCreateInfo.enabledLayerCount   = static_cast<uint32_t>(g_validationLayers.size());
            deviceCreateInfo.ppEnabledLayerNames = g_validationLayers.data();
        }
        else
        {
            deviceCreateInfo.enabledLayerCount = 0;
        }

        gtAssert(vkCreateDevice(s_physicalDevice, &deviceCreateInfo, nullptr, &device) == VK_SUCCESS);

        vkGetDeviceQueue(device, indices.graphicsFamily, 0, &s_graphicsQueue);

        return device;
    }

    void
        destroyDevice(const VkDevice &device)
    {
        vkDestroyDevice(device, nullptr);
    }
} // namespace gt::vk