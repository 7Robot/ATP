#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/socket.h>

#include "client.h"
#include "decoder.h"

int client_getsocket(Client *client)
{
    return client->socket;
}

int client_waitingtosend(Client *client)
{
    return client->sbufsize > 0;
}

int client_new(Client *client, int socket)
{
    client->socket = socket;
    if ((client->rbuf = malloc(recvbufsize * sizeof(unsigned char))) == NULL) {
        perror("malloc");
        return -1;
    }
    client->rbufsize = 0;
    client->rpos = 0;
    if ((client->sbuf = malloc(sendbufsize * sizeof(unsigned char))) == NULL) {
        perror("malloc");
        free(client->rbuf);
        return -1;
    }
    client->sbufsize = 0;
    if (client_decoder_init(client) < 0) {
        free(client->rbuf);
        free(client->sbuf);
        return -1;
    }

    return 0;
}

int client_recv(Client *client)
{
    if (client->rbufsize == recvbufsize) { // FIXME FIXME FIXME
        fprintf(stderr, "WARNING: Buffer full, dump buffer, "
                "posibility of data lost.\n");
        client->rbufsize = 0;
        client->rpos = 0;
        client_decoder_clear(client);
    }
    ssize_t received = recv(client->socket, client->rbuf + client->rbufsize,
            recvbufsize - client->rbufsize, 0);
    if (received <= 0) {
        return -1;
    }

    client->rbufsize += received;

    return 0;
}

void client_cleartrame(Client * client)
{
    memmove(client->rbuf, client->rbuf + client->rpos,
        client->rbufsize - client->rpos);
    client->rbufsize -= client->rpos;
    client->rpos = 0;
}

void client_sendfrom(Client *dst, Client *src)
{
    if (sendbufsize-dst->sbufsize < src->rpos) {
        fprintf(stderr, "WARNING: Send buffer full, trame ignored !\n");
    } else {
        memcpy(dst->sbuf + dst->sbufsize,
                src->rbuf, src->rpos);
        dst->sbufsize += src->rpos;
    }
}

int client_send(Client *client)
{
    ssize_t sended = send(client->socket, client->sbuf, client->sbufsize, 0);
    if (sended < 0) {
        return -1;
    }
    memmove(client->sbuf, client->sbuf + sended, client->sbufsize - sended);
    client->sbufsize -= sended;

    return 0;
}

void client_delete(Client *client)
{
    client_decoder_destroy(client);
    close(client->socket);
    client->socket = 0;
    free(client->rbuf);
    client->rbuf = NULL;
    client->rbufsize = 0;
    free(client->sbuf);
    client->sbuf = NULL;
    client->sbufsize = 0;
}
