#include <GLFW/glfw3.h>
#include <cassert>
#include <vulkan/vulkan.h>

#include "globals.hpp"
#include "utilities.hpp"
#include "window.hpp"

using namespace gt::globals;

namespace gt::window
{
    GLFWwindow *
        initializeWindow()
    {
        gtAssert(glfwInit() != GLFW_FALSE);

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        return glfwCreateWindow(g_width, g_height, g_title, nullptr, nullptr);
    }

    void
        destroyWindow(GLFWwindow *window)
    {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    bool
        isOpen(GLFWwindow *window)
    {
        return !glfwWindowShouldClose(window);
    }

    void
        pollEvents()
    {
        glfwPollEvents();
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
} // namespace gt::window