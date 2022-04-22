// #include <vulkan/vulkan.hpp>
// #include <vulkan/vulkan_raii.hpp>
#include <vkbootstrap/VkBootstrap.h>

#include "drivers/display_glfw.hpp"
#include "drivers/video_vulkan.hpp"
// #include "drivers/vulkan/descriptors.hpp"
// #include "drivers/vulkan/initializers.hpp"
// #include "drivers/vulkan/memory.hpp"
// #include "drivers/vulkan/window.hpp"

#if defined(__linux__) || defined(__APPLE__)
extern "C" {
hydra::drivers::VideoVulkan* allocator()
{
    return new hydra::drivers::VideoVulkan();
}

void deleter(hydra::drivers::VideoVulkan* ptr)
{
    delete ptr;
}
}
#endif

#ifdef WIN32
extern "C" {
__declspec(dllexport) hydra::drivers::VideoVulkan* allocator()
{
    return new hydra::drivers::VideoVulkan();
}

__declspec(dllexport) void deleter(hydra::drivers::VideoVulkan* ptr)
{
    delete ptr;
}
}
#endif

namespace hydra {
    namespace drivers {
        bool VideoVulkan::init(WindowInterface& window, std::list<base::Material*>& materials)
        {
            m_basicData.context = std::make_unique<vk::raii::Context>();
            vkb::InstanceBuilder instanceBuilder{};
            auto instRet = instanceBuilder.set_app_name("Example Vulkan Application")
                               .request_validation_layers()
                               .use_default_debug_messenger()
                               .desire_api_version((uint32_t)VK_HEADER_VERSION << 22, (uint32_t)VK_HEADER_VERSION << 12, (uint32_t)VK_HEADER_VERSION)
                               .build();
            if (!instRet) return false;
            m_basicData.instance = std::make_unique<vk::raii::Instance>(*m_basicData.context, instRet.value().instance);
            m_basicData.debugMessenger = std::make_unique<vk::raii::DebugUtilsMessengerEXT>(*m_basicData.instance, instRet.value().debug_messenger);

            VkSurfaceKHR surface{};
            const char* description;
            if (glfwCreateWindowSurface(**m_basicData.instance, dynamic_cast<WindowGLFW&>(window).get(), nullptr, &surface) != VK_SUCCESS) {
                glfwGetError(&description);
                throw std::runtime_error(description);
            }

            vkb::PhysicalDeviceSelector selector{instRet.value()};
            auto physRet = selector.set_surface(surface)
                               .set_minimum_version(1, 1)
                               .require_dedicated_transfer_queue()
                               .select();
            if (!physRet) return false;
            m_basicData.physicalDevice = std::make_unique<vk::raii::PhysicalDevice>(*m_basicData.instance, physRet.value().physical_device);
            m_basicData.apiVersion = m_basicData.physicalDevice->getProperties().apiVersion;

            vkb::DeviceBuilder deviceBuilder{physRet.value()};

            auto devRet = deviceBuilder.build();
            if (!devRet) return false;
            m_basicData.device = std::make_unique<vk::raii::Device>(*m_basicData.physicalDevice, devRet.value().device);
            m_basicData.graphicsQueueIndex = devRet.value().get_queue_index(vkb::QueueType::graphics).value();
            auto graphics_queue_ret = devRet.value().get_queue(vkb::QueueType::graphics);
            if (!graphics_queue_ret) return false;
            m_basicData.graphicsQueue = std::make_unique<vk::raii::Queue>(*m_basicData.device, graphics_queue_ret.value());

            vk::PipelineCacheCreateInfo pipelineCacheInfo{};
            m_basicData.pipelineCache = std::make_unique<vk::raii::PipelineCache>(*m_basicData.device, pipelineCacheInfo);
            m_basicData.memoryAllocator = std::make_unique<vlkn::MemoryAllocator>(&m_basicData);
            m_basicData.descriptorAllocator = std::make_unique<vlkn::DescriptorAllocator>(&m_basicData);

            m_basicData.commandBufferAllocator = std::make_unique<vlkn::CommandBufferAllocator>(&m_basicData);
            m_basicData.shaderAllocator = std::make_unique<vlkn::ShaderManager>(&m_basicData);

            m_window = std::make_unique<vlkn::Window>(&m_basicData, surface, vk::Extent2D(window.width(), window.height()));

            for (auto& material : materials) {
                for (auto& stage : material->stages()) {
                    if (stage.descriptor.data().size() == 0) { throw std::logic_error("EMPTY SHADER DESCRIPTION"); }
                    auto descriptor = JSON::parse(stage.descriptor.data());
                    auto name = descriptor["name"];
                    m_basicData.shaderAllocator->createShaderModule(name.get<std::string>(), stage.code.copy<uint32_t>());
                }
                
            }

            return true;
        }
        void VideoVulkan::draw(std::list<base::ManagedObject*>& objects)
        {
            m_window->draw();
        }

    }  // namespace drivers
}  // namespace hydra