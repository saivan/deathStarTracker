/**********************************************************
**  File: interface.c
***********************************************************
**  Author: Kelvin Hsu
**  Created on 2 September 2014, 11:42 PM
**  Assembled with C18 C Compiler v. 4.36
***********************************************************
**
**********************************************************/
#include "system.h"


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
	NodeFunction fp = currentNode->ownFunction;
	if (fp == NULL)
	{
		return;
	}

	/* If okay, execute it */
	(*fp)();
}

void enterRemoteMode(void)
{
    systemFlags.factory = 0;
    systemFlags.remote = 1;
    updateTreeStructure();
    systemFlags.optionsShown = 0;
    LCDOnOff(0);
}

void enterLocalMode(void)
{
    printRomString(msgBye);
    systemFlags.factory = 0;
    systemFlags.remote = 0;
    systemFlags.optionsShown = 0;
    LCDOnOff(1);
    // ... back to local interface
}

void modeSwitch(void)
{

    if(systemFlags.factory)
    {
        enterRemoteMode();
    }
    else if(systemFlags.remote)
    {
        enterLocalMode();
    }
    else // Otherwise we are in local mode
    {
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