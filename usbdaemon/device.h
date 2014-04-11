#ifndef _DEVICE_H_
#define _DEVICE_H_

#define BUF_SIZE 1024

enum state {
    TYPE = 0,
    BEGIN = 1,
    PACKET_ID = 3,
    VAR_TYPE = 4,
    VAR_B = 5,
    DEV_ID = 6,
    END = 7,
    WAITING_CONNECTION = 8,
    CONNECTED = 9,
};

struct device {
    int fd;
    unsigned char fbuf[BUF_SIZE];
    int fbufsize;
    int sock;
    unsigned char sbuf[BUF_SIZE];
    int sbufsize;
    enum state state;
    char *path;
};

int device_new(struct device **device, const char *path);
int device_connect(struct device *dev);
int device_read(int fd, unsigned char *buffer, int *bufsize);
int device_write(int fd, unsigned char *buffer, int *bufsize);
void device_delete(struct device **device);

#endif // _DEVICE_H_
