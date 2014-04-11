#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/time.h>

#include "devicemanager.h"
#include "device.h"
#include "notice.h"

#define MAX_DEVICES 32
#define TIMER       500000

static int *delay;
static char **path;
static int pathc = 0;
static int paths = 0;
static int next = -1;

static struct device *devices[MAX_DEVICES];
static int devc = 0;

static void install_handler();
static void set_timer(int delay);
static void handler(int signal);

static void set_timer(int delay)
{
    struct itimerval timer;
    next = delay;
    timer.it_value.tv_sec = next / 1000000;
    timer.it_value.tv_usec = next % 1000000;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 0;
    if (setitimer(ITIMER_REAL, &timer, NULL) < 0) {
        perror("setitimer");
        exit(EXIT_FAILURE); // FIXME
    }
}

static void install_handler()
{
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = &handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGALRM, &sa, NULL) < 0) {
        perror("sigaction");
        exit(EXIT_FAILURE); // FIXME
    }
}

static void handler(int signal)
{
    int i, t;
    int min = TIMER;

    for (i = 0 ; i < pathc ; i++) {
        delay[i] -= next;
        if (delay[i] <= 0) {
            dm_add(path[i]);
            free(path[i]);
            memmove(path+i, path+i+1, (pathc-i-1) * sizeof(*path));
            memmove(delay+i, delay+i+1, (pathc-i-1) * sizeof(*delay));
            pathc--;
            i--;
        } else {
            if (delay[i] < min)
                min = delay[i];
        }
    }

    if (min < TIMER) {
        set_timer(min);
    }
}

void dm_scheduledadd(const char *device)
{
    int i, t;
    int min = TIMER;
    static int handler = 0;
    struct itimerval timer;
    sigset_t sigset_old, sigset_mask;

    if (!handler) {
        install_handler();
    }

    sigemptyset(&sigset_mask);
    sigaddset(&sigset_mask, SIGALRM);
    sigprocmask(SIG_BLOCK, &sigset_mask, &sigset_old);

    for (i = 0 ; i < pathc ; i++) {
        if (!strcmp(device, path[i])) {
            if (getitimer(ITIMER_REAL, &timer) < 0) {
                perror("getitimer");
                exit(EXIT_FAILURE); // FIXME
            }
            t = timer.it_value.tv_usec + 1000000 * timer.it_value.tv_sec;
            delay[i] = TIMER + next - t;
            return;
        }
    }

    notice(NEW, device);

    if (pathc == paths) {
        paths += 5;
        if ((delay = realloc(delay, paths * sizeof(int))) == NULL) {
            perror("realloc");
            exit(EXIT_FAILURE);
        }
        if ((path = realloc(path, paths * sizeof(char*))) == NULL) {
            perror("realloc");
            exit(EXIT_FAILURE);
        }
    }
    if ((path[pathc] = malloc(strlen(device) * sizeof(char) + 1)) == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    strcpy(path[pathc], device);
    delay[pathc] = TIMER;
    pathc++;
    
    if (pathc == 1) {
        set_timer(TIMER);
    }

    sigprocmask(SIG_SETMASK, &sigset_old, NULL);
}

void dm_add(const char *path)
{
    if (devc == MAX_DEVICES) {
        fprintf(stderr, "%s: too many device\n", __FUNCTION__);
        notice(ADDFAIL, path);
        return;
    }

    if (device_new(devices+devc, path) < 0) {
        notice(ADDFAIL, path);
        return;
    }

    notice(ADD, path);

    devc++;
}

void dm_select(fd_set *readfds, fd_set *writefds)
{
    int i;
    for (i = 0 ; i < devc ; i++) {
        struct device *dev = devices[i];
        if (BUF_SIZE - dev->fbufsize > 0) {
            FD_SET(dev->fd, readfds);
        }
        if (BUF_SIZE - dev->sbufsize > 0 && dev->state == CONNECTED) {
            FD_SET(dev->sock, readfds);
        }
        if (dev->sbufsize > 0) {
            FD_SET(dev->fd, writefds);
        }
        if (dev->fbufsize > 0 && dev->state == CONNECTED) {
            FD_SET(dev->sock, writefds);
        }
    }
}

void dm_event(fd_set *readfds, fd_set *writefds)
{
    int i = 0;

    while (i < devc) {
        struct device *dev = devices[i];
        if (FD_ISSET(dev->fd, readfds)) {
            if (device_read(dev->fd, dev->fbuf, &(dev->fbufsize)) < 0) {
                dm_del(i);
                continue;
            }
            if (dev->state != CONNECTED) {
                if (device_connect(dev) < 0) {
                    dm_del(i);
                    continue;
                }
            }
        }
        if (dev->state == CONNECTED && FD_ISSET(dev->sock, readfds)) {
            if (device_read(dev->sock, dev->sbuf, &(dev->sbufsize)) < 0) {
                dm_del(i);
                continue;
            }
        }
        if (FD_ISSET(dev->fd, writefds)) {
            if (device_write(dev->fd, dev->sbuf, &(dev->sbufsize)) < 0) {
                dm_del(i);
                continue;
            }
        }
        if (dev->state == CONNECTED && FD_ISSET(dev->sock, writefds)) {
            if (device_write(dev->sock, dev->fbuf, &(dev->fbufsize)) < 0) {
                dm_del(i);
                continue;
            }
        }
        i++;
    }
}

void dm_del(int offset)
{
    notice(DEL, devices[offset]->path);
    device_delete(devices+offset);
    memmove(devices+offset, devices+offset+1, (--devc-offset) * sizeof(*devices));
}
