#include "drivers/vulkan/descriptors.hpp"

#include <iostream>
namespace hydra {
    namespace drivers {
        namespace vlkn {
            auto DescriptorSetLayoutData::createLayout() & -> vk::raii::DescriptorSetLayout*{
                vk::DescriptorSetLayoutCreateInfo dslCI{};
                std::vector<vk::DescriptorSetLayoutBinding> bindings; 
                for(auto& binding : m_bindings){
                    bindings.push_back(binding.binding);
                }
                dslCI.bindingCount = bindings.size();
                dslCI.pBindings = bindings.data();
                m_layout = std::make_unique<vk::raii::DescriptorSetLayout>(m_basicDataPtr->device->createDescriptorSetLayout(dslCI));
                return m_layout.get();
            }
            auto DescriptorAllocator::allocateDescriptorSet(DescriptorSetLayoutData&& layout) & -> vk::raii::DescriptorSet&&
            {
                DescriptorSetLayoutData* currLayout = nullptr;
                for (auto& dsld : m_descriptorData.layoutCache) {
                    if (layout == dsld) {
                        currLayout = &dsld;
                    }
                }
                if (!currLayout) {
                    m_descriptorData.layoutCache.push_back(std::move(layout));
                    currLayout = &m_descriptorData.layoutCache.back();
                }

                vk::DescriptorSetLayoutCreateInfo dslCI{};
                // dslCI.pBindings = ;
                if (m_descriptorData.descriptorPools.size() == 0) {}

                vk::DescriptorSetAllocateInfo allocInfo{};

                allocInfo.pSetLayouts = &*currLayout->layout();
                allocInfo.descriptorPool = *m_descriptorData.descriptorPools.back();
                allocInfo.descriptorSetCount = 1;
                auto descriptorSets = m_basicDataPtr->device->allocateDescriptorSets(allocInfo);
                if (descriptorSets.size() == 0) {
                    addPool(size::KiB(1));
                    allocInfo.descriptorPool = *m_descriptorData.descriptorPools.back();
                    descriptorSets = m_basicDataPtr->device->allocateDescriptorSets(allocInfo);
                }
                if (descriptorSets.size() == 0) {
                    throw std::runtime_error("DescriptorSetAllocateError");
                }
                return std::move(descriptorSets[0]);

                // vk::DescriptorSetLayoutBinding setbind{};
                // setbind.binding = 0;
                // setbind.descriptorCount = 1;
                // setbind.descriptorType = type;
                // setbind.stageFlags = flags;

                // 
            }

            DescriptorAllocator::DescriptorAllocator(BasicData* basicDataPtr) : m_basicDataPtr(basicDataPtr)
            {
                addPool(size::KiB(10));
            }

            auto DescriptorAllocator::addPool(size_t size) & -> void
            {
                size_t count = size;
                std::vector<vk::DescriptorPoolSize> sizes;
                sizes.reserve(PoolSizesMult().size());
                for (auto [type, mult] : PoolSizesMult()) {
                    sizes.push_back({type, uint32_t(mult * count)});
                }
                vk::DescriptorPoolCreateInfo dpCI{};
                dpCI.poolSizeCount = (uint32_t)sizes.size();
                dpCI.pPoolSizes = sizes.data();
                dpCI.maxSets = size;

                m_descriptorData.descriptorPools.push_back({*m_basicDataPtr->device, dpCI});
            }

            DescriptorSetLayoutData& DescriptorSetLayoutData::bindImage(AllocatedImage* image, DescriptorBindingInfo bindingInfo, uint32_t binding)
            {
                if (!~binding) {
                    binding = m_bindings.size();
                }

                DescriptorBinding newBinding{};

                vk::DescriptorImageInfo imageBufferInfo;
                imageBufferInfo.sampler = image->sampler();
                imageBufferInfo.imageView = image->imageView();
                imageBufferInfo.imageLayout = bindingInfo.imageLayout;

                vk::DescriptorSetLayoutBinding bindingStruct{};
                bindingStruct.descriptorCount = 1;
                bindingStruct.descriptorType = bindingInfo.type;
                bindingStruct.stageFlags = bindingInfo.stageFlags;
                bindingStruct.binding = binding;

                vk::WriteDescriptorSet newWrite{};
                newWrite.descriptorCount = 1;
                newWrite.descriptorType = bindingInfo.type;
                newWrite.pImageInfo = &imageBufferInfo;
                newWrite.dstBinding = binding;

                newBinding.binding = bindingStruct;
                newBinding.info = bindingInfo;
                newBinding.type = DescriptorBindingType::Image;
                newBinding.writeSet = newWrite;

                m_bindings.push_back(newBinding);

                std::sort(m_bindings.begin(), m_bindings.end(), [](DescriptorBinding& a, DescriptorBinding& b) {
                    return a.binding.binding < b.binding.binding;
                });
                return *this;
            }

        }  // namespace vlkn
    }      // namespace drivers
}  // namespace hydra