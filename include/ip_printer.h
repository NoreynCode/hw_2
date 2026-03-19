#ifndef IP_PRINTER
#define IP_PRINTER

#include <ostream>
#include <vector>
#include <functional>
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

    // Вспомогательные функции для фильтров
    inline auto filter_first_byte_eq(std::uint8_t value) {
        return [value](const ip_address& ip) { return ip.first_octet == value; };
    }

    inline auto filter_first_two_bytes_eq(std::uint8_t first, std::uint8_t second) {
        return [first, second](const ip_address& ip) {
            return ip.first_octet == first && ip.second_octet == second;
            };
    }

    inline auto filter_any_byte_eq(std::uint8_t value) {
        return [value](const ip_address& ip) {
            return ip.first_octet == value || ip.second_octet == value ||
                ip.third_octet == value || ip.fourth_octet == value;
            };
    }
}
#endif