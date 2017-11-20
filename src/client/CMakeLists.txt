list(APPEND client_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/client.c
    )
#add_library(freecast-client SHARED ${client_sources})
add_executable(freecast-client "${CMAKE_CURRENT_SOURCE_DIR}/client.c")
install(TARGETS freecast-client DESTINATION ${INSTALL_DIR})