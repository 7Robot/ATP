#ifndef _DEVICE_MANAGER_H
#define _DEVICE_MANAGER_H

#include <sys/select.h>

// Add device to managed devices list
void dm_add(const char *device);

// Add fd to fd set for reading and writting
void dm_select(fd_set *readfds, fd_set *writefds);

// Manage events for fd in readfds and writefds
void dm_event(fd_set *readfds, fd_set *writefds);

// Remove device from managed devices list
void dm_del(int offset);

#endif // _DEVICE_MANAGER_H
