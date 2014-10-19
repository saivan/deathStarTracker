/**********************************************************
**  File: serialSetup.c
***********************************************************
**  Author: Kelvin Hsu
**  Created on 2 September 2014, 11:42 PM
**  Assembled with C18 C Compiler v. 4.36
***********************************************************
**
**********************************************************/

#include "system.h"

/* Global String Literals */
rom char rom msgWelcome[] = "\r\nEntered Remote Mode. Welcome!\r\n";
rom char rom msgUser[] = "\r\n\nuser@";
rom char rom msgFactory[] = "\r\nfactory@";
rom char rom msgStarTracker[] = "starTracker";
rom char rom msgEndPrompt[] = "$ ";
rom char rom youTyped[] = "\r\nYou Typed: ";
rom char rom maxReached[] = "\r\n\n\tMaximum Length (100) Reached!\r\n";

/* Global Variables */
unsigned char circBuffer[SIZE];     /* This stores the received characters */
unsigned char transmitBuffer[SIZE];
//unsigned char* txPtr = transmitBuffer;
unsigned char* scPtr = circBuffer;  /* This points to locations in the buffer */
unsigned char scPosition = 0;
unsigned char rcPosition = 0;         /* This indexes the buffer */
unsigned char* stringsToTransmit[SIZE];
unsigned char cuePosition = 0;
unsigned char printPosition = 0;
unsigned char rcWord[2] = {'\0', '\0'};

/* 'setup' function: Setup the Configurations for Relevant Registers */
void serialSetup(void)
{
    /* Config: Interrupt Control Register (Disable Interrupts during Setup) */
    INTCON = ZERO;      /*Disable Global and Peripheral Interrupts*/

    /* Config: USART Transmission Status & Control Register */
    TXSTA = ZERO;       /* 8-bit transmission, Asynchronous mode */
    TXSTAbits.BRGH = 1; /* Select High Baud Rate */
    TXSTAbits.TXEN = 1; /* Transmission Enable */

    /* Config: USART Reception Status & Control Register */
    RCSTA = ZERO;       /* 8-bit reception */
    RCSTAbits.SPEN = 1; /* Serial Port Enable */
    RCSTAbits.CREN = 1; /* Continuous Receive Enable */

    /* Config: USART Baud Rate Generator Register */
    SPBRG = 64;         /* Load SPBRG with 64 for approximately 9600 BAUD */

    /* Config: Peripheral Interrupt Enable Register */
    PIE1bits.RCIE = 1;  /* Enable USART Receive  Interrupt */

    /* Config: Peripheral Interrupt Request (Flag) Register */
    IPR1 = ZERO;
    IPR2 = ZERO;
    IPR1bits.RCIP = 1;  /* Make USART Receive  Interrupt a High Priority */

    /* Config: PORTC [Make PORTC<7:6> our serial I/O pins] */
    TRISCbits.RC6 = 0;  /* Make RC6 an output as TX */
    TRISCbits.RC7 = 1;  /* Make RC7 an input  as RX */

    /* Config: Reset Control Register */
    RCONbits.IPEN = 1;  /* Enable priority levels */

    /* Initialise: Start the program with a welcome message and prompt */
    cueInRomString(msgWelcome);
    prompt();

    /* Config: Interrupt Control Register (Enable Interrupts) */
    INTCONbits.GIEH = 1;/* Enable global interrupts */
    INTCONbits.GIEL = 1;/* Enable peripheral interrupts */
}
