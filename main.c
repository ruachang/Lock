#include<reg52.h>
#include "I2c.h"
#include "key.h"
#include "lcd.h"
#include "pwm.h"
#include "motor.h"

#define u16 unsigned int 	  //对数据类型进行声明定义
#define u8 unsigned char

sbit AS=P2^2;


u8 pw_num,Error_Num,PassWord_Length=6;
u8 PASSWORD[]={8,8,8,8,8,8,0,0,0,0};
u8 key_num,Step,Step5,Load_first_flag=0;
u8 INPUT_PW_Tab[10];
bit List1=0;
bit result_flag,Input_suc_flag;

void Step_0();
void Step_1();
void Step_2();
void Step_3();
void Step_4();

void Step_5();
    void Step5_0();
    void Step5_1();
    void Step5_2();
    void Step5_3();
    void Step5_4();
    void Step5_5();
void Step_6();

void main()
{
                        //完成对锁的初始化：清屏+设置好状态待机+准备好读数
    u8 data1,a;
    LcdWriteCom(0x01);  //清屏
    for(data1=0;data1<PassWord_Length+2;data1++)
    {
        a=AtRead(data1)+0x30;
        LcdWriteData(a);
        delay(1000);
    }
    delay(1000);
    LcdInit();
    delay(1000);
    Step=0;
    Step5=0;
    Error_Num=0x00;
    Read_Password();

    while(1)
    {
        key_num=KeyDown();//读取按下的按键
         
        switch(Step)
                          //根据状态选择对应的函数
        {
            case 0: {Step_0();break;}
            case 1: {Step_1();break;}
            case 2: {Step_2();break;}
            case 3: {Step_3();break;}
            case 4: {Step_4();break;}
            case 5: {Step_5();break;}
            case 6: {Step_6();break;}
        }
    }
}

/***********************************/
/*
状态零：默认状态
状态：显示指定字
输入：按下按键
跳转条件：按键按下任意按键
*/
/*********************************/
void Step_0()
{
    LcdInit();
    ShowString(0x00,"  ");

    while (KeyDown()==0xff)
    {
        Step=1;
    }
}

/***********************************/
/*
状态一：默认状态
状态：过渡状态，显示激活后的几个选项
输入：进入该状态
跳转条件：必然跳转
*/
/*********************************/
void Step_1()
{
    LcdWriteCom(0x01);
    ShowString(0x00,"Unlock");
    ShowString(0x0f,"<");
    ShowString(0x10,"Change Password");
    ShowString(0x1f," ");
    Step=2;
}

/***********************************/
/*
状态二：判断状态
状态：根据按下的按键，确定下一个进行什么状态
输入：按下按键
跳转条件：按下确认且List1=0，跳转入状态3；=1如状态5
未确认，则显示选项，不跳转
*/
/*********************************/
void Step_2()
{
    if(key_num!=0x0b)  //确认件未按下
    {
        if((key_num==0x01) || (key_num==0x09))  //1or9键按下
        {
            List1=!List1;                       //代表是否更改密码
            if(List1==0)
            {
                ShowString(0x0f,"<");           //代表选择开门
                ShowString(0x1f," ");
            }
            else                                //代表选择改密码
            { 
                ShowString(0x0f," ");                                                                                                                                                                                                                                                                                     
                ShowString(0x1f,"<");
            }
        }
    }
    else           //确认键按下
    {
        if(List1==0){Step=3;}                //代表确认开门
        else        {Step=5;List1=0;}        //确认选择改密码
    }
        
}

/***********************************/
/*
状态三：过渡状态
状态：Wie进入状态四做准备
输入：进入状态三
跳转条件：无，将LCD清屏，并将输入密码初始化为0

*/
/*********************************/
void Step_3()
{
    Step=4;
    pw_num=0;
    LcdInit();
    ShowString(0x00,"PassWord:")
}

/***********************************/
/*
状态四：过渡状态
状态：根据按下的按键，确定下一个进行什么状态
输入：按下按键
跳转条件：继续输入=0则状态不变；flag为1入状态6
将归零
*/
/*********************************/

void Step_4()
{
    input_password(0);
    if(Input_suc_flag==1) {Step=6;}
    Input_suc_flag=0;
}

void Step_5()
{
    switch (Step5)
    {
    case 0: {Step5_0();}break;
    case 1: {Step5_1();}break;
    case 2: {Step5_2();}break;
    case 3: {Step5_3();}break;
    case 4: {Step5_4();}break;
    case 5: {Step5_5();}break;
    
    default:
        break;
    }
}

/***********************************/
/*
状态六：终止状态
状态：结束状态，判断是否正确，并返回状态零
输入：进入该状态
跳转条件：必然跳转会状态零
*/
/*********************************/
void Step_6()
{
    CipherComparison();
    if(result_flag==1)
    {
        LcdInit();
        ShowString(0x00,"Welcome");
        AS=0;
        delay(60000);
        delay(60000);
        AS=1;
        Motor_Run(1,5);
    }
    else
    {
        LcdInit();
        ShowString(0x00,"Error");
    }
    Step=0;
}

