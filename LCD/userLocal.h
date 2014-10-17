
/* 
 * File:   userLocal.h
 * Author: saivanhamama
 *
 * Created on October 1, 2014, 12:48 PM
 */

#ifndef USERLOCAL_H
#define	USERLOCAL_H

/**
 *  @brief A structure for creating menu nodes
 *  @struct menuNode
 *
 *  This struct is general enough to use for any menu that our system needs to handle.
 *  It contains pointers for both parents and for children, as well as <b>REFERENCES</b> to strings
 *  stored in data memory. We actually pull out the string with our @ref "stringToRam" function.
 *  This struct also has a pointer to a display value for menus with dynamically changing text.
 */
typedef struct menuNode {
    /// Pointing to the parent menu, null for root
    struct menuNode *parent;
    /// Does this menu have child menus, if so, define them
    unsigned char hasChildren : 1;
    unsigned char childCount: 3;					///< This shoul be different in local and remote mode
    union {
    	struct menuNode *child;                                         ///< If there is only one child, we point directly to it
    	struct menuNode *(*children[5]);                                ///< If there is more than one child, we point to an array of pointers (to the children)
    } childPointer;
    /// Handling the strings printed to the user
    char topLineReference;
    unsigned char isMenu: 1;
    unsigned char currentSelection : 3;
    unsigned char menuItemCount : 3;
    union {
    	char (*bottomLineReferenceArray)[8];                            ///< IF we have a menu, we use a pointer to an array of string references
    	char bottomLineReference;					///< Else we just store the string reference
    } bottomLineReference;
    /// Handles the display of a number to the user
    unsigned char hasDisplayValue: 1;
    unsigned char hasSuccessFail : 1;                                   ///< Does the menu need to parse a user input?
    unsigned char *displayValue; 					///< Points to the value we want to show
} menuNode;







/**
 * @brief A node in the menu tree has this type
 * @details Includes 
 * 
 */
typedef struct localMenuItem {    
    /// Pointing to the parent menu, null for root
    struct localMenuItem *parent;
    /// Does this menu have child menus, if so, define them
    unsigned char hasChildren : 1;
    unsigned char childCount: 3;
    struct localMenuItem *children[5];
    // Handling the strings printed to the user
    char *topLine;
    unsigned char isMenu: 1;
    unsigned char currentSelection : 3;
    char *bottomLineArray[7];   // Change to a union
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



#pragma udata StringData
extern char LCDSWelcomeMessage[];
extern char LCDSPressGo[];
extern char LCDSMainMenu[];
extern char LCDSStatus[];
extern char LCDSTemperature[];
extern char LCDSGoto[];
extern char LCDSLimits[];
extern char LCDSRemote[];
extern char LCDSNotFound[];
extern char LCDSSearching[];
extern char LCDSAcquired[];
extern char LCDSDist[];
extern char LCDSAzim[];
extern char LCDSElev[];
extern char LCDSAzimuth[];
extern char LCDSElevation[];
extern char LCDSManually[];
extern char LCDSSet[];
extern char LCDSGotoPos[];
extern char LCDSMove[];
extern char LCDSArrows[];
extern char LCDSAngle[];
extern char LCDSSuccess[];
extern char LCDSOutOfRange[];
extern char LCDSReenter[];
extern char LCDSMinDist[];
extern char LCDSMaxDist[];
extern char LCDSMinAzim[];
extern char LCDSMaxAzim[];
extern char LCDSMinElev[];
extern char LCDSMaxElev[];
extern char LCDSValue[];

#pragma udata
extern rom const char rom *LCDStrings[];



#endif	/* USERLOCAL_H */

