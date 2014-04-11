#include <math.h>

#include "libasserv_priv.h"

/** TODO
 * Use tics and integer instead of meter and float
 * (get_* make the conversion)
 */

#define PI 3.141592653589793

// Déclaration des structures
typedef struct { 
    int tic_by_meter; // tic par mètre (tic.m⁻¹) 
    float meter_by_tic; // mètre par tic (m.tic⁻¹) 
    float spacing; // entraxe (tic)
} Coefs;

typedef struct {
    float x;
    float y;
    float t;
} Position;

volatile static Coefs coefs;
volatile static Position pos;

void odo_init() {
    pos.x = 0; pos.y = 0; pos.t = 0;
    odo_set_tic_by_meter(DEFAULT_TIC_BY_METER);
    odo_set_spacing(DEFAULT_SPACING);
}

void odo_step(int ticsG, int ticsD, float *delta, float *alpha) {
    *delta = (ticsD + ticsG) * coefs.meter_by_tic / 2.0f;
    *alpha = (ticsD - ticsG) * coefs.meter_by_tic / coefs.spacing;
    pos.x += *delta * cos(pos.t);
    pos.y += *delta * sin(pos.t);
    pos.t += *alpha ;
    if (pos.t > PI) pos.t -= 2*PI;
    if (pos.t <= -PI) pos.t += 2*PI;
}

float odo_get_x() { return pos.x; }
float odo_get_y() { return pos.y; }
float odo_get_theta() { return pos.t; }

void odo_set_x(float x) { pos.x = x; }
void odo_set_y(float y) { pos.y = y; }
void odo_set_theta(float theta) { pos.t = theta; }

void odo_set_tic_by_meter(int tic_by_meter) {
    coefs.tic_by_meter = tic_by_meter;
    coefs.meter_by_tic = 1.0/(float)tic_by_meter;
}
void odo_set_spacing(float spacing) {
    coefs.spacing = spacing;
}
