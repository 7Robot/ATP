#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "client.h"
#include "libatp.h"

typedef enum State {
    begin,
    type,
    data,
} State;

typedef struct {
    int remainingData;
    State expected;
} Decoder;

#define BEGIN_FLAG 129
#define END_FLAG 128

int recvbufsize = 256;
int sendbufsize = 10480;

int client_decoder_init(Client *client)
{
    Decoder * decoder = NULL;
    if ((decoder = malloc(sizeof(Decoder))) == NULL) {
        perror("malloc");
        return -1;
    }
    decoder->remainingData = 0;
    decoder->expected = begin;
    client->decoder = decoder;

    return 0;
}

void client_decoder_clear(Client *client)
{
    Decoder * decoder = client->decoder;
    decoder->remainingData = 0;
    decoder->expected = begin;
}

int client_decode(Client *client)
{
    Decoder * decoder = client->decoder;
    while (client->rpos < client->rbufsize) {
        unsigned char pending = client->rbuf[client->rpos];
        switch (decoder->expected) {
            case begin:
                if (pending == BEGIN_FLAG) {
                    decoder->remainingData = 1;
                    decoder->expected = data;
                    client->rpos++;
                } else {
                    printf("Warning: begin flag expected (received %d)\n", pending);
                    memmove(client->rbuf, client->rbuf + 1, client->rbufsize - 1);
                    client->rbufsize--;
                }
                break;
            case type:
                if (pending == 128) {
                    decoder->expected = begin;
                    client->rpos++;
                    return 1;
                } else if(atp_is_valid_type(pending)) {
                    decoder->remainingData = pending & 15;
                    decoder->expected = data;
                    client->rpos++;
                } else {
                    printf("Warning: unknow type (received %d)\n", pending);
                    decoder->expected = begin;
                    client->rpos++;
                    client_cleartrame(client);
                    return -1;
                }
                break;
            case data:
                decoder->remainingData--;
                if (decoder->remainingData == 0) {
                    decoder->expected = type;
                }
                client->rpos++;
                break;
        }
    }

    return 0;
}

void client_decoder_destroy(Client *client)
{
    free(client->decoder);
}
