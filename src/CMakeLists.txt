cmake_minimum_required (VERSION 2.8.5)

find_package(ZLIB REQUIRED)

add_subdirectory(protobuf)

include_directories("${PROJECT_SOURCE_DIR}/include")
include_directories("${PROJECT_SOURCE_DIR}/src/protobuf")
include_directories("${ProtobufIncludePath}")

add_library(osmpbf_static STATIC libosmpbf.cpp)
target_link_libraries(osmpbf_static ${zlib_LIBRARIES} ${PROTOBUF_LIBRARY} messages)

add_library(osmpbf SHARED libosmpbf.cpp)
target_link_libraries(osmpbf ${zlib_LIBRARIES} ${PROTOBUF_LIBRARY} messages)

install (TARGETS osmpbf DESTINATION lib)
install (FILES "${PROJECT_SOURCE_DIR}/include/libosmpbf.h" DESTINATION include)
