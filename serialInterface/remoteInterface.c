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
        inChar = userInputBuffer[scPosition];

        if(scPosition >= 2)
        {
            if((userInputBuffer[scPosition - 2] == ESC) && (userInputBuffer[scPosition - 1] == 0x5B))
            {
                if(!systemFlags.userChosen)
                {
                    rcPosition = 0;
                    scPosition = 0;
                    scPtr = userInputBuffer;
                    return;
                }
                else if(inChar == 0x41)
                {
                    systemFlags.upPressed = 1;
                    systemFlags.downPressed = 0;
                }
                else if(inChar == 0x42)
                {
                    systemFlags.downPressed = 1;
                    systemFlags.upPressed = 0;
                }
            }
        }

        if((systemFlags.userChosen == 1) && (((inChar - '0') < numberOfChildren) && ((inChar - '0') >= 0)))
        {
            printRomString(msgBackSpace18);
        }
        
        rcWord[0] = inChar;
        /// Only echo if in remote or we are inputting numbers
        if(inChar == ESC || inChar == 0x5B)
        {
            scPosition++;
            return;
        }
        else if(!systemFlags.upPressed & !systemFlags.downPressed)
        {
            if(systemFlags.remote)
            {
                printRamString(rcWord);
            }
            else if(systemFlags.numberInput)
            {
                LCDMoveCursor(1, scPosition + 1);
                printRamString(rcWord);
            }
        }

        if(inChar == '\b')
        {
            if(scPosition > 0) // if not at the first one we can backspace
            {
                scPosition -= 2; // one for \b one for the thing to delete
                rcPosition -= 2;
                scPtr -= 2;
            }
            else
            {
                userInputBuffer[scPosition] = BACK;
                inChar = BACK;
                rcWord[0] = inChar;
                printRomString(msgSpace);
                printRamString(rcWord);
            }
        }


        if(!systemFlags.numberInput)
        {
            if(systemFlags.upPressed)
            {
                rcPosition = 0;
                scPosition = 0;
                scPtr = userInputBuffer;
                userInputBuffer[rcPosition++] =
                        (userInputBuffer[0] == '0') ?
                            (numberOfChildren - 1 + '0') : (userInputBuffer[0] - 1);
                systemFlags.upPressed = 0;
                //printRomString(msgBackSpace18);
                return;
            }
            else if(systemFlags.downPressed)
            {
                rcPosition = 0;
                scPosition = 0;
                scPtr = userInputBuffer;
                userInputBuffer[rcPosition++] =
                        (userInputBuffer[0] == (numberOfChildren - 1 + '0')) ?
                            '0' : (userInputBuffer[0] + 1);
                systemFlags.downPressed = 0;
                //printRomString(msgBackSpace18);
                return;
            }
            else if(((inChar - '0') < numberOfChildren) && ((inChar - '0') >= 0))
            {

                Node* currentChild = currentNode->child;

                selectChild(inChar - '0');

                printRomString(msgDot);
                printRomString(nodeNames[(currentNode->child)->label]);
                currentNode->child = currentChild;
                systemFlags.userChosen = 1;
                
            }
            else if(inChar == BACK)
            {
                scPtr++;
                rcPosition++;
                scPosition++;
                userInputBuffer[scPosition] = '\r';
                printRomString(msgAck);
                systemFlags.commandReceived = 1;
            }
        }
        
        /* If the character was not a carriage return, then */
        /* Note the change of position by incrementing and check whether or *
         * not we have reached the end of our buffer */
        if(scPosition++ >= MAXPOS)
        {
            printRomString(msgMaxReached);
            systemFlags.commandReceived = 1;
        }

        if(inChar == '\r')
        {
            systemFlags.commandReceived = 1;
        }
        if(systemFlags.commandReceived)
        {
            /* If the character is a carriage return, or if the maximum position of *
             * the buffer was reached, then do the following:                       */

            /* Attach LF, and NULL to the end and note the change of position */
            userInputBuffer[scPosition] = '\0';

            if(!checkClear(userInputBuffer))
            {
                if(!checkPassword(userInputBuffer))
                {
                    parseUserInput(userInputBuffer);
                }
            }

            scPosition = 0;
            rcPosition = 0;
            scPtr = userInputBuffer;
            systemFlags.numberInput = 0;
            systemFlags.userChosen = 0;
        }
        
    }
}

