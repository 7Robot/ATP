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

#include <p33Fxxxx.h>      /* Includes device header file                     */
#include <stdint.h>        /* Includes uint16_t definition                    */
#include <stdbool.h>       /* Includes true/false definition                  */
#include <math.h>

/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#include "timer.h"         /* Include timer fonctions                         */
#include "header.h"  /* Function / Parameters  */
#include "atp-asserv.h"
#include <uart.h>
#include <adc.h>

/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/

volatile short channel = 0;
volatile short Buff_adc_value[8];
volatile bool sickFlood = false, sickChange = false;

void OnSICKFloodOn() { sickFlood = true; }
void OnSICKFloodOff() { sickFlood = false; }
void OnSICKChangeOn() { sickChange = true; }
void OnSICKChangeOff() { sickChange = false; }

//void OnGetSICKValue(unsigned char id) { SendSICKValue(id, Buff_adc_value[id]); }

void __attribute__((interrupt,auto_psv)) _T5Interrupt(void) {
    switch(channel){
        case 0: AD1CHS0bits.CH0SA = AN6; break;
        case 1: AD1CHS0bits.CH0SA = AN7; break;
        default: AD1CHS0bits.CH0SA = AN6; break;
    }

    AD1CON1bits.SAMP = 1; //Start sampling
    _T5IF = 0;            // On baisse le FLAG
}

void __attribute__ ((interrupt, auto_psv)) _ADC1Interrupt(void)
{
    short zone = ADC1BUF0 >> 6;

//    if (sickFlood || (sickChange && Buff_adc_value[channel] != zone)) SendSICKValue(channel, zone);
    Buff_adc_value[channel] = zone;
    channel = (channel + 1) % NB_SENSOR;

    AD1CON1bits.SAMP = 0;      //Stop sampling
    IFS0bits.AD1IF = 0;        //Clear the interrupt flag
}

//void OnGetBackBumperState() { SendBackBumperState(0); } // TODO