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

#ifdef  USING_PIC18F4520
#define SPBRGvalue  64
#endif

#ifdef  USING_PIC18F452
#define SPBRGvalue  25
#endif

/* Global String Literals */
rom char rom msgDeathStarTracker1[] = "________  ___________   ___________________ ___     _________________________ __________  _____________________    _____  _________  ____  __._____________________ \r\n";
rom char rom msgDeathStarTracker2[] = "\\______ \\ \\_   _____/  /  _  \\__    ___/   |   \\   /   _____/\\__    ___/  _  \\\\______   \\ \\__    ___/\\______   \\  /  _  \\ \\_   ___ \\|    |/ _|\\_   _____/\\______   \\r\n";
rom char rom msgDeathStarTracker3[] = " |    |  \\ |    __)_  /  /_\\  \\|    | /    ~    \\  \\_____  \\   |    | /  /_\\  \\|       _/   |    |    |       _/ /  /_\\  \\/    \\  \\/|      <   |    __)_  |       _/\r\n";
rom char rom msgDeathStarTracker4[] = " |    `   \\|        \\/    |    \\    | \\    Y    /  /        \\  |    |/    |    \\    |   \\   |    |    |    |   \\/    |    \\     \\___|    |  \\ |        \\ |    |   \\r\n";
rom char rom msgDeathStarTracker5[] = "/_______  /_______  /\\____|__  /____|  \\___|_  /  /_______  /  |____|\\____|__  /____|_  /   |____|    |____|_  /\\____|__  /\\______  /____|__ \\/_______  / |____|_  /\r\n";
rom char rom msgDeathStarTracker6[] = "        \\/        \\/         \\/              \\/           \\/                 \\/       \\/                     \\/         \\/        \\/        \\/        \\/         \\/ \r\n";
rom char rom msgWelcome[] = "\r\nEntered Remote Mode. Welcome!\r\n";
rom char rom msgBye[] =
                        "\r\n\n\tLeaving Remote Mode to Local Mode... \r\n";
rom char rom msgUser[] = "\r\n\nuser@";
rom char rom msgFactory[] = "\r\nfactory@";
rom char rom msgStarTracker[] = "starTracker";
rom char rom msgEndPrompt[] = "$ ";
rom char rom msgMaxReached[] = "\r\n\n\tYour command is too long!!\r\n";
rom char rom msgSeparatorLine[] = "\r\n------------------------------\r\n";
rom char rom msgDot[] = ". ";
rom char rom msgSlash[] = "/";
rom char rom msgBack[] = "ack";
rom char rom msgNewLine[] = "\r\n";
rom char rom msgSpaceBackSpace[] = " \b";
rom char rom msgBackSpace[] = "\b";
rom char rom msgSpace[] = " ";
rom char rom msgDigitError[] =
                        "\r\n\t<Please Enter Single Digit Number or 'b'>\r\n";
rom char rom msgWelcomeFactory[] = "\r\nEntered Factory Mode\r\n";
rom char rom msgNumberError[] = "\r\n\t<Please input a valid number>\r\n";
char password[] = "e-VADER-s";

/* Global Variables */
char userInputBuffer[INPUTSIZE];     /* This stores the received characters */
//unsigned char* txPtr = transmitBuffer;
char* scPtr = userInputBuffer;  /* This points to locations in the buffer */
char scPosition = 0;
char rcPosition = 0;         /* This indexes the buffer */
char cuePosition = 0;
char printPosition = 0;
char rcWord[2] = {'\0', '\0'};
char cursorPosition = 0;

char numberOfChildren = 0;

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
    SPBRG = SPBRGvalue;  /* Load SPBRG with 64 for approximately 9600 BAUD */

    /* Config: Peripheral Interrupt Enable Register */
    PIE1bits.RCIE = 1;  /* Enable USART Receive  Interrupt */

    /* Config: Peripheral Interrupt Request (Flag) Register */
    IPR1 = ZERO;
    IPR2 = ZERO;
    IPR1bits.RCIP = 0;  /* Make USART Receive  Interrupt a Low Priority */

    /* Config: PORTC [Make PORTC<7:6> our serial I/O pins] */
    TRISCbits.RC6 = 0;  /* Make RC6 an output as TX */
    TRISCbits.RC7 = 1;  /* Make RC7 an input  as RX */

    /* Config: Reset Control Register */
    RCONbits.IPEN = 1;  /* Enable priority levels */

    /* Initialise: Start the program with a welcome message and prompt */
    printRomString(msgDeathStarTracker1);
    printRomString(msgDeathStarTracker2);
    printRomString(msgDeathStarTracker3);
    printRomString(msgDeathStarTracker4);
    printRomString(msgDeathStarTracker5);
    printRomString(msgDeathStarTracker6);
    printRomString(msgWelcome);

    showChildOptions();
    prompt();

    /* Config: Interrupt Control Register (Enable Interrupts) */
    INTCONbits.GIEH = 1;/* Enable global interrupts */
    INTCONbits.GIEL = 1;/* Enable peripheral interrupts */
}
