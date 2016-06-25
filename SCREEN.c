/*
 * LCD.c
 *
 * Created: 17/02/2013 13:13:37
 *  Author: arsslen
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include "SCREEN.h"
#include "IBOSTypes.h"
#include "nokia1100_lcd_lib.h"
#include "LED.h"
void InitializeScr()
{
	nlcd_Init();
	_delay_ms(100);
}
void Logo()
 {
	


	 nlcd_GotoXY(3,3);
	 
	 nlcd_PrintF(PSTR("Arsslensoft"));
	 _delay_ms(1500);
	 nlcd_Clear();

 }
void OSLoad()
 {
     nlcd_GotoXY(2,1);
	 nlcd_PrintF(PSTR("FLYDOWS MOTOR"));
	 nlcd_GotoXY(4,5);
	 nlcd_PrintF(PSTR("Loading..."));
  nlcd_Rect (3,55,93,65,FILL_ON, PIXEL_ON);
  _delay_ms(700);
  nlcd_Clear();
 }


