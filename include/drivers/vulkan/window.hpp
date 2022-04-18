#ifndef _HYDRA_NGINE_DRIVERS_VULKAN_WINDOW_HPP
#define _HYDRA_NGINE_DRIVERS_VULKAN_WINDOW_HPP
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_raii.hpp>

#include "commons.hpp"
#include "drivers/vulkan/structs.hpp"
namespace hydra {
    namespace drivers {
        namespace vlkn {
            class Window {
            private:
                BasicData* m_basicDataPtr;
                WindowData m_windowData;
                RenderData m_renderData;
                // std::unique_ptr<vk::raii::SurfaceKHR> m_surface;
                // std::unique_ptr<vk::raii::SwapchainKHR> m_swapchain;
                // vk::Format m_swapchainImageFormat;

                vk::Format m_renderFormat;
                // AllocatedImage _rawRenderImage;
                std::unique_ptr<vk::raii::Sampler> m_smoothSampler;
                VkFramebuffer m_forwardFramebuffer;
                VkFramebuffer m_shadowFramebuffer;
                std::vector<vk::raii::Framebuffer> m_framebuffers;
                void renderPassCommand(uint32_t index);


                // std::unique_ptr
            public:
                Window(BasicData* basicDataPtr, VkSurfaceKHR surface, vk::Extent2D extent);
                void draw();
                ~Window() {}
            };
        }  // namespace vlkn
    }      // namespace drivers
}  // namespace hydra

#endif  //_HYDRA_NGINE_DRIVERS_VULKAN_WINDOW_HPP
