cmake_minimum_required(VERSION 2.6)
project(OneJoker)
set(PACKAGE_VERSION "0.1")

include_directories(${OneJoker_SOURCE_DIR}/src/include)
link_directories(${OneJoker_BINARY_DIR}/src/lib)

add_subdirectory(src)
add_subdirectory(python)
