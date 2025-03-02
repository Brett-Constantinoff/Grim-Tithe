#pragma once

#include <GLFW/glfw3.h>
#include <vector>

namespace gt::window
{
    typedef GLFWwindow gtWindow;

    [[nodiscard]] gtWindow *initializeWindow();
    void                    destroyWindow(gtWindow *window);
    bool                    isOpen(gtWindow *window);
    void                    pollEvents();
    void                    getVulkanExtensions(std::vector<const char *> &extensions);
} // namespace gt::window