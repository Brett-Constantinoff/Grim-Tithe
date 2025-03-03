#include "gt-game.hpp"
#include "vk-context.hpp"
#include "vk-device.hpp"
#include "vk-instance.hpp"
#include "vk-surface.hpp"
#include "vk-swapChain.hpp"
#include "window.hpp"

namespace gt::game
{
    using namespace gt::window;
    using namespace gt::vk;

    void
        run()
    {
        // initial setup
        GLFWwindow *window = initializeWindow();

        VulkanContext vkContext{};

        std::vector<const char *> extensions{};
        getVulkanExtensions(extensions, vkContext);

        int framBufferWidth;
        int frameBufferHeight;
        getFrameBufferSize(window, &framBufferWidth, &frameBufferHeight);

        createInstance(vkContext, extensions);
        createSurface(vkContext, window);
        createDevice(vkContext);
        createSwapChain(vkContext, framBufferWidth, frameBufferHeight);

        // main loop
        while (isOpen(window))
        {
            pollEvents();
        }

        // cleanup
        destroySwapChain(vkContext);
        destroyDevice(vkContext);
        destroySurface(vkContext);
        destroyInstance(vkContext);
        destroyWindow(window);
    }

} // namespace gt::game