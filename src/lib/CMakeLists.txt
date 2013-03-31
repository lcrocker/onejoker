# CMakeLists.txt file for subdirectory "src/lib" of OneJoker project.
# This contains C sources for the shared library.
#

set(CMAKE_CXX_FLAGS "-v -g -Wall")
set(CMAKE_SHARED_LINKER_FLAGS "-nostartfiles")
set(OJ_HEADERS onejoker.h bctable.h ldctables.h)
set(OJ_CFILES onejoker.c deckinfo.c prng.c sequence.c combinatorics.c poker.c)

add_library(onejoker SHARED ${OJ_CFILES})
# install(TARGETS onejoker LIBRARY DESTINATION lib)
