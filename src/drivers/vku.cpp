#define VKU_SURFACE "VK_KHR_xcb_surface"
#define VK_USE_PLATFORM_XCB_KHR
#define GLFW_INCLUDE_VULKAN
#include "drivers/vku.hpp"

#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_raii.hpp>

#include "core/entity.hpp"
#include "core/entity_traits.hpp"
#include "drivers/glfw.hpp"
#include "drivers/vulkan/window.hpp"
#include "vkbootstrap/VkBootstrap.h"
namespace hydra {
    namespace drivers {
        void VideoVookoo::initInstance()
        {
            // vkb::InstanceBuilder instanceBuilder{};
            // auto instRet = instanceBuilder.set_app_name("Example Vulkan Application")
            //                    .request_validation_layers()
            //                    .use_default_debug_messenger()
            //                    .build();
            // if (!instRet) {
            //     // std::cerr << "Failed to create Vulkan instance. Error: " << instRet.error().message << "\n";
            //     return;
            // }
            // m_basicData.instance = std::make_unique<vk::raii::Instance>(*m_basicData.context, instRet.value().instance);
        }

        void VideoVookoo::initWindow(WindowInterface& window)
        {
            // VkSurfaceKHR surface{};
            // const char* description;
            // //     m_fw = std::make_unique<vku::Framework>(window.title(), 0);
            // // VkSurfaceKHR surface{};
            // // const char* description;

            // if (glfwCreateWindowSurface(**m_basicInfo.instance, dynamic_cast<WindowGLFW&>(window).get(), nullptr, &surface) != VK_SUCCESS) {
            //     glfwGetError(&description);
            //     throw std::runtime_error(description);
            // }
            // m_surface = std::make_unique<vk::raii::SurfaceKHR>(*m_instance, surface);
            // m_window = std::make_unique<vku::Window>(**m_basicInfo.instance, **m_basicInfo.device, **m_basicInfo.physicalDevice, m_graphicsQueueFamilyIndex, surface);
        }

        void VideoVookoo::initPhysicalDevice()
        {
            // auto pds = m_instance->enumeratePhysicalDevices();

            // m_physicalDevice = std::make_unique<vk::raii::PhysicalDevice>(std::move(pds[0]));

            // auto qprops = m_physicalDevice->getQueueFamilyProperties();
            // const auto badQueue = ~(uint32_t)0;
            // m_graphicsQueueFamilyIndex = badQueue;
            // m_computeQueueFamilyIndex = badQueue;
            // vk::QueueFlags search = vk::QueueFlagBits::eGraphics | vk::QueueFlagBits::eCompute;
            // for (uint32_t qi = 0; qi != qprops.size(); ++qi) {
            //     auto& qprop = qprops[qi];
            //     std::cout << vk::to_string(qprop.queueFlags) << "\n";
            //     if ((qprop.queueFlags & search) == search) {
            //         m_graphicsQueueFamilyIndex = qi;
            //         m_computeQueueFamilyIndex = qi;
            //         break;
            //     }
            // }

            // if (m_graphicsQueueFamilyIndex == badQueue || m_computeQueueFamilyIndex == badQueue) {
            //     std::cout << "oops, missing a queue\n";
            //     return;
            // }

            // m_memprops = m_physicalDevice->getMemoryProperties();
        }

        void VideoVookoo::initDevice()
        {
            // vku::DeviceMaker dm{};
            // dm.defaultLayers();
            // dm.queue(m_graphicsQueueFamilyIndex);
            // if (m_computeQueueFamilyIndex != m_graphicsQueueFamilyIndex) dm.queue(m_computeQueueFamilyIndex);
            // m_device = std::make_unique<vk::raii::Device>(*m_physicalDevice, dm.createInfo());

            // vk::PipelineCacheCreateInfo pipelineCacheInfo{};
            // m_pipelineCache = std::make_unique<vk::raii::PipelineCache>(*m_device, pipelineCacheInfo);
        }

        void VideoVookoo::initDescriptorPool()
        {
            // std::vector<vk::DescriptorPoolSize> poolSizes;
            // poolSizes.emplace_back(vk::DescriptorType::eUniformBuffer, 128);
            // poolSizes.emplace_back(vk::DescriptorType::eCombinedImageSampler, 128);
            // poolSizes.emplace_back(vk::DescriptorType::eStorageBuffer, 128);

            // // Create an arbitrary number of descriptors in a pool.
            // // Allow the descriptors to be freed, possibly not optimal behaviour.
            // vk::DescriptorPoolCreateInfo descriptorPoolInfo{};
            // descriptorPoolInfo.flags = vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet;
            // descriptorPoolInfo.maxSets = 256;
            // descriptorPoolInfo.poolSizeCount = (uint32_t)poolSizes.size();
            // descriptorPoolInfo.pPoolSizes = poolSizes.data();
            // m_descriptorPool = std::make_unique<vk::raii::DescriptorPool>(*m_device, descriptorPoolInfo);
        }

