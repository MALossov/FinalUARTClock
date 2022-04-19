/*
 * @Description:
 * @Author: MALossov
 * @Date: 2022-04-12 21:18:53
 * @LastEditTime: 2022-04-19 09:13:24
 * @LastEditors: MALossov
 * @Reference:
 */
#include <STC12C5A60S2.h>
#include <string.h>
#include "main.h"
#include "DS1302.h"
#include "Functions.h"
#include "Timer.h"

#ifndef _Uart
#define _Uart



#define _strLenth 100
#define MaxMode '2'
char rxStr[_strLenth];
char  rxFlg;
char* rxPtr;
char aRxBuffer[2];
extern uchar ctrBit, ctrBitFlg;
uchar ctrBitCopy;

void chkMode(char* str);
extern uchar timeCngFlg, yearCngFlg;
//串口初始化，晶振11.0592，波特率9600
void UartInit(void)		//9600bps@11.0592MHz
{
    PCON &= 0x7F;		//波特率不倍速
    SCON = 0x50;		//8位数据,可变波特率
    AUXR |= 0x04;		//独立波特率发生器时钟为Fosc,即1T
    BRT = 0xDC;		//设定独立波特率发生器重装值
    AUXR |= 0x01;		//串口1选择独立波特率发生器为波特率发生器
    AUXR |= 0x10;		//启动独立波特率发生器

    EA = 1;//开总中断
    ES = 1;//开串口中断

    rxPtr = rxStr;
    ctrBit = ctrBitCopy = '0';
}

//向串口发送一个字符
void PutChar(char ch) {
    SBUF = ch;
    while (!TI);TI = 0;
}

//向串口发送一段字符串
void prints(char* s)
{
    while (*s != '\0')//发送字符串,直到遇到0才结束
    {
        SBUF = *s++;
        while (!TI);
        TI = 0;
    }
}


//对于接收数据进行判断
void Chk_rxBuf() {
    if ((!rxFlg) && *aRxBuffer == '$') {    //以$作为接收字符串开始的标志字符
        rxFlg = 1;
    }
    else if (rxFlg && *aRxBuffer == '#') {  //若到接收到#则停止字符串接收，并且进行状态机状态的判定
        rxFlg = 0;
        *rxPtr = '\0';
        prints("The Received Sentence is:");
        prints(rxStr);
        chkMode(rxStr);
        rxPtr = rxStr;  //重新让接收指针指向字符串头
    }
    else if (rxFlg && *aRxBuffer != '#' && *aRxBuffer != '$') { //如果是普通字符，进行字符的接收
        *rxPtr = *aRxBuffer;
        rxPtr++;
    }
}



//串口中断处理函数
void uart_Interrupt() interrupt 4
{
    if (RI)
    {
        RI = 0;
        //UartData = SBUF;
        *aRxBuffer = SBUF;
        Chk_rxBuf();
        //SBUF = UartData;
        TI = 0;//向PC返回接收的数据

        ES = 0;//开串口中断
        ES = 1;//开串口中断
    }
}




void chkMode(char* str) {
    static unsigned char  yearFlg, timeFlg;
    unsigned char i;
    static unsigned char tmpTimeData[4];
	
	            uchar d, m;
            uint y;

    if (*str == 'M') {
        ctrBit = str[2];
        if (ctrBitCopy != ctrBit && ctrBit >= '0' && ctrBit <= MaxMode) {
            ctrBitFlg = 1;
            ctrBitCopy = ctrBit;
            prints("ModeSeted");
        }
    }

    if (ctrBitCopy == '0' && *str == 't')
    {
        for (i = 2;i < 8;i++) {
            timeFlg = (str[i] >= '0' && str[i] <= '9') ? 1 : 0;
            if (timeFlg == 0)break;
        }
        if (timeFlg) {
            tmpTimeData[0] = ((str[6] & 0x0f) << 4) + (str[7] & 0x0f);
            tmpTimeData[1] = ((str[4] & 0x0f) << 4) + (str[5] & 0x0f);
            tmpTimeData[2] = ((str[2] & 0x0f) << 4) + (str[3] & 0x0f);
            tmpTimeData[3] = 0;

            timeCngFlg = 1;

            prints(tmpTimeData);

            if (tmpTimeData[2] < 0x25 && tmpTimeData[1] < 0x60 && tmpTimeData[0] < 0x60) {
                TimeData[0] = tmpTimeData[0];
                TimeData[1] = tmpTimeData[1];
                TimeData[2] = tmpTimeData[2];

                prints("timeSeted");
            }
        }
    }

    if (ctrBitCopy == '0' && *str == 'y')
    {
        for (i = 2;i < 8;i++) {
            yearFlg = (str[i] >= '0' && str[i] <= '9') ? 1 : 0;
            if (yearFlg == 0)break;
        }
        if (yearFlg) {
            //时钟日历暂存数组，秒、分、时、日、月、周、年 初值为= 16年2月15日 周1 23:58:50
            tmpTimeData[0] = ((str[6] & 0x0f) << 4) + (str[7] & 0x0f);  //日
            tmpTimeData[1] = ((str[4] & 0x0f) << 4) + (str[5] & 0x0f);  //月
            tmpTimeData[2] = ((str[2] & 0x0f) << 4) + (str[3] & 0x0f);  //年


            d = (tmpTimeData[0] >> 4) * 10 + (tmpTimeData[0] & 0x0f);
            m = (tmpTimeData[1] >> 4) * 10 + (tmpTimeData[1] & 0x0f);
            y = (tmpTimeData[2] >> 4) * 10 + (tmpTimeData[2] & 0x0f) + 2000;
            tmpTimeData[3] = (d + 2 * m + 3 * (m + 1) / 5 + y + y / 4 - y / 100 + y / 400) % 7;
            yearCngFlg = 1;
            if (tmpTimeData[2] < 0x99 && tmpTimeData[1] < 0x13 && tmpTimeData[0] < 0x32) {
                TimeData[3] = tmpTimeData[0];
                TimeData[4] = tmpTimeData[1];
                TimeData[6] = tmpTimeData[2];
                TimeData[5] = tmpTimeData[3];
                prints("timeSeted");
            }
        }
    }

}



#endif // !_Uart