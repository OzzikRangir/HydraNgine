#include "drivers/vulkan/memory.hpp"
namespace hydra {
    namespace drivers {
        namespace vlkn {
            AllocatedImage MemoryAllocator::allocateImage(vk::ImageCreateInfo imageCreateInfo, vk::ImageViewCreateInfo imageViewCreateInfo, vk::MemoryPropertyFlags memflags)
            {
                vk::MemoryAllocateInfo info{};
                // std::cout << &imageCreateInfo << std::endl;
                // std::cout << m_basicDataPtr->device << std::endl;

                // imageCreateInfo.
                vk::DeviceImageMemoryRequirements devMemReq{};
                devMemReq.pCreateInfo = &imageCreateInfo;
                devMemReq.planeAspect = vk::ImageAspectFlagBits::eColor;
                // std::cout << imageCreateInfo.extent.height << std::endl;
                // std::cout << imageCreateInfo.extent.width << std::endl;

                // std::cout << (int)imageCreateInfo.format << std::endl;
                // std::cout << (int)imageCreateInfo.imageType << std::endl;
                auto memreq = m_basicDataPtr->device->getImageMemoryRequirements(devMemReq).memoryRequirements;
                auto memprops = m_basicDataPtr->physicalDevice->getMemoryProperties();
                auto memoryTypeBits = memreq.memoryTypeBits;
                info.memoryTypeIndex = -1;
                for (int i = 0; i != memprops.memoryTypeCount; ++i, memoryTypeBits >>= 1) {
                    if (memoryTypeBits & 1) {
                        if ((memprops.memoryTypes[i].propertyFlags & memflags) == memflags) {
                            info.memoryTypeIndex = i;
                            break;
                        }
                    }
                }
                if (info.memoryTypeIndex < 0)
                    throw std::logic_error("");
                info.allocationSize = memreq.size;
                auto devMem = vk::raii::DeviceMemory(*m_basicDataPtr->device, info);
                auto image = vk::raii::Image(*m_basicDataPtr->device, imageCreateInfo);
                imageViewCreateInfo.image = *image;
                imageViewCreateInfo.format = imageCreateInfo.format;

                m_basicDataPtr->device->bindImageMemory2({vk::BindImageMemoryInfo(*image, *devMem, 0)});
                auto imageView = vk::raii::ImageView(*m_basicDataPtr->device, imageViewCreateInfo);

                return {std::move(image), std::move(devMem), std::move(imageView)};
            }
        }  // namespace vlkn
    }  // namespace drivers
}  // namespace hydra
