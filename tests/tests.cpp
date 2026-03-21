#define BOOST_TEST_MODULE ip_filter_tests
#include <boost/test/unit_test.hpp>
#include <sstream>
#include "ip_address.h"
#include "ip_parser.h"
#include "ip_printer.h"

BOOST_AUTO_TEST_SUITE(ip_filter_tests)

// Парсер
BOOST_AUTO_TEST_CASE(parse_ip_from_line_test) {
    ip_filter::ip_address ip;

    BOOST_CHECK(ip_filter::parse_ip_from_line("192.168.1.1\tdata", ip));
    BOOST_CHECK_EQUAL(ip.first_octet, 192);
    BOOST_CHECK_EQUAL(ip.second_octet, 168);
    BOOST_CHECK_EQUAL(ip.third_octet, 1);
    BOOST_CHECK_EQUAL(ip.fourth_octet, 1);

    BOOST_CHECK(!ip_filter::parse_ip_from_line("", ip));
    BOOST_CHECK(!ip_filter::parse_ip_from_line("invalid", ip));
    BOOST_CHECK(!ip_filter::parse_ip_from_line("192.168.256.1", ip));
}

// split_string
BOOST_AUTO_TEST_CASE(split_string_test) {
    auto result = ip_filter::split_string("192.168.1.1", '.');
    BOOST_CHECK_EQUAL(result.size(), 4);
    BOOST_CHECK_EQUAL(result[0], "192");
    BOOST_CHECK_EQUAL(result[3], "1");
}

// Сравнение IP
BOOST_AUTO_TEST_CASE(ip_comparison_test) {
    ip_filter::ip_address ip1{ 1, 2, 3, 4 };
    ip_filter::ip_address ip2{ 1, 2, 3, 4 };
    ip_filter::ip_address ip3{ 1, 2, 3, 5 };

    BOOST_CHECK(ip1 == ip2);
    BOOST_CHECK(!(ip1 == ip3));
    BOOST_CHECK(ip_filter::compare_ips_descending(ip3, ip1));
}

// Печать
BOOST_AUTO_TEST_CASE(print_ip_test) {
    ip_filter::ip_address ip{ 192, 168, 1, 1 };
    std::ostringstream oss;

    ip_filter::print_ip(ip, oss);
    BOOST_CHECK_EQUAL(oss.str(), "192.168.1.1\n");
}

// Фильтрация с make_ip_filter
BOOST_AUTO_TEST_CASE(make_ip_filter_test) {
    std::vector<ip_filter::ip_address> ips = {
        {1, 2, 3, 4},
        {46, 70, 10, 20},
        {46, 71, 46, 30},
        {10, 0, 0, 46},
        {46, 70, 15, 25},
        {46, 70, 10, 21}
    };

    // 1 аргумент
    auto filter1 = ip_filter::filter(ips, ip_filter::make_ip_filter(46));
    BOOST_CHECK_EQUAL(filter1.size(), 4); // 46.70.10.20, 46.71.46.30, 46.70.15.25, 46.70.10.21

    // 2 аргумента
    auto filter2 = ip_filter::filter(ips, ip_filter::make_ip_filter(46, 70));
    BOOST_CHECK_EQUAL(filter2.size(), 3); // 46.70.10.20, 46.70.15.25, 46.70.10.21

    // 3 аргумента
    auto filter3 = ip_filter::filter(ips, ip_filter::make_ip_filter(46, 70, 10));
    BOOST_CHECK_EQUAL(filter3.size(), 2); // 46.70.10.20, 46.70.10.21

    // 4 аргумента
    auto filter4 = ip_filter::filter(ips, ip_filter::make_ip_filter(46, 70, 10, 20));
    BOOST_CHECK_EQUAL(filter4.size(), 1); // 46.70.10.20
    BOOST_CHECK(filter4[0] == (ip_filter::ip_address{ 46, 70, 10, 20 }));
}

// Любой байт
BOOST_AUTO_TEST_CASE(filter_any_byte_test) {
    std::vector<ip_filter::ip_address> ips = {
        {1, 2, 3, 4},
        {46, 70, 10, 20},
        {46, 71, 46, 30},
        {10, 0, 0, 46}
    };

    auto filter = ip_filter::filter(ips, ip_filter::filter_any_byte_eq(46));
    BOOST_CHECK_EQUAL(filter.size(), 3); // 46.70.10.20, 46.71.46.30, 10.0.0.46
}

BOOST_AUTO_TEST_CASE(integration_test) {
    std::vector<std::string> input = {
        "1.2.3.4",
        "46.70.10.20",
        "invalid",
        "46.71.46.30"
    };

    std::vector<ip_filter::ip_address> pool;

    for (const auto& line : input) {
        ip_filter::ip_address ip;
        if (ip_filter::parse_ip_from_line(line, ip)) {
            pool.push_back(ip);
        }
    }

    BOOST_CHECK_EQUAL(pool.size(), 3);

    std::sort(pool.begin(), pool.end(), ip_filter::compare_ips_descending);
    BOOST_CHECK(pool[0] == (ip_filter::ip_address{ 46, 71, 46, 30 }));

    auto filtered = ip_filter::filter(pool, ip_filter::make_ip_filter(46));
    BOOST_CHECK_EQUAL(filtered.size(), 2);
}

BOOST_AUTO_TEST_SUITE_END()