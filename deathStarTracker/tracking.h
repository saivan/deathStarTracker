

#ifndef TRACKING_H
#define	TRACKING_H


enum TRACKING{ 
	COLLECT_DATA,
	RUN_SENSE,
	RUN_MOTION,
	DETERMINE_POSITION
};

/// Including all of the tracking functions
void trackingWrapper( void );


#endif	/* TRACKING_H */








