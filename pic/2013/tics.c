#include "header.h"
#include "asserv/libasserv.h"

volatile int16_t ticd = 0, old_ticd = 0, diffd = 0;
volatile int16_t ticg = 0, old_ticg = 0, diffg = 0;
volatile int32_t compteur_ticd = 0;
volatile int32_t compteur_ticg = 0;

void __attribute__((interrupt,auto_psv)) _T2Interrupt(void)
{
    static int count = 0;
    count++;

    // On lit l'encodeur droit (qui est en fait le gauche)
    ticd = (uint16_t) POS1CNT;// ReadQEI2();
    diffd = ticd-old_ticd;
    old_ticd = ticd;
    compteur_ticd += diffd;

    // On lit l'encodeur gauche (qui est en fait le droit)
    ticg = (uint16_t) POS2CNT;// ReadQEI1();
    diffg = ticg-old_ticg;
    old_ticg = ticg;
    compteur_ticg += diffg;

    int consigneG, consigneD;

    motion_step(0.01, diffg, diffd, &consigneG, &consigneD);

    /*if ((count%20)==0) {
        SendDebugFloats((float)consigneG, (float)consigneD, 5);
    }*/

    Set_Vitesse_MoteurD((float)consigneD);
    Set_Vitesse_MoteurG((float)consigneG);

    _T2IF = 0;      // On baisse le FLAG
}
