

#ifndef ULTRASOUND_H
#define ULTRASOUND_H value


/// The Pin definitions are made here
#define     INITPIN     PORTCbits.RC2
#define     INITTRIS    TRISCbits.RC2
#define     ECHOPIN     PORTCbits.RC1
#define     ECHOTRIS    TRISCbits.RC1

typedef struct USFlagType {
	unsigned char distanceReady:1;
	unsigned char fireStatus:1;

} USFlagType;

extern USFlagType USFlags;

typedef struct USValueType {
	unsigned int distance;
        unsigned char sampleSize;
        unsigned char currentSampleSize;
        unsigned int maxRange;
        unsigned int minRange;
        unsigned int freq_ms;
} USValueType;

extern USValueType USValues;

/// The external variables are all declared here
extern timeTag echoCanFire;
extern unsigned int distPerMs;
extern unsigned int distPerSubMs;
extern unsigned int USCapturedValue[10];
extern unsigned int USCapturedRaw[10];

/// The functions are all declared here
void fireEcho (void);
void echoCalc (void);
void testUSState(void);
void USSetup(void);
void setUSMin (unsigned int min);
void setUSMax (unsigned int max);
void clearUSSamplePerEstimate (void);
void setUSFrequency (unsigned char frequency);

#endif