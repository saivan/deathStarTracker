



#include <stdio.h>

#define TRUE 1

typedef struct displayDigit{
    int digit[4];
    char characters[7];
} displayDigit;

displayDigit displayChars = { {8,9}, {2,3,5} };

int digitDivisors[4] = { 1000, 100, 10, 1 };     ///< Avoids an un-necessary division


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

    /// If the number was negative, add the negative sign
    if( isNegative )
        displayChars.characters[charPos++] = '-';

    /// Itterate over the characters and find their values
    for( i = 0 ; i < 4 ; i++ ){

        displayChars.digit[i] = displayVal/digitDivisors[i];                        ///< Divide out the most significant byte
        displayVal -= displayChars.digit[i]*digitDivisors[i];                       ///< Subtract off the most significant byte
        
        /// Cull out any leading zeros by marking the first non-zero number
        if( displayChars.digit[i] )
            hadNonZero = TRUE;
    
        if( hadNonZero ){    
            if( charPos == decimalPlace ){
                displayChars.characters[charPos++] = '.';                               ///< Add in the decimal Place and advance
                displayChars.characters[charPos++] = displayChars.digit[i] + 0x30;      ///< Register the current character
            } else {
                displayChars.characters[charPos++] = displayChars.digit[i] + 0x30;      ///< Register the current character and advance
            }
        }
    }

    /// Add in the null character at the end
    displayChars.characters[charPos++] = '\0';
}



int main(void){
	int elevationAngle = 2236;
	intToDisplay(elevationAngle,3);
	printf("%s\n", displayChars.characters );	
    printf("%s\n", "hello" );

}



