#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "vk-context.hpp"

namespace gt::vk
{
    void createSurface(VulkanContext& context, GLFWwindow* window);
    void destroySurface(const VulkanContext& c_context);
}