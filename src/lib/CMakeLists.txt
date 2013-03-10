set(CMAKE_SHARED_LINKER_FLAGS "-nostartfiles")
add_library(onejoker SHARED onejoker.c prng.c sequence.c combinatorics.c cardnames.c poker.c)
