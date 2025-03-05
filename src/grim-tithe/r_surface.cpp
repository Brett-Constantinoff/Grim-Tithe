#include "m_utilities.hpp"
#include "r_surface.hpp"

namespace gt::renderer
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
} // namespace gt::renderer