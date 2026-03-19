#ifndef IP_ADDRESS
#define IP_ADDRESS

#include <cstdint>
#include <tuple>

namespace ip_filter {

    struct ip_address {
        std::uint8_t first_octet;
        std::uint8_t second_octet;
        std::uint8_t third_octet;
        std::uint8_t fourth_octet;
    };

    bool operator==(const ip_address& lhs, const ip_address& rhs);
    bool compare_ips_descending(const ip_address& lhs, const ip_address& rhs);
}
#endif