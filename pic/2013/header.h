/*
* Asserv dsPIC33F
* Compiler : Microchip xC16
* µC : 33FJ64MC804
* Avril 2013
*    ____________      _           _
*   |___  /| ___ \    | |         | |
*      / / | |_/ /___ | |__   ___ | |_
*     / /  |    // _ \| '_ \ / _ \| __|
*    / /   | |\ \ (_) | |_) | (_) | |_
*   /_/    |_| \_\___/|____/ \___/'\__|
*                 7robot.fr
*/

#include "timer.h"
#include <p33Fxxxx.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <uart.h>
#include <adc.h>
#include "qei.h"
#include "libasserv_robot.h"
#include "asserv/libasserv.h"
#include "atp.h"
#include "atp-asserv.h"

/******************************************************************************/
/* System Level #define Macros                                                */
/******************************************************************************/

/* TODO Define system operating frequency */

/* Microcontroller MIPs (FCY) */
#define SYS_FREQ        80000000 //7370000L
#define FCY             SYS_FREQ/2


#include <libpic30.h>

/******************************************************************************/
/* User Level #define Macros                                                  */
/******************************************************************************/

/* TODO Application specific user parameters used in user.c may go here */

#define led1 _LATB4
#define led2 _LATA8

#define DIRA1 _LATC3    //DIRA1 sur RC3
#define DIRB1 _LATC4    //DIRB1 sur RC4
#define DIRA2 _LATA3    //DIRA2 sur RA3
#define DIRB2 _LATA4    //DIRB2 sur RA4


// SICKs
#define NB_SENSOR 2

#define AN0  0
#define AN1  1
#define AN2  2
#define AN3  3
#define AN4  4
#define AN5  5
#define AN6  6
#define AN7  7

#define SIGN(a) ((a<0)?-1:1)

/******************************************************************************/
/* Function Prototypes                                                        */
/******************************************************************************/

/* TODO User level functions prototypes (i.e. InitApp) go here */

void ConfigureOscillator(void);    /* Handles clock switching/osc initialization */

void InitApp(void);             /* I/O and Peripheral Initialization          */

void Init_PWM(void);
void Set_Vitesse_MoteurD(float);
void Set_Vitesse_MoteurG(float);

void InitAdc(void);             /*Init Analog to Digital Converter PIC*/


