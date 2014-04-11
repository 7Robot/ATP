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

#include "header.h"  /* Function / Parameters                           */

/******************************************************************************/
/* Configuartion                                                              */
/******************************************************************************/

// Select Oscillator and switching.
_FOSCSEL(FNOSC_FRCPLL & IESO_OFF);
// Select clockBuff_adc.
_FOSC(POSCMD_NONE & OSCIOFNC_ON & IOL1WAY_OFF & FCKSM_CSDCMD);
// Watchdog Timer.
_FWDT(FWDTEN_OFF);
// Select debug channel.
_FICD(ICS_PGD1 & JTAGEN_OFF);


/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/

bool odoBroadcast = true;
unsigned long int odoDelay = 1000;

int16_t main(void)
{
    // Initialize IO ports and peripherals.
    ConfigureOscillator();
    InitApp();
    InitAdc();
    AtpInit();
    odo_init(); // TODO
    motion_init(SendDone);

    SendBoardId();
    __delay_ms(3000);
    SendBoardId();


    while(1) {
        //SendBoardId();

        if (odoBroadcast) {
            OnGetPos();
        }

        __delay_ms(odoDelay);

    }
}

void OnOdoBroadcastOn() {odoBroadcast = true;}
void OnOdoBroadcastOff() {odoBroadcast = false;}
void OnOdoBroadcastToggle() {odoBroadcast = !odoBroadcast;}
void OnOdoDelay(unsigned long int delay) {odoDelay = delay;}
