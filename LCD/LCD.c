
#include "masterHeader.h"

unsigned char currentLCDRow = 0;
unsigned char currentLCDColumn = 0;


displayDigit displayChars = { 0, 0, 0, 0 };

char LCDSWelcomeMessage[] = "DeathStarTracker";
char LCDSPressGo[] = "Press <GO>";
char LCDSMainMenu[] = "Main Menu";
char LCDSStatus[] = "Target Status";
char LCDSTemperature[] = "Temperature";
char LCDSGoto[] = "Goto Position";
char LCDSLimits[] = "Set Limits";
char LCDSRemote[] = "Remote Mode";
char LCDSNotFound[] = "Target Not Found";
char LCDSSearching[] = "  Searching...";
char LCDSAcquired[] = "Target Acquired";
char LCDSDist[] = "Dist";
char LCDSAzim[] = "Azim";
char LCDSElev[] = "Elev";
char LCDSAzimuth[] = "Azimuth";
char LCDSElevation[] = "Elevation";
char LCDSManually[] = "Manually";
char LCDSSet[] = "Set ";
char LCDSGotoPos[] = "Goto ";
char LCDSMove[] = "Manual Move";
char LCDSArrows[] = "Use Arrows";
char LCDSAngle[] = "Angle: ";
char LCDSSuccess[] = "Entry Successful";
char LCDSOutOfRange[] = "Out of Range";
char LCDSReenter[] = "Re-enter Input";
char LCDSMinDist[] = "Distance Min";
char LCDSMaxDist[] = "Distance Max";
char LCDSMinAzim[] = "Azimuth Min";
char LCDSMaxAzim[] = "Azimuth Max";
char LCDSMinElev[] = "Elevation Min";
char LCDSMaxElev[] = "Elevation Max";
char LCDSValue[] = "Value:";


// These are stored in program memory,
// use the stringToRam function to
// move the string into data memory when
// it is needed
rom const char rom *LCDStrings[] = {
/*00*/ "DeathStarTracker",
/*01*/ "Press <GO>",
/*02*/ "Main Menu",
/*03*/ "Target Status",
/*04*/ "Temperature",
/*05*/ "Goto Position",
/*06*/ "Set Limits",
/*07*/ "Remote Mode",
/*08*/ "Target Not Found",
/*09*/ "  Searching...",
/*10*/ "Target Acquired",
/*11*/ "Dist",
/*12*/ "Azim",
/*13*/ "Elev",
/*14*/ "Azimuth",
/*15*/ "Elevation",
/*16*/ "Manually",
/*17*/ "Set ",
/*18*/ "Goto ",
/*19*/ "Manual Move",
/*20*/ "Use Arrows",
/*21*/ "Angle: ",
/*22*/ "Entry Successful",
/*23*/ "Out of Range",
/*24*/ "Re-enter Input",
/*25*/ "Distance Min",
/*26*/ "Distance Max",
/*27*/ "Azimuth Min",
/*28*/ "Azimuth Max",
/*29*/ "Elevation Min",
/*30*/ "Elevation Max",
/*31*/ "Value:"
};

// This function is used purely in the first initialisation of the LCD
// It <b>should not be used anywhere else</b> as it stalls the program whilst
// it is opperating. Use only if absolutely necessary
void delayMs( unsigned int miliseconds ){
    unsigned int cycles = miliseconds * 500;
    for ( ; cycles > 0 ; cycles-- ){}
}

void LCDInitialise( void ){

    // Setup to work in four bit mode
    PORTD = 0x00;                                   // Clear PortD
    delayMs(15);                                    // Wait for 15ms
    LCDInstruction( 0x30, COMMAND_LCD );            // Four bit mode - 2 line - standard font
    delayMs(10);                                    // Wait for 10ms
    LCDInstruction( 0x30, COMMAND_LCD );            // Four bit mode - 2 line - standard font       // First initiation// Second initiation value
    delayMs(1);                                     // Wait for 1ms
    LCDInstruction( 0x30, COMMAND_LCD );            // Four bit mode - 2 line - standard font       // Third initiation value
    delayMs(1);                                     // Wait for 1ms
    LCDInstruction( 0x20, COMMAND_LCD );            // Four bit mode - 2 line - standard font      // Set bus to four bit mode
    delayMs(1);                                     // Wait for 1ms

    // Now in Four bit mode, commands can be executed
    LCDInstruction( 0x28, COMMAND_LCD );            // Four bit mode - 2 line - standard font
    delayMs(1);
    LCDInstruction( 0x0C, COMMAND_LCD );            // No cursor and no blinking
    delayMs(1);
    LCDInstruction( 0x06, COMMAND_LCD );            // Automatic increment when character is added
    delayMs(1);
    LCDInstruction( 0x80, COMMAND_LCD );            // Move to origin address of the DDRAM
    delayMs(1);
    LCDInstruction( 0x01, COMMAND_LCD );            // Move to first digit
    delayMs(1);

}




/**
 * @brief Turns on or off the LCD
 * @details Call this function to turn on or off the LCD
 *          If called with turnOn = true, it will turn on
 *          else, if its called with turnOff = false, it turns off
 * 
 * @param turnOn [true/false], decides if the LCD should be on
 */ 
