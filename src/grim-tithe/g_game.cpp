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
    using namespace gt::implementations;
    using namespace gt::renderer;
    using namespace gt::misc;

    static GLFWwindow*
        init(VulkanContext& context)
    {
        GLFWwindow *window = initializeWindow();

        setInputCallbacks(window);

        std::vector<const char *> extensions{};
        getVulkanExtensions(extensions, context);

        int frameBufferWidth;
        int frameBufferHeight;
        getFrameBufferSize(window, &frameBufferWidth, &frameBufferHeight);

        createInstance(context, extensions);
        createSurface(context, window);
        createDevice(context);
        createSwapChain(context, frameBufferWidth, frameBufferHeight);
        createImageViews(context);
        createRenderPass(context);
        createPipeline(context);
        createFramebuffers(context);
        createCommandPool(context);
        createCommandBuffer(context);
        createSyncObjects(context);

        return window;
    }

    static void
        shutDown(const VulkanContext& c_context, GLFWwindow* window)
    {
        waitForGpuOperations(c_context);

        destroySyncObjects(c_context);
        destroyCommandPool(c_context);
        destroyFramebuffers(c_context);
        destroyPipeline(c_context);
        destroyRenderPass(c_context);
        destroyImageViews(c_context);
        destroySwapChain(c_context);
        destroyDevice(c_context);
        destroySurface(c_context);
        destroyInstance(c_context);
        destroyWindow(window);
    }

    void
        run()
    {
        VulkanContext vkContext{};
        GLFWwindow   *window = init(vkContext);

        showWindow(window);

        // main loop
        while (g_gameRunning)
        {
            pollEvents();
            render(vkContext);
            isOpen(window);
        }

        shutDown(vkContext, window);
    }

} // namespace gt::game