        void VideoVookoo::initPipelines(uint32_t width, uint32_t height, std::list<base::Material*>& materials)
        {
            // vku::PipelineLayoutMaker plm{};
            // m_pipelineLayout = std::make_unique<vk::raii::PipelineLayout>(*m_basicInfo.device, plm.createInfo());
            // for (auto& material : materials) {
            //     vku::PipelineMaker pm{width, height};
            //     for (auto& shader : material->shaders()) {
            //         auto shaderInfo = JSON::parse(shader.descriptor.data());
            //         vk::ShaderModuleCreateInfo shaderCreateInfo{};
            //         shaderCreateInfo.pCode = reinterpret_cast<const uint32_t*>(shader.data.data().data());
            //         shaderCreateInfo.codeSize = shader.data.data().size();
            //         m_shaderModules.emplace_back(*m_device, shaderCreateInfo);
            //         if (shaderInfo["type"] == std::string("fragment")) {
            //             pm.shader(vk::ShaderStageFlagBits::eFragment, *m_shaderModules.back());
            //             continue;
            //         }
            //         if (shaderInfo["type"] == std::string("vertex")) {
            //             pm.shader(vk::ShaderStageFlagBits::eVertex, *m_shaderModules.back());
            //             size_t bindingIndex = 0;
            //             for (auto& binding : shaderInfo["bindings"]) {
            //                 size_t bindingSize = 0;
            //                 size_t vertexOffset = 0;
            //                 size_t vertexIndex = 0;
            //                 for (auto& vertex : binding["vertices"]) {
            //                     size_t currentSize = vertex["size"];
            //                     size_t location = vertex["location"];
            //                     std::cout << vertexIndex << " " << bindingIndex << " " << vertexOffset << std::endl;
            //                     if (currentSize == 4)
            //                         pm.vertexAttribute(location, bindingIndex, vk::Format::eR32Sfloat, vertexOffset);
            //                     if (currentSize == 8)
            //                         pm.vertexAttribute(location, bindingIndex, vk::Format::eR32G32Sfloat, vertexOffset);
            //                     if (currentSize == 12)
            //                         pm.vertexAttribute(location, bindingIndex, vk::Format::eR32G32B32Sfloat, vertexOffset);
            //                     bindingSize += currentSize;
            //                     vertexOffset += currentSize;
            //                     vertexIndex++;
            //                 }
            //                 if (binding["input_rate"] == std::string("VERTEX")) {
            //                     pm.vertexBinding(bindingIndex, bindingSize, vk::VertexInputRate::eVertex);
            //                 }
            //                 if (binding["input_rate"] == std::string("INSTANCE")) {
            //                     pm.vertexBinding(bindingIndex, bindingSize, vk::VertexInputRate::eInstance);
            //                 }
            //                 bindingIndex++;
            //             }
            //         }
            //     }
            //     m_pipelines.emplace_back(*m_device, pm.createUnique(**m_device, **m_pipelineCache, **m_pipelineLayout, m_window->renderPass()).release());
            // }
        }

        bool VideoVookoo::init(WindowInterface& window, std::list<base::Material*>& materials)
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
                // surface.instance
                glfwGetError(&description);
                throw std::runtime_error(description);
            }

            vkb::PhysicalDeviceSelector selector{instRet.value()};
            auto physRet = selector.set_surface(surface)
                               .set_minimum_version(1, 1)  // require a vulkan 1.1 capable device
                               .require_dedicated_transfer_queue()
                               .select();
            if (!physRet) return false;
            m_basicData.physicalDevice = std::make_unique<vk::raii::PhysicalDevice>(*m_basicData.instance, physRet.value().physical_device);
            m_basicData.apiVersion = m_basicData.physicalDevice->getProperties().apiVersion;
            vkb::DeviceBuilder deviceBuilder{physRet.value()};
            // automatically propagate needed data from instance & physical device
            auto devRet = deviceBuilder.build();
            if (!devRet) return false;
            m_basicData.device = std::make_unique<vk::raii::Device>(*m_basicData.physicalDevice, devRet.value().device);

            auto graphics_queue_ret = devRet.value().get_queue(vkb::QueueType::graphics);
            if (!graphics_queue_ret) return false;
            m_basicData.graphicsQueue = std::make_unique<vk::raii::Queue>(*m_basicData.device, graphics_queue_ret.value());

            // VkQueue graphics_queue = graphics_queue_ret.value();

            // std::cout << m_basicData.apiVersion << std::endl;
            m_basicData.memoryAllocator = std::make_unique<vlkn::MemoryAllocator>(&m_basicData);
            std::cout << m_basicData.instance->getDispatcher()->getVkHeaderVersion() << std::endl;
            m_window = std::make_unique<vlkn::Window>(&m_basicData, surface, vk::Extent2D(window.width(), window.height()));

