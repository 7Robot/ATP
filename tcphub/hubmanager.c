#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

const char *decoder_dir = NULL;
const char *executable = "tcphub";

int *hubs;
int hub_count = 0;
int hub_size = 0;
int hub_dead = 0;

void usage(char *cmd);

#define PID(i)  (2*i)
#define PORT(i) ((2*i)+1)

int start_tcphub(const char *port, const char *decoder);

void sighandler(int signal)
{
    int status;
    int pid;
    int value;
    int i;

    while ((pid = waitpid(-1, &status, WNOHANG)) != 0) {
        if (pid < 0) {
            if (errno == EINTR) {
                continue;
            } else if (errno == ECHILD) {
                break;
            } else {
                perror("waitpid");
            }
        } else {
            if (WIFEXITED(status)) {
                value = WEXITSTATUS(status);
                for (i = 0 ; i < hub_count ; i++) {
                    if (hubs[PID(i)] == pid) {
                        printf("[%5d] tcphub terminated with status code %d\n",
                                hubs[PORT(i)], value);
                        hub_dead++;
                        break;
                    }
                }
            } else if (WIFSIGNALED(status)) {
                value = WTERMSIG(status);
                for (i = 0 ; i < hub_count ; i++) {
                    if (hubs[PID(i)] == pid) {
                        printf("[%5d] tcphub terminated by signal %d\n",
                                hubs[PORT(i)], value);
                        hub_dead++;
                        break;
                    }
                }
            } else {
                printf("event for process %d\n", pid);
            }
        }
    }
}

int main(int argc, char *argv[])
{
    int i, n, p;
    char port[16];
    char opt_decoder[256];
    char *decoder = NULL;

    if (argc < 2) {
        usage(argv[0]);
        return 0;
    }

    if (signal(SIGCHLD, sighandler) < 0) {
        perror("signal");
        return 1;
    }

    for (i = 1 ; i < argc ; i++) {
        if (argv[i][0] == '-') {
            if (!strcmp(argv[i], "-d") && (argc > i + 1)) {
                decoder_dir = argv[++i];
            } else {
                usage(argv[0]);
                return 1;
            }
        } else {
            n = sscanf(argv[i], "%d@%256s", &p, opt_decoder);
            if (n < 1) {
                fprintf(stderr, "arg %d: invalid syntaxe, ignored\n", i);
                continue;
            }
            if (n < 2) {
                decoder = NULL;
            } else {
                decoder = opt_decoder;
            }
            if (p < 1) {
                fprintf(stderr, "arg %d: invalid port, ignored\n", i);
                continue;
            }
            sprintf(port, "%d", p);
            if (start_tcphub(port, decoder) < 0) {
                fprintf(stderr, "[%s] failed to launch tcphub\n", port);
                continue;
            }
        }
    }

    pause();
    while (hub_dead < hub_count)
        pause();
}

int start_tcphub(const char *port, const char *decoder)
{
    int pid, i;
    int p = atoi(port);
    int *new = NULL;

    if (hub_count == hub_size) {
        if ((new = realloc(hubs, (hub_size+5) * sizeof(*hubs) * 2)) == NULL) {
            perror("realloc");
            return -1;
        }
        hubs = new;
        hub_size += 5;
    }

    pid = fork();
    switch (pid) {
        case -1:
            perror("fork");
            return -1;
        case 0:
            for (i = 0 ; i < getdtablesize() ; i++) {
                close(i);
            }
            execlp(executable, "tcphub", port, decoder, decoder_dir, NULL);
            perror("execlp");
            exit(EXIT_FAILURE);
    }
    hubs[PID(hub_count)] = pid;
    hubs[PORT(hub_count)] = p;
    hub_count++;

    if (decoder == NULL) {
        decoder = "default";
    }
    printf("[%5d] tcphub launched with pid %d, decoder: %s\n", p, pid, decoder);
    return 0;
}

void usage(char *cmd)
{
    printf("Usage: %s [-d decoder_directory] "
            "PORT1[@DECODER1] PORT2[@DECODER2] …\n", cmd);
}
