


#include "masterHeader.h"

void showTargetStatus(void)
{
    
}

void showTemperature(void)
{
    
}

void azimSetAngle(void)
{
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
    
}

void azimManual(void)
{

}

void elevSetAngle(void)
{
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
}

void elevManual(void)
{

}

void distMax(void)
{

}

void distMin(void)
{

}

void azimMax(void)
{

}

void azimMin(void)
{

}

void elevMax(void)
{

}

void elevMin(void)
{

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
}

void usSampleRate(void)
{
    if(systemFlags.userInputStored)
    {
        /// User inputs 2 to 13 Hz
        if((parsedNumber >= 2) && (parsedNumber <= 13))
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

}

void irSampleRate(void)
{

}


void showRaw(void)
{
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

}

void showStat(void)
{

}
