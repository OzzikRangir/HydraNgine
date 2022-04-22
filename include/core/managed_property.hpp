#ifndef _HYDRA_NGINE_CORE_MANAGED_PROPERTY_HPP
#define _HYDRA_NGINE_CORE_MANAGED_PROPERTY_HPP
#include <iostream>

#include "commons.hpp"
#include "core/managed_object.hpp"
#include "core/uuid.hpp"
namespace hydra {
    namespace base {
        class ManagedObject;
        class UnmanagedProperty;
        struct PackedPropertyCreateInfo {
            UnmanagedProperty* property;
            size_t padding;
        };

        enum class PropertyType {
            Error,
            ManagedObject,
            Fundamental,
            Trivial,
            Array,
            String
        };

        template <typename T>
        class PropertyTypedData;

        class PropertyData {
        protected:
            const size_t m_typeSize;
            const std::type_index m_typeId;

            void* m_rawPtr;
            constexpr PropertyData(size_t size, std::type_index&& typeId, void* rawPtr = nullptr) : m_typeSize{size}, m_typeId{typeId}, m_rawPtr{rawPtr} {};

        public:
            template <typename T>
            constexpr inline auto ptr() & -> const T*
            {
                if (std::type_index(typeid(T)) == m_typeId) return reinterpret_cast<T*>(m_rawPtr);
                throw std::bad_cast();
            };
            template <typename T>
            inline auto typed() & -> PropertyTypedData<T>& { return dynamic_cast<PropertyTypedData<T>&>(*this); };
            constexpr inline auto rawPtr() & -> const void* { return m_rawPtr; };
            constexpr inline auto typeSize() & -> const size_t& { return m_typeSize; };
            constexpr inline auto typeId() & -> const std::type_index& { return m_typeId; };
            constexpr inline auto rawPtr() const& -> const void* { return m_rawPtr; };
            constexpr inline auto typeSize() const& -> const size_t& { return m_typeSize; };
            constexpr inline auto typeId() const& -> const std::type_index& { return m_typeId; };

            virtual ~PropertyData() = default;
            virtual auto clone() & -> PropertyData* { return new PropertyData(m_typeSize, std::type_index{m_typeId}, m_rawPtr); };
        };
        template <typename T>
        class PropertyTypedData : public virtual PropertyData {
        private:
            std::unique_ptr<T> m_uniquePtr;

        public:
            PropertyTypedData(const T& data) : m_uniquePtr{std::make_unique<T>(data)},
                                               PropertyData{sizeof(T),
                                                            std::type_index(typeid(T))} { m_rawPtr = m_uniquePtr.get(); };
            PropertyTypedData(T&& data) : m_uniquePtr{std::make_unique<T>(data)},
                                          PropertyData{sizeof(T),
                                                       std::type_index(typeid(T))} { m_rawPtr = m_uniquePtr.get(); };

            inline auto set(T&& data) & -> void
            {
                m_uniquePtr = {std::make_unique<T>(data)};
                m_rawPtr = m_uniquePtr.get();
            };
            inline auto get() & -> const T& { return *m_uniquePtr; };
            ~PropertyTypedData() override = default;
            virtual auto clone() & -> PropertyData* override { return new PropertyTypedData(get()); };
        };

        template <typename T>
        static constexpr auto PropertyTypeEnum() -> PropertyType
        {
            if (std::is_fundamental<T>()) {
                return PropertyType::Fundamental;
            }
            else if (std::is_trivial<T>()) {
                return PropertyType::Trivial;
            }
            else if (typeid(T) == typeid(std::string)) {
                return PropertyType::String;
            }
            else if (std::is_base_of<ManagedObject, T>::value) {
                return PropertyType::ManagedObject;
            }
            else if (std::is_base_of<std::vector<UnmanagedProperty>, T>::value) {
                return PropertyType::Array;
            }
            else {
                return PropertyType::Error;
            }
        };
        class PropertyTag {
        protected:
            const std::string m_name;
            const std::type_index m_typeId;
            const PropertyType m_type;
            const std::unique_ptr<PropertyData> m_defaultValue;

