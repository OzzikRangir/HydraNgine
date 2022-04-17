#ifndef _HYDRA_NGINE_CORE_MODULE_HPP
#define _HYDRA_NGINE_CORE_MODULE_HPP
#include "commons.hpp"
#include "core/resources.hpp"
namespace hydra {
    namespace base {
        class Module {
        private:
            Resource m_res;
            bool m_state = false;

        public:
            Module(std::vector<uint8_t>&& data) { m_res.store(std::move(data)); };
        };

        class ModuleLoader {
        protected:
            std::map<const char*, Module> modules;

        public:
            ModuleLoader(){};
            void load(const char* name, std::vector<uint8_t>&& data) { modules.insert({name, std::move(data)}); };
        };
    }  // namespace base
}  // namespace hydra

#endif  //_HYDRA_NGINE_CORE_MODULE_HPP