void printRamString(static char *ramMessage)
{
    if(systemFlags.remote)
    {
        /* Transmit the message while the character is not a NULL character */
        while(*ramMessage)
        {
            /// Transmit the message once TXIF is empty
            while(!PIR1bits.TXIF) {}
            TXREG = *ramMessage++;
        }
    }
    else
    {
//        LCDMoveCursor(1, 1);
        LCDWriteHere(ramMessage);
    }
}

void printRomString(static char rom *romMessage)
{
    static char romToRamBuffer[STRINGSIZE];
    stringToRam(romMessage, romToRamBuffer);
    printRamString(romToRamBuffer);
}


void prompt(void)
{
    Node *followNodePath = &rootNode;
    
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

//char isCharDigit(static char letter)
//{
//    if((letter < '0') || (letter > '9'))
//    {
//        return 0;
//    }
//
//    return 1;
//}
//
//char isStringAllDigit(static char *inputString)
//{
//    int i = 0;
//
//    while(inputString[i] != '\r')
//    {
//        if((inputString[i] < '0') || (inputString[i] > '9'))
//        {
//            return 0;
//        }
//        i++;
//    }
//
//    return 1;
//}

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

void parseUserInput(static char *inputString)
{

    char charToCheck = inputString[scPosition - 2];
    if(systemFlags.numberInput)
    {
        unsigned int inputValue = convertToNumber(inputString);

        //imposeValueConstraints();
        // it will grab the maximum and minimum values and turn userError on if
        // it is out of it, the min/max values are set by entering each node
        // each node has a different value
        //
        
        if(systemFlags.userError)
        {
            printRomString(msgNumberError);
            return;
        }

        //writeValue();
        // it will write inputValue to the correct variable. it will be pointed
        // by a pointer. the pointer is pointed to the number when when we enter
        // the node

        //how about systemFlags.oneTime? set at the end of each
        //node function if to be executed once and in the beginning if set don't continue

        
    }
    
//    if(inputString[1] != '\r')
//    {
//
//        printRomString(msgDigitError);
//
//        return;
//    }
   
    
    if(charToCheck == BACK)
    {
        moveToParentNode();
    }
    else
    {
        char digit = charToCheck - '0';

        if(digit >= numberOfChildren || digit < 0)
        {
            printRomString(msgDigitError);
        }
        else
        {

            moveToChildSelection(digit);
        }
    }
}

char checkPassword(static char *inputString)
{
    int i = 0;
    while(password[i])
    {
        if(password[i] != inputString[i])
        {
            return 0;
        }
        i++;
    }

    if(inputString[i] != '\r')
    {
        return 0;
    }

    systemFlags.factory = 1;
    updateTreeStructure();
    printRomString(msgSeparatorLine);
    printRomString(msgWelcomeFactory);
    printRomString(msgSeparatorLine);
    systemFlags.optionsShown = 0;
    return 1;
}

char checkClear(static char *inputString)
{
    int i = 0;
    while(clear[i])
    {
        if(clear[i] != inputString[i])
        {
            return 0;
        }
        i++;
    }

    if(inputString[i] != '\r')
    {
        return 0;
    }

    clearScreen();
    systemFlags.optionsShown = 0;
    return 1;
}

void showChildOptions(void)
{
        char i = '0';

        char iString[] = {i, '\0'};

        Node *originalOptionNode = currentNode->child;

	if (systemFlags.optionsShown)
	{
		return;
	}

        numberOfChildren = 0;

        printRomString(msgNewLine);
        printRomString(msgSeparatorLine);
        printRomString(nodeNames[currentNode->label]);
        printRomString(msgSeparatorLine);

	if (currentNode->child == NULL)
	{
		return;
	}

	if (((currentNode->child)->label == OUT_OF_RANGE_NODE) ||
                ((currentNode->child)->label == ENTRY_SUCCESS_NODE) ||
                ((currentNode->child)->label == HOME_NODE))
	{
		return;
        }

	do
	{
            iString[0] = i++;
            printRamString(iString);
            printRomString(msgDot);
            printRomString(nodeNames[(currentNode->child)->label]);
            printRomString(msgNewLine);

            selectNextChild();

	} while (currentNode->child != originalOptionNode);

        numberOfChildren = i++ - '0';
        
	systemFlags.optionsShown = 1;
}