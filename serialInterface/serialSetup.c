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
rom char rom msgDeathStarTracker1[] = "\r\n________  ___________   ___________________ ___     _________________________ __________  _____________________    _____  _________  ____  __._____________________ \r\n";
rom char rom msgDeathStarTracker2[] = "\\______ \\ \\_   _____/  /  _  \\__    ___/   |   \\   /   _____/\\__    ___/  _  \\\\______   \\ \\__    ___/\\______   \\  /  _  \\ \\_   ___ \\|    |/ _|\\_   _____/\\______   \\\r\n";
rom char rom msgDeathStarTracker3[] = " |    \\  \\ |    __)_  /  /_\\  \\|    | /    ~    \\  \\_____  \\   |    | /  /_\\  \\|       _/   |    |    |       _/ /  /_\\  \\/    \\  \\/|      <   |    __)_  |       _/\r\n";
rom char rom msgDeathStarTracker4[] = " |____|   \\|        \\/    |    \\    | \\    Y    /  /        \\  |    |/    |    \\    |   \\   |    |    |    |   \\/    |    \\     \\___|    |  \\ |        \\ |    |   \\\r\n";
rom char rom msgDeathStarTracker5[] = "/_______  /_______  /\\____|__  /____|  \\___|_  /  /_______  /  |____|\\____|__  /____|_  /   |____|    |____|_  /\\____|__  /\\______  /____|__ \\/_______  / |____|_  /\r\n";
rom char rom msgDeathStarTracker6[] = "        \\/        \\/         \\/              \\/           \\/                 \\/       \\/                     \\/         \\/        \\/        \\/        \\/         \\/ \r\n";
rom char rom msgWelcome[] = "\r\nEntered Remote Mode. Welcome!\r\n";
rom char rom msgBye[] =
                        "\r\n\n\tLeaving Remote Mode to Local Mode... \r\n";
rom char rom msgUser[] = "\ruser@";
rom char rom msgFactory[] = "\rfactory@";
rom char rom msgStarTracker[] = "starTracker";
rom char rom msgEndPrompt[] = "$ ";
rom char rom msgMaxReached[] = "\r\n\n\tYou don't need to type that much!\r\n";
rom char rom msgSeparatorLine[] = "\r\n------------------------------\r\n";
rom char rom msgDot[] = ". ";
rom char rom msgSlash[] = "/";
rom char rom msgBack[] = "back";
rom char rom msgNewLine[] = "\r\n";
rom char rom msgSpaceBackSpace[] = " \b";
rom char rom msgBackSpace[] = "\b";
rom char rom msgSpace[] = " ";
rom char rom msgArrow[] = ">";
rom char rom msgDigitError[] =
                        "\r\n\t<Invalid Command! Try entering option names or numbers!>\r\n";
rom char rom msgWelcomeFactory[] = "\r\nEntered Factory Mode\r\n";
rom char rom msgNumberError[] = "\r\n\t<Please input a valid number>\r\n";
rom char rom msgBigBackSpace[] = "\b";
rom char rom msgDeleteInFront[] =
                        "                \b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
char password[] = "e-VADER-s";
char msgClear[] = "clear";
char msgReset[] = "reset";

/* Global Variables */
char userInputBuffer[INPUTSIZE] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};     /* This stores the received characters */
char* scPtr = userInputBuffer;  /* This points to locations in the buffer */
char scPosition = 0;
char rcPosition = 0;         /* This indexes the buffer */
char cuePosition = 0;
char printPosition = 0;
char rcWord[2] = {'\0', '\0'};
char cursorPosition = 0;

char numberOfChildren = 0;

// Apparently you just do this to clear screen:
// http://stackoverflow.com/questions/10105666/clearing-the-terminal-screen

void clearScreen(void)
{
    char str1[] = "[2J";
    char str2[] = "[H";
    rcWord[0] = 27;
    
    printRamString(rcWord);
    printRamString(str1);
    printRamString(rcWord);
    printRamString(str2);

    /* Initialise: Start the program with a welcome message and prompt */
    printRomString(msgDeathStarTracker1);
    printRomString(msgDeathStarTracker2);
    printRomString(msgDeathStarTracker3);
    printRomString(msgDeathStarTracker4);
    printRomString(msgDeathStarTracker5);
    printRomString(msgDeathStarTracker6);
}

void welcomeRemoteMode(void)
{
    clearScreen();
    showChildOptions();
    printRomString(msgNewLine);
    printRomString(msgNewLine);
    prompt();
}

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

    welcomeRemoteMode();

    /* Config: Interrupt Control Register (Enable Interrupts) */
    INTCONbits.GIEH = 1;/* Enable global interrupts */
    INTCONbits.GIEL = 1;/* Enable peripheral interrupts */
}
