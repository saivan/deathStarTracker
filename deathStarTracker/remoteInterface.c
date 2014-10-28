/**********************************************************
**  File: serialInterface.c
***********************************************************
**  Author: Kelvin Hsu
**  Created on 2 September 2014, 11:42 PM
**  Assembled with C18 C Compiler v. 4.36
***********************************************************
**
**********************************************************/


#include "masterHeader.h"

char currentOption = 0;

char isCharSelectionDigit(char character)
{
    if( ((character - '0') < numberOfChildren) && ((character - '0') >= 0) )
    {
        return 1;
    }

    return 0;
}

void freshNodePromptSettings(void)
{
    scPosition = 0;
    rcPosition = 0;
    systemFlags.userChosen = 0;
    systemFlags.userError = 0;
    systemFlags.commandReceived = 0;
    systemFlags.optionsShown = 0;
    systemFlags.updatePrompt = 1;
}

char scanAvailableCharacters(void)
{
    /// If the user presses enter when they have not entered anything,
    /// pretend they have entered the current option
    if(userInputBuffer[scPosition] == '\r')
    {
        if(systemFlags.userChosen)
        {
            moveToChildSelection(currentOption);
            freshNodePromptSettings();
            return 1;
        }
        else if(!scPosition)
        {
            if(currentNode->child)
            {
                userInputBuffer[scPosition] = '0';
            }
            return 0;
        }
    }

    else if(userInputBuffer[scPosition] == '\b')
    {
        /// We can only back-space if things have been typed
        /// Also, we don't want to back space when the user has chosen something
        if((scPosition > 0) && (systemFlags.userChosen == 0))
        {
            /// If so, to back-space, delete the '\b' that was received
            /// and also the character to be deleted, which is a total of
            /// 2 characters
            scPosition --;
            rcPosition -= 2;

            /// Replace the character with a space to create a back-space
            /// effect
            printRomString(msgBackSpaceSpaceBackSpace);
            return 1;
        }

        /// Otherwise, if nothing has been typed, we will treat it like a
        /// back command
        else
        {
            /// Print a space to compensate for the user's backspace
            //printRomString(msgSpace);
            
            /// Pretend we have received the back command
            userInputBuffer[scPosition] = BACK;
            return 0;
        }
    }
    else if((userInputBuffer[scPosition] == ESC) || (userInputBuffer[scPosition] == '['))
    {
        scPosition++;
        return 1;
    }
    return 0;
}

void printOption(char option)
{
        /// If the received byte was a number and the user has chosen something,
        /// clear the prompt so that we replace the user's previous choice
        /// with the current choice later on
        Node* currentChild = currentNode->child;

        if(systemFlags.userChosen)
        {
            /// Note that this reprompts the user at the same line!
            prompt();
        }

        selectChild(option);
        printRomString(numberChars[option]);
        printRomString(msgDot);
        printRomString(nodeNames[(currentNode->child)->label]);
        printRomString(msgSpace);
        printRomString(msgDeleteInFront);
        systemFlags.userChosen = 1;
        rcPosition = 0;
        scPosition = 0;
        currentNode->child = currentChild;
}

