/*
 * IBOSTypes.c
 *
 * Created: 24/02/2013 15:36:11
 *  Author: arsslen
 */ 
#include "IBOSTypes.h"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>


FM_FLOAT Add(FM_FLOAT a,FM_FLOAT b)
{
	FM_FLOAT c;
	c.V1 = 0;
c.V2 = 0;
	c.V2 = (char)((a.V2 + b.V2) % 100);
	c.V1 = (a.V1 + b.V1) + (a.V2 + b.V2 / 100);
	
	return c; 
}

FM_FLOAT Multiply(FM_FLOAT a,FM_FLOAT b)
{
FM_FLOAT c;
int x;
c.V1 = 0;
x=0;
c.V2 = 0;
x = (int)(((a.V1 * 10) + (a.V2 / 10))	* ((b.V1 * 10) + (b.V2/10)));
//c.V1 = abs((unsigned int)(x / 10000));
c.V1 = x/100;
c.V2 = (char)((x % 100));

return c;
}
int Round(FM_FLOAT a)
{
if(a.V2 >= 50)
return a.V1+1;
else
 return a.V1;	
}

FM_FLOAT INT_DIVIDE(int a,int b)
{
	FM_FLOAT c;
	
	c.V1 = (int)(a/b);
	c.V2 = (char)(((((a % b) * 10) / b) * 10) + (((((a % b) * 10) % b ) * 10) / b));
	return c;
}	

FM_FLOAT DIVIDE(FM_FLOAT a,FM_FLOAT b)
{
	
	int x,y;
	x = (a.V1 * 100) + a.V2;
	y = (b.V1 * 100) + b.V2;
	return INT_DIVIDE(abs(x),abs(y));
	
}
FM_FLOAT MINUS(FM_FLOAT a,FM_FLOAT b)
{
	FM_FLOAT c;
	int x,y;
	x = (a.V1 * 100) + a.V2;
	y = (b.V1 * 100) + b.V2;
	c.V1 = (int)((x-y)/100);
	c.V2 = (int)((x-y)%100);
	return c;
}
FM_FLOAT FROM_INT(int a)
{
	FM_FLOAT c;
   c.V1 = a;
   c.V2 = 0;
   return c;
}