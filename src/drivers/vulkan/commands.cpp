#include "drivers/vulkan/commands.hpp"

#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_raii.hpp>

namespace hydra {
    namespace drivers {
        namespace vlkn {
            auto CommandBufferAllocator::allocateCommandBuffer(vk::CommandBufferUsageFlags usage, vk::CommandPoolCreateFlags flags) & -> vk::raii::CommandBuffer
            {

                vk::CommandPoolCreateInfo cpCI{};
                cpCI.flags = flags;

                cpCI.queueFamilyIndex = m_basicDataPtr->graphicsQueueIndex;
                m_commandsData.commandPools.push_back(m_basicDataPtr->device->createCommandPool(cpCI));


                vk::CommandBufferAllocateInfo cbAI{};
                cbAI.commandPool = *m_commandsData.commandPools.back();
                cbAI.commandBufferCount = 1;
                cbAI.level = vk::CommandBufferLevel::ePrimary;

                auto cbs = m_basicDataPtr->device->allocateCommandBuffers(cbAI);
                if (cbs.size() == 0) { throw std::runtime_error("CommandBufferAllocateError"); };


                cbs.back();
                // auto newCommandBuffer = ;
                // newCommandBuffer.begin(cbBI);
                return {std::move(cbs.back())};
            }
        }  // namespace vlkn
    }      // namespace drivers
}  // namespace hydra