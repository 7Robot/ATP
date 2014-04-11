cmake_minimum_required(VERSION 2.6.0)

set_source_files_properties(checktype.c PROPERTIES COMPILE_FLAGS "-fPIC")

add_executable(microstamp microstamp.c)

target_link_libraries(microstamp libatp)

install(TARGETS microstamp RUNTIME DESTINATION bin)
