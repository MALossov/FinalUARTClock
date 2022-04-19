#ifndef __LCD_H_
#define __LCD_H_
#include <STC12C5A60S2.h>
/**********************************
����ͷ�ļ�
**********************************/

//typedef unsigned char INT8U;
//typedef unsigned char uchar;
//typedef unsigned char uint8;

//typedef unsigned int INT16U;
//typedef unsigned int uint;
#define uint unsigned int
#define INT16U unsigned int
#define INT8U unsigned char
#define uint8 unsigned char
#define uchar unsigned char
/**********************************
PIN�ڶ���
**********************************/
#define LCD1602_DB  P0      //data bus ��������
sbit LCD1602_RS = P3^5;
sbit LCD1602_RW = P3^6;
sbit LCD1602_EN = P3^4;	 

/**********************************
��������
**********************************/
void Read_Busy();           //æ��⺯�����ж�bit7��0������ִ�У�1��ֹ
void Lcd1602_Write_Cmd(unsigned char cmd);     //д����
void Lcd1602_Write_Data(unsigned char dat);   //д����
void LcdSetCursor(unsigned char x,unsigned char y);  //������ʾ
void DisplayOneChar (uchar X,uchar Y,uchar DData); // дһ���ֽ�
//��ʾһ���ֽ��ַ�
void DisplayOneStr (uchar X,uchar Y,uchar DData);
void LcdShowStr(unsigned char x,unsigned char y,unsigned char *str);     //��ʾ�ַ���
void InitLcd1602();              //1602��ʼ��
					  

#endif