void handleReception(void)
{
    /// Check if we have not scanned everything that was received
    if(scPosition < rcPosition)
    {
        if(!scPosition)
        {
            printRomString(msgBackSpace);
            printRomString(msgSpace);
        }
        /// Scan any un-scanned characters
        if(scanAvailableCharacters())
        {
            return;
        }
        
        /// If it is a BACK character ('b'), then
        if(userInputBuffer[scPosition] == BACK)
        {

            if(!((scPosition > 0) &&(userInputBuffer[scPosition - 1] == 'i')))
            {
                /// Then do not echo and
                printRomString(msgBack);
                moveToParentNode();
                freshNodePromptSettings();
                return; ///< This only echos if the previous character is i
            }
        }
        /// Do not ever echo a tab
        else if(userInputBuffer[scPosition] == '\t')
        {
            if(systemFlags.numberInput)
            {
                return;
            }
            if(scPosition)
            {
                /// Try to autocomplete the user input
                /// If it completes, we simply return
                if(!tryAutoComplete(userInputBuffer))
                {

                    /// If not, delete the tab from the buffer
                    userInputBuffer[scPosition] = '\0';
                    rcPosition--;
                }
            }
            return;
        }
        /// Now we can check if it is an up or down character sequence 
        else if((scPosition >= 2) && (userInputBuffer[scPosition - 2] == ESC)
                                  && (userInputBuffer[scPosition - 1] == '['))
        {

            if(systemFlags.userChosen & !systemFlags.numberInput)
            {
                if(userInputBuffer[scPosition] == 'A')
                {
                    currentOption = (currentOption == 0) ? (numberOfChildren - 1) : (currentOption - 1);
                    printOption(currentOption);
                    rcPosition = 0;
                    scPosition = 0;
                    return;
                }
                else if(userInputBuffer[scPosition] == 'B')
                {
                    currentOption = (currentOption == (numberOfChildren - 1)) ? 0 : (currentOption + 1);
                    printOption(currentOption);
                    rcPosition = 0;
                    scPosition = 0;
                    return;
                }
            }
            arrowKeys[2] = userInputBuffer[scPosition];
            printRamString(arrowKeys);
            rcPosition = 0;
            scPosition = 0;
            return;
        }
        /// Now we can check if it is a number
        else if(isCharSelectionDigit(userInputBuffer[scPosition]))
        {
            
            if(systemFlags.numberInput)
            {
                return;
            }
            
            currentOption = userInputBuffer[scPosition] - '0';
            printOption(currentOption);
            return;
        }
        else if(userInputBuffer[scPosition] == '\r')
        {
            scPosition++;
            userInputBuffer[scPosition] = '\0';

            if(checkClear(userInputBuffer)){}
            else if(checkPassword(userInputBuffer)){}
            else if(checkIfNodeNameTyped(userInputBuffer)){}
            else if(checkDeathStarTracker(userInputBuffer)){}
            else if(checkHelp(userInputBuffer)){}
            else if(checkSpecial(userInputBuffer)){}
            else if(checkQuick(userInputBuffer)){}
            else if(checkHome(userInputBuffer)){}
            else
            {
                checkReset(userInputBuffer);
                printRomString(msgInvalidCommand);
            }

            freshNodePromptSettings();
            return;
        }

        /// Otherwise, just echo it
        rcWord[0] = userInputBuffer[scPosition];
//        if(!((rcWord[0] >= 0x21) && (rcWord[0] <= 0x2F)))
//        {
        printRamString(rcWord);
//        }
        
        if(scPosition++ >= MAXPOS)
        {
            printRomString(msgMaxReached);
            freshNodePromptSettings();
        }
        
    }
}

void handleTransmission(void)
{
    /// If we are not during the middle of a transmission,
    /// we can handle the strings that are currently waiting to be transmitted
    if(!PIE1bits.TXIE)
    {
        /// If we have finished transmitting everything, reset the indices
        /// so that we can forget about the past and start a new cue
        if(toPrintIndex >= cueIntoIndex)
        {
            cueIntoIndex = 0;
            toPrintIndex = 0;
            PIE1bits.RCIE = 1;
        }
        if(toPrintIndex < cueIntoIndex)
        {
            if(romramIndicator[toPrintIndex].inRom)
            {
                txPtr.romPtr = toPrintStrings[toPrintIndex++].romPtr;
            }
            else
            {
                txPtr.ramPtr = toPrintStrings[toPrintIndex++].ramPtr;
            }
            PIE1bits.RCIE = 0;
            PIE1bits.TXIE = 1;
        }
    }
}

void printRamString(static char *ramMessage)
{
    if(cueIntoIndex >= CUESIZE)
    {
        handleTransmission();
        return;
    }
    romramIndicator[cueIntoIndex].inRom = FALSE;
    toPrintStrings[cueIntoIndex++].ramPtr = ramMessage;
    handleTransmission();
}

void printRomString(static char rom *romMessage)
{
    if(cueIntoIndex >= CUESIZE)
    {
        handleTransmission();
        return;
    }
    romramIndicator[cueIntoIndex].inRom = TRUE;
    toPrintStrings[cueIntoIndex++].romPtr = romMessage;
    handleTransmission();
}


void prompt(void)
{
    Node *followNodePath = &rootNode;

    printRomString(msgPressHelp);
    printRomString(msgUpUp);
    if(systemFlags.factory)
    {
        printRomString(msgFactory);
    }
    else
    {
        printRomString(msgUser);
    }

    printRomString(nodeNames[followNodePath->label]);
    while(followNodePath != currentNode)//ASSIGNMENT NOT COMPARE
    {
        printRomString(msgSlash);
        followNodePath = followNodePath->child;
        printRomString(nodeNames[followNodePath->label]);
    }
    
    printRomString(msgEndPrompt);
}

