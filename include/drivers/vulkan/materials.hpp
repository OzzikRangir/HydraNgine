#ifndef _HYDRA_NGINE_DRIVERS_VULKAN_MATERIALS_HPP
#define _HYDRA_NGINE_DRIVERS_VULKAN_MATERIALS_HPP
namespace hydra {
    namespace drivers {
        namespace vlkn {
            class MaterialManager
            {
            private:
                /* data */
            public:
                inline MaterialManager(BasicData* basicDataPtr) : m_basicDataPtr(basicDataPtr) {};
                ~MaterialManager() {}
            };

        }
    }
}

#endif //_HYDRA_NGINE_DRIVERS_VULKAN_MATERIALS_HPP
