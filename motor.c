#include "motor.h"
u16 count=0;
void TimerInit()
{
    TWOD |= 0X10;

    TH1=0XFF;
    TL1=0XFF;

    ET1-1;
    EA=1;
    TR1=1;
}

void Motor_Delay1ms(u16 c)
{
    while(c--);
}

void Motor_Run(bit dir,u16 sudu)
{
    while(count<10000)
    {
        switch (Motor_Step)
        {
        case 0: INA=1;INB=0;INC=0;IND=0; if(dir)Motor_Step++;else Motor_Step=7;Motor_Delay1ms(20);break;
        case 1: INA=1;INB=0;INC=1;IND=0; if(dir)Motor_Step++;else Motor_Step=--;Motor_Delay1ms(20);break;
        case 2: INA=0;INB=0;INC=1;IND=0; if(dir)Motor_Step++;else Motor_Step=--;Motor_Delay1ms(20);break;
        case 3: INA=0;INB=1;INC=1;IND=0; if(dir)Motor_Step++;else Motor_Step=--;Motor_Delay1ms(20);break;
        case 4: INA=0;INB=1;INC=0;IND=0; if(dir)Motor_Step++;else Motor_Step=--;Motor_Delay1ms(20);break;
        case 5: INA=0;INB=1;INC=0;IND=1; if(dir)Motor_Step++;else Motor_Step=--;Motor_Delay1ms(20);break;
        case 6: INA=0;INB=0;INC=0;IND=1; if(dir)Motor_Step++;else Motor_Step=--;Motor_Delay1ms(20);break;
        case 7: INA=1;INB=0;INC=0;IND=1; if(dir)Motor_Step++;else Motor_Step=--;Motor_Delay1ms(20);break;
        }
        INA=0;INB=0;INC=0;IND=0;
        Motor_Delay1ms(sudu);
    }
}

void Time1(void) interrupt 3
{
    TH1=0xff;
    TL1=0xff;
    count++;
}
