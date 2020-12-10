#ifndef __I2C_H_
#define __I2C_H_

#include<reg52.h>

sbit SCL=P2^1;      //接受芯片的时钟线
sbit SDA=P2^2;      //接受芯片的数据线

void I2cStart();
void I2cEnd();
unsigned char I2cSendByte(unsigned char dat);
unsigned char I2cGetByte();
void AtWrite(unsigned char addr,unsigned char dat);
unsigned char AtRead(unsigned char addr);