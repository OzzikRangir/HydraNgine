#ifndef _HYDRA_NGINE_COMMONS_HPP
#define _HYDRA_NGINE_COMMONS_HPP
#include <algorithm>
#include <cstdint>
#include <list>
#include <map>
#include <memory>
#include <set>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <typeindex>
#include <typeinfo>
// #include <cstdint>
#include <nlohmann/json.hpp>
#include <unordered_map>
#include <unordered_set>
#include <vector>
namespace hydra {
    using JSON = nlohmann::json;
    struct size {
        constexpr auto static GiB(size_t size) -> size_t { return 1024 * 1024 * 1024 * size; };
        constexpr auto static MiB(size_t size) -> size_t { return 1024 * 1024 * size; };
        constexpr auto static KiB(size_t size) -> size_t { return 1024 * size; };

        constexpr auto static GB(size_t size) -> size_t { return 1000 * 1000 * 1000 * size; };
        constexpr auto static MB(size_t size) -> size_t { return 1000 * 1000 * size; };
        constexpr auto static KB(size_t size) -> size_t { return 1000 * size; };
    };
}  // namespace hydra
#endif  // !_HYDRA_NGINE_COMMONS_H