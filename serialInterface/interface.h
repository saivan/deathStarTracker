/*
* File:   interface.h
* Author: kkeke_000
*
* Created on 16 October 2014, 9:14 PM
*/

#ifndef INTERFACE_H
#define	INTERFACE_H

extern void moveToParentNode(void);
extern void moveToChildNode(void);
extern void selectNextChild(void);
extern void selectChild(char selection);
extern void moveToChildSelection(char selection);
extern void executeCurrentNodeFunction(void);
extern void enterUserMode(void);
extern void enterLocalMode(void);
extern void modeSwitch(void);
extern void stringToRam(static char rom *source, static char *destination);

#endif	/* INTERFACE_H */

