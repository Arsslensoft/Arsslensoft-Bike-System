/*
 * Sensor.c
 *
 * Created: 17/02/2013 14:55:15
 *  Author: arsslen
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>  
#include <avr/pgmspace.h>
#include <math.h>
#include "Sensor.h"
#include "LED.h"
#include "IBOSTypes.h"

FM_FLOAT frequency;
int TF;
bool_t braking;


bool_t CheckRTS()
{
	if (PIND & RTSButton)
	   return TRUE;
	else
      return FALSE;	
}	
bool_t CheckLTS()
{
	if (PIND & LTSButton)
	   return TRUE;
	else
      return FALSE;	
}	
bool_t CheckBrake()
{
	if (PIND & BS)
	   return TRUE;
	else
      return FALSE;	
}	
void ResetSPDS(){
frequency.V1 = 0;
 frequency.V2 = 0;
 }
FM_FLOAT GetWheelFrequency()
{
  return frequency;
}
ISR(INT0_vect)
{

	frequency = INT_DIVIDE(1000,TF); 
	
	TF=0;

}
ISR(INT1_vect)
{
	braking = TRUE;	
  
}


void InitializeSensor()
{

	DDRD &= (~(1<<DDD0)); 
	DDRD &= (~(1<<DDD1)); 
	DDRD &= (~(1<<DDD2)); 
	DDRD &= (~(1<<DDD3)); 

	braking = FALSE;
ResetSPDS();


  EIMSK |= (1 << INT0) | (1 << INT1);   
  
   EICRA  = (1<<ISC01);
   
   sei(); 
   
   OCR2A = 124;

    TCCR2A |= (1 << WGM21);
    // Set to CTC Mode

    TIMSK2 |= (1 << OCIE2A);
    //Set interrupt on compare match

    TCCR2B |=  (1 << CS21);
    // set prescaler to 64 and starts PWM


    sei();
    // enable interrupts


	
}

ISR (TIMER2_COMPA_vect)
{
   TF++;
   if(TF==10000)
   {
     ResetSPDS();
     TF=0;
   }	 
}


