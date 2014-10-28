/**********************************************************
**  File: interface.c
***********************************************************
**  Author: Kelvin Hsu
**  Created on 2 September 2014, 11:42 PM
**  Assembled with C18 C Compiler v. 4.36
***********************************************************
**
**********************************************************/
#include "masterHeader.h"

int parsedNumber = 0;

void moveToParentNode(void)
{
	systemFlags.optionsShown = 0;
	if (currentNode == &rootNode)
	{
            return;
	}
	previousNode = currentNode;
	currentNode = currentNode->parent;
	currentNode->child = previousNode;
}

void moveToChildNode(void)
{
	systemFlags.optionsShown = 0;
	if (currentNode->child == NULL)
	{
            return;
	}
	previousNode = currentNode;
	currentNode = currentNode->child;
	currentNode->parent = previousNode; /* This means we don't have to declare parent */
}

void selectNextChild(void)
{
	/* If there is no child, then there is no selection to begin with */
	if (currentNode->child == NULL)
	{
		return;
	}

	/* If the child has no sibling, then there is no other selection */
	if ((currentNode->child)->sibling == NULL)
	{
		return;
	}

	currentNode->child = (currentNode->child)->sibling;
}

void selectChild(char selection)
{
	int i;

	for (i = 0; i < selection; i++)
	{
            selectNextChild();
	}
}

void moveToChildSelection(char selection)
{
    selectChild(selection);
    moveToChildNode();
}

void executeCurrentNodeFunction(void)
{
	NodeFunction fp = currentNode->stateFunction;
	if (fp == NULL)
	{
		return;
	}

	/* If okay, execute it */
	(*fp)();
}

void enterRemoteMode(void)
{
    INTCON3bits.INT1IE = LOW;
    systemFlags.LCDRequiresUpdate = 0;
    //LCDOnOff(0);
    
    systemFlags.factory = 0;
    systemFlags.remote = 1;
    systemFlags.optionsShown = 0;
    
    updateTreeStructure();
    //serialSetup();
    clearScreen();
    welcomeRemoteMode();
    PIE1bits.RCIE = 1;
}

void enterLocalMode(void)
{
    systemFlags.factory = 0;
    systemFlags.remote = 0;
    systemFlags.optionsShown = 0;
    PIE1bits.RCIE = 0;
    INTCON3bits.INT1IE = HIGH;
    updateTreeStructure();
    buttonsSetup();
    LCDInitialise();
    stringToRam(nodeNames[currentNode->label], LCDTopLine);
    LCDBottomLine[0] = '>';
    stringToRam(nodeNames[(currentNode->child)->label], LCDBottomLine + 1);
    systemFlags.LCDRequiresUpdate = 1;
}

void modeSwitch(void)
{

    if(systemFlags.factory)
    {
        currentNode = &rootNode;
        enterRemoteMode();
    }
    else if(systemFlags.remote)
    {
        printRomString(msgBye);
        while(toPrintIndex < cueIntoIndex) {handleTransmission;}
        enterLocalMode();
    }
    else // Otherwise we are in local mode
    {
        currentNode = &rootNode;
        enterRemoteMode();
    }

    currentNode = &rootNode;
}

/**
 * @brief Moves a string from from program memory to data memory
 * @details Fetches a requested string from data memory and places it in the
 *          destination variable
 *          Used specifically for strings, as the function terminates when it
 *          reaches a null character.
 *
 * @param source The source of the string in program memory
 * @param destination The position to place the characters in data memory
 */
void stringToRam(static char rom *source, static char *destination)
{
    while( (*destination++ = *source++) ){}
}

int convertToNumber(static char *inputString)
{
    int number = 0;
    unsigned char i = 0;

    unsigned int maxParseLength = 3;

    if(inputString[i] == '-')
    {
        i++;
        maxParseLength++;
    }

    while(inputString[i] != '\r')
    {
        if((inputString[i] < '0') || (inputString[i] > '9') || (i > maxParseLength))
        {
            systemFlags.userError = 1;
            return i; // for error detection
        }
        number = (number * 10) + (inputString[i++] - '0');
    }

    if(inputString[i] == '-')
    {
        number = 0 - number;
    }

    systemFlags.userError = 0;
    return number;
    // works!
}