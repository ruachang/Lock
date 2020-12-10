#include "I2c.h"

void delay10us()
{
    unsigned char a,b;
    for(b=1;b>0;b--)
        for(a=2;a>0;a--)
}

void I2cStart()
//产生一个时钟信号
{
    SDA=1;
    delay10us();
    SCL=1;
    delay10us();
    SDA=0;
    delay10us();
    SCL=0;
    delay10us();
}

void I2cEnd()
//拉高时钟线,制造一个终止读取信号
{
    SDA=0;
    delay10us();
    SCL=1;
    delay10us();
    SDA=1;
    delay10us();
}


unsigned char I2cSendByte(unsigned char dat)
//更新数据总线上的值
{
    unsigned char a=0,b=0;
    for(a=0;a<8;a++)
    {
        SDA=dat>>7;    //dat右移，从最高位开始读入数据
        dat=dat<<1;    //dat左移，从新检测最高位
        delay10us();
        SCL=1;
        delay10us();
        SCL=0;
        //建立和保持时间规则
    }
    SDA=1;
    delay10us();
    SCL=1;
    while(SDA)
    //等待应答，若接收成功，SDA拉低
    {
        b==;
        if(b>200)
        {
            SCL=0;//时序图要求
            return 0;
        }
    }
    SCL=0;
    delay10us();
    return 1;
}

unsigned char I2cGetByte()
//从数据总线上读一位数,不需要参数
{
    unsigned char a=0,dat=0;//初始化为0
    for(a=0;a<8;a++)
    {
        SCL=1;
        delay10us();//建立时间
        dat=dat<<1;//左移1位
        dat=dat|SDA;//从左到右依次求最高到最低
        SCL=0;
        delay10us();
    }
    return dat;
}

void AtWrite(unsigned char addr,unsigned char dat)
{
    I2cStart();
    I2cSendByte(0xa0);//发送写器件地址
    I2cSendByte(addr);//发送写入地址
    I2cSendByte(dat);
    I2cEnd();
}

unsigned char AtRead(unsigned char addr)
{
    unsigned char dat;
    I2cStart();
    I2cSendByte(0xa0);
    I2cSendByte(addr);
    I2cStart();
    dat=I2cGetByte();
    I2cEnd();
    return dat;
}