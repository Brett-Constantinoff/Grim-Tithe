#pragma once

#include <GLFW/glfw3.h>
#include <vector>

namespace gt::window
{
    [[nodiscard]] GLFWwindow *initializeWindow();
    void                      destroyWindow(GLFWwindow *window);
    bool                      isOpen(GLFWwindow *window);
    void                    pollEvents();
    void                    getVulkanExtensions(std::vector<const char *> &extensions);
} // namespace gt::window