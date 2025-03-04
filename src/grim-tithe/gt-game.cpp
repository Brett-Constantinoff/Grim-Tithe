#include "gt-game.hpp"
#include "vk-context.hpp"
#include "vk-device.hpp"
#include "vk-instance.hpp"
#include "vk-pipeline.hpp"
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
        GLFWwindow   *window = initializeWindow();
        VulkanContext vkContext{};

        // initial setup
        {
            std::vector<const char *> extensions{};
            getVulkanExtensions(extensions, vkContext);

            int frameBufferWidth;
            int frameBufferHeight;
            getFrameBufferSize(window, &frameBufferWidth, &frameBufferHeight);

            createInstance(vkContext, extensions);
            createSurface(vkContext, window);
            createDevice(vkContext);
            createSwapChain(vkContext, frameBufferWidth, frameBufferHeight);
            createImageViews(vkContext);
            createRenderPass(vkContext);
            createPipeline(vkContext);
        }

        // main loop
        while (isOpen(window))
        {
            pollEvents();
        }

        // cleanup
        {
            waitForGpuOperations(vkContext);

            destroyPipeline(vkContext);
            destroyRenderPass(vkContext);
            destroyImageViews(vkContext);
            destroySwapChain(vkContext);
            destroyDevice(vkContext);
            destroySurface(vkContext);
            destroyInstance(vkContext);
            destroyWindow(window);
        }
    }

} // namespace gt::game