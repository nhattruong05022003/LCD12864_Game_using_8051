#ifndef __LCD12864_H
#define __LCD12864_H


#include<reg51.h>


#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif

#define _LCD_CLEAR				1   		
#define _LCD_RETURN_HOME		2
#define _LCD_ENTRY_MODE			6
#define _LCD_TURN_OFF			8
#define _LCD_TURN_ON			12
#define _LCD_CURSOR_OFF			12 	  
#define _LCD_UNDERLINE_ON		14
#define _LCD_BLINK_CURSOR_ON	15
#define _LCD_MOVE_CURSOR_LEFT	16
#define _LCD_MOVE_CURSOR_RIGHT	17
#define _LCD_SHIFT_LEFT			24
#define _LCD_SHIFT_RIGHT		28
#define _LCD_4BIT_1LINE_5x7FONT	0x20
#define _LCD_4BIT_2LINE_5x7FONT	0x28
#define _LCD_8BIT_1LINE_5x7FONT	0x30
#define _LCD_8BIT_2LINE_5x7FONT	0x38
#define _LCD_FIRST_ROW			128
#define _LCD_SECOND_ROW			192


#define LCD12864_PICTURE

#define LCD12864_DATAPORT P0	// thanh ghi P0 de giao tiep LCD  

sbit LCD12864_RS  =  P2^6; // chan chon thanh ghi: muc 0 -> lenh (instruction); muc 1 -> du lieu(data)           
sbit LCD12864_RW  =  P2^5; // chan ghi / doc : muc 0 -> ghi; muc 1 -> doc           
sbit LCD12864_EN  =  P2^7; // chan kich xung          
sbit LCD12864_PSB =  P3^2; // chan chon che do giao tiep : muc 0 (che do serial); muc 1 (che do truyen song song 8/4 bit)
sbit LCD12864_RST =  P3^4; // chan reset				   


void LCD12864_Delay1ms(uint c);
uchar LCD12864_Busy(void);
void LCD12864_WriteCmd(uchar cmd);
void LCD12864_WriteData(uchar dat);
void LCD12864_Init();
void LCD12864_ClearScreen(void);
void LCD12864_SetWindow(uchar x, uchar y);
void LCD12864_DrowPic(uchar *a);
void LCD12864_DrowPoint(uchar x, uchar y);

#endif
