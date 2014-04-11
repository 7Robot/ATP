#ifndef _NOTICE_H_
#define _NOTICE_H_

enum notice {
    ADD,
    DEL,
    ADDFAIL,
    NEW,
    SOCKFAIL,
    IDFAIL,
    CONNECT,
};

void notice(enum notice type, const char *path);
void notice_idfail(const char *path, int id);
void notice_sock(enum notice type, const char *path, const char *host,
        const char *service);

#endif // _NOTICE_H_
