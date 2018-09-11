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

add_headers(SF_HEADERS utility.hpp ansi.hpp format.hpp sformat.hpp)

install(FILES ${SF_HEADERS} DESTINATION include/sf)

enable_testing()
add_executable(basic_print test/basic_print.cpp)
add_test(test_basic_print basic_print)
set_tests_properties(test_basic_print PROPERTIES PASS_REGULAR_EXPRESSION "\n3342\nHello, world!\n123{321}123\n")
