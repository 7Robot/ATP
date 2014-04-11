cmake_minimum_required(VERSION 2.6.0)

add_executable(usbdaemon usbdaemon.c settings.c ini.c devicemanager.c device.c
    getttyfd.c getsockfd.c notice.c)

target_link_libraries(usbdaemon libatp)

install(TARGETS usbdaemon RUNTIME DESTINATION bin)
