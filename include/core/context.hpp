#ifndef _HYDRA_NGINE_CORE_CONTEXT_HPP
#define _HYDRA_NGINE_CORE_CONTEXT_HPP
#include <mutex>

#include "commons.hpp"
#include "core/assets.hpp"
#include "core/entity.hpp"
#include "core/entity_traits.hpp"
#include "core/managed_object.hpp"
#include "core/managed_property.hpp"
#include "core/uuid.hpp"
namespace hydra {
    namespace base {
        class Context {
        private:
            std::atomic<uint64_t> m_count = 0;
            static std::vector<Context> m_contexts;
            std::mutex m_propertyMutex;
            std::mutex m_objectMutex;
            std::unordered_map<UUID, std::unique_ptr<ManagedObject>> m_objects;
            std::unordered_map<UUID, std::unique_ptr<ManagedProperty>> m_properties;
            std::unordered_map<EntityTrait::TraitType, TraitArchetype> m_traitArchetypes{
                {EntityTrait::TraitType::Transform,
                 {PropertyArchetype{"Position", glm::vec3{}},
                  PropertyArchetype{"Rotation", glm::vec3{}}}}};
            TraitArchetype m_objectArchetypes{
                {PropertyArchetype{"Name", std::string{"empty"}}}};

        public:
            auto createMaterial() & -> Material*;
            auto createEntity(const std::string& name) & -> Entity*;
            inline auto properties() & -> std::list<UUID>;  // std::unordered_map<UUID, std::unique_ptr<ManagedProperty>>&;
            inline auto objects() & -> std::list<UUID>;
            inline auto property(UUID id) & -> ManagedProperty& { return *m_properties[id]; };
            inline auto object(UUID id) & -> ManagedObject& { return *m_objects[id]; };
        };
        inline auto Context::properties() & -> std::list<UUID>
        {
            std::list<UUID> uuids;
            for (const auto& prop : m_properties) uuids.push_back(prop.second->id());
            return uuids;
        };
        inline auto Context::objects() & -> std::list<UUID>
        {
            std::list<UUID> uuids;
            for (const auto& obj : m_objects) uuids.push_back(obj.second->id());
            return uuids;
        };

    }  // namespace base
}  // namespace hydra
#endif  //_HYDRA_NGINE_CORE_CONTEXT_HPP