        public:
            template <typename T>
            inline PropertyTag(std::string&& name,
                               T&& defaultValue) : m_name{name},
                                                   m_typeId{typeid(T)},
                                                   m_type{
                                                       PropertyTypeEnum<T>()},
                                                   m_defaultValue{std::unique_ptr<PropertyData>(new PropertyTypedData<T>(defaultValue))} {};
            PropertyTag(PropertyTag&& othr) : m_name{othr.m_name}, m_typeId{othr.m_typeId}, m_type{othr.m_type}, m_defaultValue{othr.cloneDefaultData()} {};
            PropertyTag(const PropertyTag& othr) : m_name{othr.m_name}, m_typeId{othr.m_typeId}, m_type{othr.m_type}, m_defaultValue{othr.cloneDefaultData()} {};
            constexpr inline auto name() & -> const std::string& { return m_name; };
            constexpr inline auto type() & -> const PropertyType& { return m_type; };
            constexpr inline auto typeId() & -> const std::type_index& { return m_typeId; };
            inline auto operator==(const PropertyTag& other) & -> bool { return m_name == other.m_name && m_typeId == other.m_typeId && m_type == other.m_type; }
            inline auto cloneDefaultData() & -> PropertyData* { return m_defaultValue->clone(); };
            inline auto cloneDefaultData() const& -> PropertyData* { return m_defaultValue->clone(); };
        };

        class UnmanagedProperty {
        protected:
            PropertyTag* m_archetype;
            std::unique_ptr<PropertyData> m_data;

        public:
            inline UnmanagedProperty(PropertyTag* archetype) : m_archetype{archetype},
                                                               m_data{archetype->cloneDefaultData()} {};
            constexpr inline auto name() & -> const std::string& { return m_archetype->name(); }
            inline auto size() & -> const size_t& { return m_data->typeSize(); };
            inline auto data() & -> const PropertyData& { return *m_data; }
            template <typename T>
            inline auto get() & -> T { return {m_data->typed<T>().get()}; };
            template <typename T>
            inline auto set(T&& value) & -> void
            {
                if (std::type_index(typeid(T)) == m_archetype->typeId()) {
                    //             auto bindings = dynamic_cast<TypedData<T>*>(m_data.get())->bindings();
                    m_data = std::unique_ptr<PropertyData>(new PropertyTypedData<T>(value));

                    //             for (auto* ptr : bindings) {
                    //                 dynamic_cast<TypedData<T>*>(m_data.get())->bind(ptr);
                    //                 *ptr = *dynamic_cast<TypedData<T>*>(m_data.get())->get();
                    //             }
                }
                //         else
                //             throw std::exception();
            };
        };
        using Archetype = std::vector<PropertyTag>;

        class ManagedProperty : public virtual UnmanagedProperty {
        private:
            const UUID m_id;

        public:
            inline ManagedProperty(PropertyTag* archetype) : m_id{uuid::Generator::next()}, UnmanagedProperty{archetype} {};
            inline auto id() & -> const UUID& { return m_id; };
        };

        class PackedPropertyVector {
        private:
            std::vector<uint8_t> m_data;

        public:
            PackedPropertyVector(std::initializer_list<PackedPropertyCreateInfo> initilizerList)
            {
                size_t dataAllocSize = 0;
                for (auto& ppCI : initilizerList) {
                    auto tempData = std::vector<uint8_t>{};
                    size_t propertySize = ppCI.property->size();
                    size_t paddingSize = ppCI.padding;
                    auto paddingCount = propertySize / paddingSize;
                    auto dataPtr = const_cast<void*>(ppCI.property->data().rawPtr());
                    if (propertySize % paddingSize != 0) {
                        paddingCount++;
                    }
                    if (!dataPtr) {
                        throw std::logic_error("NULLPTR PROPERTY DATA");
                    }
                    tempData.reserve(paddingCount * paddingSize);

                    std::copy(reinterpret_cast<uint8_t*>(dataPtr), reinterpret_cast<uint8_t*>(dataPtr) + propertySize, std::back_inserter(tempData));
                    for (int i = tempData.size(); i < paddingCount * paddingSize; i++) {
                        tempData.push_back(0);
                    }

                    m_data.insert(m_data.end(), tempData.begin(), tempData.end());
                }
            };
            constexpr inline auto bytes() & -> const std::vector<uint8_t>& { return m_data; };
        };

    }  // namespace base
}  // namespace hydra

#endif  //_HYDRA_NGINE_CORE_MANAGED_PROPERTY_HPP