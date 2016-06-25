#include <avr/io.h>
#include <util/delay.h>
#include "Sound.h"
#include "Sensor.h"
#define  F_CPU 8000000


void InitializeSound()
{
	SOUND_ON = 0;
	
}
void gensound(int freq) {
	

OCR1A = (F_CPU / 64) / (freq * 2);
// If this new sound has shorter wavelength, relocate timer so that
// we do not miss the end
if (TCNT1 > OCR1A)
TCNT1 = OCR1A-1;

TCCR1A = (0<<WGM11) | (0<<WGM10) | (0<<COM1A1) | (1<<COM1A0);
TCCR1B = (0<<WGM13) | (1<<WGM12) | ( (0<<CS12) | (1<<CS11) | (0<<CS10) );

} 
void nosound() {
TCCR1A = (0<<WGM11) | (0<<WGM10) | (0<<COM1A1) | (0<<COM1A0);
PORTB &= ~0x02;

}
void FlipFlop(int minfreq, int maxfreq)
{
	int i,r,t;
	i=minfreq;
	r=1;
		SOUND_ON = 1;
   TCCR1A |= (1 << CS11) |(1 << CS10);
	gensound(minfreq);
 while((PIND & RTSButton) && (PIND & LTSButton) && (PIND & BS))
  {

	
	if(i==minfreq)
	r=1;
	else if(i==maxfreq)
	r = 0;
	
	if(r==1)
	{
		i++;
		gensound(i);
	}
	else
	{
		i--;
		gensound(i);
	}				
	 _delay_ms(2);
	 t++;
  }
TCCR1A = (0<<WGM11) | (0<<WGM10) | (0<<COM1A1) | (0<<COM1A0);
PORTB &= ~0x02;
  	SOUND_ON = 0;
}
void Battery()
{
	uint64_t i;
	SOUND_ON = 1;
	for(i=1;i<= 2000;i++)
{
	
gensound(i);
_delay_ms(1);
  }
  nosound();
  SOUND_ON = 0;
}
void Beep(int frequency)
{  
	if(SOUND_ON == 0)
	{
		
	
TCCR1A |= (1 << CS11) |(1 << CS10);
	gensound(frequency);
	}	
}
void Stop()
{
	if(SOUND_ON == 0)
	nosound();
}
void BrakeSound()
{
	if(SOUND_ON == 0)
	{
		Beep(700);
	_delay_ms(150);
	Stop();
	_delay_ms(150);
	Beep(700);
_delay_ms(150);
nosound();
	} 
}

