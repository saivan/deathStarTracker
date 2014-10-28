
#include "masterHeader.h"

unsigned char currentLCDRow = 0;
unsigned char currentLCDColumn = 0;
int digitDivisors[4] = { 1000, 100, 10, 1 };     ///< Avoids an un-necessary division

timeTag LCDUpdate = { 0, 0, 0, 2 };
displayDigit displayChars = { { 0, 0, 0, 0 } , { 0, 0, 0, 0, 0, 0, 0 } };
char LCDTopLine[16] = "hello Boob";
char LCDBottomLine[16] = "Tube";

/**
 * @brief Initialises the LCD module
 * @details Call this function before using any of the LCD module functions
 *          as it initialises the LCD module. This only needs to be called 
 *          <b>once at the beginning of the program</b>.
 */
void LCDInitialise( void ){

    /// Setup to work in four bit mode
    TRISD = 0x00;
    PORTD = 0x00;                                   ///< Clear PortD
    Delay10KTCYx(10);                                    ///< Wait for 15ms
    LCDInstruction( CLEAR_LCD, COMMAND_LCD );            ///< Four bit mode - 2 line - standard font
    Delay10KTCYx(1);                                    ///< Wait for 10ms
    LCDInstruction( CLEAR_LCD, COMMAND_LCD );            ///< Four bit mode - 2 line - standard font 
    Delay10KTCYx(1);                                     ///< Wait for 1ms
    LCDInstruction( CLEAR_LCD, COMMAND_LCD );            ///< Four bit mode - 2 line - standard font 
    Delay10KTCYx(1);                                     ///< Wait for 1ms
    LCDInstruction( FOURBITMODE_LCD, COMMAND_LCD );            ///< Four bit mode - 2 line - standard font 
    Delay10KTCYx(1);                                     ///< Wait for 1ms

    /// Now in Four bit mode, commands can be executed
    LCDInstruction( 0x28, COMMAND_LCD );            ///< Four bit mode - 2 line - standard font
    Delay10KTCYx(1);
    LCDInstruction( 0x0C, COMMAND_LCD );            ///< No cursor and no blinking
    Delay10KTCYx(1);
    LCDInstruction( 0x06, COMMAND_LCD );            ///< Automatic increment when character is added
    Delay10KTCYx(1);
    LCDInstruction( 0x80, COMMAND_LCD );            ///< Move to origin address of the DDRAM
    Delay10KTCYx(1);
    LCDInstruction( 0x01, COMMAND_LCD );            ///< Move to first digit
    Delay10KTCYx(1);

    /// Tell the LCD that it needs to update
    systemFlags.LCDRequiresUpdate = 1;

}


void updateLCD( void ){
    static char LCDUpdateState = 0;
    /// Update the LCD if an update is needed
    if( eventDue(&LCDUpdate) && systemFlags.LCDRequiresUpdate ){        
        if( LCDUpdateState == 0 ){             
            LCDInstruction(EMPTY_DISPLAY,COMMAND_LCD);
            setTimeTag(2,&LCDUpdate);
            LCDUpdateState++;
        } else if ( LCDUpdateState == 1 ){
            LCDWriteHere(LCDTopLine);
            LCDMoveCursor(1,0);
            setTimeTag(1,&LCDUpdate);
            LCDUpdateState++;
        } else if ( LCDUpdateState == 2 ){
            LCDWriteHere(LCDBottomLine);
            setTimeTag( LCD_REFRESH_TIME, &LCDUpdate );
            LCDUpdateState = 0;
        }
    }
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
 * @param line [0,1] - The zero indexed row to move to
 * @param character [0,15] - The zero indexed column to move to
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
 *          pipe the entire string to the LCD. There is no need to delay
 *          at the end of this function.
 * 
 * @param string A pointer to the string that you want to send to the LCD.
 */
void LCDWriteHere( char *string ){
    while( *string ){
        LCDInstruction( *string, CHARACTER_LCD );           ///< Write out each character to the current cursor location
        string++;                                           ///< Advance to the next character
        Delay10TCYx(2);                                     ///< Add in a 20 cycle delay before sending out the next character
    }
}


/**
 * @brief Converts an int to a display string
 * @details After converting the int, it is stored in the displayChars.characters
 *          variable as a string. This function <b>does not pipe the string to the 
 *          LCD</b>, it merely moves the result to the 
 * @todo ommiting the leading zeros is currently buggy. Works for four digit numbers
 * 
 * @param displayVal Is the int that you want displayed to the screen,
 *                   cast the variable to an int if you need any other type first.
 * @param decimalPlace [0-4] The digit from the right to display the decimal place.  
 *                   Make it 4 if you want no decimal place. Ie decimalPlace = 3 for
 *                   displayVal = 2341 will produce 234.1 etc...
 */
void intToDisplay( int displayVal , unsigned char decimalPlace ){
    unsigned char isNegative = 0;
    unsigned char hadNonZero = 0;
    int multiplier = 1000;
    int i;                                                                          ///< Iterrating over the digits
    int charPos = 0;                                                                ///< Iterrating over the characters


    /// Check for negative numbers
    if( displayVal < 0 ){
        isNegative = TRUE;                                                          ///< If the variable is negative, note it
        displayVal = -displayVal;                                                   ///< Then make it positive for further processing
    }

    /// Check for oversized Numbers, exit if oversized
    if( displayVal > 9999 )
        return;                                                                     ///< If the value to be displayed is too large, exit

    if( displayVal == 0 ){
        displayChars.characters[0] = '0';
        displayChars.characters[1] = '\0';
        return;
    }
//    systemFlags.LCDUpdate = 0;

    /// If the number was negative, add the negative sign
    if( isNegative )
        displayChars.characters[charPos++] = '-';

    /// Itterate over the characters and find their values
    for( i = 0 ; i < 4 ; i++ ){
        displayChars.digit[i] = displayVal/digitDivisors[i];                        ///< Divide out the most significant byte
        displayVal -= displayChars.digit[i]*digitDivisors[i];                       ///< Subtract off the most significant byte        
        if( displayChars.digit[i] )                                                 ///< Cull out any leading zeros by marking the first non-zero number
            hadNonZero = TRUE;        
        if( hadNonZero ){                                                            ///< Add the character to the string 
            displayChars.characters[charPos++] = displayChars.digit[i] + 0x30;  ///< Register the current character        
        } else {
            displayChars.characters[charPos++] = ' ';
        }
    }

    if( decimalPlace ){
        displayChars.characters[charPos+1] = '\0';
        for( i = 0 ; i < decimalPlace ; i++ ){
            displayChars.characters[charPos] = displayChars.characters[charPos-1];  ///< Move the current character over by one
            charPos--;                                                              ///< Move back one character
        }
        displayChars.characters[charPos] = '.';
    } else {
        displayChars.characters[charPos] = '\0';
    }

//    systemFlags.LCDUpdate = 1;
}



