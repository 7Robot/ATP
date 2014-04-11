cmake_minimum_required(VERSION 2.6.0)

project(Asserv C)

add_library(asserv SHARED odo.c pid.c asserv.c motion.c
    ramp_speed.c ramp_dist.c)

add_subdirectory(test)

#install(TARGETS asserv LIBRARY DESTINATION lib)