char stringCompare(static char *inputString, static rom char rom *romString)
{
    int i = 0;
    while(romString[i])
    {
        if(romString[i] != inputString[i])
        {
            return 0;
        }
        i++;
    }

    if(inputString[i] != '\r')
    {
        return 0;
    }

    return 1;
}
char checkPassword(static char *inputString)
{
    if(stringCompare(inputString, password))
    {
        printRomString(msgNewLine);
        printRomString(msgSeparatorLine);
        printRomString(msgWelcomeFactory);
        printRomString(msgSeparatorLine);
        freshNodePromptSettings();
        systemFlags.factory = 1;
        updateTreeStructure();
        return 1;
    }
    return 0;
}

char checkClear(static char *inputString)
{
    if(stringCompare(inputString, msgClear))
    {
        clearScreen();
        systemFlags.updatePrompt = 1;
        return 1;
    }
    return 0;
}

char checkDeathStarTracker(static char *inputString)
{
    if(stringCompare(inputString, nodeNames[0]))
    {
        printRomString(msgInfo);
        systemFlags.updatePrompt = 1;
        return 1;
    }
    return 0;
}

char checkHelp(static char *inputString)
{
    if(stringCompare(inputString, msgHelp))
    {
        printRomString(msgHelpDescription);
        systemFlags.updatePrompt = 1;
        return 1;
    }
    return 0;
}

char checkSpecial(static char *inputString)
{
    if(stringCompare(inputString, msgSpecial))
    {
        printRomString(msgHelpSpecialCommands);
        systemFlags.updatePrompt = 1;
        return 1;
    }
    return 0;
}

char checkQuick(static char *inputString)
{
    if(stringCompare(inputString, msgQuick))
    {
        printRomString(msgHelpMoreCommands);
        systemFlags.updatePrompt = 1;
        return 1;
    }
    return 0;
}

char checkHome(static char *inputString)
{
    if(stringCompare(inputString, msgHome))
    {
        currentNode = &rootNode;
        systemFlags.updatePrompt = 1;
        return 1;
    }
    return 0;
}

char tryAutoComplete(static char *inputString)
{
    if(numberOfChildren)
    {
        Node *originalOptionNode = currentNode->child;
        int i = 0;
        do
        {
            selectNextChild();

            for(i = 0; inputString[i] == nodeNames[(currentNode->child)->label][i]; i++)
            {
                if(inputString[i] == '\t')
                {
                    break;
                }
            }

            /// IF NULL, it means everything is the same until now
            if(inputString[i] == '\t')
            {
                prompt();
                printRomString(nodeNames[(currentNode->child)->label]);
                moveToChildNode();
                freshNodePromptSettings();
                return 1;
            }          
        }
        while(currentNode->child != originalOptionNode);
    }

    return 0;
}

char checkIfNodeNameTyped(static char *inputString)
{
    if(numberOfChildren)
    {
        Node *originalOptionNode = currentNode->child;
        do
        {
            selectNextChild();
            if(stringCompare(inputString, nodeNames[(currentNode->child)->label]))
            {
                moveToChildNode();
                systemFlags.optionsShown = 0;
                return 1;
            }
        }
        while(currentNode->child != originalOptionNode);
    }

    return 0;
}

void checkReset(static char *inputString)
{
    if(stringCompare(inputString, msgReset))
    {
        systemReset();
    }
}

void showChildOptions(void)
{
        char i = 0;

        Node *originalOptionNode = currentNode->child;
        
	if (systemFlags.optionsShown)
	{
		return;
	}

        numberOfChildren = 0;

        printRomString(msgNewLine);
        printRomString(msgSeparatorLine);
        printRomString(msgTab);
        printRomString(nodeNames[currentNode->label]);
        printRomString(msgSeparatorLine);

	if (currentNode->child == NULL)
	{
		return;
	}


	do
	{
            printRomString(msgTab);
            printRomString(numberChars[i]);
            printRomString(msgDot);
            printRomString(nodeNames[(currentNode->child)->label]);
            if(!i)
            {
                printRomString(msgDefault);
            }
            
            printRomString(msgNewLine);

            selectNextChild();
            i++;
	} while (currentNode->child != originalOptionNode);

        numberOfChildren = i++;
        
	systemFlags.optionsShown = 1;
}

void reprompt(void)
{
    serialSetup();
    prompt();
}