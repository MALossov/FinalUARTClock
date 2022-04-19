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


unsigned char KeyScan()  //����ɨ�躯����ʹ�����з�תɨ�跨
{
  unsigned char cord_h, cord_l;//����ֵ�м����
  P3 = 0x0f;            //�������ȫΪ0
  cord_h = P3 & 0x0f;     //��������ֵ
  if (cord_h != 0x0f)    //�ȼ�����ް�������
  {
    DelayMs(10);        //ȥ��
    if ((P3 & 0x0f) != 0x0f)
    {
      cord_h = P3 & 0x0f;  //��������ֵ
      P3 = cord_h | 0xf0;  //�����ǰ����ֵ
      cord_l = P3 & 0xf0;  //��������ֵ
      while ((P3 & 0xf0) != 0xf0);//�ȴ��ɿ������ 
      return(cord_h + cord_l);//������������ֵ
    }
  }
  return(0xff);  //���ظ�ֵ
}

unsigned char KeyPro()
{
  switch (KeyScan())
  {
  case 0xee:return '1';break;//0 ������Ӧ�ļ���ʾ���Ӧ����ֵ
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