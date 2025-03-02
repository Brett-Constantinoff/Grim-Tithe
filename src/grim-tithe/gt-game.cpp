#include "gt-game.hpp"
#include "vk-instance.hpp"
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

        std::vector<const char *> extensions{};
        getVulkanExtensions(extensions);

        VkInstance instance = createInstance(extensions);

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