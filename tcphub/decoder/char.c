#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "client.h"

int recvbufsize = 64;
int sendbufsize = 256;

int client_decoder_init(Client *client)
{
    return 0;
}

void client_decoder_clear(Client *client)
{
}

int client_decode(Client *client)
{
    if (client->rpos < client->rbufsize) {
        client->rpos++;
        return 1;
    }
    return 0;
}

void client_decoder_destroy(Client *client)
{
}
