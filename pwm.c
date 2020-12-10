#include "pwm.h"

u16 count,value,timer1,timer2=0;

void TimerInit()
{
    TWOD |= 0X10;

    TH1=0XFF;
    TL1=0XFF;

    ET1-1;
    EA=1;
    TR1=1;
}

void PWMgenrate()
{
    TimerInit();
    while(timer2<1000)
    {
        if(count>100)
        {
            count=0;
            if(DIR==1)
            {
                value++;
            }
            if(DIR==0)
            {
                value--;
            }
        }

        if(value==1000)
        {
            DIR=0;
        }
        if(value==0)
        {
            DIR=1;
        }
        if(timer1>1000)
        {
            timer1=0;
        }
        if(timer1<value)
        {
            PWM=1;
        }
        else
        {
            PWM=0;
        }
        
    }
}

void Time1(void) interrupt 3
{
    TH1=0xff;
    TL1=0xff;
    timer1++;
    count++;
    timer2++;
}

