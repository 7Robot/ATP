cmake_minimum_required(VERSION 2.6.0)

project(TcpHub C)

add_subdirectory(decoder)
add_executable(hubmanager hubmanager.c)

add_executable(tcphub tcphub.c server.c client.c decoder.c)

target_link_libraries(tcphub dl)

install(TARGETS tcphub hubmanager RUNTIME DESTINATION bin)
