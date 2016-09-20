cmake_minimum_required (VERSION 2.8.5)

find_package(Protobuf REQUIRED)
PROTOBUF_GENERATE_CPP(ProtoSources ProtoHeaders "${CMAKE_CURRENT_SOURCE_DIR}/osm.proto")

add_library(messages STATIC ${ProtoSources} ${ProtoHeaders})
target_link_libraries(messages ${zlib_LIBRARIES} ${PROTOBUF_LIBRARY})

# To fix compilation problem: relocation R_X86_64_32 against `a local symbol' can not be
# used when making a shared object; recompile with -fPIC
# See http://www.cmake.org/pipermail/cmake/2007-May/014350.html
#
IF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
  SET_TARGET_PROPERTIES(messages PROPERTIES COMPILE_FLAGS "-fPIC")
ENDIF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )

set(ProtobufIncludePath ${CMAKE_CURRENT_BINARY_DIR}/../ CACHE INTERNAL "Path to generated protobuf files.")
