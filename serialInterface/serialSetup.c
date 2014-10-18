#include "system.h"

/* Global String Literals */
unsigned char msgWelcome[] = "\r\nEntered Remote Mode. Welcome!\r\n";
unsigned char msgUser[] = "\r\n\nuser@";
unsigned char msgFactory[] = "\r\nfactory@";
unsigned char msgStarTracker[] = "starTracker";
unsigned char msgEndPrompt[] = "$ ";
unsigned char youTyped[] = "\r\nYou Typed: ";
unsigned char maxReached[] = "\r\n\n\tMaximum Length (100) Reached!\r\n";

/* Global Variables */
unsigned char circBuffer[SIZE];     /* This stores the received characters */
unsigned char* scPtr = circBuffer;  /* This points to locations in the buffer */
unsigned char* txPtr;
unsigned char scPosition = 0;
unsigned char rcPosition = 0;         /* This indexes the buffer */

unsigned char rcWord[2] = {'\0', '\0'};

/* Serial Flags */
struct
{
  unsigned int RECEIVED : 1;
} Serial;

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
    transmit(msgWelcome);
    prompt();

    /* Config: Interrupt Control Register (Enable Interrupts) */
    INTCONbits.GIEH = 1;/* Enable global interrupts */
    INTCONbits.GIEL = 1;/* Enable peripheral interrupts */
}
