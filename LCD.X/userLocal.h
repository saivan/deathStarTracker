/* 
 * File:   userLocal.h
 * Author: saivanhamama
 *
 * Created on October 1, 2014, 12:48 PM
 */

#ifndef USERLOCAL_H
#define	USERLOCAL_H

typedef struct localMenuItem {    
    // Pointing to the parent menu, null for root
    struct localMenuItem *parent;
    // Does this menu have child menus, if so, define them
    unsigned char hasChildren : 1;
    unsigned char childCount: 3;
    struct localMenuItem *children[5];
    // Handling the strings printed to the user
    char *topLine;
    unsigned char isMenu: 1;
    unsigned char currentSelection : 3;
    char *bottomLineArray[7];
    // Handles the display of a number to the user
    unsigned char hasDisplayValue: 1;
    unsigned char *displayValue; // Points to the value we want to show
    // Possibly a function pointer to handle button presses ??
    // - may not be needed if structs are handled in a wrapper
} localMenuItem;

void userLocalEventHandler( void );

// Declaring all of the menus here
#pragma udata StringData
extern localMenuItem targetStatus;
extern localMenuItem temperature;
extern localMenuItem manualMovement;
extern localMenuItem unsuccessfulGotoEntry;
extern localMenuItem successfulGotoEntry;
extern localMenuItem gotoParam;
extern localMenuItem setParam;
extern localMenuItem gotoPosition;
extern localMenuItem unsuccessfulLimitEntry;
extern localMenuItem successfulLimitEntry;
extern localMenuItem limitEntry;
extern localMenuItem setLimits;
extern localMenuItem remoteMode;
extern localMenuItem mainMenu;
extern localMenuItem welcomeMenu;
#pragma udata

// Declare the head and current pointer
extern localMenuItem *rootMenu;
extern localMenuItem *currentMenu;

#endif	/* USERLOCAL_H */

