#ifndef IP_PRINTER
#define IP_PRINTER

#include <ostream>
#include <vector>
#include <functional>
#include <type_traits>
#include <tuple>
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

    // Генератор с переменным количеством аргументов
    template<typename... Bytes>
    auto make_ip_filter(Bytes... bytes) {
        static_assert(sizeof...(bytes) >= 1 && sizeof...(bytes) <= 4,
            "make_ip_filter accepts 1 to 4 arguments");

        static_assert((std::is_integral_v<std::decay_t<Bytes>> && ...),
            "All arguments must be integral types");

        // Все аргументы в uint8_t и сохранение в tuple
        auto values = std::make_tuple(static_cast<std::uint8_t>(bytes)...);
        constexpr size_t count = sizeof...(bytes);

        if constexpr (count == 1) {
            return [first = std::get<0>(values)](const ip_address& ip) {
                return ip.first_octet == first;
                };
        }
        else if constexpr (count == 2) {
            return [first = std::get<0>(values),
                second = std::get<1>(values)](const ip_address& ip) {
                return ip.first_octet == first && ip.second_octet == second;
                };
        }
        else if constexpr (count == 3) {
            return [first = std::get<0>(values),
                second = std::get<1>(values),
                third = std::get<2>(values)](const ip_address& ip) {
                return ip.first_octet == first &&
                    ip.second_octet == second &&
                    ip.third_octet == third;
                };
        }
        else { // count == 4
            return [first = std::get<0>(values),
                second = std::get<1>(values),
                third = std::get<2>(values),
                fourth = std::get<3>(values)](const ip_address& ip) {
                return ip.first_octet == first &&
                    ip.second_octet == second &&
                    ip.third_octet == third &&
                    ip.fourth_octet == fourth;
                };
        }
    }

    // Фильтр по наличию байта в любом месте
    inline auto filter_any_byte_eq(std::uint8_t value) {
        return [value](const ip_address& ip) {
            return ip.first_octet == value || ip.second_octet == value ||
                ip.third_octet == value || ip.fourth_octet == value;
            };
    }
}
#endif