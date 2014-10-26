

#ifndef ULTRASOUND_H
#define ULTRASOUND_H value


/// The Pin definitions are made here
#define     INITPIN     PORTCbits.RC2
#define     INITTRIS    TRISCbits.RC2
#define     ECHOPIN     PORTCbits.RC1
#define     ECHOTRIS    TRISCbits.RC1
#define     OVERFCONST  0xFF


/// The external variables are all declared here
extern unsigned int  milliTime;
extern unsigned int  subMilliTime;
extern unsigned char USStatus;
extern unsigned char USArrayPosition;
extern volatile unsigned char USOverF;
extern unsigned int distance;
extern unsigned int tempDistance;
extern unsigned int speed;
extern unsigned int distPerMs;
extern unsigned int distPerSubMs;
extern unsigned int UScapturedValue[10];
extern timeTag echo;


/// The functions are all declared here
void high_interrupt(void);
void highPriorityIsr(void);
void fireEcho (void);
void echoCalc (void);
void low_interrupt(void);

#endif