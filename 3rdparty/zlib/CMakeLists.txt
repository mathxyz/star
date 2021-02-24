set(LILY_MODULE lily_z)

file(GLOB SRCS *.cpp *.cxx *.c)
#message(STATUS "${SRCS}")


if (ANDROID)
    add_library(${LILY_MODULE} SHARED ${SRCS})
else ()
    add_library(${LILY_MODULE} STATIC ${SRCS})
endif ()

target_link_libraries(${LILY_MODULE} ${LILY_LIBS})

set(LILY_LIBS ${LILY_LIBS} ${LILY_MODULE} PARENT_SCOPE)

install(TARGETS ${LILY_MODULE}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static)

#install(TARGETS ${LILY_MODULE}
#        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
#        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/static)
