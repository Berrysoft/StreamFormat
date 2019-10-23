cmake_minimum_required(VERSION 3.1)
project(stream_format)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
include_directories(include)

file(GLOB SF_HEADERS "include/sf/*.hpp")
add_library(stream_format INTERFACE)

install(FILES ${SF_HEADERS} DESTINATION include/sf)
install(TARGETS stream_format EXPORT sf_targets)

include(CMakePackageConfigHelpers)

export(EXPORT sf_targets
  FILE "${CMAKE_CURRENT_BINARY_DIR}/sf/sf-targets.cmake"
  NAMESPACE sf::
)
configure_file(cmake/sf-config.cmake
  "${CMAKE_CURRENT_BINARY_DIR}/sf/sf-config.cmake"
  COPYONLY
)

set(ConfigPackageLocation lib/sf)
install(EXPORT sf_targets
  FILE
    sf-targets.cmake
  NAMESPACE
    sf::
  DESTINATION
    ${ConfigPackageLocation}
)
install(
  FILES
    cmake/sf-config.cmake
  DESTINATION
    ${ConfigPackageLocation}
  COMPONENT
    Devel
)

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
