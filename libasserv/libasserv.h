#ifndef _LIBASSERV_H_
#define _LIBASSERV_H_

/* motion */
void motion_init(void(*callback_done)(void));
void motion_step(float period, int ticsG, int ticsD, int *consigneG, int *consigneD);
void motion_set_epsilons(float Ed, float Es, float Et, float Eo);
//void motion_pause(float aMax, float dMax);

/* motion order */
void motion_stop();
void motion_dist(float dist, float vMax, float aMax);
void motion_rot(float rot, float vMax, float aMax);
void motion_dist_rot(float dist, float rot, float vDist, float aDist, float vRot, float aRot);
void motion_dist_free(float dist);
void motion_rot_free(float rot);
void motion_reach_x(float x, float vMax, float aMax);
void motion_reach_y(float y, float vMax, float aMax);
void motion_reach_theta(float theta, float vMax, float aMax);
void motion_speed(float speed, float aMax, float dMax);
void motion_speed_free(float speed);
void motion_omega(float omega, float aMax, float dMax);
void motion_omega_free(float omega);

/* odo */
float odo_get_x();
float odo_get_y();
float odo_get_theta();

void odo_set_x(float x);
void odo_set_y(float y);
void odo_set_theta(float theta);

void odo_set_tic_by_meter(int tic_by_meter);
void odo_set_spacing(float spacing);

#endif // _LIBASSERV_H_
