#include "libasserv_priv.h"

#define M_OFF  0 // asserv off
#define M_POS   1 // asserv en position
#define M_SPEED 2 // asserv en vitesse

void asserv_init(volatile Asserv *asserv, PidCoefs posCoefs, PidCoefs speedCoefs, volatile Order *order) {
    asserv->mode = M_OFF;
    asserv->order = order;
    pid_set_coefs(&(asserv->posPid), posCoefs);
    pid_set_order(&(asserv->posPid), &(order->x));
    pid_set_coefs(&(asserv->speedPid), speedCoefs);
    pid_set_order(&(asserv->speedPid), &(order->v));
}

void asserv_off(volatile Asserv *asserv) {
    asserv->mode = M_OFF;
}

void asserv_set_pos_mode(volatile Asserv *asserv) {
    if (asserv->mode != M_POS) {
        pid_reset(&(asserv->posPid));
    }
    asserv->mode = M_POS;
}

void asserv_set_speed_mode(volatile Asserv *asserv) {
    if (asserv->mode != M_SPEED) {
        pid_reset(&(asserv->speedPid));
    }
    asserv->mode = M_SPEED;
}

float asserv_step(volatile Asserv *asserv, float period, State state) {
    if (asserv->mode == M_POS) {
        return pid_process(&(asserv->posPid), period, state.x);
    } else if (asserv->mode == M_SPEED) {
        return pid_process(&(asserv->speedPid), period, state.v);
    } else {
        return 0;
    }
}

int asserv_done(volatile Asserv *asserv, float epsErr, float epsDeriv) {
    return pid_done(&(asserv->posPid), epsErr, epsDeriv);;
}
