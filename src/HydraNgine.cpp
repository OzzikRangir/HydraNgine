#include <csignal>

#include "commons.hpp"
#include "core/context.hpp"
#define GLFW_INCLUDE_VULKAN
#include "drivers/display_glfw.hpp"
#include "drivers/video_vulkan.hpp"

volatile std::sig_atomic_t gSignalStatus;

hydra::base::Context context;

void signalHandler(int signum)
{
    gSignalStatus = signum;
}

int main(int argc, char const* argv[])
{
    auto window = hydra::drivers::WindowGLFW("Hydra", 1024, 768);

    hydra::drivers::VideoVulkan display{};
    window.init();
    std::list<hydra::base::Material*> materials;
    std::list<hydra::base::ManagedObject*> entities;

    display.init(window, materials);
    while (gSignalStatus == 0) {
        display.draw(entities);
    }
}