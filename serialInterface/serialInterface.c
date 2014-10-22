
/**********************************************************
**  File: serialInterface.c
***********************************************************
**  Author: Kelvin Hsu
**  Created on 2 September 2014, 11:42 PM
**  Assembled with C18 C Compiler v. 4.36
***********************************************************
**
**********************************************************/


#include "system.h"

void handleReception(void)
{
    /* Relies on outer loop for multiple bytes */
    /* In order to save processor time for other stuff */
    if(scPosition < rcPosition)
    {
        rcWord[0] = circBuffer[scPosition];

        /* Echo what was received */
        cueInRamString(rcWord);

        /* If the character was not a carriage return, then */
        /* Note the change of position by incrementing and check whether or *
         * not we have reached the end of our buffer */
        if(scPosition++ >= MAXPOS)
        {
            cueInRomString(maxReached);
            rcWord[0] = '\r';
        }

        if(rcWord[0] == '\r')
        {
            /* If the character is a carriage return, or if the maximum position of *
             * the buffer was reached, then do the following:                       */

            /* Attach LF, and NULL to the end and note the change of position */
            circBuffer[scPosition] = '\0';

            /* Transmit data back to the user and re-prompt them again */
            cueInRomString(youTyped);
            cueInRamString(circBuffer);
            prompt();

            scPosition = 0;
            rcPosition = 0;
        }

    }

}

/* 'tx232' function: Transmits characters of a given null terminated string */
/* Arguments: [1] A pointer to the start of a string */
void transmit(unsigned char* str)
{ 
    /* Copy the pointer, so that the original message is not modified */
    unsigned char* pchar = str;

    /* Transmit the message while the character is not a NULL character */
    while(*pchar != '\0')
    {
        /* Wait for Transmit Buffer, TXREG, to be empty */
        while(PIR1bits.TXIF == 0) {/* Do nothing and just wait */}

        /* Once it is empty, transmit the message*/
        TXREG = *pchar++;
    }
}

//void transmitFromRom(static char rom *romMessage)
//{
//    stringToRam(romMessage, transmitBuffer);
//    transmit(transmitBuffer);
//}

void cueInRamString(unsigned char* ramMessage)
{
    stringsToTransmit[cuePosition] = ramMessage;
    cuePosition++;
    if(cuePosition > SIZE)
    {
        cuePosition = 0;
    }
}

void cueInRomString(static char rom *romMessage)
{
    stringToRam(romMessage, stringsToTransmit[cuePosition]);
    cuePosition++;
    if(cuePosition > SIZE)
    {
        cuePosition = 0;
        systemFlags.cueOverflow = 1;
    }
}

void handleTransmission(void)
{
    if(systemFlags.cueOverflow || (printPosition < cuePosition))
    {
        transmit(stringsToTransmit[printPosition]);
        printPosition++;
        if(printPosition > SIZE)
        {
            printPosition = 0;
            systemFlags.cueOverflow = 0;
        }
    }
}
// ----- functions -----
void prompt(void)
{
    
    if(systemFlags.factory)
    {
        cueInRomString(msgFactory);
    }
    else
    {
        cueInRomString(msgUser);
    }
    
    cueInRomString(msgStarTracker);
    cueInRomString(msgEndPrompt);
}
