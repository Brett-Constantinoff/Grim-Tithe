#include "gt-game.hpp"
#include "vk.hpp"
#include "window.hpp"

namespace gt::game
{
    using namespace gt::window;
    using namespace gt::vk;

    void
        run()
    {
        // initial setup
        gtWindow *c_window = initializeWindow();

        uint32_t     extensionCount = 0;
        const char **c_extensions = getVulkanExtensions(&extensionCount);

        VkInstance instance = createInstance(extensionCount, c_extensions);

        // main loop
        while (isOpen(c_window))
        {
            pollEvents();
        }

        // cleanup
        destroyInstance(instance);
        destroyWindow(c_window);
    }

} // namespace gt::game