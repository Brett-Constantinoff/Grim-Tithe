#pragma once

#include <GLFW/glfw3.h>
#include <vector>

#include "r_context.hpp"

namespace gt::implementations
{
    using namespace gt;

    [[nodiscard]] GLFWwindow *initializeWindow();
    void                      destroyWindow(GLFWwindow *window);
    void                      showWindow(GLFWwindow *window);
    void                      isOpen(GLFWwindow *window);
    void                      pollEvents();
    void                      wait();
    void                      getVulkanExtensions(std::vector<const char *> &extensions, const renderer::VulkanContext& c_context);
    void                      getFrameBufferSize(GLFWwindow *window, int *width, int *height);
} // namespace gt::implmentations