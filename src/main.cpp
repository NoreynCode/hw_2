#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

using ip_type = std::vector<std::string>;

void print_ip(const std::vector<ip_type> ip_pool) {
    for(const auto& ip: ip_pool) {
        std::string addres;
        for(const auto& ip_part: ip) {
            if(!addres.empty()) addres += '.';
            addres += ip_part;
        }
        std::cout << addres << '\n';
    }
}

ip_type split(const std::string &str, char separator)
{
    ip_type result_ip;
    std::stringstream stream(str);
    std::string token;

    // Более читабельно и для используемых данных не большая раздница в производительности
    while(std::getline(stream, token, separator)) {
        result_ip.push_back(token);
    }

    return result_ip;
}

// Сортировка в обратном лексикографическом порядке
void revers_sort(std::vector<ip_type>& ip_pool) {
    std::sort(ip_pool.begin(), ip_pool.end(), [](const ip_type& first, const ip_type& second){
        for(int item_index = 0; item_index < first.size(); ++item_index)
        {
            int f_item = std::stoi(first[item_index]);
            int s_item = std::stoi(second[item_index]);

            if(f_item != s_item) return f_item > s_item;
        }
        return false;
    });
    print_ip(ip_pool);
}

// Первый байт 1
void sort_first_one(std::vector<ip_type>& ip_pool) {
    std::vector<ip_type> first_one;
    for(const auto& ip: ip_pool) {
        if (std::stoi(ip[0]) == 1) first_one.emplace_back(ip);
    }
    print_ip(first_one);
}

// Первый байт 46, второй 70
void sort_first_fs_second_st(std::vector<ip_type>& ip_pool) {
    std::vector<ip_type> first_one;
    for(const auto& ip: ip_pool) {
        if (std::stoi(ip[0]) == 46 && std::stoi(ip[1]) == 70) first_one.emplace_back(ip);
    }
    print_ip(first_one);
}

// Любой байт 46
void sort_any_fs(std::vector<ip_type>& ip_pool) {
std::vector<ip_type> first_one;
    for(const auto& ip: ip_pool) {
       for(const auto& ip_item: ip) {
        if(std::stoi(ip_item) == 46) first_one.emplace_back(ip);
       }        
    }
    print_ip(first_one);
}

int main(int argc, char const *argv[])
{
    try {
        std::vector<ip_type> ip_pool;

        for(std::string line; std::getline(std::cin, line);) {
            ip_pool.push_back(split(split(line, '\t').at(0), '.'));
        }

        revers_sort(ip_pool);
        sort_first_one(ip_pool);
        sort_first_fs_second_st(ip_pool);
        sort_any_fs(ip_pool);
    }
    catch(const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
