/**********************************************************
**  File: interface.c
***********************************************************
**  Author: Kelvin Hsu
**  Created on 2 September 2014, 11:42 PM
**  Assembled with C18 C Compiler v. 4.36
***********************************************************
**
**********************************************************/

#include "interface.h"

/**
 * These strings are stored in an array in program
 * memory, call stringToRam to move these functions 
 * from program memory to data memory.
 */
rom const char rom *LCDStrings[] = {
/*00*/ "DeathStarTracker",  /* SYSTEM_NAME */
/*01*/ "Press <GO>",        /* PRESS_GO */
/*02*/ "Main Menu",         /* MAIN_MENU */
/*03*/ "Target Status",     /* TARGET_STATUS */
/*04*/ "Temperature",       /* TEMPERATURE */
/*05*/ "Goto Position",     /* GOTO_POSITION */
/*06*/ "Set Limits",        /* SET_LIMITS */
/*07*/ "Remote Mode",       /* REMOTE_MODE */
/*08*/ "Target Not Found",  /* NOT_FOUND */
/*09*/ "  Searching...",    /* SEARCHING */
/*10*/ "Target Acquired",   /* ACQURIED */
/*11*/ "Dist",              /* DIST */
/*12*/ "Azim",              /* AZIM */
/*13*/ "Elev",              /* ELEV */
/*14*/ "Azimuth",           /* AZIMUTH */
/*15*/ "Elevation",         /* ELEVATION */
/*16*/ "Manually",          /* MANUALLY */
/*17*/ "Set ",              /* SET */
/*18*/ "Goto ",             /* GOTO */
/*19*/ "Manual Move",       /* MANUAL_MOVE*/
/*20*/ "Use Arrows",        /* USE_ARROWS */
/*21*/ "Angle: ",           /* ANGLES */
/*22*/ "Entry Successful",  /* SUCCESS */
/*23*/ "Out of Range",      /* OUT_OF_RANGE */
/*24*/ "Re-enter Input",    /* RE_ENTER */
/*25*/ "Distance Min",      /* DIST_MIN */
/*26*/ "Distance Max",      /* DIST_MAX */
/*27*/ "Azimuth Min",       /* AZIM_MIN */
/*28*/ "Azimuth Max",       /* AZIM_MAX */
/*29*/ "Elevation Min",     /* ELEV_MIN */
/*30*/ "Elevation Max",     /* ELEV_MAX */
/*31*/ "Value:"             /* VALUE */
};

/**
 * @brief Follows a child pointer
 * @details Uses the current pointer and goes to the
 * 			child specified by childIndex
 * 
 * @param char [description]
 */

void moveToChild( unsigned char childIndex ){

}



