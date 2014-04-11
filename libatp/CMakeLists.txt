cmake_minimum_required(VERSION 2.6.0)

set_source_files_properties(libatp.c PROPERTIES COMPILE_FLAGS "-fPIC")

add_library(libatp STATIC libatp.c)
