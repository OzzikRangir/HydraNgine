#include "drivers/display_glfw.hpp"

namespace hydra {
    namespace drivers {
        void WindowGLFW::init()
        {
            glfwInit();
            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
            glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

            m_windowPtr = {
                glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr), {}};
        }

    }  // namespace drivers
}  // namespace hydra
