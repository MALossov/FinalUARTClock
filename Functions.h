/*
 * @Description:
 * @Author: MALossov
 * @Date: 2022-04-18 21:23:56
 * @LastEditTime: 2022-04-19 01:30:43
 * @LastEditors: MALossov
 * @Reference:
 */

#ifndef _Functions
#define _Functions

#include <STC12C5A60S2.h>
#include "KeyScan.h"
#include "LCD1602.h"
#include "stdio.h"

extern void UartInit(void);
extern uchar ctrBit, ctrBitFlg;


void Begin_jsq()
{
   unsigned char num, i, sign;
   unsigned char temp[16];   //�������16��
   bit firstflag;
   float a = 0, b = 0;
   unsigned char s;

   Lcd1602_Write_Cmd(0x01); //����
   LcdShowStr(0, 0, "I LIKE MCU");    //д���һ����Ϣ����ѭ���в��ٸ��Ĵ���Ϣ��������while֮ǰд��
   LcdShowStr(0, 1, "QXMCU");    //д��ڶ�����Ϣ����ʾ��������
   for (s = 0;s < 25;s++)//��ʱ5s
      DelayMs(200);
   Lcd1602_Write_Cmd(0x01);


   while (0==ctrBitFlg)         //��ѭ��
   {

      num = KeyPro();  //ɨ�����
      if (num != 0xff)  //���ɨ���ǰ�����Чֵ����д���
      {
         if (i == 0)    //�����ǵ�һ���ַ���ʱ����Ҫ�Ѹ�����գ�����ۿ�
            Lcd1602_Write_Cmd(0x01);
         if (('+' == num) || (i == 16) || ('-' == num) || ('x' == num) || ('/' == num) || ('=' == num))//�����������ֵ16��������ű�ʾ�������
         {
            i = 0;  //��������λ
            if (firstflag == 0)  //���������ĵ�һ�����ݣ���ֵ��a�����ѱ�־λ��1������һ����������ʱ������ת��ֵ��b
            {
               sscanf(temp, "%f", &a);
               firstflag = 1;
            }
            else
               sscanf(temp, "%f", &b);
            for (s = 0;s < 16;s++) //��ֵ��ɺ�ѻ��������㣬��ֹ�´�����Ӱ����
               temp[s] = 0;
            DisplayOneChar(0, 1, num);
            ///////////////////////
            if (num != '=')      //�жϵ�ǰ����λ������Ӧ����
               sign = num;      //������ǵȺż��±�־λ
            else
            {
               firstflag = 0;   //��⵽����=�ţ��ж��ϴζ���ķ���
               switch (sign)
               {
               case '+':a = a + b;
                  break;
               case '-':a = a - b;
                  break;
               case 'x':a = a * b;
                  break;
               case '/':a = a / b;
                  break;
               default:break;
               }
               sprintf(temp, "%g", a);    //��������ͣ����õ�0�����
               LcdShowStr(1, 1, temp);//��ʾ��Һ����
               sign = 0;a = b = 0;            //�����������������
               for (s = 0;s < 16;s++)
                  temp[s] = 0;
            }
         }
         else if (i < 16)
         {
            if ((1 == i) && (temp[0] == '0'))//�����һ���ַ���0���ж��ڶ����ַ�
            {
               if (num == '.')  //�����С�������������룬���λ�ü�1
               {
                  temp[1] = '.';
                  DisplayOneChar(1, 0, num);//�������
                  i++;
               } //����û���ж�������С���㣬��0.0.0
               else
               {
                  temp[0] = num; //�����1-9���֣�˵��0û���ã���ֱ���滻��һλ0
                  DisplayOneChar(0, 0, num);//�������
               }
            }
            else
            {
               temp[i] = num;
               DisplayOneChar(i, 0, num);//�������
               i++;   //������ֵ�ۼ�
            }
         }

      }
   }
}


#endif // !_Functions