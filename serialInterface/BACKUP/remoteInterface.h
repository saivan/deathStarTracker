/* 
 * File:   serialInterface.h
 * Author: kkeke_000
 *
 * Created on 16 October 2014, 9:14 PM
 */

#ifndef SERIALINTERFACE_H
#define	SERIALINTERFACE_H


/* Global Defines for ASCII Characters */
#define INPUTSIZE    25     /* The reserved size of the string we can store */
#define STRINGSIZE   170
#define TOPRINTSIZE  25
#define MAXPOS       16
#define BACK         'b'

union RamRomPointer
{
    char * ramPtr;
    rom char rom * romPtr;
};

/* Global String Literals */
extern rom char rom msgWelcome[];
extern rom char rom msgBye[];
extern rom char rom msgUser[];
extern rom char rom msgFactory[];
extern rom char rom msgStarTracker[];
extern rom char rom msgEndPrompt[];
extern rom char rom msgMaxReached[];
extern rom char rom msgSeparatorLine[];
extern rom char rom msgDot[];
extern rom char rom msgSlash[];
extern rom char rom msgBack[];
extern rom char rom msgNewLine[];
extern rom char rom msgSpaceBackSpace[];
extern rom char rom msgBackSpace[];
extern rom char rom msgSpace[];
extern rom char rom msgDigitError[];
extern rom char rom msgWelcomeFactory[];
extern rom char rom msgNumberError[];
extern rom char rom msgArrow[];
extern rom char rom msgBigBackSpace[];
extern rom char rom msgDeleteInFront[];
extern char password[];
extern char msgClear[];
extern char msgReset[];

/* Global Variables */
extern char userInputBuffer[INPUTSIZE];     /* This stores the received characters */
extern char romToRamBuffer[STRINGSIZE];
extern char* scPtr;  /* This points to locations in the buffer */
extern char scPosition;
extern char rcPosition;         /* This indexes the buffer */
extern char cuePosition;
extern char printPosition;
extern char rcWord[2];

//extern union RamRomPointer txPtr = NULL;
extern char * txPtrRam;
extern rom char rom * txPtrRom;
extern char cueIntoIndex;
extern char toPrintIndex;
//extern union RamRomPointer toPrintStrings[TOPRINTSIZE];
extern char * toPrintStringsRam[TOPRINTSIZE];
extern char * toPrintStringsRom[TOPRINTSIZE];
extern unsigned long romramIndicator;

extern char numberOfChildren;

/* Function Declaration */
extern void serialSetup(void);
extern void handleReception(void);
extern void printRamString(static char *ramMessage);
extern void printRomString(static char rom *romMessage);
extern void prompt(void);
extern void parseUserInput(static char *inputString);
extern char checkPassword(static char *inputString);
extern void showChildOptions(void);
extern char checkClear(static char *inputString);
extern void checkReset(static char *inputString);
extern void clearScreen(void);
extern char checkIfNodeNameTyped(static char *inputString);
extern char tryAutoComplete(static char *inputString);
#endif	/* SERIALINTERFACE_H */


// DEATHSTAR ASCII ART

