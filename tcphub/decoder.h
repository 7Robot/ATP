#ifndef _DECODER_H_
#define _DECODER_H_

#include "client.h"

extern int sendbufsize;
extern int recvbufsize;

extern int (*client_decoder_init)(Client *client);
extern int (*client_decoder_clear)(Client *client);
extern int (*client_decode)(Client *client);
extern int (*client_decoder_destroy)(Client *client);

int load_decoder(char *decoder, char *decoder_dir);

#endif // _DECODER_H_
