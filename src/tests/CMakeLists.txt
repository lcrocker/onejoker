# CMakeLists.txt file for subdirectory "src/tests" of OneJoker project.
# This contains C sources for test code of the C library.
#
link_directories(${OneJoker_BINARY_DIR}/src/lib)

add_executable(randomization randomization.c)
target_link_libraries(randomization onejoker m)
add_test(randomization randomization)

add_executable(sequences sequences.c)
target_link_libraries(sequences onejoker)
add_test(sequences sequences)

add_executable(combinations combinations.c)
target_link_libraries(combinations onejoker m)
add_test(combinations combinations)

add_executable(test004 test004.c)
target_link_libraries(test004 onejoker)

add_executable(test005 test005.c)
target_link_libraries(test005 onejoker)
