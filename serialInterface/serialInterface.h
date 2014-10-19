/* 
 * File:   serialInterface.h
 * Author: kkeke_000
 *
 * Created on 16 October 2014, 9:14 PM
 */

#ifndef SERIALINTERFACE_H
#define	SERIALINTERFACE_H


/* Global Defines for ASCII Characters */
#define SIZE    10     /* The reserved size of the string we can store */
#define MAXPOS  5

/* Global String Literals */
extern rom char rom msgWelcome[];
extern rom char rom msgUser[];
extern rom char rom msgFactory[];
extern rom char rom msgStarTracker[];
extern rom char rom msgEndPrompt[];
extern rom char rom youTyped[];
extern rom char rom maxReached[];

/* Global Variables */
extern unsigned char circBuffer[SIZE];     /* This stores the received characters */
extern unsigned char* scPtr;  /* This points to locations in the buffer */
extern unsigned char transmitBuffer[SIZE];
extern unsigned char scPosition;
extern unsigned char rcPosition;         /* This indexes the buffer */
extern unsigned char* stringsToTransmit[SIZE];
extern unsigned char cuePosition;
extern unsigned char printPosition;
extern unsigned char rcWord[2];

/* Function Declaration */
extern void serialSetup(void);
extern void handleReception(void);
extern void handleTransmission(void);
extern void transmit(unsigned char* txPtr);
//extern void transmitFromRom(static char rom *romMessage);
extern void cueInRamString(unsigned char* ramMessage);
extern void cueInRomString(static char rom *romMessage);
extern void prompt(void);

#endif	/* SERIALINTERFACE_H */

