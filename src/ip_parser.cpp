#include <algorithm>
#include <cctype>
#include <charconv>
#include <system_error>

#include "ip_parser.h"

namespace ip_filter {

    std::vector<std::string> split_string(std::string_view str, char delimiter) {
        std::vector<std::string> result;
        result.reserve(std::count(str.begin(), str.end(), delimiter) + 1);

        std::string_view::size_type start = 0;
        std::string_view::size_type end = str.find(delimiter);

        while (end != std::string_view::npos) {
            result.emplace_back(str.substr(start, end - start));
            start = end + 1;
            end = str.find(delimiter, start);
        }

        result.emplace_back(str.substr(start));
        return result;
    }

    bool string_to_byte(const std::string& str, std::uint8_t& out_byte) {
        if (str.empty() || str.length() > 3) return false;

        for (char c : str) {
            if (!std::isdigit(static_cast<unsigned char>(c))) return false;
        }

        // Для преобразования без исключений
        int value = 0;
        auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), value);

        if (ec != std::errc() || ptr != str.data() + str.size()) return false;
        if (value < 0 || value > 255) return false;

        out_byte = static_cast<std::uint8_t>(value);
        return true;
    }

    bool parse_ip_from_strings(const std::vector<std::string>& octets, ip_address& out_ip) {
        if (octets.size() != 4) return false;

        std::uint8_t first, second, third, fourth;
        if (!string_to_byte(octets[0], first)) return false;
        if (!string_to_byte(octets[1], second)) return false;
        if (!string_to_byte(octets[2], third)) return false;
        if (!string_to_byte(octets[3], fourth)) return false;

        out_ip = ip_address{ first, second, third, fourth };
        return true;
    }

    bool parse_ip_from_line(const std::string& line, ip_address& out_ip) {
        if (line.empty()) return false;

        std::vector<std::string> tab_split = split_string(line, '\t');
        if (tab_split.empty()) return false;

        std::vector<std::string> ip_octets = split_string(tab_split[0], '.');
        return parse_ip_from_strings(ip_octets, out_ip);
    }
}