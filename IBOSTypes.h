/*
 * IBOSTypes.h
 *
 * Created: 17/02/2013 10:07:12
 *  Author: arsslen
 */ 

#ifndef IBOSTYPES_H_
#define IBOSTYPES_H_

typedef enum {
            TRUE  = 1,
            FALSE = 0,
    } bool_t;
	
struct DIY_FLOAT
{
   int V1;
   unsigned char V2;
   
};
typedef struct DIY_FLOAT FM_FLOAT;

FM_FLOAT Add(FM_FLOAT a,FM_FLOAT b);
FM_FLOAT Multiply(FM_FLOAT a,FM_FLOAT b);
int Round(FM_FLOAT a);
FM_FLOAT INT_DIVIDE(int a,int b);
FM_FLOAT DIVIDE(FM_FLOAT a,FM_FLOAT b);
FM_FLOAT MINUS(FM_FLOAT a,FM_FLOAT b);
FM_FLOAT FROM_INT(int a);
#endif /* IBOSTYPES_H_ */