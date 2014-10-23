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

        executeCurrentNodeFunction();

        if(systemFlags.commandReceived & systemFlags.remote)
        {
            systemFlags.commandReceived = 0;
            showChildOptions();
            prompt();
        }
    }
}