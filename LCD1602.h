#ifndef __LCD_H_
#define __LCD_H_
#include <STC12C5A60S2.h>
/**********************************
包含头文件
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
PIN口定义
**********************************/
#define LCD1602_DB  P0      //data bus 数据总线
sbit LCD1602_RS = P3^5;
sbit LCD1602_RW = P3^6;
sbit LCD1602_EN = P3^4;	 

/**********************************
函数声明
**********************************/
void Read_Busy();           //忙检测函数，判断bit7是0，允许执行；1禁止
void Lcd1602_Write_Cmd(unsigned char cmd);     //写命令
void Lcd1602_Write_Data(unsigned char dat);   //写数据
void LcdSetCursor(unsigned char x,unsigned char y);  //坐标显示
void DisplayOneChar (uchar X,uchar Y,uchar DData); // 写一个字节
//显示一个字节字符
void DisplayOneStr (uchar X,uchar Y,uchar DData);
void LcdShowStr(unsigned char x,unsigned char y,unsigned char *str);     //显示字符串
void InitLcd1602();              //1602初始化
					  

#endif
