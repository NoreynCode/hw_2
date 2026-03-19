#ifndef IP_PARSER
#define IP_PARSER

#include "ip_address.h"
#include <string>
#include <vector>

namespace ip_filter {

    std::vector<std::string> split_string(std::string_view str, char delimiter);
    bool string_to_byte(const std::string& str, std::uint8_t& out_byte);
    bool parse_ip_from_strings(const std::vector<std::string>& octets, ip_address& out_ip);
    bool parse_ip_from_line(const std::string& line, ip_address& out_ip);
}
#endif