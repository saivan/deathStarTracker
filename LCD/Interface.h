/*
 * File:   interface.h
 * Author: kkeke_000
 *
 * Created on 16 October 2014, 9:14 PM
 */

#ifndef INTERFACE_H
#define	INTERFACE_H


enum{ 
    SYSTEM_NAME,
    PRESS_GO,
    MAIN_MENU,
    TARGET_STATUS,
    TEMPERATURE,
    GOTO_POSITION,
    SET_LIMITS,
    REMOTE_MODE,
    NOT_FOUND,
    SEARCHING,
    ACQUIRED,
    DIST,
    AZIM,
    ELEV,
    AZIMUTH,
    ELEVATION,
    MANUALLY,
    SET,
    GOTO,
    MANUAL_MOVE,
    USE_ARROWS,
    ANGLE,
    SUCCESS,
    OUT_OF_RANGE,
    RE_ENTER,
    DIST_MIN,
    DIST_MAX,
    AZIM_MIN,
    AZIM_MAX,
    ELEV_MIN,
    ELEV_MAX,
    VALUE,
    TOTAL_STRINGS
 };

#pragma idata
extern rom const char rom *interfaceString[];                 ///< Declaring all of the strings in program memory


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
    struct menuNode *parent;                        ///< Points at the parent of the current menu node
    /// Does this menu have child menus, if so, define them    
    unsigned char childCount;                       ///< Counts the number of children (zero if none)
    union { 
    	struct menuNode *child;                     ///< If there is only one child, we point directly to it
    	struct menuNode *(*children[5]);            ///< If there is more than one child, we point to an array of pointers (to the children)
    } childPointer; 
    /// Handling the strings printed to the user    
    unsigned char titleReference;                   ///< Holds the reference number for the LCDString    
    unsigned char menuItemCount;                    ///< Stores the number of menu items (zero if none)
    union { 
    	char (*bottomLineReferenceArray)[8];        ///< If we have a menu, we use a pointer to an array of string references
    	char bottomLineReference;                   ///< Else we just store the string reference
    } bottomLineReference;  
    /// Handles the display of a number to the user 
    unsigned char hasDisplayValue: 1;               ///< Does the menu need to display a value to the user
    unsigned char hasSuccessFail : 1;               ///< Does the menu need to parse a user input?
    unsigned char *displayValue;                    ///< Points to the value we want to show
} MenuNode;

/// Declaring all of the local interface pointers
extern MenuNode *rootMenu;                              ///< Points to the root menu node (Home)
extern MenuNode *currentMenu;                           ///< Points at the current menu to follow children
extern unsigned char currentMenuSelection;              ///< Used in menus to select and display a particular item

/// Functions that deal with tree navigation
void moveToChild( unsigned char childIndex );

#endif	/* INTERFACE_H */

