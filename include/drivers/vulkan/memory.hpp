#ifndef _HYDRA_NGINE_DRIVERS_VULKAN_MEMORY_HPP
#define _HYDRA_NGINE_DRIVERS_VULKAN_MEMORY_HPP

#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_raii.hpp>

#include "commons.hpp"
#include "drivers/vulkan/structs.hpp"
namespace hydra {
    namespace drivers {
        namespace vlkn {

            struct BasicData;

            class AllocatedBuffer {
            private:
                std::unique_ptr<vk::raii::Buffer> m_buffer;
                std::unique_ptr<vk::raii::DeviceMemory> m_memory;

            public:
                inline auto buffer() & -> vk::Buffer { return **m_buffer; };
                inline auto memory() & -> vk::DeviceMemory { return **m_memory; };
                AllocatedBuffer(vk::raii::Buffer&& buffer, vk::raii::DeviceMemory&& memory) : m_buffer{std::make_unique<vk::raii::Buffer>(std::forward<vk::raii::Buffer>(buffer))}, m_memory{std::make_unique<vk::raii::DeviceMemory>(std::forward<vk::raii::DeviceMemory>(memory))} {};
            };

            class AllocatedImage {
            private:
                std::unique_ptr<vk::raii::Image> m_image;
                std::unique_ptr<vk::raii::DeviceMemory> m_memory;
                std::unique_ptr<vk::raii::ImageView> m_defaultView;
                vk::raii::Sampler* m_samplerPtr;
                int mipLevels;

            public:
                inline auto sampler(vk::raii::Sampler* samplerPtr) & -> AllocatedImage&
                {
                    m_samplerPtr = samplerPtr;
                    return *this;
                };
                inline auto sampler() & -> vk::Sampler { return **m_samplerPtr; };
                inline auto image() & -> vk::Image { return **m_image; };
                inline auto memory() & -> vk::DeviceMemory { return **m_memory; };
                inline auto imageView() & -> vk::ImageView { return **m_defaultView; };
                AllocatedImage(vk::raii::Image&& image, vk::raii::DeviceMemory&& memory, vk::raii::ImageView defaultView) : m_image{std::make_unique<vk::raii::Image>(std::forward<vk::raii::Image>(image))}, m_memory{std::make_unique<vk::raii::DeviceMemory>(std::forward<vk::raii::DeviceMemory>(memory))}, m_defaultView{std::make_unique<vk::raii::ImageView>(std::forward<vk::raii::ImageView>(defaultView))} {};
            };

            class MemoryAllocator {
            private:
                BasicData* m_basicDataPtr;

            public:
                inline MemoryAllocator(BasicData* basicDataPtr) : m_basicDataPtr(basicDataPtr) {};
                AllocatedImage allocateImage(vk::ImageCreateInfo imageCreateInfo, vk::ImageViewCreateInfo imageViewCreateInfo, vk::MemoryPropertyFlags memflags = vk::MemoryPropertyFlagBits::eDeviceLocal);
                ~MemoryAllocator(){};
            };

        }  // namespace vlkn
    }      // namespace drivers
}  // namespace hydra

#endif  //_HYDRA_NGINE_DRIVERS_VULKAN_MEMORY_HPP
