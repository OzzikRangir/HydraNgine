#ifndef _HYDRA_NGINE_DRIVERS_VULKAN_STRUCTS_HPP
#define _HYDRA_NGINE_DRIVERS_VULKAN_STRUCTS_HPP

#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_raii.hpp>

#include "commons.hpp"
// #include "vulkan/commands.hpp"
// #include "vulkan/descriptors.hpp"
// #include "vulkan/memory.hpp"
namespace hydra {
    namespace drivers {
        namespace vlkn {
            class MemoryAllocator;
            class DescriptorAllocator;
            class CommandBufferAllocator;
            class AllocatedImage;
            class CommandBufferData;
            struct BasicData {
                std::unique_ptr<vk::raii::Context> context;
                std::unique_ptr<vk::raii::Instance> instance;
                std::unique_ptr<vk::raii::Device> device;
                std::unique_ptr<vk::raii::PhysicalDevice> physicalDevice;
                std::unique_ptr<vk::raii::DebugUtilsMessengerEXT> debugMessenger;
                std::unique_ptr<vk::raii::Queue> graphicsQueue;

                std::unique_ptr<DescriptorAllocator> descriptorAllocator;
                std::unique_ptr<CommandBufferAllocator> commandBufferAllocator;
                std::unique_ptr<MemoryAllocator> memoryAllocator;
                // std::unique_ptr<MaterialManager> memoryAllocator;
                uint32_t apiVersion;
                uint32_t graphicsQueueIndex;
                uint32_t bufferedFrames = {2};
            };

            struct WindowData {
                std::unique_ptr<vk::raii::SurfaceKHR> surface;
                std::unique_ptr<vk::raii::SwapchainKHR> swapchain;

                std::unique_ptr<AllocatedImage> rawImage;
                std::unique_ptr<AllocatedImage> depthStencil;

                vk::Extent2D extent;
                vk::Format swapchainImageFormat;
                std::vector<vk::Image> swapchainImages;
                std::vector<vk::raii::ImageView> swapchainImageViews;
                std::vector<vk::raii::Framebuffer> framebuffers;
            };

            struct RenderFrame {
                vk::raii::Framebuffer* framebuffer;
                vk::raii::Fence* commandBufferFence;
                vk::raii::Fence* dynamicCommandBufferFence;
                vk::raii::ImageView* imageView;
                vk::raii::CommandBuffer* commandBuffer;
            };

            struct FrameData {
                vk::raii::CommandBuffer commandBuffer;
                vk::raii::Fence fence;
                vk::raii::Semaphore presentSemaphore;
                vk::raii::Semaphore renderSemaphore;
            };

            struct RenderData {
                size_t currentFrame = {0};

                std::unique_ptr<vk::raii::RenderPass> renderPass;
                std::vector<FrameData> framesData;
                std::unique_ptr<vk::raii::Fence> contextFence;
                std::unique_ptr<vk::raii::CommandBuffer> contextCommandBuffer;

                // std::vector<vk::raii::Framebuffer> framebuffers;

                // std::unique_ptr<vk::raii::Semaphore> commandSemaphore;
                // std::unique_ptr<vk::raii::Semaphore> dynamicSemaphore;
                // std::unique_ptr<vk::raii::Semaphore> imageSemaphore;

                // std::vector<vk::raii::Fence> commandBufferFences;
                // std::vector<vk::raii::Fence> dynamicCommandBufferFences;

                // std::unique_ptr<vk::raii::CommandPool> commandPool;
                // std::vector<vk::raii::CommandBuffer> staticDrawBuffers;
                // std::vector<vk::raii::CommandBuffer> dynamicDrawBuffers;

                // std::vector<vk::raii::Fence> frameFences;
                //

                // std::vector<vk::raii::CommandBuffer> frameCommandBuffers;

                // std::vector<RenderFrame> renderFrames;
            };
        }  // namespace vlkn
    }      // namespace drivers
}  // namespace hydra

#endif  //_HYDRA_NGINE_DRIVERS_VULKAN_STRUCTS_HPP
