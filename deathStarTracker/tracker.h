/* 
 * File:   tracker.h
 * Author: Jimmy
 *
 * Created on October 25, 2014, 10:00 PM
 */

#ifndef TRACKER_H
#define	TRACKER_H

#define CHECK_STATIONARY 0
#define CHECK_NEIGHBOURS 1
#define CALCULATE_NEXT_POSITION 0
#define SENSOR_MEASUREMENT 1

#define AZIM 0
#define ELEV 1
#define A 40
#define B 30


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
extern unsigned int distance;
extern int lastKnownAzimuth;
extern int lastKnownElevation;
extern int lastKnownAzimuth;
extern int lastKnownElevation;

#endif	/* TRACKER_H */