            vk::PipelineCacheCreateInfo pipelineCacheInfo{};
            m_pipelineCache = std::make_unique<vk::raii::PipelineCache>(*m_basicData.device, pipelineCacheInfo);

            // // Get the graphics queue with a helper function
            // auto graphics_queue_ret = vkb_device.get_queue(vkb::QueueType::graphics);
            // if (!graphics_queue_ret) {
            //     std::cerr << "Failed to get graphics queue. Error: " << graphics_queue_ret.error().message() << "\n";
            //     return false;
            // }
            // VkQueue graphics_queue = graphics_queue_ret.value();

            // m_device = std::make_unique<vk::raii::Device>(*m_physicalDevice, dm.createInfo());

            // vk::PipelineCacheCreateInfo pipelineCacheInfo{};
            // m_pipelineCache = std::make_unique<vk::raii::PipelineCache>(*m_device, pipelineCacheInfo);

            // initInstance();
            // initPhysicalDevice();
            // initDevice();
            // initWindow(window);
            // initDescriptorPool();
            // initPipelines(window.width(), window.height(), materials);
            // Look for an omnipurpose queue family first
            // It is better if we can schedule operations without barriers and semaphores.
            // The Spec says: "If an implementation exposes any queue family that supports graphics operations,
            // at least one queue family of at least one physical device exposed by the implementation
            // must support both graphics and compute operations."
            // Also: All commands that are allowed on a queue that supports transfer operations are
            // also allowed on a queue that supports either graphics or compute operations...
            // As a result we can expect a queue family with at least all three and maybe all four modes.

            // todo: find optimal texture format
            // auto rgbaprops = physical_device_.getFormatProperties(vk::Format::eR8G8B8A8Unorm);

            // m_window = std::make_unique<vku::Window>(m_fw->instance(), m_fw->device(), m_fw->physicalDevice(), m_fw->graphicsQueueFamilyIndex(), dynamic_cast<WindowGLFW&>(window).get());
            return true;
        }

        struct Instance {
            glm::vec3 pos;
            glm::vec3 rot;
            float scale;
        };
        void VideoVookoo::draw(std::list<base::ManagedObject*>& objects)
        {
            // m_hostVertices.clear();
            // std::vector<Instance> instances;
            // m_window->draw(
            //     **m_device, *m_device->getQueue(m_graphicsQueueFamilyIndex, 0),
            //     [&](vk::CommandBuffer cb, int imageIndex, vk::RenderPassBeginInfo& rpbi) {
            //         vk::CommandBufferBeginInfo bi{};
            //         cb.begin(bi);
            //         cb.beginRenderPass(rpbi, vk::SubpassContents::eInline);
            //         cb.bindPipeline(vk::PipelineBindPoint::eGraphics, *m_pipelines.back());
            //         for (auto& object : objects) {
            //             Instance instance;
            //             if (object->type() == base::ManagedObject::Type::Entity) {
            //                 auto* transform = object->as<base::Entity>()->trait<base::EntityTraitTransform>();
            //                 if (transform) {
            //                     for (auto& prop : transform->properties()) {
            //                         if (prop->name() == "Position")
            //                             instance.pos = prop->get<glm::vec3>();
            //                         if (prop->name() == "Rotation")
            //                             instance.rot = prop->get<glm::vec3>();
            //                         if (prop->name() == "Scale")
            //                             instance.scale = prop->get<float>();
            //                     }
            //                     instances.push_back(instance);
            //                     // for (auto& prop : transform->properties())
            //                 }
            //                 auto mesh = object->as<base::Entity>()->trait<base::EntityTraitMesh>();
            //                 if (mesh) {
            //                     m_hostVertices.push_back({**m_device, m_memprops, mesh->resource().data()});
            //                     cb.bindVertexBuffers(0, m_hostVertices.back().buffer(), vk::DeviceSize(0));
            //                 }
            //             }
            //         }
            //         m_hostVertices.push_back({**m_device, m_memprops, instances});
            //         cb.bindVertexBuffers(1, m_hostVertices.back().buffer(), vk::DeviceSize(0));
            //         // cb.bindVertexBuffers(0, bufferVertices.buffer(), vk::DeviceSize(0));                         // Binding point VERTEX_BUFFER_BIND_ID : Mesh vertex buffer
            //         // cb.bindVertexBuffers(1, bufferInstances.buffer(), vk::DeviceSize(0));  // Binding point INSTANCE_BUFFER_BIND_ID : Instance data buffer
            //         cb.draw(4, instances.size(), 0, 0);
            //         cb.endRenderPass();
            //         cb.end();
            //     });
            // std::this_thread::sleep_for(std::chrono::seconds(160));
        }
    }  // namespace drivers
}  // namespace hydra
