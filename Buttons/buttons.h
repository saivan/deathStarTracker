
#ifndef HELLO ///< if accidentally include header file twice, stops headers. if not defined, define it as hello
#define HELLO

#include "masterHeader.h"

extern volatile char gotShit;

#define NUMBERBUFFERSIZE 8
unsigned extern char numberBuffer[NUMBERBUFFERSIZE] = { 0, 0, 0, 0, 0, 0, 0, 0 }; //< extern makes these global

extern volatile int interuptCount;


extern int buttonPressValue;
extern int bufferLocation;
extern char buttonBuffer[4];


//typedef extern struct ButtonFlags{
///// Execution Flags
//unsigned char buttonPressed : 1; ///< A button has been pressed and we need to handle it
//unsigned char acceptingInput : 1; ///< The keypad is active and buttons can be pressed
//unsigned char numericalInput : 1; ///< The progam is expecting numerical input (up and down disabled)
///// Specific Button Flags
//unsigned char upPressed : 1;
//unsigned char downPressed : 1;
//unsigned char goPressed : 1;
//unsigned char backPressed : 1;
//} ButtonFlags;


#endif /*HELLO*/

