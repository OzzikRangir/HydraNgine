#include "core/assets.hpp"
#include "core/entity.hpp"
#include "core/entity_traits.hpp"
#include "core/managed_object.hpp"
namespace hydra {
    namespace base {

        // std::vector<PropertyTypeName> ManagedProperty::TypeNames{};
        auto ManagedObject::dump() & -> std::string
        {
            std::string str = "OBJECT: " + uuids::to_string(m_id) + "\n";
            return std::move(str);
        };

        auto Asset::dump() & -> std::string
        {
            std::string str = "ASSET: " + uuids::to_string(m_id) + "\n";
            return std::move(str);
        };

        auto Material::dump() & -> std::string
        {
            std::string str = "ASSET:MATERIAL: " + uuids::to_string(m_id) + "\n";
            return std::move(str);
        };

        auto EntityTrait::dump() & -> std::string
        {
            std::string str = "TRAIT: " + uuids::to_string(m_id) + "\n\tPROPERTIES:\n";
            for (auto* property : m_properties) {
                // str = str + "\t\t" + property->dump() + "\n";
            }

            return std::move(str);
        };
        auto Entity::dump() & -> std::string
        {
            std::string str = "ENTITY: " + uuids::to_string(m_id) + "\n\tName:";
            for (auto& property : m_properties) {
                if (property->name() == "Name") {
                    str += property->get<std::string>();
                }
            }
            str += "\n\tTRAITS:\n";
            for (auto& trait : m_traits) {
                str += "\t\t" + uuids::to_string(trait->id());
            }

            return std::move(str);
        };
        // template <typename T>
        // auto Entity::trait() & -> T*

        // Base::IdMap{}; = std::unordered_map<Id, Base*, Id::Hash>
    }  // namespace base
}  // namespace hydra