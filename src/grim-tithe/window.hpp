#pragma once

#include <GLFW/glfw3.h>
#include <vector>

#include "vk-context.hpp"

namespace gt::window
{
    using namespace gt::vk;

    [[nodiscard]] GLFWwindow *initializeWindow();
    void                      destroyWindow(GLFWwindow *window);
    bool                      isOpen(GLFWwindow *window);
    void                    pollEvents();
    void                    getVulkanExtensions(std::vector<const char *> &extensions, const VulkanContext& c_context);
} // namespace gt::window