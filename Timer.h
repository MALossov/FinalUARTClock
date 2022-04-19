/*
 * @Description: TimerOption to 计时一个1%的1秒
 * @Author: MALossov
 * @Date: 2022-03-15 20:43:48
 * @LastEditTime: 2022-04-19 01:30:45
 * @LastEditors: MALossov
 * @Reference:
 */
 //#include <REG52.H>
#include <STC12C5A60S2.h>
#include "LCD1602.h"
#include "string.h"
#include "KeyScan.h"

#define uchar unsigned char
#ifndef _Timer
#define _Timer

sbit key_s2 = P3 ^ 0;//独立按键S2
sbit key_s3 = P3 ^ 1;//独立按键S3
sbit led = P1 ^ 4;
uint T0Count = 0;

uchar OrderedTime[6] = { 0,0,0,0,0,0 };
uchar* ptr = &OrderedTime[5];

void Timer0Init(void)		//1000微秒@11.0592MHz/一毫秒
{
    //AUXR |= 0x80;		//定时器时钟1T模式
    TMOD &= 0xF0;		//设置定时器模式
    TL0 = 0x66;		//设置定时初始值
    TH0 = 0xFC;		//设置定时初始值
    TF0 = 0;		//清除TF0标志

    EA = 1;
    ET0 = 1;
    TR0 = 1;		//定时器0开始计时
}

void CheckTime(uchar* TimeTable) {
    if (TimeTable[5] > '9') {
        TimeTable[5] = '0';
        TimeTable[4]++;
    }

    if (TimeTable[4] == '6') {
        TimeTable[4] = '0';
        TimeTable[3]++;
    }
    //低位两位为10进制

    if (TimeTable[3] > '9') {
        TimeTable[3] = '0';
        TimeTable[2]++;
    }
    //秒的后一位为十进制

    if (TimeTable[2] == '6') {
        TimeTable[2] = '0';
        TimeTable[1]++;
    }
    //秒的前一位为6进制


    if (TimeTable[1] > '9') {
        TimeTable[1] = '0';
        TimeTable[0]++;
    }
    //分钟的后一位为十进制

    if (TimeTable[0] == '6') {
        TimeTable[2] = '0';
    }
    //分钟的前一位为6进制：当计数满的时候自动清零
}
void Begin_MB() {
    static uchar tmpTime[6] = { 0,0,0,0,0,0 };
    uchar i;

    Lcd1602_Write_Cmd(0x01); //清屏
    LcdShowStr(0, 0, "Stop Watch:");
    LcdShowStr(0, 1, "00'00\"00");
    strcpy(OrderedTime, "000000");
    Timer0Init();
    strcpy(tmpTime, OrderedTime);
    while (ctrBitFlg==0) {
        //			LcdShowStr(0, 0, OrderedTime);
        for (i = 0; i < 6; i++)
        {
            if (OrderedTime[i] != tmpTime[i]) {
                switch (i)
                {
                case 0:
                    DisplayOneChar(0, 1, OrderedTime[0]);
                    break;
                case 1:
                    DisplayOneChar(1, 1, OrderedTime[1]);
                    break;
                case 2:
                    DisplayOneChar(3, 1, OrderedTime[2]);
                    break;
                case 3:
                    DisplayOneChar(4, 1, OrderedTime[3]);
                    break;
                case 4:
                    DisplayOneChar(6, 1, OrderedTime[4]);
                    break;
                case 5:
                    DisplayOneChar(7, 1, OrderedTime[5]);
                    break;
                default:
                    break;
                }
                tmpTime[i] = OrderedTime[i];
            }
        }


        if (key_s2 == 0)//判断S2是否被按下，暂停
        {
            DelayMs(20);//按键消抖
            if (key_s2 == 0)
            {
                TR0 = !TR0;
                while (!key_s2);//松手检测
            }
        }
        if (key_s3 == 0)//判断S3是否被按下，清零
        {
            DelayMs(20);//按键消抖
            if (key_s3 == 0)
            {
                strcpy(OrderedTime, "000000");
                while (!key_s3);//松手检测
            }
        }

    }
}

void T0Itr() interrupt 1
{
    TL0 = 0x66;		//设置定时初始值
    TH0 = 0xFC;		//设置定时初始值//1微秒
    T0Count++;

    if (T0Count == 100) { //10毫秒：即0.01秒
        T0Count = 0;
        (*ptr)++;
        CheckTime(OrderedTime);
        led = !led;
    }

}

#endif // !_Timer