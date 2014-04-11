#ifndef _LIBASSERV_PRIV_H_
#define _LIBASSERV_PRIV_H_

#include <stdbool.h>

#include "libasserv.h"
#include "libasserv_default.h"

#ifndef NULL
#define NULL 0
#endif


/* odo */
void odo_init();
void odo_step(int ticsG, int ticsD, float *delta, float *alpha);

/* pid */
typedef struct {
    float kp;
    float ki;
    float kd;
} PidCoefs;
typedef struct Pid {
    PidCoefs coefs;
    bool first;
    float oldErr;
    float intErr;
    float derivErr;
    volatile float *order;
} Pid;
void pid_reset(volatile Pid *pid);
void pid_set_order(volatile Pid *pid, volatile float *order);
float pid_process(volatile Pid *pid, float period, float value);
int pid_done(volatile Pid *pid, float epsilonErr, float epsilonDeriv);
void pid_set_coefs(volatile Pid *pid, PidCoefs coefs);
void pid_set_kp(volatile Pid *pid, float kp);
void pid_set_ki(volatile Pid *pid, float ki);
void pid_set_kd(volatile Pid *pid, float kd);

/* asserv */
typedef struct {
    float x; // position
    float v; // vitesse
} State;
typedef struct {
    float x; // position
    float v; // vitesse
    float a; // accélération
} Order;
typedef struct {
    int mode;
    volatile Order *order;
    // position pid
    Pid posPid;
    float epsPosErr;
    float epsPosDerv;
    // speed pid
    Pid speedPid;
    float epsSpeedErr;
    float epsSpeedDerv;
} Asserv;
void asserv_init(volatile Asserv *asserv, PidCoefs posCoefs, PidCoefs speedCoefs, volatile Order *order);
float asserv_step(volatile Asserv *asserv, float period, State state);
int asserv_done(volatile Asserv *asserv, float epsilonErr, float epsilonDeriv);
void asserv_off(volatile Asserv *asserv);
void asserv_set_pos_mode(volatile Asserv *asserv);
void asserv_set_speed_mode(volatile Asserv *asserv);

/* ramp */
int ramp_dist(float period,
        volatile float *pos, volatile float *speed, volatile float *acc,
        float posEnd, float speedEnd,
        float speedMax, float accMax);
int ramp_speed(float period,
        volatile float *pos, volatile float *speed,
        float speedEnd,
        float accMax, float speedMax, float decMax);

#endif // _LIBASSERV_PRIV_H_
