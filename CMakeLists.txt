cmake_minimum_required (VERSION 2.8.5)

project (libosmpbf)

add_subdirectory(src)
add_subdirectory(example EXCLUDE_FROM_ALL)

include_directories("${PROJECT_SOURCE_DIR}/include")

# uninstall target
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
