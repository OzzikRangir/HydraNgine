#include "drivers/vulkan/shaders.hpp"
namespace hydra {
    namespace drivers {
        namespace vlkn {

            Shader::Shader(ShaderStageModuleMap stages) : m_stages{stages}
            {
            }
            auto ShaderManager::createShaderModule(std::string name, std::vector<uint32_t>&& code) & -> ShaderModule*
            {
                vk::ShaderModuleCreateInfo smCI{};
                smCI.codeSize = code.size() * sizeof(uint32_t);
                smCI.pCode = code.data();

                m_shadersData.m_shaders.emplace_back(name,
                                                     m_basicDataPtr->device->createShaderModule(smCI));

                // ShaderModule{}
                return &m_shadersData.m_shaders.back();
            }
        }  // namespace vlkn
    }      // namespace drivers
}  // namespace hydra
