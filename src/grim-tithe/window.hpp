#pragma once

#include <GLFW/glfw3.h>

namespace gt::window
{
    typedef GLFWwindow gtWindow;

    [[nodiscard]] gtWindow    *initializeWindow();
    void                       destroyWindow(gtWindow *window);
    bool                       isOpen(gtWindow *window);
    void    pollEvents();
    [[nodiscard]] const char **getVulkanExtensions(uint32_t *count);
} // namespace gt::window