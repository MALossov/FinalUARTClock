#ifndef _DS1302
#define _DS1302


#include <STC12C5A60S2.h>
/*********************************************************************************
* 【编写时间】： 2016年02月15日
* 【作    者】： 清翔电子:03
* 【版    本】： 1.0
* 【网    站】：xmcu.com/ 
* 【淘宝店铺】：u.taobao.com/ 
* 【实验平台】： QX-MCS51 单片机开发板
* 【外部晶振】 11.0592mhz	
* 【主控芯片】： STC89C52
* 【编译环境】： Keil μVisio3	
* 【  
* 【使用说明】： 试验前请用杜邦线分别把j5排针根据背面丝印相应接到P10、P11、P12
                 并接上LCD1602液晶调节好对比度再下载此程序

*  说明：免费开源，不提供源代码分析.
**********************************************************************************/

//调用1602显示驱动，在此文件内已包含51头文件和定义数据类型
#include "LCD1602.h"
#include "intrins.h"

extern void PutChar(char ch);

sbit keyS4 = P3 ^ 2;//独立按键S4
/*====================================
 IO接口定义
====================================*/
sbit TSCLK = P1 ^ 0; //时钟
sbit TIO = P1 ^ 1;   //数据
sbit TRST = P1 ^ 2;  //使能




#define MAIN_Fosc		11059200UL	//宏定义主时钟HZ
#define DS1302_W_ADDR 0x80  //写时钟日历寄存器起始地址
#define DS1302_R_ADDR 0x81	//读时钟日历寄存器起始地址

//时钟日历暂存数组，秒、分、时、日、月、周、年 初值为= 16年2月15日 周1 23:58:50
uchar TimeData[7] = { 11, 45, 14, 18, 4, 1, 22 };
extern uchar ctrBit, ctrBitFlg;
uchar yearCngFlg, timeCngFlg;
extern void UartInit(void);

/*=================================================
*函数名称：Delay_Ms
*函数功能：12T 51单片机自适应主时钟毫秒级延时函数
*说明：需定义MAIN_Fosc（晶振主频单位HZ）
*输入：ms：要延时的毫秒
=================================================*/
void Delay_Ms(unsigned char delay)		//@11.0592MHz
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
/*=================================================
*函数名称：DS1302_W_Byte
*函数功能：DS1302写一字节数据
*输入：dat：要写入的数据
=================================================*/
void DS1302_W_Byte(uchar dat)
{
    uchar i;
    for (i = 0; i < 8; i++) //每次写1bit，写8次
    {
        TSCLK = 0;		   //拉低时钟总线
        TIO = dat & 0x01;  //从一字节最低位开始写
        dat >>= 1;		   //数据右移一位 
        TSCLK = 1;		   //拉高时钟总线，DS1302把数据读走
        // dat >>= 1;		   //数据右移一位 
    }
}
/*=================================================
*函数名称：DS1302_R_Byte
*函数功能：DS1302读一字节
*输出：dat：读取的数据
=================================================*/
uchar DS1302_R_Byte()
{
    uchar i, dat;
    for (i = 0; i < 8; i++)  //每次写1bit，写8次
    {
        TSCLK = 0;			//拉低时钟总线，DS1302把数据放到数据总线上
        dat >>= 1; 			//数据右移一位，数据从最低位开始读 
        if (TIO)	dat |= 0x80;//读取数据
        TSCLK = 1;			//拉高时钟总线
    }
    return dat;				//返回读取的数据
}
/*=================================================
*函数名称：DS1302_W_DAT
*函数功能：写DS1302数据一次写2个字节
*说明：先写命令后写数据
*调用：DS1302_W_Byte()
*输入：cmd：需要写的命令 ，dat：需要些的数据
=================================================*/
void DS1302_W_DAT(uchar cmd, uchar dat)
{
    TRST = 0;			_nop_(); //拉低使能端
    TSCLK = 0;			_nop_(); //拉低数据总线
    TRST = 1;			_nop_(); //拉高使能端，开始写数据
    DS1302_W_Byte(cmd);	 //写命令
    DS1302_W_Byte(dat);	 //写数据
}
/*=================================================
*函数名称：DS1302_R_DAT
*函数功能：读DS1302数据
*说明：先写入命令字节后读出对应数据
*调用：	DS1302_W_Byte();DS1302_R_Byte();
*输入：	cmd：需要写的命令
*输出：	dat：读出的数据
=================================================*/
uchar DS1302_R_DAT(uchar cmd)
{
    uchar dat;
    TRST = 0;			 	//拉低使能端
    TSCLK = 0;				//拉低数据总线
    TRST = 1;				//拉高使能端，开始写数据
    DS1302_W_Byte(cmd);		//写命令
    dat = DS1302_R_Byte();	//读出数据
    return dat;				//返回读出数据
}