/***********************************/
/*
状态五：输入状态
状态：输入状态，相当于输入函数
输入：进入该状态
跳转条件：必然跳转
*/
/*********************************/

// 清屏，提示开始输入
void Step5_0()
{
    LcdWriteCom(0x01);
    ShowString(0x00,"input password:");
    Step5=1;
    pw_num=0;
}

// 定义密码阶段
void Step5_1()
{
    input_password(0);
    if(Input_suc_flag==1)
    {
        Step5=2;
        Input_suc_flag=0;
    }
}

// 比较输入密码是否正确
void Step5_2()
{
    CipherComparison();
    Step5=3;
}

// 判断密码错误次数
void Step5_3()
{
    if(result_flag==0)
    {
        // 如果小于三，则清零，初始化，重新输入
        if(Error_Num<3)
        {
            Error_Num++;
            LcdInit();
            ShowString(0x00,"error")
            delay(20000);
            Step5=0;
        }
        // 如果大于三，无法继续输入
        else
        {
            Error_Num=0;
            Step=0;
        }
    }
    // 输入正确，允许重置密码
    else
    {
        LcdInit();
        ShowString(0x00,"New Password");
        pw_num=0;
        Step5=4;
    }
    
}

// 初始化修改密码界面
void Step5_4()
{
    input_password(1);
    if(Input_suc_flag==1)
    {
        Step5=5;
        Input_suc_flag=0;
        LcdWriteCom(0x01);
        ShowString(0x00,"Success");
    }
}

// 将改变的密码的内容，长度，位置存入内存
void Step5_5()
{
    unsigned char j;
    PassWord_Length=pw_num;
    AtWrite(0,Load_first_flag);
    delay(100);
    AtWrite(1,PassWord_Length);
    delay(100);
    for(j=0;j<PassWord_Length;j++)
    {
        PASSWORD[j]=INPUT_PW_Tab[j];
        AtWrite(j+2,INPUT_PW_Tab[j]);
        delay(100);
    }
    Step5=0;
    Step=0;
}

void Read_Password()
{
    unsigned char j;

    Load_first_flag=AtRead(0x00);
    if(Load_first_flag==0)
    {
        Load_first_flag=1;
        AtWrite(0,0x01);
        delay(100);
        AtWrite(1,0x06);
        for(j=0;j<PassWord_Length;j++)
        {
            AtWrite(j+2,8);
            PASSWORD[j]=INPUT_PW_Tab[j];
            delay(100);
        }
    }

    Load_first_flag=AtRead(0x00);
    PassWord_Length=AtRead(0x01); //读取密码长度
    // 读取实际密码
    for(j=0;j<PassWord_Length;j++)
    {
        PASSWORD[j]=AtRead(j+2);
    }
}

// 输入函数
void input_password(bit m)
{
    unsigned char j;
    // 尚未按下确认
    if(key_num!=0x0b)
    {
        // 尚未按下取消
        if(key_num<0x0a)
        {
            INPUT_PW_Tab[pw_num]=key_num;
            pw_num=pw_num+1;
            LcdWriteCom(0xc0);
            for(j=0;j<pw_num;j++)
            {
                if(m==0) {LcdWriteData("*");};
                else     {LcdWriteData(INPUT_PW_Tab[j]+0x30;)}
            }
        }
        // 按下取消
        if(key_num==0x0a)
        {
            if(pw_num!=0)    {pw_num=pw_num-1;}
            else             {Step=0;}

            LcdWriteCom(0xc0);
            for(j=0;j<pw_num;j++)
            {
                if(m==0) {LcdWriteData("*");};
                else     {LcdWriteData(INPUT_PW_Tab[j]+0x30;)}
            }
            LcdWriteData(" ");
        }
    }

    else
    {
        // 此时输入的密码剩余字符数为零
        if(pw_num==0)
        {
            Step=0;
            LcdWriteData(0x01);
            // 说明没有输入
            ShowString(0x00,"Error 02");
            delay(100000);
        }

        else
        {
            Input_suc_flag=1;
        }
    }
}   

// 比较密码是否正确
void CipherComparison()
{
    u8 i,j=0;
    // 比较位数是否正确
    if(PassWord_Length==pw_num)
    {
        for(i=0;i<PassWord_Length;i++)
        {
            if(PASSWORD[i]!=INPUT_PW_Tab[i])
            {
                result_flag=0;
                break;
            }
            else
            {
                result_flag=1;
            }
            INPUT_PW_Tab[i]=0xff;
        }
        
    }
    else
    {
        result_flag=0;
    }
}