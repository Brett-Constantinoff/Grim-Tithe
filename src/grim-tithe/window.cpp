#include <cassert>
#include <GLFW/glfw3.h>

#include "utilities.hpp"
#include "window.hpp"

namespace gt::window
{
    static uint32_t s_width = 800;
    static uint32_t s_height = 600;
    static const char* s_title  = "Grim Tithe";

    gtWindow *
        initializeWindow()
    {
        gtAssert(glfwInit() != GLFW_FALSE);

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        return glfwCreateWindow(s_width, s_height, s_title, nullptr, nullptr);
    }

    void
        destroyWindow(gtWindow *window)
    {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    bool
        isOpen(gtWindow *window)
    {
        return !glfwWindowShouldClose(window);
    }

    void
        pollEvents()
    {
        glfwPollEvents();
    }

    const char **
        getVulkanExtensions(uint32_t *count)
    {
        return glfwGetRequiredInstanceExtensions(count);
    }
} // namespace gt::window