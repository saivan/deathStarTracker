
/*
* File:   buttons.h
* Author: kkeke_000
*
* Created on 16 October 2014, 9:14 PM
*/

#ifndef BUTTONS_H
#define	BUTTONS_H

typedef struct {
    /// Execution Flags
    unsigned int buttonPressed : 1; ///< A button has been pressed and we need to handle it
    unsigned int acceptingInput : 1; ///< The keypad is active and buttons can be pressed
    unsigned int numericalInput : 1; ///< The progam is expecting numerical input (up and down disabled)
    /// Specific Button Flags
    unsigned int upPressed : 1;
    unsigned int downPressed : 1;
    unsigned int goPressed : 1;
    unsigned int backPressed : 1;
    unsigned int bufferFull : 1;
} ButtonFlags;

extern ButtonFlags buttonFlags;
extern void buttonsSetup(void);

extern unsigned char buttonPressValue;

#endif	/* BUTTONS_H */

