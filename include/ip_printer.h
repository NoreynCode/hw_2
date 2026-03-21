#ifndef IP_PRINTER
#define IP_PRINTER

#include <ostream>
#include <vector>
#include <functional>
#include <type_traits>
#include <tuple>
#include <algorithm>
#include <array>
#include "ip_address.h"

namespace ip_filter {

    void print_ip(const ip_address& ip, std::ostream& output);

    template <typename Collection>
    void print_ip_collection(const Collection& collection, std::ostream& output) {
        for (const auto& item : collection) {
            print_ip(item, output);
        }
    }

    template <typename Collection>
    void print_ip_collection(const Collection& collection) {
        print_ip_collection(collection, std::cout);
    }

    // Универсальная функция фильтрации
    std::vector<ip_address> filter(const std::vector<ip_address>& pool,
        std::function<bool(const ip_address&)> predicate);

    template<typename... Bytes>
    auto make_ip_filter(Bytes... bytes) {
        static_assert(sizeof...(bytes) >= 1 && sizeof...(bytes) <= 4,
            "make_ip_filter accepts 1 to 4 arguments");
        static_assert((std::is_integral_v<std::decay_t<Bytes>> && ...),
            "All arguments must be integral types");

        std::vector<std::uint8_t> values = { static_cast<std::uint8_t>(bytes)... };

        // Cравние первых N октетов
        return [values = std::move(values)](const ip_address& ip) {
            std::array<std::uint8_t, 4> ip_arr{
                ip.first_octet, ip.second_octet, ip.third_octet, ip.fourth_octet
            };
            
            return std::equal(values.begin(), values.end(), ip_arr.begin());
            };
    }

    // Наличие байта в любом месте
    inline auto filter_any_byte_eq(std::uint8_t value) {
        return [value](const ip_address& ip) {
            return ip.first_octet == value || ip.second_octet == value ||
                ip.third_octet == value || ip.fourth_octet == value;
            };
    }
}
#endif