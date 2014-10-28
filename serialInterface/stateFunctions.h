/* 
 * File:   stateFunctions.h
 * Author: kkeke_000
 *
 * Created on 27 October 2014, 1:47 PM
 */

#ifndef STATEFUNCTIONS_H
#define	STATEFUNCTIONS_H

typedef struct FunctionStatus FunctionStatus;

struct FunctionStatus {

    unsigned int okayToExecute : 1;
};

extern void showTargetStatus(void);
extern void showTemperature(void);
extern void azimSetAngle(void);
extern void azimManual(void);
extern void elevSetAngle(void);
extern void elevManual(void);
extern void distMax(void);
extern void distMin(void);
extern void azimMax(void);
extern void azimMin(void);
extern void elevMax(void);
extern void elevMin(void);
extern void caliTemp(void);
extern void caliAzim(void);
extern void caliElev(void);
extern void usSamplePerEst(void);
extern void usSampleRate(void);
extern void irSamplePerEst(void);
extern void irSampleRate(void);
extern void showRaw(void);
extern void showStat(void);

#endif	/* STATEFUNCTIONS_H */

