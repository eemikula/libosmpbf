cmake_minimum_required (VERSION 2.8.5)

project (libosmpbf_examples)

include_directories("${PROJECT_SOURCE_DIR}/include")

add_executable(example_static example.cpp)
target_link_libraries(example_static osmpbf_static z)

add_executable(example_shared example.cpp)
target_link_libraries(example_shared osmpbf z protobuf)
