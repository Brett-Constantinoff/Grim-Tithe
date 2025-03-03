#include "utilities.hpp"
#include "vk-surface.hpp"

namespace gt::vk
{
    void createSurface(VulkanContext& context, GLFWwindow *window)
    {
        gtAssert(glfwCreateWindowSurface(context.instance, window, nullptr, &context.surface) == VK_SUCCESS);
    }

    void
        destroySurface(const VulkanContext& c_context)
    {
        vkDestroySurfaceKHR(c_context.instance, c_context.surface, nullptr);
    }
}