#ifndef __motor_H
#define __motor_H
#include<reg52.h>

#ifndef u8
#define u8 unsigned char
#endif

#ifndef u16
#define u16 unsigned int
#endif

sbit INA=P1^0;
sbit INB=P1^1;
sbit INC=P1^2;
sbit IND=P1^3;

void Motor_Delay1ms(u16 c)
void Motor_Run(bit dir,u16 sudu)
void TimerInit()