#ifndef _HYDRA_NGINE_CORE_ASSETS_HPP
#define _HYDRA_NGINE_CORE_ASSETS_HPP
#include "commons.hpp"
#include "core/managed_object.hpp"
#include "core/resources.hpp"
namespace hydra {
    namespace base {
        class Asset : public virtual ManagedObject {
        public:
            enum class AssetType {
                Undefined,
                Material,
                Sound,
                Image,
                Mesh,
                Extension
            };
            Asset() : ManagedObject(){};
            inline auto type() & -> const ObjectType override { return ObjectType::Asset; };
            virtual inline auto assetType() & -> const AssetType { return AssetType::Undefined; };
            virtual auto dump() & -> std::string override;
        };

        class Material : public virtual Asset {
        public:
            struct StageData {
                Resource descriptor;
                Resource code;
            };

        private:
            std::list<StageData> m_stages;

        public:
            Material() : Asset(){};
            inline auto stages() & -> std::list<StageData>& { return m_stages; };
            inline auto assetType() & -> const AssetType override { return Asset::AssetType::Material; };
            virtual auto dump() & -> std::string override;
        };
        class Image : public Asset {
        public:
            enum class Format {
                RGB,
                RGBA_32,
            };

        private:
            std::vector<unsigned char> m_data;
            uint32_t m_height;
            uint32_t m_width;
            Format m_format;

        public:
            Image() = default;
            Image(unsigned char* dataPtr, uint32_t width, uint32_t height, Format format) : m_data{dataPtr, dataPtr + width * height * 4}, m_width{width}, m_height{height}, m_format{format} {};
            inline auto data() & -> std::vector<unsigned char>&;
            inline auto height() & -> uint32_t&;
            inline auto width() & -> uint32_t&;
        };
        inline auto Image::data() & -> std::vector<unsigned char>& { return m_data; };
        inline auto Image::height() & -> uint32_t& { return m_height; };
        inline auto Image::width() & -> uint32_t& { return m_width; }
    }  // namespace base
}  // namespace hydra
#endif  //_HYDRA_NGINE_CORE_ASSETS_HPP
