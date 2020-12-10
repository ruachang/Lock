#ifndef __pwm_H
#define __pwm_H
#include<reg52.h>

#ifndef u8
#define u8 unsigned char
#endif

#ifndef u16
#define u16 unsigned int
#endif



sbit PWM=P2^1;
bit DIR;

void TimerInit()
