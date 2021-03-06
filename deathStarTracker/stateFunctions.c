


#include "system.h"

void showTargetStatus(void)
{
    //show what mode the main is in
}

void showTemperature(void)
{
    
}

void azimSetAngle(void)
{
<<<<<<< HEAD
    systemFlags.numberInput = 1;
    systemFlags.toTrack = 0;
    
    if(systemFlags.numberParsed)
    {
        if((parsedNumber < -90) || (parsedNumber > 90))
        {
            printRomString(msgOutOfRange);
            moveToParentNode();
        }
        else
        {
            printRomString(msgInputDone);
            updateCCPServoAngle( (unsigned int)(10*(parsedNumber + 90)), currentElevation);
        }
        systemFlags.numberParsed = 0;
        systemFlags.updatePrompt = 1;
    }
    
=======
	//NOTE SOME MODE MUST BE SET THAT IS NOT SCANNING OR TRACKING

	if(systemFlags.userInputStored)
    {
		if ((parsedNumber >= 0) && (parsedNumber <= 180))
		{
			updateCCPServoAngle(parsedNumber*10, currentElevation);
			printRomString(msgValueGood);
            moveToParentNode();
            systemFlags.updatePrompt = 1;
		}
		else 
		{
			systemFlags.userInputStored = 0;
            printRomString(msgValueBad);
            systemFlags.updatePrompt = 1;
		}
	}
	
>>>>>>> 7f45885b5747f0ae6ead4414b7c6e5d00d5240b2
}

void azimManual(void)
{
	//NOTE SOME MODE MUST BE SET THAT IS NOT SCANNING OR TRACKING
	//MUST ALSO BE SET TO TRIGGER FROM BUTTON PRESSES

	if (1) {   //if some positive button
		updateCCPServoAngle(currentAzimuth+50, currentElevation);
		printRomString(msgValueGood);
		moveToParentNode();
		systemFlags.updatePrompt = 1;
	}
	else 
	{
		updateCCPServoAngle(currentAzimuth-50, currentElevation);
		printRomString(msgValueGood);
		moveToParentNode();
		systemFlags.updatePrompt = 1;
	}
}

void elevSetAngle(void)
{
<<<<<<< HEAD
    systemFlags.numberInput = 1;
    systemFlags.toTrack = 0;

    if(systemFlags.numberParsed)
    {
        if((parsedNumber < -90) || (parsedNumber > 90))
        {
            printRomString(msgOutOfRange);
            moveToParentNode();
        }
        else
        {
            printRomString(msgInputDone);
            updateCCPServoAngle( currentAzimuth, (unsigned int)(10*(parsedNumber + 90)));
        }
        systemFlags.numberParsed = 0;
        systemFlags.updatePrompt = 1;
    }
=======
	//NOTE SOME MODE MUST BE SET THAT IS NOT SCANNING OR TRACKING

	if(systemFlags.userInputStored)
    {
		if ((parsedNumber >= 0) && (parsedNumber <= 180))
		{
			updateCCPServoAngle(currentAzimuth, parsedNumber*10);
			printRomString(msgValueGood);
            moveToParentNode();
            systemFlags.updatePrompt = 1;
		}
		else 
		{
			systemFlags.userInputStored = 0;
            printRomString(msgValueBad);
            systemFlags.updatePrompt = 1;
		}
	}
>>>>>>> 7f45885b5747f0ae6ead4414b7c6e5d00d5240b2
}

void elevManual(void)
{
	//NOTE SOME MODE MUST BE SET THAT IS NOT SCANNING OR TRACKING
	//MUST ALSO BE SET TO TRIGGER FROM BUTTON PRESSES

	if (1) {   //if some positive button
		updateCCPServoAngle(currentAzimuth, currentElevation+50);
		printRomString(msgValueGood);
		moveToParentNode();
		systemFlags.updatePrompt = 1;
	}
	else 
	{
		updateCCPServoAngle(currentAzimuth, currentElevation-50);
		printRomString(msgValueGood);
		moveToParentNode();
		systemFlags.updatePrompt = 1;
	}
	
}

void distMax(void)
{
	if(systemFlags.userInputStored)
    {
		if ((parsedNumber >= 45) && (parsedNumber <= 210))
		{
			setIRMax(parsedNumber);
			setUSMax(parsedNumber);
			printRomString(msgValueGood);
            moveToParentNode();
            systemFlags.updatePrompt = 1;
		}
		else 
		{
			systemFlags.userInputStored = 0;
            printRomString(msgValueBad);
            systemFlags.updatePrompt = 1;
		}
	}
}

void distMin(void)
{
	if(systemFlags.userInputStored)
    {
		if ((parsedNumber >= 45) && (parsedNumber <= 210))
		{
			setIRMin(parsedNumber);
			setUSMin(parsedNumber);
			printRomString(msgValueGood);
            moveToParentNode();
            systemFlags.updatePrompt = 1;
		}
		else 
		{
			systemFlags.userInputStored = 0;
            printRomString(msgValueBad);
            systemFlags.updatePrompt = 1;
		}
	}
}

void azimMax(void)
{
	if(systemFlags.userInputStored)
    {
        if((parsedNumber > 0) && (parsedNumber <= 180))
        {
			azimuthMax = parsedNumber*10;   //converts degrees to tens of degrees
			printRomString(msgValueGood);
            moveToParentNode();
            systemFlags.updatePrompt = 1;
		}
		else
		{
			systemFlags.userInputStored = 0;
            printRomString(msgValueBad);
            systemFlags.updatePrompt = 1;
		}
	}
}

