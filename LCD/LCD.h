/* 
 * File:   LCD.h
 * Author: saivanhamama
 *
 * Created on September 29, 2014, 10:43 AM
 */

#ifndef LCD_H
#define	LCD_H

#include "masterHeader.h"

// Note that the LCD is being used in four bit mode
// RD<0:3> are used for the data bytes
// RD4 is the RS pin
// RD5 is the R/W - but we are always writing, so we ground
// RD6 is the enable pin (high then low to latch in a message)
#define LCD_DATAPins        0x0F // All lower Pins RD<0:3>
#define LCD_RSPin           PORTDbits.RD4
#define LCD_RWPin           PORTDbits.RD5
#define LCD_REPin           PORTDbits.RD6

// A range of LCD commands to execute
#define INITIALISE_LCD      0x03
#define FOURBITMODE_LCD     0x02
#define SET_DDRAM_ADDRESS   1<<7

#define COMMAND_LCD         1
#define CHARACTER_LCD       0

extern unsigned char currentLCDRow;
extern unsigned char currentLCDColumn;

typedef struct displayDigit{
    unsigned char upper;
    unsigned char middle;
    unsigned char lower;
    char characters[3];
} displayDigit;

extern displayDigit displayChars;



#pragma udata StringData
extern char LCDSWelcomeMessage[];
extern char LCDSPressGo[];
extern char LCDSMainMenu[];
extern char LCDSStatus[];
extern char LCDSTemperature[];
extern char LCDSGoto[];
extern char LCDSLimits[];
extern char LCDSRemote[];
extern char LCDSNotFound[];
extern char LCDSSearching[];
extern char LCDSAcquired[];
extern char LCDSDist[];
extern char LCDSAzim[];
extern char LCDSElev[];
extern char LCDSAzimuth[];
extern char LCDSElevation[];
extern char LCDSManually[];
extern char LCDSSet[];
extern char LCDSGotoPos[];
extern char LCDSMove[];
extern char LCDSArrows[];
extern char LCDSAngle[];
extern char LCDSSuccess[];
extern char LCDSOutOfRange[];
extern char LCDSReenter[];
extern char LCDSMinDist[];
extern char LCDSMaxDist[];
extern char LCDSMinAzim[];
extern char LCDSMaxAzim[];
extern char LCDSMinElev[];
extern char LCDSMaxElev[];
extern char LCDSValue[];


#pragma udata
extern rom const char rom *LCDStrings[];




void LCDEventHandler( void );

void LCDInitialise( void );
void delayMs( unsigned int miliseconds );
void intToDisplay( unsigned int displayVal );
void stringToRam( static char rom *source, static char *destination );
void LCDInstruction( char data , unsigned char isCommand );
void LCDMoveCursor( unsigned char line, unsigned char character );
void LCDPushString( char *string, unsigned char line );
void LCDWriteHere( char *string );

#endif	/* LCD_H */
