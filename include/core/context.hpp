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
            std::unordered_map<enum Trait::Type, Archetype> m_traitArchetypes{
                {Trait::Type::Transform,
                 {{"Position",
                   glm::vec3{}},
                  {"Rotation",
                   glm::vec3{}}}},
                {Trait::Type::Camera,
                 {{"View",
                   glm::mat4{}},
                  {"Rotation",
                   glm::mat4{}},
                  {"Projection",
                   glm::mat4{}}}}};

            Archetype m_entityArchetypes{
                {{"Name", std::string{"empty"}}}};

            inline auto createEntity(std::list<ManagedProperty*>&& properties, Entity* parent = nullptr) -> Entity*
            {
                auto obj = std::make_unique<Entity>(std::move(properties), parent);
                auto id = obj->id();
                std::scoped_lock lock(m_objectMutex);
                m_objects[id] = std::move(obj);
                return dynamic_cast<Entity*>(m_objects[id].get());
            };

        public:
            auto createMaterial() & -> Material*;
            auto createEntity(const std::string& name, Entity* parent = nullptr) & -> Entity&;
            template <typename T>
            inline auto addTrait(Entity* entity) & -> T&
            {
                static_assert(std::is_base_of<Trait, T>::value, "T must inherit from Trait");
                std::list<ManagedProperty*> traitProperties;
                for (auto& archetype : m_traitArchetypes[T::TypeEnum()]) {
                    auto prop = std::make_unique<ManagedProperty>(&archetype);
                    auto id = prop->id();
                    std::scoped_lock lock1(m_propertyMutex);
                    std::scoped_lock lock2(m_objectMutex);
                    m_properties[id] = std::move(prop);
                    entity->properties().push_back(m_properties[id].get());
                    traitProperties.push_back(m_properties[id].get());
                }
                auto obj = std::make_unique<T>(std::move(traitProperties));
                auto id = obj->id();
                std::scoped_lock lock(m_objectMutex);
                m_objects[id] = std::move(obj);
                entity->addTrait(dynamic_cast<T*>(m_objects[id].get()));
                return dynamic_cast<T&>(*m_objects[id].get());
            }
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