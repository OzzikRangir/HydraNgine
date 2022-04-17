#include "core/context.hpp"

// #include "core/assets.hpp"
#include "commons.hpp"
namespace hydra {
    namespace base {
        auto Context::createMaterial() & -> Material*
        {
            auto obj = std::make_unique<Material>();
            auto id = obj->id();
            std::scoped_lock lock(m_objectMutex);
            m_objects[id] = std::move(obj);
            return dynamic_cast<Material*>(m_objects[id].get());
        }

        auto Context::createEntity(const std::string& name) & -> Entity*
        {
            std::list<ManagedProperty*> entityProperties;
            std::list<ManagedProperty*> transformProperties;
            // {
            //     auto prop = std::make_unique<ManagedProperty>(&m_objectArchetypes[0]);
            //     auto id = prop->id();
            //     std::scoped_lock lock(m_propertyMutex);
            //     m_properties[id] = std::move(prop);

            //     entityProperties.push_back(m_properties[id].get());
            // }
            for (auto& archetype : m_traitArchetypes[EntityTrait::TraitType::Transform]) {
                auto prop = std::make_unique<ManagedProperty>(&archetype);
                auto id = prop->id();
                std::scoped_lock lock(m_propertyMutex);
                m_properties[id] = std::move(prop);

                entityProperties.push_back(m_properties[id].get());
                transformProperties.push_back(m_properties[id].get());
            }
            for (auto& archetype : m_objectArchetypes) {
                auto prop = std::make_unique<ManagedProperty>(&archetype);
                auto id = prop->id();
                std::scoped_lock lock(m_propertyMutex);
                m_properties[id] = std::move(prop);

                entityProperties.push_back(m_properties[id].get());
            }
            // {
            //     auto prop = std::make_unique<ManagedProperty>("Transform", glm::mat4());
            //     auto id = prop->id();
            //     std::scoped_lock lock(m_propertyMutex);
            //     m_properties[id] = std::move(prop);

            //     entityProperties.push_back(m_properties[id].get());
            //     transformProperties.push_back(m_properties[id].get());
            // }
            // {
            //     auto prop = std::make_unique<ManagedProperty>("Position", glm::vec3());
            //     auto id = prop->id();
            //     std::scoped_lock lock(m_propertyMutex);
            //     m_properties[id] = std::move(prop);

            //     entityProperties.push_back(m_properties[id].get());
            //     transformProperties.push_back(m_properties[id].get());
            // }

            // {
            //     auto prop = std::make_unique<ManagedProperty>("Rotation", glm::vec3());
            //     auto id = prop->id();
            //     std::scoped_lock lock(m_propertyMutex);
            //     m_properties[id] = std::move(prop);

            //     entityProperties.push_back(m_properties[id].get());
            //     transformProperties.push_back(m_properties[id].get());
            // }

            // {
            //     auto prop = std::make_unique<ManagedProperty>("Scale", 1.0f);
            //     auto id = prop->id();
            //     std::scoped_lock lock(m_propertyMutex);
            //     m_properties[id] = std::move(prop);

            //     entityProperties.push_back(m_properties[id].get());
            //     transformProperties.push_back(m_properties[id].get());
            // }

            Entity* entity;
            {
                auto obj = std::make_unique<Entity>(entityProperties);
                auto id = obj->id();
                std::scoped_lock lock(m_objectMutex);
                m_objects[id] = std::move(obj);
                entity = dynamic_cast<Entity*>(m_objects[id].get());
            }

            {
                auto obj = std::make_unique<EntityTraitTransform>(std::move(transformProperties));
                auto id = obj->id();
                std::scoped_lock lock(m_objectMutex);
                m_objects[id] = std::move(obj);
                entity->addTrait(dynamic_cast<EntityTraitTransform*>(m_objects[id].get()));
            }

            {
                auto obj = std::make_unique<EntityTraitMesh>();
                auto id = obj->id();
                std::scoped_lock lock(m_objectMutex);
                m_objects[id] = std::move(obj);
                entity->addTrait(dynamic_cast<EntityTraitMesh*>(m_objects[id].get()));
            }
            return dynamic_cast<Entity*>(entity);
        }
    }  // namespace base
}  // namespace hydra