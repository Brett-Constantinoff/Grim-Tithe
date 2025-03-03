#include "utilities.hpp"
#include "vk-surface.hpp"

namespace gt::vk
{
    [[nodiscard]] VkSurfaceKHR
        createSurface(const VkInstance &c_instance, GLFWwindow *window)
    {
        VkSurfaceKHR surface = VK_NULL_HANDLE;

        gtAssert(glfwCreateWindowSurface(c_instance, window, nullptr, &surface) == VK_SUCCESS);

        return surface;
    }

    void
        destroySurface(const VkInstance& c_instance, const VkSurfaceKHR& c_surface)
    {
        vkDestroySurfaceKHR(c_instance, c_surface, nullptr);
    }
}