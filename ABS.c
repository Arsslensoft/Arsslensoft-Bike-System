
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "IBOSTypes.h"
#include <avr/sleep.h>
#define F_CPU 8000000
#include <avr/interrupt.h>
#include <math.h>
#include "LED.h"
#include "Sensor.h"
#include "SCREEN.h"
#include "Sound.h"
#include "nokia1100_lcd_lib.h"


bool_t Braking;

char cg;
char FC;
FM_FLOAT FACTOR;
FM_FLOAT PFACTOR;
FM_FLOAT V;
FM_FLOAT R;	
FM_FLOAT speed;
FM_FLOAT dbc,lspeed;
int vi;
int acceleration;
int vp,rpm;
FM_FLOAT db,tb;
char* Suggestion;

// debug vars
FM_FLOAT vcc;//variable to hold the value of Vcc
bool_t braked;

void setup_adc(void)
{
    ADMUX = 0xE; //Set the Band Gap voltage as the ADC input
    ADCSRA = (1<<ADEN)|(1<<ADATE)|(1<<ADIE)|(1<<ADSC)|5;
}

ISR(ADC_vect) //ADC End of Conversion interrupt 
{
unsigned char adc_data;
adc_data = ADC>>2; //read 8 bit value
vcc.V1 = (int)(1.1 * 255 / adc_data);
vcc.V2 = (char)(((1.1 * 255 / adc_data)-vcc.V1)*100);
}


void port_init(void)
{
 PORTB = 0x00;
 DDRB  = 0xFF;
 PORTB = 0x00;
 PORTC = 0x00; 
 DDRC  = 0x00;
 PORTD = 0x00;
 DDRD  = 0xFF;
 }

// initialize devices and drivers
void InitializeFly()
{
	
port_init();
		
	InitializeScr();
	setup_adc(); //setup the ADC
	 Logo();
	 
	 braked = FALSE;
	 nlcd_GotoXY(0,0);
	   nlcd_PrintF(PSTR("PORT INITIALIZED"));
	  // SENSOR INIT
	  InitializeSensor();
	 nlcd_PrintF(PSTR("SENS INITIALIZED"));
	 
	 //LED Init
	 	InitializeLed();
	 nlcd_PrintF(PSTR("LED INITIALIZED "));

	 //Sound Init
	 InitializeSound();
	 nlcd_PrintF(PSTR("SYNT INITIALIZED "));
	 // 
	  nlcd_GotoXY(0,4);
	  nlcd_Inverse(INV_MODE_ON);
	 nlcd_PrintF(PSTR(" BLACKBOX INFOS "));
	 nlcd_PrintF(PSTR("(c) 2013 ASLHRCC"));
	 nlcd_PrintF(PSTR("FIRMWARE V1.0.50"));
	 nlcd_PrintF(PSTR("DEV-ID:FM-0001-A"));
	_delay_ms(700);
		Flash();
	 	nlcd_Inverse(INV_MODE_OFF);
		 nlcd_Clear();

     	OSLoad();
	    Signal();


FLE = TRUE;
TurnFL();

}

void Display()
{
	 char d[22];
    char t[18];
	 char s[15];
 	char a[9];
	char v[11];
	char r[9];
	char p[7];
	
	if(acceleration < 0)
	sprintf(a,"-%.2i.%.2i",abs(acceleration/100), abs(acceleration%100));
	else		 
		sprintf(a,"%.2i.%.2i",(acceleration/100), (acceleration%100));


	 sprintf(d,"BPD : %.2i.%.2i m !%d",db.V1, db.V2,dbc.V1-db.V1);
	 
	sprintf(t,"BPT : %.2i.%.2i s",tb.V1,tb.V2);
	sprintf(r,"%.3i rpm",rpm);	
	
	 sprintf(s,"%.2i.%.2i KM/h", speed.V1, speed.V2);
	  sprintf(v,"%.2i.%.2i",V.V1,V.V2);
	
	 
	
	
	 	sprintf(p,"%.2i.%.2i V",vcc.V1,vcc.V2);	 
		 

		 nlcd_GotoXY(0,4);
     nlcd_Print(a);

		  nlcd_GotoXY(11,4);
	
	 	  nlcd_Print(v);
	 nlcd_Line(0,43,95,43, 0);
		 	nlcd_GotoXY(0,0);
	if((PIND & LTSButton )&& RTSB == TRUE )
	{
	
		 nlcd_Inverse(INV_MODE_ON);
	 nlcd_PrintF(PSTR(" Double Signaler"));
	}
	else if(PIND & LTSButton)
	{
		nlcd_Inverse(INV_MODE_ON);
		 nlcd_PrintF(PSTR("  Turning Left   "));
	}
	else if(RTSB == TRUE)
	{
	
		nlcd_Inverse(INV_MODE_ON);
		 nlcd_PrintF(PSTR("  Turning Right  "));
	}
	
	else
	{	
nlcd_Inverse(INV_MODE_ON);
			 nlcd_PrintF(PSTR("  FLYDOWS MOTOR  "));
	}
		
	nlcd_Inverse(INV_MODE_OFF);
	
	if(Braking == TRUE)
	{

			nlcd_GotoXY(0,1);
	  nlcd_PrintF(PSTR("BRAKING"));

		nlcd_GotoXY(0,6);
		nlcd_Print(d);
	    nlcd_GotoXY(0,7);
		nlcd_Print(t);
		
	}
    else
	   {
		   	nlcd_GotoXY(0,1);

	    nlcd_Print(r);
		nlcd_GotoXY(0,6);
			nlcd_Print("Have a good ride");
				nlcd_GotoXY(0,7);
			nlcd_Print("    Thank you   ");
	   }
	   		  	nlcd_GotoXY(9,1);	
	
	  nlcd_Print(p);
		 nlcd_Line(0,18,95,18, 0); 
	  nlcd_GotoXY(3,3);
	  nlcd_Print(s);

	
}


