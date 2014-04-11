#include <stdbool.h>
#include <math.h>

#include "libasserv_priv.h"

/* TODO:
 * - Intégrer sur une fenêtre bornée
 * - erreur proportionnelle = b*order - value avec b entre 0 et 1
 *   Ceci permet de diminuer le dépassement, notamment lors d’une consigne
 *   importante. Normalement peu utile dans notre cas où les consignes sont
 *   élémentaire grâce aux générateurs de rampes.
 * - erreur dérivé = c*order - value avec c entre 0 et 1, généralement 0
 *   Ceci permet de rendre le système moins sensible aux brusques variations de
 *   la consigne.
 */

void pid_set_coefs(volatile Pid *pid, PidCoefs coefs) {
    pid_set_kp(pid, coefs.kp);
    pid_set_ki(pid, coefs.ki);
    pid_set_kd(pid, coefs.kd);
}

void pid_set_kp(volatile Pid *pid, float kp) {
    (pid->coefs).kp = kp;
}
void pid_set_ki(volatile Pid *pid, float ki) {
    (pid->coefs).ki = ki;
}
void pid_set_kd(volatile Pid *pid, float kd) {
    (pid->coefs).kd = kd;
}

void pid_reset(volatile Pid *pid) {
    pid->first = true;
    pid->intErr = 0;
}

void pid_set_order(volatile Pid *pid, volatile float *order) {
    pid_reset(pid);
    pid->order = order;
}

float pid_process(volatile Pid *pid, float period, float value) {
    float err;
    if (pid->order) {
        err = *(pid->order) - value;
    } else {
        err = -value;
    }

    if (pid->first) {
        pid->derivErr = 0;
        pid->first = false;
    } else {
        pid->derivErr = (pid->oldErr - err) / period;
    }
    pid->intErr += err * period;
    pid->oldErr = err;

    return (pid->coefs).kp * err
        + (pid->coefs).ki * pid->intErr
        - (pid->coefs).kd * pid->derivErr;
}

int pid_done(volatile Pid *pid, float epsilonErr, float epsilonDeriv) {
    return (fabsf(pid->oldErr) < epsilonErr
            && fabsf(pid->derivErr) < epsilonDeriv);
}
