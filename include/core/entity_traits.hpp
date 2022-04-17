#ifndef _HYDRA_NGINE_CORE_ENTITY_TRAIT_HPP
#define _HYDRA_NGINE_CORE_ENTITY_TRAIT_HPP
#include <glm/glm.hpp>

#include "commons.hpp"
#include "core/managed_property.hpp"
#include "core/resources.hpp"
#include "core/uuid.hpp"
namespace hydra {
    namespace base {
        class EntityTrait :  public virtual ManagedObject {
        public:
            enum class TraitType {
                Undefined,
                Transform,
                Mesh
            };

        protected:
            std::list<ManagedProperty*> m_properties;

        public:
            inline EntityTrait(std::list<ManagedProperty*> properties) : m_properties{properties} {};
            ~EntityTrait() {}
            virtual inline auto properties() -> std::list<ManagedProperty*>& { return m_properties; };
            virtual inline TraitType traitType() { return TraitType::Undefined; };
            inline auto type() & -> const Type override { return Type::Trait; };

            virtual auto dump() & -> std::string override;
        };

        class EntityTraitTransform : public virtual EntityTrait {
        public:
            EntityTraitTransform(std::list<ManagedProperty*> properties) : EntityTrait(properties){};
            inline TraitType traitType() override { return TraitType::Transform; };
        };

        class EntityTraitMesh : public virtual EntityTrait {
        private:
            Resource m_resource;

        public:
            inline auto resource() & -> Resource& { return m_resource; };
            EntityTraitMesh(std::list<ManagedProperty*> properties = {}) : EntityTrait(properties){};
            inline TraitType traitType() override { return TraitType::Mesh; };
        };

    }  // namespace base
}  // namespace hydra

#endif  //_HYDRA_NGINE_CORE_ENTITY_TRAIT_HPP
