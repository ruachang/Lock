#include "key.h"

u8 KeyValue=0

void delay(u16 i)
{
    while(i--);
}

u8 KeyDown(void)
{
    char a=0;
    GPIO_KEY=0x0f;

    if(GPIO_KEY!=0x0f)
    {
        delay(1000);
        if(GPIO_KEY!=0x0f)     
        {
            GPIO_KEY=0x0f;      //零高四位为零，确定列
            switch (GPIO_KEY)
            {
            case 0x07:KeyValue=0;break;
            case 0x0b:KeyValue=1;break;
            case 0x0d:KeyValue=2;break;
            case 0x0e:KeyValue=3;break;
            default:break;
            }
        
            GPIO_KEY=0xf0;     //零低四位为零，确定行
            switch (GPIO_KEY)
            {
            case 0x70:KeyValue=KeyValue;break;
            case 0xb0:KeyValue=KeyValue+4;break;
            case 0xd0:KeyValue=KeyValue+8;break;
            case 0xe0:KeyValue=KeyValue+12;break;
            default:break;
           }
           while((a<50)&&GPIO_KEY!=0x0f)
           {
               delay(1000);
               a++
           }
        }
    }
    else
    {
        KeyValue=0xff;           //无按键被按下
    }
    return KeyValue;
}