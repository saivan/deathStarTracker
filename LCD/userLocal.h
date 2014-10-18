
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
    struct menuNode *parent;                            ///< Points at the parent of the current menu node
    /// Does this menu have child menus, if so, define them    
    unsigned char childCount;					        ///< Counts the number of children (zero if none)
    union { 
    	struct menuNode *child;                         ///< If there is only one child, we point directly to it
    	struct menuNode *(*children[5]);                ///< If there is more than one child, we point to an array of pointers (to the children)
    } childPointer; 
    /// Handling the strings printed to the user    
    unsigned char titleReference;                       ///< Holds the reference number for the LCDString    
    unsigned char menuItemCount;                        ///< Stores the number of menu items (zero if none)
    union { 
    	char (*bottomLineReferenceArray)[8];            ///< If we have a menu, we use a pointer to an array of string references
    	char bottomLineReference;					    ///< Else we just store the string reference
    } bottomLineReference;  
    /// Handles the display of a number to the user 
    unsigned char hasDisplayValue: 1;                   ///< Does the menu need to display a value to the user
    unsigned char hasSuccessFail : 1;                   ///< Does the menu need to parse a user input?
    unsigned char *displayValue; 					    ///< Points to the value we want to show
} menuNode;

/// Declaring all of the local interface pointers
extern menuNode *rootMenu;                              ///< Points to the root menu node (Home)
extern menuNode *currentMenu;                           ///< Points at the current menu to follow children
extern unsigned char currentMenuSelection;              ///< Used in menus to select and display a particular item

void userLocalEventHandler( void );

#pragma udata
extern rom const char rom *LCDString[];                 ///< Declaring all of the strings in program memory

#endif	/* USERLOCAL_H */
