#ifndef _HYDRA_NGINE_CORE_ENTITY_TRAIT_HPP
#define _HYDRA_NGINE_CORE_ENTITY_TRAIT_HPP
#include <glm/glm.hpp>

#include "commons.hpp"
#include "core/managed_property.hpp"
#include "core/resources.hpp"
#include "core/uuid.hpp"
namespace hydra {
    namespace base {
        class Trait : public virtual ManagedObject {
        public:
            enum class Type {
                Undefined,
                Transform,
                Camera,
                Mesh
            };

        protected:
            std::list<ManagedProperty*> m_properties;

        public:
            inline Trait(std::list<ManagedProperty*> properties) : m_properties{properties} {};
            ~Trait() {}
            virtual inline auto properties() & -> std::list<ManagedProperty*>& { return m_properties; };
            inline auto operator[](const char* name) & -> ManagedProperty&
            {
                for (auto& prop : m_properties) {
                    if (prop->name() == std::string(name))
                        return *prop;
                }
                throw std::range_error("PROPERTY NOT FOUND");
            };

            inline auto operator[](const std::string& name) & -> ManagedProperty&
            {
                for (auto& prop : m_properties) {
                    if (prop->name() == name)
                        return *prop;
                }
                throw std::range_error("PROPERTY NOT FOUND");
            };
            virtual inline auto traitType() & -> Trait::Type { return Type::Undefined; };
            inline auto type() & -> const ObjectType override { return ObjectType::Trait; };
            inline static auto TypeEnum() -> Type { return Type::Undefined; };

            virtual auto dump() & -> std::string override;
        };

        class TraitTransform : public virtual Trait {
        public:
            TraitTransform(std::list<ManagedProperty*> properties) : Trait{properties} {};
            inline auto traitType() & -> Type override { return Type::Transform; };
            inline static auto TypeEnum() -> Type { return Type::Transform; };
        };

        class TraitMesh : public virtual Trait {
        private:
            Resource m_resource;

        public:
            inline auto resource() & -> Resource& { return m_resource; };
            TraitMesh(std::list<ManagedProperty*> properties = {}) : Trait{properties} {};
            inline auto traitType() & -> Type override { return Type::Mesh; };
            inline static auto TypeEnum() -> Type { return Type::Mesh; };
        };

        class TraitCamera : public virtual Trait {
        public:
            TraitCamera(std::list<ManagedProperty*> properties = {}) : Trait{properties} {};
            inline auto traitType() & -> Type override { return Type::Camera; };
            inline static auto TypeEnum() -> Type { return Type::Camera; };
        };

    }  // namespace base
}  // namespace hydra

#endif  //_HYDRA_NGINE_CORE_ENTITY_TRAIT_HPP
