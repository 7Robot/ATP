#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "libasserv_priv.h"


int main(int argc, char **argv) {
    if (argc != 6) {
        fprintf(stderr, "Usage: %s dist speedInit speedEnd speedMax accMax\n", argv[0]);
        return 1;
    }

    float dt = 1.0f/60;
    float pos = 0;
    float posEnd = atof(argv[1]);
    float speed = atof(argv[2]);
    float speedEnd = atof(argv[3]);
    float speedMax = atof(argv[4]);
    float acc = 0;
    float accMax = atof(argv[5]);

    fprintf(stderr, "Parameters:  \n");
    fprintf(stderr, "\tperiod:   %f\n", dt);
    fprintf(stderr, "\tposInit:   %f\n", pos);
    fprintf(stderr, "\tposEnd:   %f\n", posEnd);
    fprintf(stderr, "\tspeedInit: %f\n", speed);
    fprintf(stderr, "\tspeedEnd:  %f\n", speedEnd);
    fprintf(stderr, "\tspeedMax:  %f\n", speedMax);
    fprintf(stderr, "\taccMax:    %f\n", accMax);

    float t = 0;
    float tMax = 10;
    int ret = 0;
    int cont = 0;
    while (t < tMax && (ret == 0 || cont == 1)) {
        if (ret)
            cont = 0;
        ret = ramp_dist(dt, &pos, &speed, &acc, posEnd, speedEnd, speedMax, accMax);
        t = t+dt;
        printf("%f %f %f %f %d\n", t, pos, speed, acc, ret);
    }
    if (t > tMax) {
        fprintf(stderr, "Timeout\n");
    }
    if (ret == -1) {
        fprintf(stderr, "Error\n");
        return 1;
    }

    return 0;
}
