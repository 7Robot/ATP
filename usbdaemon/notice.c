#include <stdlib.h>
#include <stdio.h>

#include "notice.h"

void notice(enum notice type, const char *path)
{
    notice_sock(type, path, NULL, NULL);
}

void notice_idfail(const char *path, int id)
{
    printf("[ID %d FAIL] %s\n", id, path);
}

void notice_sock(enum notice type, const char *path, const char *host,
        const char *service)
{
    switch (type) {
        case ADD:
            printf("[OPEN     ] %s\n", path);
            break;
        case ADDFAIL:
            printf("[FAIL     ] %s\n", path);
            break;
        case DEL:
            printf("[CLOSE    ] %s\n", path);
            break;
        case NEW:
            printf("[NEW      ] %s\n", path);
            break;
        case CONNECT:
            printf("[CONNECTED] %s ↔ %s:%s\n", path, host, service);
            break;
        case SOCKFAIL:
            printf("[FAIL     ] %s ↔ %s:%s\n", path, host, service);
            break;
        default:
            printf("[?????????] %s\n", path);
    }
}
