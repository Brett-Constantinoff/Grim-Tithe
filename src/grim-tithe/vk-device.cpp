#include <set>
#include <vector>

#include "globals.hpp"
#include "utilities.hpp"
#include "vk-device.hpp"

namespace gt::vk
{
    using namespace gt::globals;

    static VkPhysicalDevice s_physicalDevice = VK_NULL_HANDLE;
    static VkQueue          s_graphicsQueue  = VK_NULL_HANDLE;
    static VkQueue          s_presentQueue   = VK_NULL_HANDLE;

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
        isGpuGood(const VkPhysicalDevice& c_device, const VkSurfaceKHR& c_surface)
    {
        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(c_device, &properties);

        return getQueueFamilies(c_device, c_surface).isComplete();
    }

    static void
        pickGpu(const VkInstance &c_instance, const VkSurfaceKHR &c_surface)
    {
        uint32_t gpuCount = 0;
        vkEnumeratePhysicalDevices(c_instance, &gpuCount, nullptr);

        gtAssert(gpuCount != 0);

        std::vector<VkPhysicalDevice> gpus{};
        gpus.resize(gpuCount);

        vkEnumeratePhysicalDevices(c_instance, &gpuCount, gpus.data());

        for (const auto &gpu : gpus)
        {
            if (isGpuGood(gpu, c_surface))
            {
                s_physicalDevice = gpu;
            }
        }

        gtAssert(s_physicalDevice != VK_NULL_HANDLE);
    }

    VkDevice
        getDevice(const VkInstance &c_instance, const VkSurfaceKHR &c_surface)
    {
        VkDevice device = VK_NULL_HANDLE;

        pickGpu(c_instance, c_surface);

        QueueFamilyIndices indices = getQueueFamilies(s_physicalDevice, c_surface);

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
        vkGetDeviceQueue(device, indices.presentFamily, 0, &s_presentQueue);

        return device;
    }

    void
        destroyDevice(const VkDevice &c_device)
    {
        vkDestroyDevice(c_device, nullptr);
    }
} // namespace gt::vk