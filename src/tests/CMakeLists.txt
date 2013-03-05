include_directories (${OneJoker_SOURCE_DIR}/lib) 
link_directories (${OneJoker_BINARY_DIR}/lib) 

add_executable (test001 test001.c) 
target_link_libraries (test001 onejoker) 

add_executable (test002 test002.c) 
target_link_libraries (test002 onejoker) 

add_executable (test003 test003.c) 
target_link_libraries (test003 onejoker) 

add_executable (test004 test004.c) 
target_link_libraries (test004 onejoker) 

add_executable (test005 test005.c) 
target_link_libraries (test005 onejoker) 

