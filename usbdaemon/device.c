#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "device.h"
#include "settings.h"
#include "notice.h"

int getttyfd(const char *device, int speed);
int getsockfd(const char *host, const char *port);

char format[256];

int device_new(struct device **device, const char *path)
{
    struct device *dev = *device;
    if ((dev = malloc(sizeof(struct device))) == NULL) {
        perror("malloc");
        return -1;
    }
    if ((dev->fd = getttyfd(path, speed)) < 0) {
        free(dev);
        return -1;
    }
    dev->fbufsize = 0;
    
    dev->sock = -1;
    sprintf(dev->sbuf, "%c%c%c", 129, 254, 128);
    dev->sbufsize = 3;

    dev->state = BEGIN;

    if ((dev->path = malloc(strlen(path) + 1)) == NULL) {
        perror("malloc");
        close(dev->fd);
        free(dev);
        return -1;
    }
    strcpy(dev->path, path);

    *device = dev;
    return 0;
}

int device_connect(struct device *dev)
{
    int i, port;
    char service[128];
   
    for (i = 0 ; i < dev->fbufsize ; i++) {
        if (dev->state == WAITING_CONNECTION) {
            break;
        }
        switch (dev->state) {
            case BEGIN:
                if (dev->fbuf[i] == 129) {
                    dev->state = PACKET_ID;
                }
                break;
            case PACKET_ID:
                if (dev->fbuf[i] == 255) {
                    dev->state = VAR_B;
                } else {
                    dev->state = TYPE;
                }
                break;
            case TYPE:
                if (dev->fbuf[i] == 128) {
                    dev->state = BEGIN;
                } else if (atp_is_valid_type(dev->fbuf[i])) {
                    dev->state = -(dev->fbuf[i] & 0xF);
                } else {
                    dev->state = BEGIN;
                }
                break;
            case VAR_B:
                if (dev->fbuf[i] == 0x1) {
                    dev->state = DEV_ID;
                } else {
                    dev->state = BEGIN;
                }
                break;
            case DEV_ID:
                dev->sock = dev->fbuf[i];
                dev->state = END;
                break;
            case END:
                if (dev->fbuf[i] == 128) {
                    dev->state = WAITING_CONNECTION;
                } else if (atp_is_valid_type(dev->fbuf[i])) {
                    dev->state = -(dev->fbuf[i] & 0xF);
                } else {
                    dev->state = BEGIN;
                }
                break;
            default:
                dev->state += 1;
        }
    }

    memmove(dev->fbuf, dev->fbuf + i, (dev->fbufsize - i) * sizeof(*(dev->fbuf)));
    dev->fbufsize -= i;

    if (dev->state != WAITING_CONNECTION)
        return 0;

    if (!(port = getportbyid(dev->sock))) {
        fprintf(stderr, "getportbyid: no port associated with this id (%d)\n", dev->sock);
        //notice_id(IDFAIL, dev->path, dev->sock);
        return -1;
    }

    sprintf(service, "%d", port);
    
    if ((dev->sock = getsockfd(host, service)) < 0) {
        notice_sock(SOCKFAIL, dev->path, host, service);
        return -1;
    }

    dev->state = CONNECTED;
    notice_sock(CONNECT, dev->path, host, service);

    return 0;
}


int device_read(int fd, unsigned char *buffer, int *bufsize)
{
    int readed;
    
    if ((readed = read(fd, buffer + *bufsize, BUF_SIZE - *bufsize)) <= 0) {
        return -1;
    }
    *bufsize += readed;

    return 0;
}

int device_write(int fd, unsigned char *buffer, int *bufsize)
{
    int written;
    
    if ((written = write(fd, buffer, *bufsize)) <= 0) {
        return -1;
    }

    memmove(buffer, buffer + written, (*bufsize - written) * sizeof(*buffer));
    *bufsize -= written;

    return 0;
}

void device_delete(struct device **device)
{
    struct device *dev = *device;
   
    // Fermeture des fd
    close(dev->fd);
    if (dev->state == CONNECTED) {
        close(dev->sock);
    }

    // Libération des ressources alloué dans device_new
    free(dev->path);
    free(dev);

    *device = NULL;
}
