
/* 
 * File:   LCD.h
 * Author: saivanhamama
 *
 * Created on September 29, 2014, 10:43 AM
 */

#ifndef LCD_H
#define	LCD_H

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
#define EMPTY_DISPLAY		0x01
#define CLEAR_LCD      		0x03
#define FOURBITMODE_LCD     0x02
#define SET_DDRAM_ADDRESS   1<<7

#define COMMAND_LCD         1
#define CHARACTER_LCD       0

extern unsigned char currentLCDRow;
extern unsigned char currentLCDColumn;
extern int digitDivisors[4];

typedef struct displayDigit{
    int digit[4];
    char characters[7];
} displayDigit;

extern displayDigit displayChars;

extern void LCDEventHandler( void );

extern void LCDInitialise( void );
extern void intToDisplay( int displayVal , unsigned char decimalPlace );
extern void LCDInstruction( char data , unsigned char isCommand );
extern void LCDMoveCursor( unsigned char line, unsigned char character );
extern void LCDWriteHere( char *string );

#endif	/* LCD_H */
