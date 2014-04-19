cmake_minimum_required(VERSION 2.6.0)

add_library(jsread SHARED jsread.c)

install(TARGETS jsread LIBRARY DESTINATION lib)
