#include "ip_address.h"

namespace ip_filter {

    bool operator==(const ip_address& lhs, const ip_address& rhs) {
        return std::tie(lhs.first_octet, lhs.second_octet, lhs.third_octet, lhs.fourth_octet) ==
            std::tie(rhs.first_octet, rhs.second_octet, rhs.third_octet, rhs.fourth_octet);
    }

    bool compare_ips_descending(const ip_address& lhs, const ip_address& rhs) {
        return std::tie(lhs.first_octet, lhs.second_octet, lhs.third_octet, lhs.fourth_octet) >
            std::tie(rhs.first_octet, rhs.second_octet, rhs.third_octet, rhs.fourth_octet);
    }
}