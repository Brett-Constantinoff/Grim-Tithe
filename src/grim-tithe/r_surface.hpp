#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "r_context.hpp"

namespace gt::renderer
{
    void createSurface(VulkanContext& context, GLFWwindow* window);
    void destroySurface(const VulkanContext& c_context);
} // namespace gt::renderer