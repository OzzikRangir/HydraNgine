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

        // template <typename T>
        // inline auto Context::addTrait(Entity* entity) & -> void
        // {

        // }

        auto Context::createEntity(const std::string& name, Entity* parent) & -> Entity&
        {
            std::list<ManagedProperty*> entityProperties;
            std::list<ManagedProperty*> transformProperties;

            for (auto& archetype : m_entityArchetypes) {
                auto prop = std::make_unique<ManagedProperty>(&archetype);
                auto id = prop->id();
                std::scoped_lock lock(m_propertyMutex);
                m_properties[id] = std::move(prop);

                entityProperties.push_back(m_properties[id].get());
            }

            Entity* entity = createEntity(std::move(entityProperties));
            addTrait<TraitTransform>(entity);
            // parent.

            return *entity;
        }
    }  // namespace base
}  // namespace hydra