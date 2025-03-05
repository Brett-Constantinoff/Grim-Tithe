#include "m_globals.hpp"
#include "i_input.hpp"

namespace gt::implementations
{
    using namespace gt::misc;

    static void 
        keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        {
            g_gameRunning = false;
        }
    }

    void
        setInputCallbacks(GLFWwindow *window)
    {
        glfwSetKeyCallback(window, keyCallback);
    }
}