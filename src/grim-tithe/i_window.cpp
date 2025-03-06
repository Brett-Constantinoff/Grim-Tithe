#include <GLFW/glfw3.h>
#include <cassert>
#include <vulkan/vulkan.h>

#include "m_globals.hpp"
#include "m_utilities.hpp"
#include "i_window.hpp"

namespace gt::implementations
{
    using namespace gt;

    GLFWwindow *
        initializeWindow()
    {
        gtAssert(glfwInit() != GLFW_FALSE);

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

        GLFWwindow *window = glfwCreateWindow(misc::g_width, misc::g_height, misc::g_title, nullptr, nullptr);
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
            misc::g_gameRunning = false;
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
        getVulkanExtensions(std::vector<const char *> &extensions, const renderer::VulkanContext& c_context)
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