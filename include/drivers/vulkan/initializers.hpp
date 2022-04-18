#ifndef _HYDRA_NGINE_DRIVERS_VULKAN_INITIALIZERS_HPP
#define _HYDRA_NGINE_DRIVERS_VULKAN_INITIALIZERS_HPP
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_raii.hpp>
namespace hydra {
    namespace drivers {
        namespace vlkn {
            namespace init {
                inline vk::ImageCreateInfo imageCreateInfo(vk::Format format, vk::ImageUsageFlags usageFlags, vk::Extent3D extent, vk::ImageCreateInfo info = {})
                {
                    info.format = format;
                    info.extent = extent;
                    info.usage = usageFlags;
                    return info;
                };

                inline vk::ImageCreateInfo depthStencilImageCreateInfo(uint32_t width, uint32_t height, vk::Format format = vk::Format::eD24UnormS8Uint)
                {
                    vk::ImageCreateInfo info;
                    info.flags = {};

                    info.imageType = vk::ImageType::e2D;
                    info.format = format;
                    info.extent = vk::Extent3D{width, height, 1U};
                    info.mipLevels = 1;
                    info.arrayLayers = 1;
                    info.samples = vk::SampleCountFlagBits::e1;
                    info.tiling = vk::ImageTiling::eOptimal;
                    info.usage = vk::ImageUsageFlagBits::eDepthStencilAttachment | vk::ImageUsageFlagBits::eTransferSrc | vk::ImageUsageFlagBits::eSampled;
                    info.sharingMode = vk::SharingMode::eExclusive;
                    info.queueFamilyIndexCount = 0;
                    info.pQueueFamilyIndices = nullptr;
                    info.initialLayout = vk::ImageLayout::eUndefined;
                    return info;
                };

                inline vk::ImageCreateInfo textureImageCreateInfo(uint32_t width, uint32_t height, uint32_t mipLevels = 1, vk::Format format = vk::Format::eR8G8B8A8Unorm)
                {
                    vk::ImageCreateInfo info;
                    info.flags = {};
                    info.imageType = vk::ImageType::e2D;
                    info.format = format;
                    info.extent = vk::Extent3D{width, height, 1U};
                    info.mipLevels = mipLevels;
                    info.arrayLayers = 1;
                    info.samples = vk::SampleCountFlagBits::e1;
                    info.tiling = vk::ImageTiling::eOptimal;
                    info.usage = vk::ImageUsageFlagBits::eSampled | vk::ImageUsageFlagBits::eTransferSrc | vk::ImageUsageFlagBits::eTransferDst;
                    info.sharingMode = vk::SharingMode::eExclusive;
                    info.queueFamilyIndexCount = 0;
                    info.pQueueFamilyIndices = nullptr;
                    info.initialLayout = vk::ImageLayout::eUndefined;
                    return info;
                };

                inline vk::ImageViewCreateInfo imageViewCreateInfo(vk::Format format, vk::Image image, vk::ImageAspectFlags aspectFlags)
                {
                    vk::ImageViewCreateInfo info;
                    info.image = image;
                    info.viewType = vk::ImageViewType::e2D;
                    info.format = format;
                    info.subresourceRange = {aspectFlags, 0, 1, 0, 1};
                    return std::move(info);
                };

                inline vk::MemoryAllocateInfo memoryAllocateInfo();
            }  // namespace init
        }      // namespace vlkn
    }          // namespace drivers
}  // namespace hydra
#endif  //_HYDRA_NGINE_DRIVERS_VULKAN_INITIALIZERS_HPP
