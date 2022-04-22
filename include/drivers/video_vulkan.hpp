#ifndef _HYDRA_NGINE_DRIVERS_VIDEO_VULKAN_HPP
#define _HYDRA_NGINE_DRIVERS_VIDEO_VULKAN_HPP
// #include "vulkan/descriptors.hpp"
// #include <vulkan/vulkan.hpp>
// #include <vulkan/vulkan_raii.hpp>

#include "commons.hpp"
#include "core/display.hpp"
#include "core/managed_object.hpp"
#include "core/managed_property.hpp"
#include "core/video.hpp"
#include "vulkan/commands.hpp"
#include "vulkan/descriptors.hpp"
#include "vulkan/memory.hpp"
#include "vulkan/shaders.hpp"
#include "vulkan/structs.hpp"
#include "vulkan/window.hpp"

namespace hydra {
    namespace drivers {
        class VideoVulkan : public virtual video::VideoInterface {
        private:
            vlkn::BasicData m_basicData;
            std::unique_ptr<vlkn::Window> m_window;


        public:
            VideoVulkan(){};
            ~VideoVulkan() override
            {
                if (m_basicData.device) m_basicData.device->waitIdle();
            };
            bool init(WindowInterface& window, std::list<base::Material*>& materials) override;
            void draw(std::list<base::ManagedObject*>& objects) override;
        };
    }  // namespace drivers
}  // namespace hydra

#endif  //_HYDRA_NGINE_DRIVERS_VIDEO_VULKAN_HPP
