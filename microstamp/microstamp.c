#include <stdio.h>
#include <sys/time.h>

#include "microstamp.h"

#define BUF_SIZE 1024

int main(int argc, char *argv[])
{
    microstamp(stdin, stdout);
}

void microstamp(FILE *in, FILE *out)
{
    char buffer[BUF_SIZE];
    int bufptr = 0;
    int state = 1;
    int pending;
    int len, ptr;
    struct timeval time;

    while ((pending = fgetc(in)) != EOF) {
        buffer[bufptr++] = pending;
        switch (state) {
            case 1:
                // attente de l’entête
                if (pending = 129) {
                    state = 2;
                }
                break;
            case 2:
                state = 3;
                break;
            case 3:
                // attente d’un type d’argument
                if (pending == 128) {
                    // fin de trame
                    gettimeofday(&time, NULL);
                    buffer[bufptr-1] = 132;
                    int sec = time.tv_sec;
                    buffer[bufptr++] = sec & 0xFF;
                    buffer[bufptr++] = (sec >> 8) & 0xFF;
                    buffer[bufptr++] = (sec >> 16) & 0xFF;
                    buffer[bufptr++] = (sec >> 24) & 0xFF;
                    buffer[bufptr++] = 148;
                    int msec = time.tv_usec;
                    buffer[bufptr++] = msec & 0xFF;
                    buffer[bufptr++] = (msec >> 8) & 0xFF;
                    buffer[bufptr++] = (msec >> 16) & 0xFF;
                    buffer[bufptr++] = (msec >> 24) & 0xFF;
                    buffer[bufptr] = 128;
                    fwrite(buffer, sizeof(char), bufptr+1, out);
                    fflush(out);
                    bufptr = 0;
                    state = 1;
                } else if (atp_is_valid_type(pending)) {
                    // autre type
                    len = pending & 0xF;
                    ptr = 0;
                    state = 4;
                } else {
                    // erreur
                    bufptr = 0;
                    state = 1;
                }
                break;
            case 4:
                // donnée d’un argument
                ptr++;
                if (ptr == len) {
                    state = 3;
                }
                break;
        }
    }
}
