cmake_minimum_required (VERSION 2.6)
project (OneJoker)

include_directories (${OneJoker_SOURCE_DIR}/include)
link_directories (${OneJoker_BINARY_DIR}/lib)

add_subdirectory (lib)
add_subdirectory (tests)

