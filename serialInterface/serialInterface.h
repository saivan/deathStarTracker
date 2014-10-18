/* 
 * File:   serialInterface.h
 * Author: kkeke_000
 *
 * Created on 16 October 2014, 9:14 PM
 */

#ifndef SERIALINTERFACE_H
#define	SERIALINTERFACE_H


    


/* Global Defines for ASCII Characters */
#define SIZE    100     /* The reserved size of the string we can store */
#define MAXPOS  50

/* Global String Literals */
extern unsigned char msgWelcome[];
extern unsigned char msgUser[];
extern unsigned char msgFactory[];
extern unsigned char msgStarTracker[];
extern unsigned char msgEndPrompt[];
extern unsigned char youTyped[];
extern unsigned char maxReached[];

/* Global Variables */
extern unsigned char circBuffer[SIZE];     /* This stores the received characters */
extern unsigned char* scPtr;  /* This points to locations in the buffer */
extern unsigned char* txPtr;
extern unsigned char scPosition;
extern unsigned char rcPosition;         /* This indexes the buffer */

extern unsigned char rcWord[2];

/* Serial Flags */
extern struct
{
  unsigned int RECEIVED : 1;
} Serial;

/* Function Declaration */
extern void highPriorityISR(void);
extern void receive(void);
extern void transmit(unsigned char* txPtr);
extern void prompt(void);
extern void serialSetup(void);

#endif	/* SERIALINTERFACE_H */

