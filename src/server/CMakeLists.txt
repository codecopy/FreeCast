list(APPEND server_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/server.c
    )
#add_library(freecast-server SHARED ${server_sources})
add_executable(freecast-server "${CMAKE_CURRENT_SOURCE_DIR}/server.c")
install(TARGETS freecast-server DESTINATION ${INSTALL_DIR})
