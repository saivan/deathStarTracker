/**********************************************************
**  File: serialSetup.c
***********************************************************
**  Author: Kelvin Hsu
**  Created on 2 September 2014, 11:42 PM
**  Assembled with C18 C Compiler v. 4.36
***********************************************************
**
**********************************************************/

#include "masterHeader.h"

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
rom char rom msgPressHelp[] = "\r\n\n\t<Enter 'help' for help!>\r";
rom char rom msgUpUp[] = {ESC, '[', 'A', ESC, '[', 'A', '\0'};
rom char rom msgInfo[] = "\r\n\n\tThis death star tracker is built by:\r\n\n\t\tTeam Germain\r\n\t\t------------\r\n\t\tKelvin Hsu\r\n\t\tSaivan Hamama\r\n\t\tKye Ridley-Smith\r\n\t\tDavid Jimenez\r\n\t\tMichael Rowlinson\r\n\n";
rom char rom msgHelpDescription[] =     "\r\n\n\tHelp Menu                \r\n\n\t---------\r\n\n\tTo select an option from the menu, you can:\r\n\n\t\t1. Enter the option number, as shown in the options menu\r\n\t\t2. Use the option cycle feature: Cycle through the options by pressing up or down after you enter at least one option number\r\n\t\t3. Enter the full name of the option\r\n\t\t4. Use the Auto-Complete Feature: Enter the option name partially and press tab to quickly obtain the option name*\r\n\n\tNote<*>: The auto complete feature will find the first option match in the menu starting from the first non-default option (numbered 1).\r\n\t   If you would like to quickly select the default option instead, simply press enter to get the default option (numbered 0)\r\n\n\tHint: Enter 'special' for a list of special commands and 'quick' for a list of quick navigation tricks!\r\n\n";
rom char rom msgHelpSpecialCommands[] = "\r\n\n\tSpecial Commands         \r\n\n\t----------------\r\n\n\tYou also have access to the follow commands:\r\n\n\t\t1. clear \t\t\t\t<Clear the screen>\r\n\t\t2. reset \t\t\t\t<Reset the death star tracker>\r\n\t\t3. home \t\t\t\t<Go back to home menu>\r\n\t\t4. DeathStarTracker \t\t\t<Get to know us!>\r\n\n";
rom char rom msgHelpMoreCommands[] =    "\r\n\n\tQuick Navigation         \r\n\n\t----------------\r\n\n\tOn top of the option cycle and auto-complete features, you can navigate through-out the menus quickly by:\r\n\n\t\t1. Pressing 'b' for going back\r\n\t\t2. Pressing backspace at the prompt for going back\r\n\t\t3. Pressing enter at the prompt to choose the default option (numbered 0)\r\n\t\t4. Use the command 'home' to quickly get back to the main menu\r\n\n";
rom char rom msgWelcome[] = "\r\nEntered Remote Mode. Welcome!\r\n";
rom char rom msgBye[] =
                        "\r\n\n\t<Leaving Remote Mode to Local Mode...>\r\n";
rom char rom msgUser[] = "\ruser@";
rom char rom msgFactory[] = "\rfactory@";
rom char rom msgEndPrompt[] = "$ ";
rom char rom msgMaxReached[] = "\r\n\n\t<You don't need to type that much!>\r\n";
rom char rom msgSeparatorLine[] = "\r\n-----------------------------------------\r\n";
rom char rom msgTab[] = "\t";
rom char rom msgDot[] = ". ";
rom char rom msgSlash[] = "/";
rom char rom msgBack[] = "back";
rom char rom msgNewLine[] = "\r\n";
rom char rom msgSpaceBackSpace[] = " \b";
rom char rom msgBackSpace[] = "\b";
rom char rom msgBackSpaceSpaceBackSpace[] = "\b \b";
rom char rom msgSpace[] = " ";
rom char rom msgArrow[] = ">";
rom char rom msgInvalidCommand[] =
                        "\r\n\n\t<Invalid Command! Try entering option names or numbers!>\r\n";
rom char rom msgWelcomeFactory[] = "|         Entering Factory Mode         |";
rom char rom msgNumberError[] = "\r\n\t<Please input a valid number>\r\n";
rom char rom msgBigBackSpace[] = "\b";
rom char rom msgDeleteInFront[] =
                        "                \b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
rom char rom msgDefault[] = "\r\t\t\t\t<Default>";
rom char rom msgCR[] = "\r";
rom char rom msgSpaces[] = "     ";
rom char rom msgValueGood[] = "\r\n\n\t<Input Number Received>\r\n\n";
rom char rom msgValueBad[] = "\r\n\n\t<Not a number>\r\n\n";
rom char rom msgOutOfRange[] = "\r\n\n\t<Number of out range>\r\n\n";
rom char rom msgInputDone[] = "\r\n\n\t<Input Number Accepted>\r\n\n";

rom char rom *numberChars[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
rom char rom clearChars1[] = "[2J";
rom char rom clearESC[] = {ESC, NULL};
rom char rom clearChars2[] = "[H";

rom char rom password[] = "e~VADER|s";
rom char rom msgClear[] = "clear";
rom char rom msgReset[] = "reset";
rom char rom msgHelp[] = "help";
rom char rom msgSpecial[] = "special";
rom char rom msgQuick[] = "quick";
rom char rom msgHome[] = "home";
char arrowKeys[] = {ESC, '[', NULL, NULL};

/* Global Variables */
char userInputBuffer[INPUTSIZE];     /* This stores the received characters */
char scPosition = 0;
char rcPosition = 0;         /* This indexes the buffer */
char rcWord[2] = {'\0', '\0'};

union RamRomPointer txPtr = NULL;
char cueIntoIndex = 0;
char toPrintIndex = 0;
union RamRomPointer toPrintStrings[TOPRINTSIZE];
RomRamIndicator romramIndicator[TOPRINTSIZE];
char numberOfChildren = 0;

//volatile char junkInput = 0;
// Apparently you just do this to clear screen:
// http://stackoverflow.com/questions/10105666/clearing-the-terminal-screen

void clearScreen(void)
{   
    printRomString(clearESC);
    printRomString(clearChars1);
    printRomString(clearESC);
    printRomString(clearChars2);
    
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
    showChildOptions();
    printRomString(msgNewLine);
    printRomString(msgNewLine);
    prompt();
}

/* 'setup' function: Setup the Configurations for Relevant Registers */
void serialSetup(void)
{
    //transmitBuffer[INPUTSIZE - 1] = '\0';
    /* Config: Interrupt Control Register (Disable Interrupts during Setup) */
    //INTCON = ZERO;      /*Disable Global and Peripheral Interrupts*/

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
//    IPR1 = ZERO;
//    IPR2 = ZERO;
    IPR1bits.RCIP = 1;  /* Make USART Receive  Interrupt a Low Priority */
    IPR1bits.TXIP = 1;
    
    /* Config: PORTC [Make PORTC<7:6> our serial I/O pins] */
    TRISCbits.RC6 = 0;  /* Make RC6 an output as TX */
    TRISCbits.RC7 = 1;  /* Make RC7 an input  as RX */

    /* Config: Reset Control Register */
    RCONbits.IPEN = 1;  /* Enable priority levels */

    /* Config: Interrupt Control Register (Enable Interrupts) */
    INTCONbits.GIEH = 1;/* Enable global interrupts */
    INTCONbits.GIEL = 1;/* Enable peripheral interrupts */
}
