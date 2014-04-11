#ifndef _CLIENT_H_
#define _CLIENT_H_

typedef struct {
    int socket;
    unsigned char *rbuf;
    int rbufsize;
    char *sbuf;
    unsigned int sbufsize;
    int rpos;
    void * decoder;
} Client;

int client_getsocket(Client *client);
int client_waitingtosend(Client *client);

int client_new(Client *client, int socket);
int client_recv(Client *client);
void client_cleartrame(Client *dst);
void client_sendfrom(Client *dst, Client *src);
int client_send(Client *client);
void client_delete(Client *client);

#endif // _CLIENT_H_