void azimMin(void)
{
	if(systemFlags.userInputStored)
    {
        if((parsedNumber > 0) && (parsedNumber <= 180))
        {
			azimuthMin = parsedNumber*10;   //converts degrees to tens of degrees
			printRomString(msgValueGood);
            moveToParentNode();
            systemFlags.updatePrompt = 1;
		}
		else
		{
			systemFlags.userInputStored = 0;
            printRomString(msgValueBad);
            systemFlags.updatePrompt = 1;
		}
	}
}

void elevMax(void)
{
	if(systemFlags.userInputStored)
    {
        if((parsedNumber > 0) && (parsedNumber <= 90))
        {
			elevationMax = parsedNumber*10;   //converts degrees to tens of degrees
			printRomString(msgValueGood);
            moveToParentNode();
            systemFlags.updatePrompt = 1;
		}
		else
		{
			systemFlags.userInputStored = 0;
            printRomString(msgValueBad);
            systemFlags.updatePrompt = 1;
		}
	}
}

void elevMin(void)
{
	if(systemFlags.userInputStored)
    {
        if((parsedNumber > 0) && (parsedNumber <= 90))
        {
			elevationMin = parsedNumber*10;   //converts degrees to tens of degrees
			printRomString(msgValueGood);
            moveToParentNode();
            systemFlags.updatePrompt = 1;
		}
		else
		{
			systemFlags.userInputStored = 0;
            printRomString(msgValueBad);
            systemFlags.updatePrompt = 1;
		}
	}
}


void caliTemp(void)
{

}

void caliAzim(void)
{

}

void caliElev(void)
{

}

void usSamplePerEst(void)
{
	unsigned char i = 0;

	if(systemFlags.userInputStored)
    {
        if((parsedNumber > 0) && (parsedNumber <= 10))
        {
			while (i < 10) 
			{
				UScapturedValue[i] = 0;
				i++;
			}
			USValues.sampleSize = parsedNumber;
			printRomString(msgValueGood);
            moveToParentNode();
            systemFlags.updatePrompt = 1;
		}
		else
		{
			systemFlags.userInputStored = 0;
            printRomString(msgValueBad);
            systemFlags.updatePrompt = 1;
		}
	}
}

void usSampleRate(void)
{
    if(systemFlags.userInputStored)
    {
        /// User inputs 2 to 13 Hz
<<<<<<< HEAD
        if((parsedNumber >= 2) && (parsedNumber <= 13))
=======
        if((parsedNumber >= 2) && (parsedNumber <= 12))
>>>>>>> 7f45885b5747f0ae6ead4414b7c6e5d00d5240b2
        {
            USValues.freq_ms = 1000/parsedNumber;

            printRomString(msgValueGood);
            moveToParentNode();
            systemFlags.updatePrompt = 1;
        }
        else
        {
            systemFlags.userInputStored = 0;
            printRomString(msgValueBad);
            systemFlags.updatePrompt = 1;
        }
    }
}

void irSamplePerEst(void)
{
	if(systemFlags.userInputStored)
    {
        if((parsedNumber > 0) && (parsedNumber <= 10))
        {
			setIRSamplesPerEstimate(parsedNumber);
			printRomString(msgValueGood);
            moveToParentNode();
            systemFlags.updatePrompt = 1;
		}
		else
		{
			systemFlags.userInputStored = 0;
            printRomString(msgValueBad);
            systemFlags.updatePrompt = 1;
		}
	}
}

void irSampleRate(void)
{
	if(systemFlags.userInputStored)
    {
        if((parsedNumber >= 200)
        {
			setIRSampleRate(parsedNumber);
            printRomString(msgValueGood);
            moveToParentNode();
            systemFlags.updatePrompt = 1;
        }
        else
        {
            systemFlags.userInputStored = 0;
            printRomString(msgValueBad);
            systemFlags.updatePrompt = 1;
        }
    }
}


void showRaw(void)
{
<<<<<<< HEAD
//    static timeTag showRawTag = {0, 0, 0, 0};
//    static char i = 0;

//    if(eventDue(&showRawTag))
//    {
//        if(i < 10)
//        {
//            intToDisplay(USCapturedRaw[i++], 0);
//            printRamString(displayChars.characters);
//
//            printRomString(msgSpaces);
//        }
//        else
//        {
//            printRomString(msgCR);
//        }

//        setTimeTag(100, &showRawTag);
//    }

=======
    
    intToDisplay(USCapturedRaw[0], 0);
    printRamString(displayChars.characters);

	printRomString(msgSpaces);

	intToDisplay(getIRRaw(), 0);
    printRamString(displayChars.characters);

    printRomString(msgCR);
    handleTransmission;
>>>>>>> 7f45885b5747f0ae6ead4414b7c6e5d00d5240b2
}

void showStat(void)
{
	unsigned char i = 0;
    unsigned int maxVariance = 0;
    unsigned int variance = 0;
    while (i < USValues.currentSampleSize){
        variance = ((USCapturedRaw[i])<<1)>>1;
        if ( variance > maxVariance){
            maxVariance = variance;
        }
    }
	intToDisplay(maxVariance, 0);
    printRamString(displayChars.characters);

	printRomString(msgSpaces);

	intToDisplay(getIRVariance(), 0);
    printRamString(displayChars.characters);
}
