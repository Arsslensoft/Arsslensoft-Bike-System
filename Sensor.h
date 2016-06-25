/*
 * Sensor.h
 *
 * Created: 17/02/2013 14:44:13
 *  Author: arsslen
 */ 
 #include "IBOSTypes.h"
 #include <avr/io.h>
	
#define RTSButton 0x01
#define LTSButton 0x02


#define SPDS 0x04
#define BS 0x08

bool_t BMSG;

bool_t CheckRTS();
bool_t CheckLTS();
bool_t CheckBrake();
FM_FLOAT GetWheelFrequency();


void ResetSPDS();
ISR(INT0_vect);
ISR(INT1_vect);

void InitializeSensor();
