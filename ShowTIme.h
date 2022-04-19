/*
 * @Description: 显示需要的函数内容捏
 * @Author: MALossov
 * @Date: 2022-03-15 20:43:23
 * @LastEditTime: 2022-04-18 22:53:20
 * @LastEditors: MALossov
 * @Reference:
 */
#include "Timer.h"
#include "string.h"

#ifndef _ShowTime
#define _ShowTime

#define uchar unsigned char
#define uint unsigned int

sbit dula = P2 ^ 6;
sbit wela = P2 ^ 7;


int i, j, k;

uchar code table[] = { 0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,
                    0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71 };  //不含点数分隔的显示函数

uchar code ledBit[] = { 0xfe,0xfd,0xfb,0xf7,0xef,0xdf };    //从高位到低位的led位数


void sendLEDInf(uchar ledBit, uchar num2Show) {
  dula = 0;
  P0 = num2Show;
  dula = 1;
  dula = 0;

  wela = 0;
  P0 = ledBit;
  wela = 1;
  wela = 0;

  for (k = 11;k > 0;k--);
}

void display(uchar* timeTable)
{
  sendLEDInf(ledBit[0], table[timeTable[0]]);
  sendLEDInf(ledBit[1], table[timeTable[1]] | 0x80);
  sendLEDInf(ledBit[2], table[timeTable[2]]);
  sendLEDInf(ledBit[3], table[timeTable[3]] | 0x80);
  sendLEDInf(ledBit[4], table[timeTable[4]]);
  sendLEDInf(ledBit[5], table[timeTable[5]] | 0x80);

}
#endif // !_ShowTime
