/*
 * @Description:
 * @Author: MALossov
 * @Date: 2022-04-12 21:21:27
 * @LastEditTime: 2022-04-19 01:28:27
 * @LastEditors: MALossov
 * @Reference:
 */

#include "KeyScan.h"
#include "Uart.h"
#include "LCD1602.h"
#include "Functions.h"
#include "Timer.h"
#include "DS1302.h"


uchar ctrBit, ctrBitFlg;

void main() {
    led = 1;
    ctrBitFlg = 0;

    UartInit();
    InitLcd1602(); //初始化液晶屏
    DelayMs(10);//延时用于稳定，可以去掉

    PutChar('H');
    prints("ello World!\r\n");
		
    Begin_Clock();

    while (1)
    {
        if (ctrBitFlg) {
					ctrBitFlg=0;
            switch (ctrBit)
            {
            case '0':
                Begin_Clock();
                break;
            case '1':
                Begin_jsq();
                break;
            case '2':
                Begin_MB();
                break;
            }
        }
        prints("Mode Quited!");
    }

}