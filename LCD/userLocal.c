
#include "masterHeader.h"

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




