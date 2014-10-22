/* 
 * File:   serialInterface.h
 * Author: kkeke_000
 *
 * Created on 16 October 2014, 9:14 PM
 */

#ifndef SERIALINTERFACE_H
#define	SERIALINTERFACE_H


/* Global Defines for ASCII Characters */
#define INPUTSIZE    15     /* The reserved size of the string we can store */
#define STRINGSIZE   40
#define MAXPOS       10
#define BACK         'b'

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
extern char password[];

/* Global Variables */
extern char userInputBuffer[INPUTSIZE];     /* This stores the received characters */
extern char* scPtr;  /* This points to locations in the buffer */
extern char scPosition;
extern char rcPosition;         /* This indexes the buffer */
extern char cuePosition;
extern char printPosition;
extern char rcWord[2];
extern char cursorPosition;

extern char numberOfChildren;

/* Function Declaration */
extern void serialSetup(void);
extern void handleReception(void);
//extern void handleTransmission(void);
//extern void transmit(static char* txPtr);
//extern void transmitFromRom(static char rom *romMessage);
extern void printRamString(static char *ramMessage);
extern void printRomString(static char rom *romMessage);
extern void prompt(void);
extern void parseUserInput(static char *inputString);
extern char checkPassword(static char *inputString);
extern void showChildOptions(void);
#endif	/* SERIALINTERFACE_H */


// DEATHSTAR ASCII ART

