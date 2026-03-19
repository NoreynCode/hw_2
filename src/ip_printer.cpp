#include <iostream>
#include <algorithm>
#include <iterator>

#include "ip_printer.h"

namespace ip_filter {

    void print_ip(const ip_address& ip, std::ostream& output) {
        output << static_cast<int>(ip.first_octet) << '.'
            << static_cast<int>(ip.second_octet) << '.'
            << static_cast<int>(ip.third_octet) << '.'
            << static_cast<int>(ip.fourth_octet) << '\n';
    }

    std::vector<ip_address> filter(const std::vector<ip_address>& pool,
        std::function<bool(const ip_address&)> predicate) {
        std::vector<ip_address> result;
        result.reserve(pool.size());
        std::copy_if(pool.begin(), pool.end(), std::back_inserter(result), predicate);
        result.shrink_to_fit();
        return result;
    }
}