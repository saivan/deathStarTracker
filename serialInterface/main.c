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
    treeSetup();
    serialSetup();
    setupRealTimeTimer();
    enterRemoteMode();
    //enterLocalMode();
}

/* 'main' function */
void main(void)
{
    /* Setup and Config the microcontroller */
    setup();
    for(EVER)
    {
        updateTime();

        if(systemFlags.remote)
        {
            handleReception();
            
            handleTransmission();
            if(systemFlags.updatePrompt)
            {
                systemFlags.updatePrompt = 0;
                showChildOptions();
                printRomString(msgNewLine);
                printRomString(msgNewLine);
                prompt();
            }
        }
        else
        {
            handleReceptionLocal();
            handleTransmissionLocal();
            updateLCD();

        }

        executeCurrentNodeFunction();
    }
}