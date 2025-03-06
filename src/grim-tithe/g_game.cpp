#include "g_game.hpp"

#include "i_input.hpp"
#include "i_window.hpp"

#include "m_globals.hpp"

#include "r_commands.hpp"
#include "r_context.hpp"
#include "r_device.hpp"
#include "r_instance.hpp"
#include "r_pipeline.hpp"
#include "r_surface.hpp"
#include "r_swapChain.hpp"
#include "r_utilities.hpp"

namespace gt::game
{
    using namespace gt;

    static GLFWwindow*
        init(renderer::VulkanContext &context)
    {
        GLFWwindow *window = implementations::initializeWindow();

        implementations::setInputCallbacks(window);

        std::vector<const char *> extensions{};
        implementations::getVulkanExtensions(extensions, context);

        int frameBufferWidth;
        int frameBufferHeight;
        implementations::getFrameBufferSize(window, &frameBufferWidth, &frameBufferHeight);

        renderer::createInstance(context, extensions);
        renderer::createSurface(context, window);
        renderer::createDevice(context);
        renderer::createSwapChain(context, frameBufferWidth, frameBufferHeight);
        renderer::createImageViews(context);
        renderer::createRenderPass(context);
        renderer::createPipeline(context);
        renderer::createFramebuffers(context);
        renderer::createCommandPool(context);
        renderer::createCommandBuffer(context);
        renderer::createSyncObjects(context);

        return window;
    }

    static void
        shutDown(const renderer::VulkanContext& c_context, GLFWwindow* window)
    {
        renderer::waitForGpuOperations(c_context);

        renderer::destroySyncObjects(c_context);
        renderer::destroyCommandPool(c_context);
        renderer::destroyFramebuffers(c_context);
        renderer::destroyPipeline(c_context);
        renderer::destroyRenderPass(c_context);
        renderer::destroyImageViews(c_context);
        renderer::destroySwapChain(c_context);
        renderer::destroyDevice(c_context);
        renderer::destroySurface(c_context);
        renderer::destroyInstance(c_context);
        implementations::destroyWindow(window);
    }

    void
        run()
    {
        renderer::VulkanContext vkContext{};
        GLFWwindow   *window = init(vkContext);

        implementations::showWindow(window);

        // main loop
        uint32_t currentFrame = 0;
        while (misc::g_gameRunning)
        {
            implementations::pollEvents();

            if (misc::g_resize)
            {
                vkContext.oldSwapChain = vkContext.swapChain;

                int frameBufferWidth;
                int frameBufferHeight;
                implementations::getFrameBufferSize(window, &frameBufferWidth, &frameBufferHeight);

                while (frameBufferHeight == 0 || frameBufferWidth == 0)
                {
                    implementations::getFrameBufferSize(window, &frameBufferWidth, &frameBufferHeight);
                    implementations::wait();
                }

                renderer::waitForGpuOperations(vkContext);

                renderer::destroyFramebuffers(vkContext);
                renderer::destroyImageViews(vkContext);

                renderer::createSwapChain(vkContext, frameBufferWidth, frameBufferHeight);
                renderer::createImageViews(vkContext);
                renderer::createFramebuffers(vkContext);
            }

            renderer::render(vkContext, currentFrame, misc::g_resize);

            currentFrame = (currentFrame + 1) % vkContext.c_framesInFlight;

            implementations::isOpen(window);
            misc::g_resize = false;
        }

        shutDown(vkContext, window);
    }

} // namespace gt::game