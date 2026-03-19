cmake_minimum_required(VERSION 3.12)

set(PATCH_VERSION "1" CACHE STRING "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(IpFilter VERSION ${PROJECT_VERSION})

option(WITH_BOOST_TEST "Whether to build Boost test" ON)

# Библиотека ip_filter_lib
add_library(ip_filter_lib STATIC
    src/ip_address.cpp
    src/ip_parser.cpp
    src/ip_printer.cpp
)

target_include_directories(ip_filter_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set_target_properties(ip_filter_lib PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

# Основной исполняемый файл
add_executable(ip_filter src/main.cpp)

set_target_properties(ip_filter PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(ip_filter
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(ip_filter PRIVATE
    ip_filter_lib
)

if(WITH_BOOST_TEST)
    find_package(Boost COMPONENTS unit_test_framework REQUIRED)
    
    add_executable(ip_filter_tests tests/tests.cpp)
    
    set_target_properties(ip_filter_tests PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
    )
    
    target_include_directories(ip_filter_tests
        PRIVATE 
            ${CMAKE_CURRENT_SOURCE_DIR}/include
    )
    
    set_target_properties(ip_filter_tests PROPERTIES
        COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
    )
    
    target_link_libraries(ip_filter_tests
        ${Boost_LIBRARIES}
        ip_filter_lib
    )
endif()

if (MSVC)
    target_compile_options(ip_filter PRIVATE /W4)
    target_compile_options(ip_filter_lib PRIVATE /W4)
    if(WITH_BOOST_TEST)
        target_compile_options(ip_filter_tests PRIVATE /W4)
    endif()
else ()
    target_compile_options(ip_filter PRIVATE -Wall -Wextra -pedantic -Werror)
    target_compile_options(ip_filter_lib PRIVATE -Wall -Wextra -pedantic -Werror)
    if(WITH_BOOST_TEST)
        target_compile_options(ip_filter_tests PRIVATE -Wall -Wextra -pedantic -Werror)
    endif()
endif()

install(TARGETS ip_filter RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_NAME "ip_filter")  # Явно указываем имя пакета
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)

if(WITH_BOOST_TEST)
    enable_testing()
    add_test(ip_filter_tests ip_filter_tests)
endif()