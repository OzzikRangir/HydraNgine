#ifndef _HYDRA_NGINE_DRIVERS_VULKAN_DESCRIPTORS_HPP
#define _HYDRA_NGINE_DRIVERS_VULKAN_DESCRIPTORS_HPP
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_raii.hpp>

#include "commons.hpp"
#include "drivers/vulkan/memory.hpp"
#include "drivers/vulkan/structs.hpp"

namespace hydra {
    namespace drivers {
        namespace vlkn {
            struct DescriptorBindingInfo {
                vk::DescriptorType type;
                vk::ShaderStageFlags stageFlags;
                vk::ImageLayout imageLayout;
            };
            enum class DescriptorBindingType {
                Image,
                Buffer
            };
            struct DescriptorBinding {
                union Pointer {
                    AllocatedImage* image;
                    AllocatedBuffer* buffer;
                };

                vk::WriteDescriptorSet writeSet;
                vk::DescriptorSetLayoutBinding binding;
                vk::DeviceSize offset = 0;
                Pointer pointer;
                DescriptorBindingInfo info;
                DescriptorBindingType type;

                constexpr inline bool operator==(const DescriptorBinding& other) const { return binding == other.binding; };
                constexpr inline bool operator!=(const DescriptorBinding& other) const { return binding != other.binding; };
            };

            class DescriptorSetLayoutData {
            private:
                BasicData* m_basicDataPtr;
                std::vector<DescriptorBinding> m_bindings;
                std::unique_ptr<vk::raii::DescriptorSetLayout> m_layout;

                auto createLayout() & -> vk::raii::DescriptorSetLayout*;

            public:
                DescriptorSetLayoutData(vk::raii::DescriptorSetLayout&& layout) : m_layout{std::make_unique<vk::raii::DescriptorSetLayout>(std::move(layout))} {};
                DescriptorSetLayoutData(DescriptorSetLayoutData&& layout) : m_layout{std::move(layout.m_layout)}, m_bindings{std::move(layout.m_bindings)} {};
                DescriptorSetLayoutData(BasicData* basicDataPtr) : m_basicDataPtr(basicDataPtr){};

                inline bool operator==(const DescriptorSetLayoutData& other) const
                {
                    if (other.m_bindings.size() != m_bindings.size()) {
                        return false;
                    }
                    else {
                        size_t i = 0;
                        for (auto& binding : m_bindings) {
                            auto& othrBinding = other.m_bindings[i];
                            if (binding != othrBinding) return false;
                            i++;
                        }
                        return true;
                    }
                }
                // constexpr inline auto info() & ->  const vk::DescriptorSetLayoutCreateInfo& { return m_createInfo; };
                constexpr inline auto bindings() & -> const std::vector<DescriptorBinding>& { return m_bindings; };
                inline auto layout() & -> const vk::raii::DescriptorSetLayout&
                {
                    if (m_layout) return *m_layout;
                    else
                        return *createLayout();
                };
                DescriptorSetLayoutData& bindImage(AllocatedImage* image, DescriptorBindingInfo bindingInfo, uint32_t binding = ~0);
                DescriptorSetLayoutData& bindBuffer(AllocatedBuffer* buffer, DescriptorBindingInfo bindingInfo, vk::DeviceSize bufferOffset = 0, uint32_t binding = ~0);
            };

            struct DescriptorsData {
                std::vector<vk::raii::DescriptorPool> descriptorPools;
                std::vector<DescriptorSetLayoutData> layoutCache;
                std::map<vk::DescriptorSet, DescriptorSetLayoutData*> descriptorSetsLayouts;
            };

            class DescriptorAllocator {
            private:
                inline static std::map<vk::DescriptorType, float> PoolSizesMult()
                {
                    return {
                        {vk::DescriptorType::eSampler, 0.5f},
                        {vk::DescriptorType::eCombinedImageSampler, 4.f},
                        {vk::DescriptorType::eSampledImage, 4.f},
                        {vk::DescriptorType::eStorageImage, 1.f},
                        {vk::DescriptorType::eUniformTexelBuffer, 1.f},
                        {vk::DescriptorType::eStorageTexelBuffer, 1.f},
                        {vk::DescriptorType::eUniformBuffer, 2.f},
                        {vk::DescriptorType::eStorageBuffer, 2.f},
                        {vk::DescriptorType::eUniformBufferDynamic, 1.f},
                        {vk::DescriptorType::eStorageBufferDynamic, 1.f},
                        {vk::DescriptorType::eInputAttachment, 0.5f}};
                };

            private:
                BasicData* m_basicDataPtr;
                DescriptorsData m_descriptorData;
                auto addPool(size_t size) & -> void;

            public:
                // auto allocateDescriptorSet(vk::DescriptorType type, vk::ShaderStageFlags flags) & -> vk::raii::DescriptorSet&&;
                auto allocateDescriptorSet(DescriptorSetLayoutData&& layout) & -> vk::raii::DescriptorSet;

                DescriptorAllocator(BasicData* basicDataPtr);
                ~DescriptorAllocator(){};
            };
        }  // namespace vlkn
    }      // namespace drivers
}  // namespace hydra

#endif  //_HYDRA_NGINE_DRIVERS_VULKAN_DESCRIPTORS_HPP
