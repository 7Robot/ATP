#include <stdlib.h>
#include <errno.h>

#include "ini.h"

#include "settings.h"

#define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0

static int default_ports[MAX_ID][2] = {
    { 1, 1301 },
    { 2, 1302 },
    { 3, 1303 },
    { 4, 1304 },
    { 5, 1305 },
    { 6, 1306 },
    { 7, 1307 },
    { 8, 1308 },
    { 9, 1309 },
    { 10, 1310 },
    { 0, 0 }, // Keep this line to mark the end of the array
};

int speed = DEFAULT_SPEED;
char *host = DEFAULT_HOST;
static int *loaded_ports = NULL;
static int loaded_ports_count = 0;
static int loaded_ports_size = 0;

static int handler(void *user, const char *section, const char *name,
        const char *value)
{
    char *ret; int id; int port;

    id = strtol(name, &ret, 10);
    if (*ret != '\0' || id < 0 || id > MAX_ID) {
        return 0;
    }
    port = atoi(value);
    if (port < 1 || port > 65535) {
        return 0;
    }

    if (loaded_ports_count == loaded_ports_size) {
        loaded_ports_size += 5;
        if ((loaded_ports = realloc(loaded_ports, loaded_ports_size
                        * sizeof(int[2]))) == NULL) {
            perror("realloc");
            exit(EXIT_FAILURE);
        }
    }
    loaded_ports[loaded_ports_count * 2 + 0] = id;
    loaded_ports[loaded_ports_count * 2 + 1] = port;
    loaded_ports_count += 1;

    return 1;
}

int loadports(const char *filename)
{
    int ret;

    if ((ret = ini_parse(filename, handler, NULL)) != 0) {
        if (ret < 0) {
            printf("Warning: can't open config file '%s' (%m)\n", filename);
            return -1;
        } else {
            printf("Warning: errors occurred during config file"
                    " parsing (first error at line %d)\n", ret);
        }
    }

    return 0;
}

int getportbyid(int id)
{
    int i = 0;
    int port;

    if (loaded_ports) {
        for (i = 0 ; i < loaded_ports_count ; i++) {
            port = loaded_ports[i * 2 + 1];
            if (loaded_ports[i * 2 + 0] == id)
                break;
        }
        if (i == loaded_ports_count) {
            port = 0;
        }
    } else {
        do {
            port = default_ports[i][1];
            if (default_ports[i][0] == id)
                break;
            i++;
        } while (port != 0);
    }

    return port;
}
