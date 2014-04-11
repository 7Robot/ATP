#include <stdlib.h>
#include <dlfcn.h>
#include <stdio.h>

#include "decoder.h"

#define BUF_SIZE        1024

int sendbufsize = 0;
int recvbufsize = 0;

int (*client_decoder_init)(Client *client) = NULL;
int (*client_decoder_clear)(Client *client) = NULL;
int (*client_decode)(Client *client) = NULL;
int (*client_decoder_destroy)(Client *client) = NULL;

int load_decoder(char *decoder, char *decoder_dir)
{
    void * hndl;
    int * bufsize;
    char path[BUF_SIZE];

    snprintf(path, BUF_SIZE, "%s", decoder);
    hndl = dlopen(path, RTLD_LAZY);
    if (hndl != NULL) { goto loaded; }

    snprintf(path, BUF_SIZE, "lib%s.so", decoder);
    hndl = dlopen(path, RTLD_LAZY);
    if (hndl != NULL) { goto loaded; }

    if (decoder_dir != NULL) {
        snprintf(path, BUF_SIZE, "%s/lib%s.so", decoder_dir, decoder);
        hndl = dlopen(path, RTLD_LAZY);
        if (hndl != NULL) { goto loaded; }
    }

    snprintf(path, BUF_SIZE, "/lib/tcphub/lib%s.so",  decoder);
    hndl = dlopen(path, RTLD_LAZY);
    if (hndl != NULL) { goto loaded; }

    snprintf(path, BUF_SIZE, "/usr/lib/tcphub/lib%s.so",  decoder);
    hndl = dlopen(path, RTLD_LAZY);
    if (hndl != NULL) { goto loaded; }

    snprintf(path, BUF_SIZE, "/usr/local/lib/tcphub/lib%s.so",  decoder);
    hndl = dlopen(path, RTLD_LAZY);
    if (hndl != NULL) { goto loaded; }

    return -1;

loaded:
    printf("Decoder loaded from '%s'\n", path);

    client_decoder_init = dlsym(hndl, "client_decoder_init");
    if (client_decoder_init == NULL) {
        fprintf(stderr, "dlsym: %s\n", dlerror());
        return -1;
    }

    client_decoder_clear = dlsym(hndl, "client_decoder_clear");
    if (client_decoder_clear == NULL) {
        fprintf(stderr, "dlsym: %s\n", dlerror());
        return -1;
    }

    client_decode = dlsym(hndl, "client_decode");
    if (client_decode == NULL) {
        fprintf(stderr, "dlsym: %s\n", dlerror());
        return -1;
    }

    client_decoder_destroy = dlsym(hndl, "client_decoder_destroy");
    if (client_decoder_destroy == NULL) {
        fprintf(stderr, "dlsym: %s\n", dlerror());
        return -1;
    }

    bufsize = dlsym(hndl, "sendbufsize");
    if (bufsize == NULL) {
        fprintf(stderr, "dlsym: %s\n", dlerror());
        return -1;
    }
    sendbufsize = *bufsize;

    bufsize = dlsym(hndl, "recvbufsize");
    if (bufsize == NULL) {
        fprintf(stderr, "dlsym: %s\n", dlerror());
        return -1;
    }
    recvbufsize = *bufsize;
}
