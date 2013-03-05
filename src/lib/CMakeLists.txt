set(CMAKE_SHARED_LINKER_FLAGS "-nostartfiles")
add_library(onejoker SHARED onejoker.c prng.c poker.c)
