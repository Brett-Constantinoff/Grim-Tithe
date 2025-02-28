#include <vector>

#include "utilities.hpp"
#include "vk.hpp"

namespace gt::vk
{
    static const char *s_engineName = "No Engine";
    static const char *s_appName    = "Grim Tithe";

    static const std::vector<const char *> s_validationLayers = {"VK_LAYER_KHRONOS_validation"};

    #ifdef NODEBUG
    static const bool s_enableValidation = false;
    #else
    static const bool s_enableValidation = true;
    #endif

    VkInstance &
        createInstance(uint32_t extensionCount, const char **c_extensions)
    {
        // check for validation
        uint32_t layerCount{};
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> layers{};
        layers.resize(layerCount);

        vkEnumerateInstanceLayerProperties(&layerCount, layers.data());

        bool validationSupport = true;
        for (const auto &layer : s_validationLayers)
        {
            bool found = false;
            for (const auto& property : layers)
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
            }
        }

        gtAssert(s_enableValidation && validationSupport);

        VkInstance instance{};

        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = s_appName;
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName        = s_engineName;
        appInfo.engineVersion      = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion         = VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;
        createInfo.enabledExtensionCount = extensionCount;
        createInfo.ppEnabledExtensionNames = c_extensions;

        createInfo.enabledLayerCount = 0;

        gtAssert(vkCreateInstance(&createInfo, nullptr, &instance) == VK_SUCCESS);

        return instance;
    }

    void
        destroyInstance(VkInstance &instance)
    {
        vkDestroyInstance(instance, nullptr);
    }
}