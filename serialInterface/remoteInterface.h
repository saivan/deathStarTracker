/* 
 * File:   serialInterface.h
 * Author: kkeke_000
 *
 * Created on 16 October 2014, 9:14 PM
 */

#ifndef SERIALINTERFACE_H
#define	SERIALINTERFACE_H

#include "realTimer.h"
/* Global Defines for ASCII Characters */
#define INPUTSIZE    20     /* The reserved size of the string we can store */
#define TOPRINTSIZE  75
#define CUESIZE      72
#define MAXPOS       16
#define BACK         'b'

extern timeTag enableReceptionTag;

union RamRomPointer
{
    char * ramPtr;
    rom char rom * romPtr;
};

typedef struct RomRamIndicator RomRamIndicator;

struct RomRamIndicator {
    
    unsigned int inRom : 1;
};

/* Global String Literals */
extern rom char rom msgPressHelp[];
extern rom char rom msgUpUp[];
extern rom char rom msgInfo[];
extern rom char rom msgHelpDescription[];
extern rom char rom msgHelpSpecialCommands[];
extern rom char rom msgHelpMoreCommands[];
extern rom char rom msgWelcome[];
extern rom char rom msgBye[];
extern rom char rom msgUser[];
extern rom char rom msgFactory[];
extern rom char rom msgEndPrompt[];
extern rom char rom msgMaxReached[];
extern rom char rom msgSeparatorLine[];
extern rom char rom msgTab[];
extern rom char rom msgDot[];
extern rom char rom msgSlash[];
extern rom char rom msgBack[];
extern rom char rom msgNewLine[];
extern rom char rom msgSpaceBackSpace[];
extern rom char rom msgBackSpace[];
extern rom char rom msgBackSpaceSpaceBackSpace[];
extern rom char rom msgSpace[];
extern rom char rom msgInvalidCommand[];
extern rom char rom msgWelcomeFactory[];
extern rom char rom msgNumberError[];
extern rom char rom msgArrow[];
extern rom char rom msgBigBackSpace[];
extern rom char rom msgDeleteInFront[];
extern rom char rom msgDefault[];
extern rom char rom msgCR[];
extern rom char rom msgValueGood[];
extern rom char rom msgValueBad[];
extern rom char rom msgSpaces[];

extern rom char rom password[];
extern rom char rom msgClear[];
extern rom char rom msgReset[];
extern rom char rom msgHelp[];
extern rom char rom msgSpecial[];
extern rom char rom msgQuick[];
extern rom char rom msgHome[];
extern char arrowKeys[];
extern rom char rom *numberChars[];
/* Global Variables */
/* Global Variables */
extern char userInputBuffer[INPUTSIZE];     /* This stores the received characters */
extern char scPosition;
extern char rcPosition;         /* This indexes the buffer */
extern char rcWord[2];

extern union RamRomPointer txPtr;
extern char cueIntoIndex;
extern char toPrintIndex;
extern union RamRomPointer toPrintStrings[TOPRINTSIZE];
extern RomRamIndicator romramIndicator[TOPRINTSIZE];
extern char numberOfChildren;

/* Function Declaration */
extern void serialSetup(void);
extern void welcomeRemoteMode(void);
extern void handleReception(void);
extern void handleTransmission(void);
extern void printRamString(static char *ramMessage);
extern void printRomString(static char rom *romMessage);
extern void prompt(void);
//extern void parseUserInput(static char *inputString);
extern char checkPassword(static char *inputString);
extern void showChildOptions(void);
extern char checkClear(static char *inputString);
extern void checkReset(static char *inputString);
extern void clearScreen(void);
extern char checkIfNodeNameTyped(static char *inputString);
extern char checkDeathStarTracker(static char *inputString);
extern char checkHelp(static char *inputString);
extern char checkSpecial(static char *inputString);
extern char checkQuick(static char *inputString);
extern char checkHome(static char *inputString);
extern char tryAutoComplete(static char *inputString);
extern void reprompt(void);
//extern volatile char junkInput;
#endif	/* SERIALINTERFACE_H */


// DEATHSTAR ASCII ART

