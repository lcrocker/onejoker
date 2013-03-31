# CMakeLists.txt file for subdirectory "src" of OneJoker project.
# This directory contains all the C code.
#
include_directories(${OneJoker_SOURCE_DIR}/src/include)

add_subdirectory(lib)
add_subdirectory(tests)
