#ifndef _HYDRA_NGINE_CORE_UUID_HPP
#define _HYDRA_NGINE_CORE_UUID_HPP
#include <stduuid/uuid.h>

#include "commons.hpp"
#include <mutex>
namespace hydra {
    using UUID = uuids::uuid;
    namespace uuid {
        class Generator {
        private:
            std::mt19937 m_gen;
            std::mutex m_mutex;
            std::list<UUID> m_pregenerated;
            Generator()
            {
                std::random_device rd;
                auto seed_data = std::array<int, std::mt19937::state_size>{};
                std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
                std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
                m_gen = std::mt19937(seq);
            }
            static Generator& instance()
            {
                static Generator instance;
                return instance;
            }
            inline UUID getUUID()
            {
                std::scoped_lock lock(m_mutex);
                if (m_pregenerated.size()) {
                    UUID tmp = std::move(m_pregenerated.back());
                    m_pregenerated.pop_back();
                    return std::move(tmp);
                }
                else
                    return uuids::uuid_random_generator(m_gen)();
            };

        public:
            static UUID next()
            {
                return instance().getUUID();
            }
            static void generate(size_t size)
            {
                for (size_t i = 0; i < size; i++) instance().m_pregenerated.push_back(uuids::uuid_random_generator(instance().m_gen)());
            };

            Generator(Generator const&) = delete;
            void operator=(Generator const&) = delete;
        };
    }  // namespace uuid
}  // namespace hydra

#endif  //_HYDRA_NGINE_CORE_UUID_HPP