/*=================================================
*函数名称：DS1302_Clear_WP
*函数功能：清除DS1302写保护
*说明：先写入命令0x8e（写控制寄存器）接着向该寄存器写0
*调用：DS1302_W_DAT()
=================================================*/
void DS1302_Clear_WP()
{
    DS1302_W_DAT(0x8e, 0x00);  //把控制寄存器WP位置0
}
/*=================================================
*函数名称：DS1302_Clear_WP
*函数功能：设置DS1302写保护
*说明：先写入命令0x8e（写控制寄存器）接着向该寄存器写0x80
*调用：DS1302_W_DAT()
=================================================*/
void DS1302_Set_WP()
{
    DS1302_W_DAT(0x8e, 0x80); //把控制寄存器WP位置1
    TRST = 0;				 //拉低使能端
    TSCLK = 0;				 //拉低数据总线
}
/*=================================================
*函数名称：Set_DS1302_Time
*函数功能：设置DS1302时钟日历数据
*说明：把时钟日历暂存数组TimeData数据转换为BCD码并
       写入到DS1302时钟日历寄存器中
*调用：DS1302_Clear_WP();DS1302_W_DAT();DS1302_Set_WP();
*输入：addr:需要写入寄存器的地址 ，TimeData数组：时钟日历初始值
=================================================*/
void Set_DS1302_Time(uchar addr)
{
    uchar i, j;
    DS1302_Clear_WP();		//清除写保护
    for (i = 0; i < 7; i++)	//写入7个字节的时钟初始值
    {
        j = TimeData[i] / 10;	 //BCD码转换
        TimeData[i] %= 10;	 //BCD码转换
        TimeData[i] += j * 16; //BCD码转换
        DS1302_W_DAT(addr, TimeData[i]); //先写DS1302时钟日历起始地址，再写数据
        addr += 2;	 //时钟日历寄存器地址+2转向下一个寄存器
    }
    DS1302_Set_WP(); //开起写保护		
}

void set_sfm(unsigned char shi, unsigned char fen, unsigned char miao)
{
    DS1302_W_DAT(0x8e, 0x00);//关闭保护位
    DS1302_W_DAT(0x80, (miao / 10) * 16 + miao % 10);//设置秒
    DS1302_W_DAT(0x82, (fen / 10) * 16 + fen % 10);//设置分
    DS1302_W_DAT(0x84, (shi / 10) * 16 + shi % 10);//设置小时
    DS1302_W_DAT(0x8e, 0x80);//打开保护位
}


void set_sfm_BCD(unsigned char shi, unsigned char fen, unsigned char miao)
{
    DS1302_W_DAT(0x8e, 0x00);//关闭保护位
    DS1302_W_DAT(0x80, miao);//设置秒
    DS1302_W_DAT(0x82, fen);//设置分
    DS1302_W_DAT(0x84, shi);//设置小时
    DS1302_W_DAT(0x8e, 0x80);//打开保护位
}

void set_year_BCD(uchar d, uchar m, uchar w, uchar y)
{
    DS1302_W_DAT(0x8e, 0x00);//关闭保护位
    DS1302_W_DAT(0x86, d);//设置秒
    DS1302_W_DAT(0x88, m);//设置分
    DS1302_W_DAT(0x8A, w);//设置小时
    DS1302_W_DAT(0x8C, y);
    DS1302_W_DAT(0x8e, 0x80);//打开保护位
}
/*=================================================
*函数名称：Read_DS1302_Time
*函数功能：读取DS1302时钟数据
*说明：	读取DS1302时钟数据 返回数据存入时钟日历暂存
        数组TimeData（数据格式BCD码）
*调用：DS1302_Clear_WP();DS1302_R_DAT();DS1302_Set_WP();
*输入：	addr：需要读取时钟日历寄存器的起始地址
=================================================*/
void Read_DS1302_Time(uchar addr)
{
    uchar i;
    DS1302_Clear_WP();    	//清楚些保护
    for (i = 0; i < 7; i++)	//从DS1302读取7个字节的时钟日历数据
    {
        TimeData[i] = DS1302_R_DAT(addr);//先写入要读取数据的寄存器起始地址，再读出数据存入TimeData数组
        addr += 2;						 //时钟日历寄存器地址+2转向下一个寄存器
    }
    DS1302_Set_WP();   //开起写保护
}

void Begin_Clock() {

    Lcd1602_Write_Cmd(0x01); //清屏
    Set_DS1302_Time(DS1302_W_ADDR);	//先写入时钟日历寄存器起始地址再设置时钟日历初值
    LcdShowStr(0, 0, "Date:");  //1602显示字符串
    LcdShowStr(0, 1, "Time:");  //1602显示字符串
    while (ctrBitFlg == 0)
    {
        if (timeCngFlg) {
            timeCngFlg = 0;
            set_sfm_BCD(TimeData[2], TimeData[1], TimeData[0]);
            //PutChar(TimeData[1]);
        }
        if (yearCngFlg) {
            yearCngFlg = 0;
            set_year_BCD(TimeData[3], TimeData[4], TimeData[5],TimeData[6]);
            //PutChar(TimeData[1]);
        }
        Read_DS1302_Time(DS1302_R_ADDR); //先写入时钟日历寄存器起始地址再读出时钟日历写入到TimeData数组中
        DisplayOneStr(5, 0, TimeData[6]); //年
        DisplayOneChar(7, 0, '-');		 //-
        DisplayOneStr(8, 0, TimeData[4]); //月
        DisplayOneChar(10, 0, '-');		 //-
        DisplayOneStr(11, 0, TimeData[3]);//日
        DisplayOneChar(13, 0, '-');		 //-
        DisplayOneStr(14, 0, TimeData[5]);//星期

        DisplayOneStr(6, 1, TimeData[2]); //时
        DisplayOneChar(8, 1, ':');		 //：
        DisplayOneStr(9, 1, TimeData[1]); //分
        DisplayOneChar(11, 1, ':');		 //：
        DisplayOneStr(12, 1, TimeData[0]);//秒
        Delay_Ms(1000);					 //延时1秒

        if (keyS4 == 0)//判断S3是否被按下，清零
        {
            DelayMs(20);//按键消抖
            if (keyS4 == 0)
            {
                set_sfm_BCD(0X11, 0X45, 0X14);
                while (!keyS4);//松手检测
            }
        }
    }
}

#endif // !_DS1302