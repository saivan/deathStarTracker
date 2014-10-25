/**********************************************************
**  File: main.c
***********************************************************
**  Author: Kelvin Hsu
**  Created on 2 September 2014, 11:42 PM
**  Assembled with C18 C Compiler v. 4.36
***********************************************************
**
**********************************************************/

#include "system.h"
#ifdef  USING_PIC18F4520
#include "ConfigPIC18F4520.h"
#endif

#ifdef  USING_PIC18F452
#include "ConfigPIC18F452.h"
#endif

void setup(void)
{
    systemFlags.remote = 1;
    treeSetup();
    serialSetup();
    buttonsSetup();
    LCDSetup();
    LCDOnOff(HIGH);
}

/* 'main' function */
void main(void)
{
    /* Setup and Config the microcontroller */
    setup();

    for(EVER)
    {
        if(systemFlags.remote)
        {
            handleReception();
        }
        else
        {
            handleReceptionLocal();
        }
        
        if(!PIE1bits.TXIE)
        {
            PIE1bits.RCIE = 1;
            if(toPrintIndex == cueIntoIndex)
            {
                cueIntoIndex = 0;
                toPrintIndex = 0;
            }
            if(toPrintIndex < cueIntoIndex)
            {
                if(romIndicator[toPrintIndex])
                {
                    txPtrRom = toPrintStrings[toPrintIndex++].romPtr;
                }
                else
                {
                    txPtrRam = toPrintStrings[toPrintIndex++].ramPtr;
                }
//                if(romramIndicator & BIT(toPrintIndex))
//                {
//                    txPtrRom = toPrintStringsRom[toPrintIndex++];
//                }
//                else
//                {
//                    txPtrRam = toPrintStringsRam[toPrintIndex++];
//                }
                PIE1bits.TXIE = 1;
            }
            PIE1bits.RCIE = 1;
        }

        executeCurrentNodeFunction();

        if(systemFlags.commandReceived)
        {
            systemFlags.commandReceived = 0;

            if(systemFlags.remote)
            {
                showChildOptions();
                printRomString(msgNewLine);
                printRomString(msgNewLine);
                prompt();
            }
            else
            {
                updateLocalInterface();
            }
        }
    }
}