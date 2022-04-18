#ifndef _HYDRA_NGINE_DRIVERS_GLFW_HPP
#define _HYDRA_NGINE_DRIVERS_GLFW_HPP

#include "commons.hpp"
#include "core/display.hpp"
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

namespace hydra {
    namespace drivers {
        struct GLFWwindow_destroyer {
            void operator()(GLFWwindow* w) const { glfwDestroyWindow(w); };
        };
        class WindowGLFW : public virtual WindowInterface {
        private:
            std::unique_ptr<GLFWwindow, GLFWwindow_destroyer> m_windowPtr;

        public:
            WindowGLFW(std::string title, uint32_t width, uint32_t height) : WindowInterface(title, width, height){};
            void init() override;
            inline GLFWwindow* get() { return m_windowPtr.get(); };
            ~WindowGLFW() override { m_windowPtr.reset(); glfwTerminate();};
        };
    }  // namespace drivers
}  // namespace hydra

#endif  //_HYDRA_NGINE_DRIVERS_GLFW_HPP