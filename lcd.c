#include "lcd.h"

uchar i;

void Lcd1602_Delay1ms(uint c)
//延时函数
{
    uchar a,b;
    for(;c>0;c--)
    {
        for(b=199;b>0;b--)
        for(a=1;a>0;a--)
    }
}

void LcdWriteData(uchar dat)
// 将输入的数据写入LCD的内部ROM中
{
    LCD1602_E=0;
    LCD1602_RW=1;       //选择写入
    LCD1602_RS=0;

    LCD1602_DARAPINS=dat;
    Lcd1602_Delay1ms(1);

    LCD1602_E=1;
    Lcd1602_Delay1ms(5);
    LCD1602_E=0;
}

void LcdWriteCom(uchar com)
// 将即将进行的指令赋给LCD
{
    LCD1602_E=0;
    LCD1602_RW=0;
    LCD1602_RS=0;

    LCD1602_DARAPINS=com;
    Lcd1602_Delay1ms(1);

    LCD1602_E=1;        //写入时序
    Lcd1602_Delay1ms(5);
    LCD1602_E=0;
}

void LcdInit()
// 将LCD状态初始化
{
    LcdWriteCom(0x38);    //开显示
    LcdWriteCom(0x0c);    //开显示功能，光标不开
    LcdWriteCom(0x06);    //将模式设置为没写入一位指针加1
    LcdWriteCom(0x01);    //将显示屏清零
    LcdWriteCom(0x80);    //标定光标初始位置
}

void ShowString(unsigned char coordinate,char *ptr)
// 在显示屏上显示对应字母，其中
// 第一个变量为显示位置，高四位为行，低四位为列
// 第二个变量为需要显示的内容
{
    if(coordinate<=0x0f)                      //判断输入的显示位置在第一行还是第二行
    LcdWriteCom((coordinate&0x0f)+0x80);      //若在第一行，则光标开始在第一行开始处
    else
    {
        LcdWriteCom((coordinate&0x0f)+0xc0);  //若开始在第二行，则光标开始在第二行处
    }
    
    while(*ptr!='\0')
    {
        LcdWriteData(ptr);
        ptr++;
    }
}