void LCDOnOff( unsigned char turnOn ){
    if( turnOn ){
        LCDInstruction( 0b00001100 , COMMAND_LCD );
    } else { // Turn off
        LCDInstruction( 0b00001000 , COMMAND_LCD );
    }
}

/**
 * @brief Send an instruction to the LCD
 * @details The instruction can either be a command or 
 *          a piece of data for display. 
 *          
 *          :: If A command is sent, it will be executed,
 *             just <em> include enough time for the 
 *             execution (1ms)</em>.
 *          :: Similarly, if a character is sent, it will be
 *             displayed at the current position and the cursor
 *             will increment, just <em>remember the 1ms delay</em>. 
 *             
 *          Note that you can write multiple characters before needing
 *          to delay the microcontroller for an update, but only one command.
 * 
 * @param data The actual data to send to the LCD
 * @param command [True/False] - true if the data is a command
 */
void LCDInstruction( char data , unsigned char isCommand ){

    static char highNibble;

    if ( isCommand ){ LCD_RSPin = 0; }                          // Turn off RS to send a command
    else { LCD_RSPin = 1; }                                     // Turn on RS to send a character

    // Send the Upper nibble
    highNibble = ((data & 0x0F)<<4) | (( data & 0xF0 )>>4);     // We store the high nibble
    PORTD = (PORTD & 0xF0) | ( 0x0F & highNibble );             // We send the lower nibble of the command with Enable
    LCD_REPin = 0;                                              // Twiddle Enable to latch command in
    LCD_REPin = 1;

    // Send the Lower nibble    
    PORTD = (PORTD & 0xF0) | ( 0x0F & data );                   // We send the Upper byte of the command with Enable
    LCD_REPin = 0;                                              // Twiddle Enable to latch command in
    LCD_REPin = 1;

}

/**
 * @brief Moves the write position on the display
 * @details This function moves the write head to a new position so that
 *          all new input characters will be writen to the location specified.
 *          
 *          Note that the line and the characters are <em> zero indexed </em>, so
 *          the first line is accessed by line = 0 and the first character by
 *          character = 0. Remember to <b> delay for 1ms </b> after calling.           
 * 
 * @param line [0,1] - The line to move to
 * @param character [0,15] - The pixel to move to
 */
void LCDMoveCursor( unsigned char line, unsigned char character ){
    currentLCDRow = line;                                                ///< Keep track of the current line
    currentLCDColumn = character;                                        ///< Keep track of the current column
    line *= 0x40;                                                        ///< Line 2 is found at 0x40 on the LCDs internal RAM
    LCDInstruction( SET_DDRAM_ADDRESS|(line+character) , COMMAND_LCD );  ///< Command to move to input location
}

/**
 * @brief Prints a string in RAM to the LCD at the given location
 * @details Given the address of a string, this function will
 *          pipe the entire string to the LCD. Remeber to delay
 *          after this function is called.
 * 
 * @param string A pointer to the string that you want to send to the LCD.
 */
void LCDWriteHere( char *string ){
    while( *string ){
        LCDInstruction( *string, CHARACTER_LCD );           ///< Write out each character to the current cursor location
        string++;                                           ///< Advance to the next character
    }
}


void LCDPushString( char *string, unsigned char line ){

    if ( currentLCDRow != line ){
        if ( line == 0 ){
            LCDMoveCursor( line, 0 );
        } else if ( line == 1 ) {
            LCDMoveCursor( line, 0 );
        }
        // else // Error

        // Setup next time to run this function
        return; // quit  the function
    }

    while( *string ){
        LCDInstruction( *string, CHARACTER_LCD );
        string++;
    }
    // Turn off the sending flag
    // finished sending

}

/**
 * @brief Moves a string from Program to data Memory
 * @details Fetches a requested string from data memory and places it in the
 *          destination variable. Use specifically for strings, as the function
 *          terminates when it reaches a null character.
 * 
 * @param source The source of the string in program memory
 * @param destination The position to place the characters in data memory
 */
void stringToRam( static char rom *source, static char *destination ){
    while( (*destination++ = *source++) != '\0' ){}
}




void intToDisplay( unsigned int displayVal ){

    if( displayVal > 999 )
        return;                             ///< If the value to be displayed is to large, exit
    /// Working on the most significant byte
    displayChars.upper = displayVal/100;    ///< Get the highest byte
    displayVal -= (unsigned int)(displayChars.upper)*100;   ///< Subtract the highest byte from the input
    /// Working on the middle byte
    displayChars.middle = displayVal/10;    ///< Get the middle byte
    displayVal -= (unsigned int)(displayChars.middle)*10;   ///< Subtract the middle byte from the input
    /// Working on the low byte
    displayChars.lower = displayVal;        ///< We have the value left alone

    /// Now we copy the characters into the string and add 0x30 to convert them to ascii values
    displayChars.characters[0] = displayChars.upper + 0x30;
    displayChars.characters[1] = displayChars.middle + 0x30;
    displayChars.characters[2] = displayChars.lower + 0x30;

}