void Calculate()
{
	FM_FLOAT F;
	int vx,vy,ay,vpy;
	F = GetWheelFrequency();
	if((F.V1+F.V2) != 0)
	{
vp=(V.V1*100)+V.V2;
vpy = V.V1;
	V = Multiply(PFACTOR, F);
	vi = (V.V1*100)+V.V2;
	vx = V.V1;
	vy = V.V1+1;
	acceleration = (int)((vi-vp));
	ay = vy-vpy;
	lspeed.V1 = speed.V1;
	lspeed.V2 = speed.V2;
	
	speed = Multiply(F, FACTOR);
	rpm = Multiply(F,R).V1 * 10;
	if(Braking)
	{
		if(vi != 0 && acceleration < 0)
		{
		tb = INT_DIVIDE(abs(vi),abs(acceleration));
		
		db=INT_DIVIDE(abs((vx*vx* 10)), abs(2*acceleration/10));
		dbc = INT_DIVIDE(abs((vy*vy* 10)), abs(2*acceleration/10));		
		}	
		else
		{
			db.V1 = 0;
			db.V2 = 0;
			tb.V1 = 0;
			tb.V2 = 0;
		}	
	}
	else
	{	
db.V1 = 0;
			db.V2 = 0;
			tb.V1 = 0;
			tb.V2 = 0;
	}

		}
		else
		{
		vi=0;
		V.V1 = 0;
		V.V2=0;
		
		acceleration =0;
		db.V1 = 0;
			db.V2 = 0;
			tb.V1 = 0;
			tb.V2 = 0;
		rpm =0;

}
if(vi+acceleration == 0)
speed.V1 = speed.V2=0;
	}
bool_t Changed()
{
	if(speed.V1 != lspeed.V1 || speed.V2 != lspeed.V2 || Braking != CheckBrake() || LTSB != CheckLTS() || RTSB != CheckRTS())
	  return TRUE;
	 else
	 return FALSE;
}
int main()
{


bool_t chng;
	InitializeFly();
	
TurnSignalOn();
		FC = 0;
		
	R.V1 = 6;
	R.V2 = 0;
	FACTOR.V1 = 7;
	FACTOR.V2 = 45;
	PFACTOR.V1 = 2;
	PFACTOR.V2 = 7;
	vi=0;
	vp=0;
	acceleration=0;
	speed.V1=0;
_delay_ms(20);
braked = TRUE;
Display();
while(1)
{		
	if(FC == 10)
	{
			Calculate(); 
		// check sensors
	    chng = Changed();
	}			
	
		if(chng == TRUE)
		{
		 LTSB = CheckLTS();	
	    RTSB = CheckRTS();
	    Braking = CheckBrake();
		}
         if((LTSB == FALSE ) && (PIND & LEFT_LED)) 
		 {
           PORTD &= ~LEFT_LED;
		   Stop();
		 }		   
        if((RTSB == FALSE ) && (PIND & RIGTH_LED)) 
		{
           PORTD &= ~RIGTH_LED;
		       Stop();
		}
   
		if(Braking == FALSE)
		{
			RLE = FALSE;
			TurnRL();
			if(braked == FALSE)
			{
				//BrakeSound();
				braked = TRUE;
			}
		}	
       else
		{
			RLE = TRUE;
			TurnRL();
			braked = FALSE;
		} 
		//1hz operation (adjust it to adjust frequency counting
		if(chng == TRUE)
		 {	 
     				  
			Display();
			  FC=-1;		 
			  chng = FALSE;
		 } 
	
		FC++;
		Flash();
		if((PIND & RTSButton) && (PIND & LTSButton) && (PIND & BS))
		{

	   	RLE = TRUE;
	    TurnRL();
		 
		  FlipFlop(500,1200);
		}		    
		_delay_ms(50);
		
}

		 }
