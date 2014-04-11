#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "libasserv_priv.h"


int main(int argc, char **argv) {
    if (argc != 6) {
        fprintf(stderr, "Usage: %s speedInit speedEnd accMax speedMax decMax\n", argv[0]);
        return 1;
    }

    float dt = 0.01;
    float pos = 0;
    float speed = atof(argv[1]);
    float speedEnd = atof(argv[2]);
    float accMax = atof(argv[3]);
    float speedMax = atof(argv[4]);
    float decMax = atof(argv[5]);

    fprintf(stderr, "Parameters:  \n");
    fprintf(stderr, "\tposInit:   %f\n", pos);
    fprintf(stderr, "\tspeedInit: %f\n", speed);
    fprintf(stderr, "\tspeedEnd:  %f\n", speedEnd);
    fprintf(stderr, "\taccMax:    %f\n", accMax);
    fprintf(stderr, "\tspeedMax:  %f\n", speedMax);
    fprintf(stderr, "\tdecMax:    %f\n", decMax);

    float t = 0;
    float tMax = 70;
    int ok;
    while (t < tMax) {
        ok = ramp_speed(dt, &pos, &speed, speedEnd, accMax, speedMax, decMax);
        t = t+dt;
        printf("%f %f %f %d\n", t, pos, speed, ok);
        if (t > 8) dt = 0.1;
        if (t > 16) speedEnd = -0.5;
        if (t > 35) speedEnd = 1;
    }
    if (t > tMax) {
        fprintf(stderr, "Timeout\n");
    }
}
