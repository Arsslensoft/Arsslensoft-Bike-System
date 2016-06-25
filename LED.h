/*
 * LED.h
 *
 * Created: 17/02/2013 13:00:06
 *  Author: arsslen
 */ 
#include "IBOSTypes.h"


	// LEDS PORT DEFINITION
#define FRONT_LED 0x20
#define REAR_LED  0x40
#define LEFT_LED 0x10
#define RIGTH_LED 0x80
#define BGAIN 0x04



void InitializeLed();
void TurnFL();
void TurnRL();
void TurnSignalOn();
void TurnSignalOff();
void Signal();
void Flash();

ISR (TIMER0_COMPA_vect) ;
bool_t FLE;
 bool_t RLE;
 bool_t TS;
 bool_t LTSB;
 bool_t RTSB;