#ifndef _HYDRA_NGINE_DRIVERS_VOOKOO_HPP
#define _HYDRA_NGINE_DRIVERS_VOOKOO_HPP

// #include <vku/vku.hpp>
// #include <vku/vku_framework.hpp>
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_raii.hpp>

#include "commons.hpp"
#include "core/display.hpp"
#include "core/managed_object.hpp"
#include "core/managed_property.hpp"
#include "core/video.hpp"
#include "vulkan/descriptors.hpp"
#include "vulkan/structs.hpp"
#include "vulkan/window.hpp"
#include "vulkan/memory.hpp"
namespace hydra {
    namespace drivers {
        class VideoVookoo : public virtual video::VideoInterface {
        private:
            // std::unique_ptr<vk::raii::Context> m_context;
            // std::unique_ptr<vk::raii::Instance> m_instance;
            // std::unique_ptr<vk::raii::Device> m_device;
            // std::unique_ptr<vk::raii::PhysicalDevice> m_physicalDevice;

            vlkn::BasicData m_basicData;
            std::unique_ptr<vlkn::Window> m_window;

            std::unique_ptr<vk::raii::PipelineCache> m_pipelineCache;
            std::unique_ptr<vk::raii::DescriptorPool> m_descriptorPool;

            std::unique_ptr<vk::raii::DescriptorSetLayout> m_descriptorSetLayout;
            std::vector<vk::raii::DescriptorSet> m_descriptorSets;

            std::unique_ptr<vk::raii::PipelineLayout> m_pipelineLayout;
            std::vector<vk::raii::Pipeline> m_pipelines;

            std::vector<vk::raii::ShaderModule> m_shaderModules;

            vk::PhysicalDeviceMemoryProperties m_memprops;
            // std::unique_ptr<vku::DebugCallback> m_callback;

            // std::unique_ptr<vku::Window> m_window;
            // std::list<vku::HostVertexBuffer> m_hostVertices;

            uint32_t m_graphicsQueueFamilyIndex;
            uint32_t m_computeQueueFamilyIndex;
            bool m_ok = false;

            void initInstance();
            void initPhysicalDevice();
            void initDevice();
            void initWindow(WindowInterface& window);
            void initDescriptorPool();
            void initPipelines(uint32_t width, uint32_t height, std::list<base::Material*>& materials);

        public:
            VideoVookoo(void* prefs = nullptr){};
            ~VideoVookoo() override { m_basicData.device->waitIdle(); };
            bool init(WindowInterface& window, std::list<base::Material*>& materials);
            void draw(std::list<base::ManagedObject*>& objects);
        };
    }  // namespace drivers
}  // namespace hydra

#endif  //_HYDRA_NGINE_DRIVERS_VOOKOO_HPP
