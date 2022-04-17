#ifndef _HYDRA_NGINE_CORE_MANAGED_OBJECT_HPP
#define _HYDRA_NGINE_CORE_MANAGED_OBJECT_HPP
#include "commons.hpp"
#include "core/uuid.hpp"
namespace hydra {
    namespace base {
        class ManagedObject {
        public:
            enum class Type {
                Undefined,
                Asset,
                Entity,
                Trait,
                External
            };

        protected:
            const UUID m_id;
            explicit ManagedObject() : m_id{uuid::Generator::next()} {};

        public:
            virtual inline ~ManagedObject(){};

            constexpr virtual inline auto type() & -> const Type { return Type::Undefined; };
            constexpr virtual inline auto id() & -> const UUID & { return m_id; };
            virtual auto dump() & -> std::string;

            template <typename T>
            inline auto as() & -> T * { return dynamic_cast<T *>(this); };
        };
    }  // namespace base
}  // namespace hydra
#endif  //_HYDRA_NGINE_CORE_MANAGED_OBJECT_HPP