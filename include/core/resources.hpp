#ifndef _HYDRA_NGINE_CORE_RESOURCES_HPP
#define _HYDRA_NGINE_CORE_RESOURCES_HPP

#include "commons.hpp"
namespace hydra {
    namespace base {
        class Resource {
        public:
            struct Data {
                std::vector<uint8_t> bytes;
                uint64_t lastAccess;
            };

        protected:
            Data m_data;

        public:
            Resource(){};
            Resource(const std::vector<uint8_t>& bytes) : m_data{bytes, 0} {};
            template <typename T>
            inline auto copy(uint64_t accessTick = 0) & -> std::vector<T>
            {
                m_data.lastAccess = accessTick;
                size_t pad = sizeof(T);
                size_t newSize = m_data.bytes.size() / sizeof(T);
                if (m_data.bytes.size() % newSize > 0) {
                    newSize++;
                }
                std::vector<T> data;
                data.reserve(newSize);
                std::vector<uint8_t> bytes = {m_data.bytes};
                for (int i = bytes.size(); i < newSize * sizeof(T); i++) bytes.push_back(0);
                for (int i = 0; i < newSize; i++) data.push_back(*reinterpret_cast<T*>(&bytes[i * sizeof(T)]));
                return std::move(data);
            };

            inline auto data(uint64_t accessTick = 0) & -> const std::vector<uint8_t>&
            {
                m_data.lastAccess = accessTick;
                return m_data.bytes;
            };
            template <typename T>
            inline void store(std::vector<T>&& data, uint64_t accessTick = 0)
            {
                std::copy(reinterpret_cast<uint8_t*>(data.data()), reinterpret_cast<uint8_t*>(data.data()) + data.size() * sizeof(T), std::back_inserter(m_data.bytes));
            };
            //
            ~Resource(){};
        };
    }  // namespace base
}  // namespace hydra

#endif  //_HYDRA_NGINE_CORE_RESOURCES_HPP
