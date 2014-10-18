
#include "masterHeader.h"

/**
 * These strings are stored in an array in program
 * memory, call stringToRam to move these functions 
 * from program memory to data memory.
 */
rom const char rom *LCDString[] = {
	/*00*/ "DeathStarTracker",
	/*01*/ "Press <GO>",
	/*02*/ "Main Menu",
	/*03*/ "Target Status",
	/*04*/ "Temperature",
	/*05*/ "Goto Position",
	/*06*/ "Set Limits",
	/*07*/ "Remote Mode",
	/*08*/ "Target Not Found",
	/*09*/ "  Searching...",
	/*10*/ "Target Acquired",
	/*11*/ "Dist",
	/*12*/ "Azim",
	/*13*/ "Elev",
	/*14*/ "Azimuth",
	/*15*/ "Elevation",
	/*16*/ "Manually",
	/*17*/ "Set ",
	/*18*/ "Goto ",
	/*19*/ "Manual Move",
	/*20*/ "Use Arrows",
	/*21*/ "Angle: ",
	/*22*/ "Entry Successful",
	/*23*/ "Out of Range",
	/*24*/ "Re-enter Input",
	/*25*/ "Distance Min",
	/*26*/ "Distance Max",
	/*27*/ "Azimuth Min",
	/*28*/ "Azimuth Max",
	/*29*/ "Elevation Min",
	/*30*/ "Elevation Max",
	/*31*/ "Value:"
};

/// Declaring all of the classes (without linking to parents)
/// Declared bottom-up to allow linking to all childeren immediately
/// The declarations are made in the following form:
/// { isMenu, hasChildren, hasDisplayValue, childCount, selection, *parent, *childArray[5], *topLine, *bottomLineArray[5] }


// Declare all of the child menu arrays

// Link the child menu Arrays


// Linking all of the parents




