cmake_minimum_required(VERSION 3.1)
project(stream_format CXX)

if(NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 17)
endif()

file(GLOB SF_HEADERS "include/sf/*.hpp")
add_library(stream_format INTERFACE)

target_include_directories(stream_format
    INTERFACE 
        $<INSTALL_INTERFACE:include>    
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

option(SF_INSTALL "Install StreamFormat" OFF)
option(SF_BUILD_TESTS "Build tests." OFF)

if(${SF_INSTALL})
    install(FILES ${SF_HEADERS} DESTINATION include/sf)
    install(TARGETS stream_format EXPORT sf-targets)
    install(EXPORT sf-targets DESTINATION lib/cmake/sf)
    install(FILES cmake/sf-config.cmake DESTINATION lib/cmake/sf)
endif()

if(${SF_BUILD_TESTS})
    enable_testing()

    add_executable(print test/print.cpp)
    target_link_libraries(print stream_format)
    add_test(test_print print)
    set_tests_properties(test_print PROPERTIES PASS_REGULAR_EXPRESSION "Success.\n")

    add_executable(scan test/scan.cpp)
    target_link_libraries(scan stream_format)
    add_test(test_scan_1 scan "1 1" "1 + 1 = 2")
    set_tests_properties(test_scan_1 PROPERTIES PASS_REGULAR_EXPRESSION "Success.\n")
    add_test(test_scan_2 scan "111 222 333" "333")
    set_tests_properties(test_scan_2 PROPERTIES PASS_REGULAR_EXPRESSION "Success.\n")

    add_executable(color test/color.cpp)
    target_link_libraries(color stream_format)
    add_test(test_color color)
    set_tests_properties(test_color PROPERTIES PASS_REGULAR_EXPRESSION "Success.\n")
endif()
