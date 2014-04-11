#include <math.h>
#include <stdio.h>

int ramp_speed(float dt,
        volatile float *pos, volatile float *speed,
        float speedEnd,
        float accMax, float speedMax, float decMax)
{
    int ret = 0;

    if (accMax == 0 || speedMax == 0 || decMax == 0) return -1;
    if (accMax < 0) accMax = -accMax;
    if (speedMax < 0) speedMax = -speedMax;
    if (decMax < 0) decMax = -decMax;

    if (speedEnd > speedMax) speedEnd = speedMax;
    else if (speedEnd < -speedMax) speedEnd = -speedMax;

    if (speedEnd - *speed < 0) {
        accMax = -accMax;
        speedMax = -speedMax;
        decMax = -decMax;
    }

    int test = *speed > speedEnd;

    if (*speed == speedEnd) {
        ret = 1;
    } else if ((*speed <= 0 && speedEnd < *speed)
            || (*speed >= 0 && speedEnd > *speed)) {
        *speed += accMax * dt;
        if (test ^ (*speed > speedEnd)) {
            *speed = speedEnd;
            ret = 1;
        }
    } else if ((*speed <= 0 && speedEnd > *speed)
            || (*speed >= 0 && speedEnd < *speed)) {
        *speed += decMax * dt;
        if (test ^ (*speed > speedEnd)) {
            *speed = speedEnd;
            ret = 1;
        }
    } else {
        ret = 1;
    }

    *pos += *speed * dt;

    return ret;
}
