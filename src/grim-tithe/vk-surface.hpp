#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace gt::vk
{
    [[nodiscard]] VkSurfaceKHR createSurface(const VkInstance &c_instance, GLFWwindow* window);
    void                       destroySurface(const VkInstance &c_instance, const VkSurfaceKHR &c_surface);
}