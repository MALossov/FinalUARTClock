/*
 * @Description:
 * @Author: MALossov
 * @Date: 2022-04-18 21:23:17
 * @LastEditTime: 2022-04-19 01:27:49
 * @LastEditors: MALossov
 * @Reference:
 */
#ifndef _KeyScan
#define _KeyScan
#include <STC12C5A60S2.h>
#include <intrins.h>

void DelayMs(unsigned char delay)		//@11.0592MHz
{
  unsigned char i, j, tmp;
  for (;tmp < delay;tmp++) {
    _nop_();
    _nop_();
    _nop_();
    i = 11;
    j = 190;
    do
    {
      while (--j);
    } while (--i);
  }
}


unsigned char KeyScan()  //键盘扫描函数，使用行列反转扫描法
{
  unsigned char cord_h, cord_l;//行列值中间变量
  P3 = 0x0f;            //行线输出全为0
  cord_h = P3 & 0x0f;     //读入列线值
  if (cord_h != 0x0f)    //先检测有无按键按下
  {
    DelayMs(10);        //去抖
    if ((P3 & 0x0f) != 0x0f)
    {
      cord_h = P3 & 0x0f;  //读入列线值
      P3 = cord_h | 0xf0;  //输出当前列线值
      cord_l = P3 & 0xf0;  //读入行线值
      while ((P3 & 0xf0) != 0xf0);//等待松开并输出 
      return(cord_h + cord_l);//键盘最后组合码值
    }
  }
  return(0xff);  //返回该值
}

unsigned char KeyPro()
{
  switch (KeyScan())
  {
  case 0xee:return '1';break;//0 按下相应的键显示相对应的码值
  case 0xed:return '2';break;//1
  case 0xeb:return '3';break;//2
  case 0x7e:return '+';break;//3
  case 0xe7:return '4';break;//4
  case 0xde:return '5';break;//5
  case 0xdd:return '6';break;//6
  case 0x7d:return '-';break;//7
  case 0xdb:return '7';break;//8
  case 0xd7:return '8';break;//9
  case 0xbe:return '9';break;//a
  case 0x7b:return 'x';break;//b
  case 0xbd:return '0';break;//c
  case 0xbb:return '.';break;//d
  case 0xb7:return '=';break;//e
  case 0x77:return '/';break;//f
  default:return 0xff;break;
  }
}

#endif // !_KeyScan