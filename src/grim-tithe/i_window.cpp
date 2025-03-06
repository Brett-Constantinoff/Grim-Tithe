#include <GLFW/glfw3.h>
#include <cassert>
#include <vulkan/vulkan.h>

#include "m_globals.hpp"
#include "m_utilities.hpp"
#include "i_window.hpp"

using namespace gt::misc;

namespace gt::implementations
{
    GLFWwindow *
        initializeWindow()
    {
        gtAssert(glfwInit() != GLFW_FALSE);

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

        GLFWwindow *window = glfwCreateWindow(g_width, g_height, g_title, nullptr, nullptr);
        gtAssert(window != nullptr);

        return window;
    }

    void
        destroyWindow(GLFWwindow *window)
    {
        glfwDestroyWindow(window);
        glfwTerminate();
    }
    
    void
        showWindow(GLFWwindow* window)
    {
        glfwShowWindow(window);
    }

    void
        isOpen(GLFWwindow *window)
    {
        if (glfwWindowShouldClose(window))
        {
            g_gameRunning = false;
        }
    }

    void
        pollEvents()
    {
        glfwPollEvents();
    }

    void
        wait()
    {
        glfwWaitEvents();
    }

    void
        getVulkanExtensions(std::vector<const char *> &extensions, const VulkanContext& c_context)
    {
        extensions.clear();

        uint32_t     glfwExtensionCount = 0;
        const char **glfwExtensions     = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        extensions.assign(glfwExtensions, glfwExtensions + glfwExtensionCount);

        if (c_context.enableValidation)
        {
            extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }
    }

    void
        getFrameBufferSize(GLFWwindow* window, int* width, int* height)
    {
        glfwGetFramebufferSize(window, width, height);
    }
} // namespace gt::implementations