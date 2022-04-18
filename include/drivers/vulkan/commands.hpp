#ifndef _HYDRA_NGINE_DRIVERS_VULKAN_COMMANDS_HPP
#define _HYDRA_NGINE_DRIVERS_VULKAN_COMMANDS_HPP
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_raii.hpp>

#include "commons.hpp"
#include "drivers/vulkan/structs.hpp"
namespace hydra {
    namespace drivers {
        namespace vlkn {
            class BasicData;
            class CommandBufferData {
            private:
                std::unique_ptr<vk::raii::CommandBuffer> m_commandBuffer;

            public:
                inline auto operator*() & -> const vk::CommandBuffer& { return **m_commandBuffer; };
                CommandBufferData(vk::raii::CommandBuffer&& commandBuffer) : m_commandBuffer{std::make_unique<vk::raii::CommandBuffer>(std::move(commandBuffer))} {};
            };

            struct CommandsData {
                std::vector<vk::raii::CommandPool> commandPools;
                std::vector<CommandBufferData> commandCache;
                // std::map<vk::DescriptorSet, DescriptorSetLayoutData*> descriptorSetsLayouts;
            };

            class CommandBufferAllocator {
            private:
                BasicData* m_basicDataPtr;
                CommandsData m_commandsData;

            public:
                auto allocateCommandBuffer(vk::CommandBufferUsageFlags usage, vk::CommandPoolCreateFlags flags) & -> vk::raii::CommandBuffer;
                inline CommandBufferAllocator(BasicData* basicDataPtr) : m_basicDataPtr(basicDataPtr) {};
                ~CommandBufferAllocator() {}
            };

        }  // namespace vlkn
    }      // namespace drivers
}  // namespace hydra

#endif  //_HYDRA_NGINE_DRIVERS_VULKAN_COMMANDS_HPP
