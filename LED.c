#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "LED.h"
#include "Sensor.h"
#include "Sound.h"


 volatile uint8_t count;

void InitializeLed()
{
 PORTD = 0x00;
 DDRD  = 0xFF;
LTSB = FALSE;
RTSB= FALSE;
}
void Flash()
{
	PORTB |= 0x01;
	_delay_ms(50);
	PORTB &= ~0x01;
}
void TurnFL()
{
	if (FLE)
        PORTD |= FRONT_LED; 
	 else
	   PORTD &= ~FRONT_LED;    
}
void TurnRL()
{
	if (RLE)
	{
		PORTB |= BGAIN; 
        PORTD |= REAR_LED; 
	}		
	 else
	 {
	   PORTD &= ~REAR_LED;    
	  PORTB &= ~BGAIN; 
	 }	   
}
void TurnSignalOn()
 {
	// Set the Timer Mode to CTC
    TCCR0A |= (1 << WGM01);

    // Set the value that you want to count to
    OCR0A = 166;

    TIMSK0 |= (1 << OCIE0A);    //Set the ISR COMPA vect

    sei();         //enable interrupts


    TCCR0B |= (1 << CS02);
    // set prescaler to 256 and start the timer
   count=0;

	
 }
void TurnSignalOff()
 {

	
   //Enable Overflow Interrupt Enable
   TIMSK0 &=(0<<TOIE0);

   //Initialize Counter
   TCNT0=0;

   //Initialize our varriable
   count=0;
   // cli();
 }

void Signal()
 {
	 Beep(7000);
	PORTD |= FRONT_LED; 
	PORTD |= REAR_LED; 
	PORTD |= RIGTH_LED; 
	PORTD |= LEFT_LED;
	PORTB |= BGAIN;
	_delay_ms(1000);
	 Stop();
    _delay_ms(250);
	 Beep(7000);
	 _delay_ms(250);
	 Stop();
	PORTD &= ~FRONT_LED;    
    PORTD &= ~REAR_LED;    
	PORTD &= ~LEFT_LED;    
	PORTD &= ~RIGTH_LED; 
	PORTB &= ~BGAIN;   
 }
 void  TurnTS()
{
	 if (TS == FALSE)
	 {
		 if(PIND & LTSButton)
		 {
		   PORTD |= LEFT_LED;
	       Beep(470);
		 }				   
		 if(RTSB == TRUE)
		 {
		   PORTD |= RIGTH_LED;
		    Beep(470);
		 }		
	
    	TS = TRUE;
	 }		
	 else
	 {
		 	 if(PIND & LTSButton)
		 {
				 	
		    PORTD &= ~LEFT_LED;  
		Stop();
		 }					
		 if(RTSB == TRUE)
		  {
		   PORTD &= ~RIGTH_LED;   
		    	Stop();
		  }	
			 
	
     	TS = FALSE;
	 }		
	 
	 }	 

ISR (TIMER0_COMPA_vect) 
{
   //This is the interrupt service routine for TIMER0 OVERFLOW Interrupt.
   //CPU automatically call this when TIMER0 overflows.

   //Increment our variable
   count++;
   //
if(count == 10)
{
	TurnTS();
	count = 0;
}



//else if(count == )
//{
	//TurnTS();
//}		
//else if(count == 45)
//{
	//TurnTS();
//}	
//else if(count == 61)
   //{
	  //
      //count=0;
	//TurnTS();
   //}
//

}