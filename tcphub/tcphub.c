#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h> // O_NONBLOCK
#include <signal.h>

#include <arpa/inet.h>

#include "server.h"
#include "client.h"
#include "decoder.h"

#define MAX_CONNECTION 64

#define ENV_PORT            "TCPHUB_PORT"
#define ENV_DECODER         "TCPHUB_DECODER"
#define ENV_DECODER_DIR     "TCPHUB_DECODER_DIR"
#define DEFAULT_DECODER     "text"
#define DEFAULT_DECODER_DIR "tcphub"
#define TOOMANYCONNECTION   "Sorry, too many connection, try again later."

void usage(const char *cmd);
void hub(char *port);

int main(int argc, char *argv[])
{
    char *port = NULL;

    static char *decoder = DEFAULT_DECODER;
    char *opt_decoder = NULL;

    static char *decoder_dir = DEFAULT_DECODER_DIR;
    char *opt_decoder_dir = NULL;

    char *env;

    env = getenv(ENV_PORT);
    if ((env != NULL) && (strlen(env) != 0)) {
        if ((port = malloc(strlen(env) + 1)) == NULL) {
            perror("malloc");
            return EXIT_FAILURE;
        }
        strcpy(port, env);
    }

    env = getenv(ENV_DECODER);
    if ((env != NULL) && (strlen(env) != 0)) {
            opt_decoder = malloc(strlen(env) + 1);
            if (opt_decoder == NULL) {
                perror("malloc");
                return EXIT_FAILURE;
            }
            strcpy(opt_decoder, env);
            decoder = opt_decoder;
    }

    env = getenv(ENV_DECODER_DIR);
    if ((env != NULL) && (strlen(env) != 0)) {
            opt_decoder_dir = malloc(strlen(env) + 1);
            if (opt_decoder_dir == NULL) {
                perror("malloc");
                return EXIT_FAILURE;
            }
            strcpy(opt_decoder_dir, env);
            decoder_dir = opt_decoder_dir;
    }

    if (port == NULL) {
        if (argc < 2) {
            usage(argv[0]);
            return EXIT_FAILURE;
        }
        port = argv[1];
    }
    if (argc > 2) {
        decoder = argv[2];
    }
    if (argc > 3) {
        decoder_dir = argv[3];
    }
    if (argc > 4) {
        usage(argv[0]);
        return EXIT_FAILURE;
    }

    if (load_decoder(decoder, decoder_dir) < 0) {
        fprintf(stderr, "Error: can't load decoder '%s'\n", decoder);
        return EXIT_FAILURE;
    }

    printf("Start tcphub on port %s\n", port);
    fflush(stdout);

    hub(port);

    return EXIT_SUCCESS;
}

void hub(char *port)
{
    int sockserver;
    struct sockaddr_in address;
    socklen_t length = sizeof(struct sockaddr_in);
    Client clientv[MAX_CONNECTION];
    int clientc = 0;
    int optvalue = 1;
    int i, j, ret;

    signal(SIGPIPE, SIG_IGN);

    if ((sockserver = create_tcp_server(NULL, port)) < 0) {
        exit(EXIT_FAILURE);
    }
    if (setsockopt(sockserver, SOL_SOCKET, SO_OOBINLINE, &optvalue,
                sizeof(i)) < 0) {
        perror("setsocketopt");
        fprintf(stderr, "WARNING: out-of-band data will be ignored.\n");
    }

    fd_set readfds, writefds;

    while (1) {
        FD_ZERO(&readfds);
        FD_SET(sockserver, &readfds);
        FD_ZERO(&writefds);
        for (i = 0 ; i < clientc ; i++) {
            FD_SET(client_getsocket(clientv+i), &readfds);
            if (client_waitingtosend(clientv+i)) {
                FD_SET(client_getsocket(clientv+i), &writefds);
            }
        }
        
        if (select(FD_SETSIZE, &readfds, &writefds, NULL, NULL) < 0) {
            if (errno == EINTR) {
                continue;
            } else {
                perror("select");
                exit(EXIT_FAILURE);
            }
        }

        if (FD_ISSET(sockserver, &readfds)) {
            int sock = accept4(sockserver, (struct sockaddr*)&address, &length,
                    O_NONBLOCK);
            if (sock < 0) {
                if (errno == EBADF || errno == EFAULT || errno == EMFILE
                        || errno == ENFILE || errno == ENOMEM) {
                    perror("accept");
                    exit(EXIT_FAILURE);
                } else {
                    perror("WARNING (accept)"); // FIXME
                }
            }
            if (clientc == MAX_CONNECTION) {
                write(sock, TOOMANYCONNECTION, strlen(TOOMANYCONNECTION));
                close(sock);
            } else {
                if (setsockopt(sock, SOL_SOCKET, SO_OOBINLINE, &optvalue,
                            sizeof(i)) < 0) {
                    perror("setsocketopt");
                    fprintf(stderr, "WARNING: "
                            "out-of-band data will be ignored.\n");
                }
                if (client_new(clientv+clientc, sock) < 0) {
                    exit(EXIT_FAILURE);
                }
                clientc++;
                printf("Client connected, total: %d\n", clientc);
            }
        }

        for (i = 0 ; i < clientc ; i++) {
            if (FD_ISSET(client_getsocket(clientv+i), &readfds)) {
                if ((ret = client_recv(clientv+i)) < 0) {
                    client_delete(clientv+i);
                    if (clientc-i-1) {
                        memmove(clientv+i, clientv+i+1,
                                (clientc-i-1) * sizeof(*clientv));
                    }
                    clientc--;
                    i--;
                    printf("Client disconnected, total: %d\n", clientc);
                } else {
                    while (client_decode(clientv+i)) {
                        for (j = 0 ; j < clientc ; j++) {
                            if (j != i) {
                                client_sendfrom(clientv+j, clientv+i);
                            }
                        }
                        client_cleartrame(clientv+i);
                    }
                }
            }
            if (FD_ISSET(client_getsocket(clientv+i), &writefds)) {
                if ((ret = client_send(clientv+i)) < 0) {
                    client_delete(clientv+i);
                    if (clientc-i-1) {
                        memmove(clientv+i, clientv+i+1,
                                (clientc-i-1) * sizeof(*clientv));
                    }
                    clientc--;
                    i--;
                    printf("Client disconnected, total: %d\n", clientc);
                }
            }
        }
    }
}

void usage(const char *cmd)
{
    printf("Usage: %s port [decoder [decoders directory]]\n", cmd);
}
