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
            std::string m_path;

        public:
            Resource(){};
            template <typename T>
            inline auto copy(uint64_t accessTick = 0) & -> std::vector<T>&&
            {
                m_data.lastAccess = accessTick;
                size_t paddedLength;
                size_t chunks = m_data.bytes.size() / sizeof(T);
                std::vector<T> data{chunks};
                if (m_data.bytes.size() % chunks > 0) chunks++;
                for (size_t i = 0; i < chunks; i++)
                    data.push_back(*reinterpret_cast<T*>(&m_data.bytes.data()[chunks * sizeof(T)]));
                return std::move(data);
            };

            inline auto data(uint64_t accessTick = 0) & -> const std::vector<uint8_t>&
            {
                m_data.lastAccess = accessTick;
                return m_data.bytes;
            };
            // inline std::vector<uint8_t>& load(uint64_t accessTick = 0) {}
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
