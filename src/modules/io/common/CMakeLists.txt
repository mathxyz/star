file(GLOB SRCS *.cpp *.cxx *.cc *.c)
list(REMOVE_ITEM SRCS "${CMAKE_CURRENT_SOURCE_DIR}/MakeTable.cxx")
#message(STATUS "${SRCS}")
message(FATAL_ERROR "CHECK")

add_library(lily_io SHARED ${SRCS})

set(LILY_BASE ${LILY_BASE} lily_io PARENT_SCOPE)
set(LILY_LIBS ${LILY_LIBS} lily_io PARENT_SCOPE)

install(TARGETS lily_io DESTINATION bin)
file(GLOB HEADERS *.h)
install(FILES ${HEADERS} DESTINATION include)