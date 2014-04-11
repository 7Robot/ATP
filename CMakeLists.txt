cmake_minimum_required(VERSION 2.6.0)

project(7Robot)

include_directories("libatp")

add_subdirectory("libatp")
add_subdirectory("microstamp")
add_subdirectory("tcphub")
add_subdirectory("usbdaemon")
