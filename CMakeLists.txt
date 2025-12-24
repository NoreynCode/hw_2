cmake_minimum_required(VERSION 3.12)
project(ipfilter)

add_executable(ip_filter src/main.cpp)
target_include_directories(ip_filter PRIVATE include)