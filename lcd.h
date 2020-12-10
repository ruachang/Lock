#ifndef __LCD_H_
#define __LCD_H_

#include<reg52.h>

#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint
#define uint unsigned int
#endif

// 定义端口
#define LCD1602_DARAPINS P0
sbit LCD1602_E=P2^7;
sbit LCD1602_RW=P2^5;
sbit LCD1602_RS=P2^6;

// 函数声明
void Lcd1602_Delay1ms(uint c);
void LcdWriteCom(uchar com);
void LcdWriteData(uchar dat);
void LcdInit();
void Display_Standby();
void Display_result(bit rt);
void Display_Password(unsigned char i);
void ShowString (unsigned char Coordinate,char *ptr);