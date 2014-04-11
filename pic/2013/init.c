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

#include "header.h"  /* Function / Parameters  */

void ConfigureOscillator(void)
{
    // Configure PLL prescaler, PLL postscaler, PLL divisor
    PLLFBDbits.PLLDIV = 41; // M=43
    CLKDIVbits.PLLPOST = 0; // N1=2
    CLKDIVbits.PLLPRE  = 0; // N2=2
    // Fosc = M/(N1.N2)*Fin
}

void InitApp(void)
{
    //LEDs en sorties
    _TRISB4 = 0;
    _TRISA9 = 0;
    led1 = 0;
    led2 = 0;

    // activation de la priorité des interruptions
    _NSTDIS = 0;

    //On ouvre le Timer2 qui gère l'asservissement toutes les 10ms
    OpenTimer2(T2_ON & T2_GATE_OFF & T2_PS_1_256 & T2_32BIT_MODE_OFF & T2_SOURCE_INT, 0x600);
    ConfigIntTimer2(T2_INT_PRIOR_3 & T2_INT_ON); //Interruption ON et priorité 3

    // On ouvre le timer 5 qui sert aux SICKs
    ConfigIntTimer5(T5_INT_PRIOR_2 & T5_INT_ON);
    OpenTimer5(T5_ON & T5_GATE_OFF & T5_PS_1_256 & T5_SOURCE_INT, 0x1FFF);
    EnableIntT5; // TODO: Nim: WTF ?

    //On ouvre le module QEI qui compte les tics
    OpenQEI1(QEI_DIR_SEL_QEB & QEI_INT_CLK & QEI_INDEX_RESET_DISABLE & QEI_CLK_PRESCALE_1 & QEI_NORMAL_IO & QEI_MODE_x4_MATCH & QEI_UP_COUNT,0);
    OpenQEI2(QEI_DIR_SEL_QEB & QEI_INT_CLK & QEI_INDEX_RESET_DISABLE & QEI_CLK_PRESCALE_1 & QEI_NORMAL_IO & QEI_MODE_x4_MATCH & QEI_UP_COUNT,0);
    _QEA1R = 23;     //Module QEI 1 phase A sur RP9, RB9
    _QEB1R = 24;    //Module QEI 1 phase B sur RP22, RC6
    _QEA2R = 9;    //Module QEI 2 phase A sur RP23, RC7
    _QEB2R = 22;    //Module QEI 2 phase B sur RP24, RC8
    POS1CNT = 0;    // 0 tic pour l'instant
    POS2CNT = 0;

    Init_PWM();     //Fonction d'initialisation du PWM
}

void InitAdc()
{

   //Configuration du convertisseur Analog to Digital (ADC) du dspic33f
   //Cf page 286 dspic33f Data Sheet
   //Eteindre A/D converter pour la configuration
   AD1CON1bits.ADON = 0;
   //Configure le format de la sortie de l'ADC ( 3=signed float, 2=unsigned float, 1=signed integer, 0=unsigned integer
   AD1CON1bits.FORM = 0;
   //Config de l'échantillonnage : auto-convert
   AD1CON1bits.SSRC = 7;
   //Début d'échantillonnage (1=tout de suite  0=dès que AD1CON1bits.SAMP est activé)
   AD1CON1bits.ASAM = 0;
   //Choix du type de converter (10 ou 12 bits) 0 = 10 bits , 1 = 12bits
   AD1CON1bits.AD12B = 0;
   //Choix du type de clock interne (=1) ou externe (=0)
   AD1CON3bits.ADRC = 1;
   //Choix des références liées à la mesure analogique sur le channel 0
   AD1CHS0bits.CH0SA = 0; // Choix du (+) de la mesure pour le channel CH0 (0 = AN0) par défault
   AD1CHS0bits.CH0NA = 0; // Choix du (-) de la mesure pour le channel CH0 (0 = Masse interne pic)

   //Met tous les ports AN en Digital Input
   AD1PCFGL = 0xFFFF;

   //On met en entree les pin analogiques
   //_TRISA0 = 1;
   //_TRISA1 = 1;
   _TRISC0 = 1;
   _TRISC1 = 1;

   //Selectionne quelles pins sont analogiques
   /*AD1PCFGLbits.PCFG0 = 0;
   AD1PCFGLbits.PCFG1 = 0;
   AD1PCFGLbits.PCFG2 = 0;
   AD1PCFGLbits.PCFG3 = 0;
   AD1PCFGLbits.PCFG4 = 0;
   AD1PCFGLbits.PCFG5 = 0;*/
   AD1PCFGLbits.PCFG6 = 0;
   AD1PCFGLbits.PCFG7 = 0;

   //Mise à 0 du flag d'interrupt de ADC1
   IFS0bits.AD1IF = 0;
   //Enable les interruptions d'ADC1
   IEC0bits.AD1IE = 1;
   //Et les prioritées (ici prio = 3)
   IPC3bits.AD1IP = 2;

   AD1CON1bits.SAMP = 0;
   AD1CON1bits.ADON = 1; // Turn on the A/D converter
}

