#ifndef _HYDRA_NGINE_DRIVERS_VULKAN_MATERIALS_HPP
#define _HYDRA_NGINE_DRIVERS_VULKAN_MATERIALS_HPP
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_raii.hpp>

#include "commons.hpp"
#include "drivers/vulkan/descriptors.hpp"
#include "drivers/vulkan/structs.hpp"
namespace hydra {
    namespace drivers {
        namespace vlkn {
            class ShaderModule {
            private:
                const std::string m_name;
                std::unique_ptr<vk::raii::ShaderModule> m_module;

            public:
                inline ShaderModule(std::string name, vk::raii::ShaderModule&& module) : m_name{name}, m_module{std::make_unique<vk::raii::ShaderModule>(std::move(module))} {};
                ShaderModule(ShaderModule&&) = default;
                ShaderModule() = delete;
                ShaderModule(const ShaderModule&) = delete;
                // inline ShaderModule(std::string&& name, vk::raii::ShaderModule&& module) : m_name{name}, m_module{std::make_unique<vk::raii::ShaderModule>(std::move(module))} {};
                ~ShaderModule(){};
            };
            using ShaderStageModuleMap = std::map<vk::ShaderStageFlags, ShaderModule*>;
            class Shader {
            private:
                BasicData* basicDataPtr;
                ShaderStageModuleMap m_stages;
                std::vector<DescriptorSetLayoutData> m_descriptorSetLayouts;
                std::unique_ptr<vk::raii::PipelineLayout> m_layout;
                /* data */
                // std::unique_ptr<Module> m_module;

            public:
                Shader(ShaderStageModuleMap stages);
                ~Shader() {}
            };
            class ShaderManager {
            private:
                BasicData* m_basicDataPtr;
                ShadersData m_shadersData;

            public:
                inline ShaderManager(BasicData* basicDataPtr) : m_basicDataPtr(basicDataPtr){};
                auto createShaderModule(std::string name, std::vector<uint32_t>&& code) & -> ShaderModule*;
                ~ShaderManager(){};
            };

        }  // namespace vlkn
    }      // namespace drivers
}  // namespace hydra

#endif  //_HYDRA_NGINE_DRIVERS_VULKAN_MATERIALS_HPP
