#ifndef _HYDRA_NGINE_CORE_ENTITY_HPP
#define _HYDRA_NGINE_CORE_ENTITY_HPP
#include "commons.hpp"
#include "core/entity_traits.hpp"
#include "core/managed_object.hpp"
#include "core/managed_property.hpp"
#include "core/uuid.hpp"
namespace hydra {
    namespace base {
        class Entity : public virtual ManagedObject {
        private:
            std::list<ManagedProperty*> m_properties;
            Entity* m_parent;
            std::list<Entity*> m_children;
            std::list<EntityTrait*> m_traits;

        public:
            inline Entity(std::list<ManagedProperty*> properties, Entity* parent = nullptr) : m_properties{properties}, m_parent{parent}, ManagedObject(){};
            inline auto addTrait(EntityTrait* trait) & -> void { m_traits.push_back(trait); };
            template <typename T>
            inline auto trait() & -> T*
            {
                for (auto trait : m_traits) {
                    try {
                        T* ptr = dynamic_cast<T*>(trait);
                        if (ptr)
                            return ptr;
                    }
                    catch (const std::bad_cast& e) {
                    }
                }
                return nullptr;
            }
            virtual inline auto properties() -> std::list<ManagedProperty*>& { return m_properties; };
            inline auto type() & -> const Type override { return Type::Entity; };

            virtual auto dump() & -> std::string override;
        };
    }  // namespace base
}  // namespace hydra
#endif  //_HYDRA_NGINE_CORE_ENTITY_HPP