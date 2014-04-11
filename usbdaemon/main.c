#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/inotify.h>

#include "settings.h"

#define EVENT_SIZE      (sizeof(struct inotify_event))
#define BUF_LEN         (8*(EVENT_SIZE+16))

#define ENV_PREFIX      "USBDAEMON"
#define MAX_DEVICE      16

const char *basedir = "/dev";

void usage(const char *cmd);
int addexistingdevices(const char *dir);
int getinotifyfd(const char *dir);
void gateway(int inotifyfd);
int filter(const char *filename);

int main(int argc, char *argv[])
{
    int option, inotifyfd;
    char *env;
    char *opt_host = NULL;
    char *configfile = NULL;
    char *opt_basedir = NULL;

    env = getenv(ENV_PREFIX"_HOST");
    if ((env != NULL) && (strlen(env) != 0)) {
        if ((opt_host = malloc(strlen(env) + 1)) == NULL) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }
        strcpy(opt_host, env);
        host = opt_host;
    }
    env = getenv(ENV_PREFIX"_SPEED");
    if ((env != NULL) && (strlen(env) != 0)) {
        sscanf(env, "%d", &speed);
    }
    env = getenv(ENV_PREFIX"_CONFIG");
    if ((env != NULL) && (strlen(env) != 0)) {
        if ((configfile = malloc(strlen(env) + 1)) == NULL) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }
        strcpy(configfile, env);
    }
    env = getenv(ENV_PREFIX"_DIR");
    if ((env != NULL) && (strlen(env) != 0)) {
        if ((opt_basedir = malloc(strlen(env) + 1)) == NULL) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }
        strcpy(opt_basedir, env);
        basedir = opt_basedir;
    }

    opterr = 1;
    while (1) {
        option = getopt(argc, argv, "h:s:c:d:");
        if (option == -1)
            break;
        switch (option) {
            case 'h':
                if ((opt_host = malloc(strlen(optarg) + 1)) == NULL) {
                    perror("malloc");
                    exit(EXIT_FAILURE);
                }
                strcpy(opt_host, optarg);
                host = opt_host;
                break;
            case 's':
                sscanf(optarg, "%d", &speed);
                break;
            case 'c':
                if ((configfile = malloc(strlen(optarg) + 1)) == NULL) {
                    perror("malloc");
                    exit(EXIT_FAILURE);
                }
                strcpy(configfile, optarg);
                break;
            case 'd':
                if ((opt_basedir = malloc(strlen(optarg) + 1)) == NULL) {
                    perror("malloc");
                    exit(EXIT_FAILURE);
                }
                strcpy(opt_basedir, optarg);
                basedir = opt_basedir;
                break;
            case '?':
                usage(argv[0]);
                exit(EXIT_FAILURE);
            default:
                break;
        }
    }

    printf("Gateway settings:\n");
    printf("\tHost: %s\n", host);
    printf("\tSpeed: %d\n", speed);
    printf("\tMonitoring device in: %s\n", basedir);
    if (configfile) {
        printf("\tConfig file: %s\n", configfile);
        if (loadports(configfile) < 0) {
            fprintf(stderr, "Warning: using defaults ports instead\n");
        }
    }

    if (addexistingdevices(basedir) < 0) {
        fprintf(stderr, "Warning: already connected device not managed\n");
    }

    if ((inotifyfd = getinotifyfd(basedir)) < 0) {
        fprintf(stderr, "Warning: monitoring disabled");
    }

    gateway(inotifyfd);

    return 0;
}

int filter(const char *filename)
{
    int a;
    return sscanf(filename, "ttyUSB%d", &a) == 1;
}

int addexistingdevices(const char *basedir)
{
    struct dirent *item;
    char path[256];
    DIR *dir = opendir(basedir);

    if (dir == NULL) {
        perror("opendir");
        return -1;
    }
    while (item = readdir(dir)) {
        if (filter(item->d_name)) {
            sprintf(path, "%s/%s", basedir, item->d_name);
            dm_add(path);
        }
    }

    closedir(dir);

    return 0;
}

int getinotifyfd(const char *dir)
{
    int fd, wd;
    
    if ((fd = inotify_init1(O_NONBLOCK)) < 0) {
        perror("intotify_init1");
        return -1;
    }

    if (inotify_add_watch(fd, dir, IN_CREATE) < 0) {
        perror("inotify_add_watch");
        return -1;
    }

    return fd;
}

void gateway(int inotifyfd)
{
    int length, i;
    char buffer[BUF_LEN];
    char path[256];
    fd_set readfds, writefds;

    while (1) {
        FD_ZERO(&readfds);
        if (inotifyfd > -1) {
            FD_SET(inotifyfd, &readfds);
        }
        FD_ZERO(&writefds);
        dm_select(&readfds, &writefds);
        
        if (select(FD_SETSIZE, &readfds, &writefds, NULL, NULL) < 0) {
            if (errno == EINTR) {
                continue;
            } else {
                perror("select");
                exit(EXIT_FAILURE); // FIXME
            }
        }

        if (FD_ISSET(inotifyfd, &readfds)) {
            if ((length = read(inotifyfd, buffer, BUF_LEN)) < 0) {
                perror("read");
                exit(EXIT_FAILURE); // FIXME
            }
            i = 0;
            while (i < length) {
                struct inotify_event *event = (struct inotify_event*)&(buffer[i]);
                if (event->len && !(event->mask & IN_ISDIR)) {
                    if (filter(event->name)) {
                        sprintf(path, "%s/%s", basedir, event->name);
                        dm_scheduledadd(path);
                    }
                }
                i += EVENT_SIZE + event->len;
            }
        }

        dm_event(&readfds, &writefds);
    }
}

void usage(const char *cmd)
{
    printf("Usage: %s [-h host] [-s speed] [-c config_file] [-d device_dir]\n", cmd);
}
