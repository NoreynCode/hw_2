#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

#include "ip_address.h"
#include "ip_parser.h"
#include "ip_printer.h"

int main() {
    std::vector<ip_filter::ip_address> ip_pool;
    ip_pool.reserve(1000);

    std::string line;
    std::size_t valid_lines = 0;
    std::size_t invalid_lines = 0;

    while (std::getline(std::cin, line)) {
        ip_filter::ip_address ip;
        if (ip_filter::parse_ip_from_line(line, ip)) {
            ip_pool.push_back(ip);
            ++valid_lines;
        }
        else {
            ++invalid_lines;
            if (!line.empty()) {
                std::cerr << "Warning: Skipping invalid line: " << line << std::endl;
            }
        }
    }

    std::sort(ip_pool.begin(), ip_pool.end(), ip_filter::compare_ips_descending);

    auto filter1 = ip_filter::filter(ip_pool, ip_filter::filter_first_byte_eq(1));
    auto filter2 = ip_filter::filter(ip_pool, ip_filter::filter_first_two_bytes_eq(46, 70));
    auto filter3 = ip_filter::filter(ip_pool, ip_filter::filter_any_byte_eq(46));

    ip_filter::print_ip_collection(ip_pool);
    ip_filter::print_ip_collection(filter1);
    ip_filter::print_ip_collection(filter2);
    ip_filter::print_ip_collection(filter3);

    return 0;
}