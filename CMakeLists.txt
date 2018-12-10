cmake_minimum_required(VERSION 3.1)
project(StreamFormat)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
include_directories(include)

function(add_headers VAR)
  set(headers ${${VAR}})
  foreach (header ${ARGN})
    set(headers ${headers} include/sf/${header})
  endforeach()
  set(${VAR} ${headers} PARENT_SCOPE)
endfunction()

add_headers(SF_HEADERS utility.hpp ansi.hpp color.hpp string_view.hpp format.hpp sformat.hpp)

install(FILES ${SF_HEADERS} DESTINATION include/sf)

enable_testing()

add_executable(print test/print.cpp)
add_test(test_print print)
set_tests_properties(test_print PROPERTIES PASS_REGULAR_EXPRESSION "Success.\n")

add_executable(scan test/scan.cpp)
add_test(test_scan_1 scan "1 1" "1 + 1 = 2")
set_tests_properties(test_scan_1 PROPERTIES PASS_REGULAR_EXPRESSION "Success.\n")
add_test(test_scan_2 scan "111 222 333" "333")
set_tests_properties(test_scan_2 PROPERTIES PASS_REGULAR_EXPRESSION "Success.\n")

add_executable(color test/color.cpp)
add_test(test_color color)
set_tests_properties(test_color PROPERTIES PASS_REGULAR_EXPRESSION "Success.\n")
