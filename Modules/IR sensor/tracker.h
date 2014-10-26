/* 
 * File:   tracker.h
 * Author: Jimmy
 *
 * Created on October 25, 2014, 10:00 PM
 */

#ifndef TRACKER_H
#define	TRACKER_H

void track(void);
void scan(void);
void follow(void);
void getDistance(void);

typedef struct trackFlagType{
    unsigned char targetFound : 1;
    unsigned char IRAccuracy : 1;
    unsigned char USAccuracy : 1;
    unsigned char distanceReady : 1;
}trackFlagType;

extern trackFlagType trackFlags;


#endif	/* TRACKER_H */

