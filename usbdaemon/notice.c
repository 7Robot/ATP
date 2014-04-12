#include <stdlib.h>
#include <stdio.h>

#include "notice.h"

void notice(enum notice type, const char *path)
{
    notice_sock(type, path, NULL, NULL);
}

void notice_idfail(const char *path, int id)
{
    fprintf(stderr, "[ID %d FAIL] %s\n", id, path);
}

void notice_sock(enum notice type, const char *path, const char *host,
        const char *service)
{
    switch (type) {
        case ADD:
            fprintf(stderr, "[OPEN     ] %s\n", path);
            break;
        case ADDFAIL:
            fprintf(stderr, "[FAIL     ] %s\n", path);
            break;
        case DEL:
            fprintf(stderr, "[CLOSE    ] %s\n", path);
            break;
        case NEW:
            fprintf(stderr, "[NEW      ] %s\n", path);
            break;
        case CONNECT:
            fprintf(stderr, "[CONNECTED] %s ↔ %s:%s\n", path, host, service);
            break;
        case SOCKFAIL:
            fprintf(stderr, "[FAIL     ] %s ↔ %s:%s\n", path, host, service);
            break;
        default:
            fprintf(stderr, "[?????????] %s\n", path);
    }
}
