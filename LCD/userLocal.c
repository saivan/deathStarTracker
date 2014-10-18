
#include "masterHeader.h"


char LCDSWelcomeMessage[] = "DeathStarTracker";
char LCDSPressGo[] = "Press <GO>";
char LCDSMainMenu[] = "Main Menu";
char LCDSStatus[] = "Target Status";
char LCDSTemperature[] = "Temperature";
char LCDSGoto[] = "Goto Position";
char LCDSLimits[] = "Set Limits";
char LCDSRemote[] = "Remote Mode";
char LCDSNotFound[] = "Target Not Found";
char LCDSSearching[] = "  Searching...";
char LCDSAcquired[] = "Target Acquired";
char LCDSDist[] = "Dist";
char LCDSAzim[] = "Azim";
char LCDSElev[] = "Elev";
char LCDSAzimuth[] = "Azimuth";
char LCDSElevation[] = "Elevation";
char LCDSManually[] = "Manually";
char LCDSSet[] = "Set ";
char LCDSGotoPos[] = "Goto ";
char LCDSMove[] = "Manual Move";
char LCDSArrows[] = "Use Arrows";
char LCDSAngle[] = "Angle: ";
char LCDSSuccess[] = "Entry Successful";
char LCDSOutOfRange[] = "Out of Range";
char LCDSReenter[] = "Re-enter Input";
char LCDSMinDist[] = "Distance Min";
char LCDSMaxDist[] = "Distance Max";
char LCDSMinAzim[] = "Azimuth Min";
char LCDSMaxAzim[] = "Azimuth Max";
char LCDSMinElev[] = "Elevation Min";
char LCDSMaxElev[] = "Elevation Max";
char LCDSValue[] = "Value:";


// These are stored in program memory,
// use the stringToRam function to
// move the string into data memory when
// it is needed
rom const char rom *LCDStrings[] = {
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



///////////////////////////////////////////////////////////////
// THESE NEED TO BE REDECLARED IN THE NEW FORM TO REDUCE THE
// MEMORY USAGE... CURRENTLY WORKING, BUT OCCUPYING TOO MUCH
// MEMORY FOR THE TIME BEING
///////////////////////////////////////////////////////////////

/// Declaring all of the classes (without linking to parents)
/// Declared bottom-up to allow linking to all childeren immediately
/// The declarations are made in the following form:
/// { isMenu, hasChildren, hasDisplayValue, childCount, selection, *parent, *childArray[5], *topLine, *bottomLineArray[5] }
localMenuItem targetStatus = { 0, FALSE, 0, {0, 0}, LCDSNotFound, FALSE, 0, {LCDSSearching, 0}, FALSE, 0 };
localMenuItem temperature = { 0, FALSE, 0, {0, 0}, LCDSTemperature, FALSE, 0, {LCDSValue, 0}, TRUE, 0 };
localMenuItem manualMovement = { 0, FALSE, 0, {0, 0}, LCDSMove, FALSE, 0, {LCDSArrows, 0}, FALSE, 0 };
localMenuItem unsuccessfulGotoEntry = { 0, FALSE, 0, {0, 0}, LCDSOutOfRange, FALSE, 0, {LCDSReenter,  0}, FALSE, 0 };
localMenuItem successfulGotoEntry = { 0, FALSE, 0, {0, 0}, LCDSSuccess, FALSE, 0, {LCDSPressGo,  0}, FALSE, 0 };
localMenuItem gotoParam = { 0, TRUE, 2, { &unsuccessfulGotoEntry,  &successfulGotoEntry }, LCDSGotoPos, FALSE, 0, {LCDSAngle,  0}, TRUE, 0 };
localMenuItem setParam = { 0, TRUE, 2, { &manualMovement,  &gotoParam,  0 }, LCDSSet, TRUE, 0, {LCDSManually,  LCDSGoto,  0}, FALSE, 0 };
localMenuItem gotoPosition = { 0, TRUE, 1, { &setParam,  0 } , LCDSGoto, TRUE, 0, {LCDSAzimuth,  LCDSElevation,  0}, FALSE, 0 };
localMenuItem unsuccessfulLimitEntry = { 0, FALSE, 0, {0, 0}, LCDSOutOfRange, FALSE, 0, {LCDSReenter,  0}, FALSE, 0 };
localMenuItem successfulLimitEntry = { 0, FALSE, 0, {0, 0}, LCDSSuccess, FALSE, 0, {LCDSPressGo,  0}, FALSE, 0 };
localMenuItem limitEntry = { 0, TRUE, 2, { &unsuccessfulLimitEntry,  &successfulLimitEntry , 0 }, LCDSMinDist, FALSE, 0, {LCDSValue, 0}, TRUE, 0 };
localMenuItem setLimits = { 0, TRUE, 1, { &limitEntry,  0 }, LCDSLimits, TRUE, 0, {LCDSMaxDist,  LCDSMinDist,  LCDSMaxAzim,  LCDSMinAzim,  LCDSMinElev,  LCDSMaxElev}, FALSE, 0 };
localMenuItem remoteMode = { 0, FALSE, 0, {0, 0}, LCDSRemote, FALSE, 0, {LCDSPressGo,  0}, FALSE, 0 };
localMenuItem mainMenu = { 0, TRUE, 5, { &targetStatus,  &temperature,  &gotoPosition,  &setLimits,  &remoteMode }, LCDSMainMenu, TRUE, 0, {LCDSStatus,  LCDSTemperature,  LCDSGoto,  LCDSLimits,  LCDSRemote}, FALSE, 0 };
localMenuItem welcomeMenu = { 0, TRUE, 1, { &mainMenu }, LCDSWelcomeMessage, FALSE, 0, {LCDSPressGo,  0}, FALSE, 0 };


// Declare all of the child menu arrays

// Link the child menu Arrays


// Linking all of the parents




