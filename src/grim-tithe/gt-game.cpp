#include "gt-game.hpp"
#include "vk-context.hpp"
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

        VulkanContext vkContext{};

        std::vector<const char *> extensions{};
        getVulkanExtensions(extensions, vkContext);

        createInstance(vkContext, extensions);
        createSurface(vkContext, c_window);
        createDevice(vkContext);

        // main loop
        while (isOpen(c_window))
        {
            pollEvents();
        }

        // cleanup
        destroyDevice(vkContext);
        destroySurface(vkContext);
        destroyInstance(vkContext);
        destroyWindow(c_window);
    }

} // namespace gt::game