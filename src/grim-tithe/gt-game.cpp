#include "gt-game.hpp"
#include "vk-device.hpp"
#include "vk-instance.hpp"
#include "vk-surface.hpp"
#include "window.hpp"

namespace gt::game
{
    using namespace gt::window;
    using namespace gt::vk;

    void
        run()
    {
        // initial setup
        GLFWwindow *c_window = initializeWindow();

        std::vector<const char *> extensions{};
        getVulkanExtensions(extensions);

        VkInstance instance = createInstance(extensions);
        VkSurfaceKHR surface  = createSurface(instance, c_window);
        VkDevice   device   = getDevice(instance, surface);

        // main loop
        while (isOpen(c_window))
        {
            pollEvents();
        }

        // cleanup
        destroyDevice(device);
        destroySurface(instance, surface);
        destroyInstance(instance);
        destroyWindow(c_window);
    }

} // namespace gt::game