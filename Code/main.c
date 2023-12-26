#include<reg51.h>
#include<stdio.h>
#include<stdlib.h>
#include"lcd12864.h"
#include"i2c.h"

unsigned char pos1 = 16;
unsigned char pos2 = 16;
unsigned char pos3 = 16;
unsigned char pos4 = 16;
unsigned char count = 0;
unsigned char count1 = 0;
unsigned int score = 0;
unsigned char vitri = 1;
unsigned char dokho = 5;
bit start = 0;
bit menu = 1;
bit update = 1;
bit gameover = 0;
bit check_spam1 = 0;
bit check_spam2 = 0;
bit check_spam3 = 0;
bit check_spam4 = 0;
unsigned char hang1[] = {0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20};
unsigned char hang2[] = {0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20};
unsigned char hang3[] = {0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20};
unsigned char hang4[] = {0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20};
unsigned int random;
unsigned int temp;
unsigned int bxh[] = {0, 0, 0};
int i = 0;
int j = 0;
sbit NUT_LEN = P3^0;
sbit NUT_XUONG = P3^1;
sbit OK = P3^3;

void Delay10ms(unsigned int c);

void delay_ms(unsigned int t){
	for(i = 0; i < t; i++)
		for(j = 0; j < 113; j++);
}

void delay1MS ()
{
	TMOD = 0X01;	// timer 0, mode 1 :16bit
	TH0  = 0XFC; 	//8bit trong so cao
	TL0  = 0X18; 	//8bit trong do thap
	TR0  = 1;			// start timer 0
	//(FFFF+1 - FC18). 1us = 1ms
	while (TF0 == 0);
	TR0  =0;			//stop timer 0
	TF0  =0;			//clear flag
	// khi TF0==1 gan lai gia tri
}

unsigned char At24c02Read(unsigned char addr)
{
	unsigned char num = 0;
	I2cStart();
	I2cSendByte(0xa0);
	I2cSendByte(addr);
	I2cStart();
	I2cSendByte(0xa1);
	num = I2cReadByte();	
	I2cStop();
	return num;
}


void At24c02Write(unsigned char addr,unsigned char dat)
{
	I2cStart();
	I2cSendByte(0xa0);
	I2cSendByte(addr);
	I2cSendByte(dat);	
	I2cStop();
}


void LCD12864_WriteStr(unsigned char *s){
	i = 0;
	while(i < 16){
		LCD12864_WriteData(s[i]);
		i ++;
	}
}

void WriteString(unsigned char *str){
	i = 0;
	while(str[i] != 0){
		LCD12864_WriteData(str[i]);
		i ++;
	}
}
void SapXepBXH(void){
	temp = At24c02Read(0);
	Delay10ms(1);
	temp <<= 8;
	temp |= At24c02Read(1);
	Delay10ms(1);
	bxh[0] = temp;
	
	temp = At24c02Read(2);
	Delay10ms(1);
	temp <<= 8;
	temp |= At24c02Read(3);
	Delay10ms(1);
	bxh[1] = temp;
	
	temp = At24c02Read(4);
	Delay10ms(1);
	temp <<= 8;
	temp |= At24c02Read(5);
	Delay10ms(1);
	bxh[2] = temp;
	
	for(i = 0; i < 3; i++){
		if(bxh[i] > 9999){
			bxh[i] = 0;
		}
	}
	for(i = 0; i < 3; i++){
		for(j = i+1; j < 3; j++){
			if(bxh[i] < bxh[j]){
				temp = bxh[i];
				bxh[i] = bxh[j];
				bxh[j] = temp;
			}
		}
	}
}

void EndGame(void){
	unsigned char *str = "Game over ";
	EA = 0;
	SapXepBXH();
	LCD12864_WriteCmd(0x01);
	LCD12864_SetWindow(0, 2);
	WriteString(str);
	LCD12864_SetWindow(1, 3);
	LCD12864_WriteData(score / 100 + 0x30);
	LCD12864_WriteData(score / 10 % 10 + 0x30);
	LCD12864_WriteData(score % 10 + 0x30);
	LCD12864_WriteData(' ');
	start = 0;
	menu = 1;
	if(score > bxh[0]){
		P1 = score;
		temp = bxh[0];
		bxh[0] = score;
		At24c02Write(0, bxh[0] >> 8);
		Delay10ms(2);
		At24c02Write(1, bxh[0] & 0xFF);
		Delay10ms(2);
		score = bxh[1];
		bxh[1] = temp;
		At24c02Write(2, bxh[1] >> 8);
		Delay10ms(2);
		At24c02Write(3, bxh[1] & 0xFF);
		Delay10ms(2);
		bxh[2] = score;
		At24c02Write(4, bxh[2] >> 8);
		Delay10ms(2);
		At24c02Write(5, bxh[2] & 0xFF);
	}
	else if(score > bxh[1]){
		temp = bxh[1];
		bxh[1] = score;
		At24c02Write(2, bxh[1] >> 8);
		Delay10ms(2);
		At24c02Write(3, bxh[1] & 0xFF);
		Delay10ms(2);
		bxh[2] = temp;
		At24c02Write(4, bxh[2] >> 8);
		Delay10ms(2);
		At24c02Write(5, bxh[2] & 0xFF);
	}
	else if(score > bxh[2]){
		bxh[2] = score;
		At24c02Write(4, bxh[2] >> 8);
		Delay10ms(2);
		At24c02Write(5, bxh[2] & 0xFF);
	}
	check_spam1 = 0;
	check_spam2 = 0;
	check_spam3 = 0;
	check_spam4 = 0;
	i = 0;
	while(i < 16){
		hang1[i] = 0x20;
		hang2[i] = 0x20;
		hang3[i] = 0x20;
		hang4[i] = 0x20;
		i ++;
	}
	pos1 = 16;
	pos2 = 16;
	pos3 = 16;
	pos4 = 16;
	score = 0;
	gameover = 0;
	count = 0;
	count1 = 0;
	while(1){
		if(OK == 0){
			delay_ms(20);
			while(OK == 0);
			LCD12864_WriteCmd(0x01);
			return;
		}
	}
}


void btn_setup ()
{
	// EX0 = 1; // ngat ngoai INT0
	EX1 = 1; // ngat ngoai INT1
	EA = 1;
}


void Len(void){
	if(vitri == 4){
		hang4[0] = ' ';
		LCD12864_SetWindow(3, 0);
		LCD12864_WriteStr(hang4);
	}
	else if(vitri == 3){
		hang3[0] = ' ';
		LCD12864_SetWindow(2, 0);
		LCD12864_WriteStr(hang3);
	}
	else if(vitri == 2){
		hang2[0] = ' ';
		LCD12864_SetWindow(1, 0);
		LCD12864_WriteStr(hang2);
	}
	else{
		hang1[0] = ' ';
		LCD12864_SetWindow(0, 0);
		LCD12864_WriteStr(hang1);
	}
	vitri = vitri == 0 ? 0 : vitri - 1;
	if(vitri == 4){
		hang4[0] = 'I';
		LCD12864_SetWindow(3, 0);
		LCD12864_WriteStr(hang4);
	}
	else if(vitri == 3){
		hang3[0] = 'I';
		LCD12864_SetWindow(2, 0);
		LCD12864_WriteStr(hang3);
	}
	else if(vitri == 2){
		hang2[0] = 'I';
		LCD12864_SetWindow(1, 0);
		LCD12864_WriteStr(hang2);
	}
	else{
		hang1[0] = 'I';
		LCD12864_SetWindow(0, 0);
		LCD12864_WriteStr(hang1);
	}
	if((vitri == 1 && (pos1 == 16 || pos1 == 0) && check_spam1) || (vitri == 2 && (pos2 == 16 || pos2 == 0) && check_spam2) || (vitri == 3 && (pos3 == 16 || pos3 == 0) && check_spam3) || (vitri == 4 && (pos4 == 16 || pos4 == 0) && check_spam4)){
		gameover = 1;
		score --;
	}
		
	update = 1;
}

void Xuong(void){
	if(vitri == 4){
		hang4[0] = ' ';
		LCD12864_SetWindow(3, 0);
		LCD12864_WriteStr(hang4);
	}
	else if(vitri == 3){
		hang3[0] = ' ';
		LCD12864_SetWindow(2, 0);
		LCD12864_WriteStr(hang3);
	}
	else if(vitri == 2){
		hang2[0] = ' ';
		LCD12864_SetWindow(1, 0);
		LCD12864_WriteStr(hang2);
	}
	else{
		hang1[0] = ' ';
		LCD12864_SetWindow(0, 0);
		LCD12864_WriteStr(hang1);
	}
	vitri = vitri == 4 ? 4 : vitri + 1;
	if(vitri == 4){
		hang4[0] = 'I';
		LCD12864_SetWindow(3, 0);
		LCD12864_WriteStr(hang4);
	}
	else if(vitri == 3){
		hang3[0] = 'I';
		LCD12864_SetWindow(2, 0);
		LCD12864_WriteStr(hang3);
	}
	else if(vitri == 2){
		hang2[0] = 'I';
		LCD12864_SetWindow(1, 0);
		LCD12864_WriteStr(hang2);
	}
	else{
		hang1[0] = 'I';
		LCD12864_SetWindow(0, 0);
		LCD12864_WriteStr(hang1);
	}
	if((vitri == 1 && (pos1 == 16 || pos1 == 0) && check_spam1) || (vitri == 2 && (pos2 == 16 || pos2 == 0) && check_spam2) || (vitri == 3 && (pos3 == 16 || pos3 == 0) && check_spam3) || (vitri == 4 && (pos4 == 16 || pos4 == 0) && check_spam4)){
		gameover = 1;
		score --;
	}
	update = 1;
}

void SpamObstacle(void){
	random = rand() % 4;
	if(random == 0 && !check_spam1){
		hang1[15] = 0x06;
		pos1 = 15;
		check_spam1 = 1;
	}
	else if(random == 1 && !check_spam2){
		hang2[15] = 0x06;
		pos2 = 15;
		check_spam2 = 1;
	}
	else if(random == 2 && !check_spam3){
		hang3[15] = 0x06;
		pos3 = 15;
		check_spam3 = 1;
	}
	else if(random == 3 && !check_spam4){
		hang4[15] = 0x06;
		pos4 = 15;
		check_spam4 = 1;
	}
	if(vitri == 4){
		hang4[0] = 'I';
		LCD12864_SetWindow(3, 0);
		LCD12864_WriteStr(hang4);
	}
	else if(vitri == 3){
		hang3[0] = 'I';
		LCD12864_SetWindow(2, 0);
		LCD12864_WriteStr(hang3);
	}
	else if(vitri == 2){
		hang2[0] = 'I';
		LCD12864_SetWindow(1, 0);
		LCD12864_WriteStr(hang2);
	}
	else{
		hang1[0] = 'I';
		LCD12864_SetWindow(0, 0);
		LCD12864_WriteStr(hang1);
	}
	update = 1;
}

void MoveObstacle(void){
	hang1[pos1] = 0x20;
	hang2[pos2] = 0x20;
	hang3[pos3] = 0x20;
	hang4[pos4] = 0x20;
	check_spam1 = (pos1 == 0 || pos1 > 15) ? 0 : 1;
	check_spam2 = (pos2 == 0 || pos2 > 15) ? 0 : 1;
	check_spam3 = (pos3 == 0 || pos3 > 15) ? 0 : 1;
	check_spam4 = (pos4 == 0 || pos4 > 15) ? 0 : 1;
	if(check_spam1){
		pos1 = pos1 == 0 ? 16 : pos1 - 1;
		if(pos1 == 0 && vitri == 1)
			gameover = 1;
		else if(pos1 == 0)
			score ++;
		hang1[pos1] = 0x06;
	}
	else{
		pos1 = 16;
	}
	if(check_spam2){
		pos2 = pos2 == 0 ? 16 : pos2 - 1;
		if(pos2 == 0 && vitri == 2)
			gameover = 1;
		else if(pos2 == 0)
			score ++;
		hang2[pos2] = 0x06;
	}
	else{
		pos2 = 16;
	}
	if(check_spam3){
		pos3 = pos3 == 0 ? 16 : pos3 - 1;
		if(pos3 == 0 && vitri == 3)
			gameover = 1;
		else if(pos3 == 0)
			score ++;
		hang3[pos3] = 0x06;
	}
	else{
		pos3 = 16;
	}
	if(check_spam4){
		pos4 = pos4 == 0 ? 16 : pos4 - 1;
		if(pos4 == 0 && vitri == 4)
			gameover = 1;
		else if(pos4 == 0)
			score ++;
		hang4[pos4] = 0x06;
	}
	else{
		pos4 = 16;
	}
	if(vitri == 4 && gameover != 1){
		hang4[0] = 'I';
		LCD12864_SetWindow(3, 0);
		LCD12864_WriteStr(hang4);
	}
	else if(vitri == 3 && gameover != 1){
		hang3[0] = 'I';
		LCD12864_SetWindow(2, 0);
		LCD12864_WriteStr(hang3);
	}
	else if(vitri == 2 && gameover != 1){
		hang2[0] = 'I';
		LCD12864_SetWindow(1, 0);
		LCD12864_WriteStr(hang2);
	}
	else if(vitri == 1 && gameover != 1){
		hang1[0] = 'I';
		LCD12864_SetWindow(0, 0);
		LCD12864_WriteStr(hang1);
	}
	LCD12864_SetWindow(0, 0);
	LCD12864_WriteStr(hang1);
	LCD12864_SetWindow(1, 0);
	LCD12864_WriteStr(hang2);
	LCD12864_SetWindow(2, 0);
	LCD12864_WriteStr(hang3);
	LCD12864_SetWindow(3, 0);
	LCD12864_WriteStr(hang4);
	update = 1;
	count ++;
}
void DieuKhien (void) interrupt 2  
{
	Delay10ms(2);
	while(OK == 0);
	if(NUT_LEN == 0){
		Delay10ms(2);
		Len();
	}
	if(NUT_XUONG == 0){
		Delay10ms(2);
		Xuong();
	}
}

void DisplayDoKho(void){
	LCD12864_WriteCmd(0x01);
	LCD12864_SetWindow(0, 2);
	WriteString("Do kho");
	LCD12864_SetWindow(2, 1);
	if(dokho == 1){
		WriteString("<   Kho");
	}
	else if(dokho == 5){
		WriteString("< Thuong >");
	}
	else{
		WriteString("    De   >");
	}
	while(1){
		if(NUT_LEN == 0){
			Delay10ms(2);
			while(NUT_LEN == 0);
			dokho = dokho == 1 ? 1 : dokho - 4;
			LCD12864_WriteCmd(0x01);
			LCD12864_SetWindow(0, 2);
			WriteString("Do kho");
			LCD12864_SetWindow(2, 1);
			if(dokho == 1){
				WriteString("<   Kho");
			}
			else if(dokho == 5){
				WriteString("< Thuong >");
			}
			else{
				WriteString("    De   >");
			}
		}
		if(NUT_XUONG == 0){
			Delay10ms(2);
			while(NUT_XUONG == 0);
			dokho = dokho == 9 ? 9 : dokho + 4;
			LCD12864_WriteCmd(0x01);
			LCD12864_SetWindow(0, 2);
			WriteString("Do kho");
			LCD12864_SetWindow(2, 1);
			if(dokho == 1){
				WriteString("<   Kho");
			}
			else if(dokho == 5){
				WriteString("< Thuong >");
			}
			else{
				WriteString("    De   >");
			}
		}
		if(OK == 0){
			LCD12864_WriteCmd(0x01);
			menu = 1;
			return;
		}
	}
}



void DisplayBXH(void){
	SapXepBXH();
	LCD12864_WriteCmd(0x01);
	LCD12864_SetWindow(0, 0);
	WriteString("Bang xep hang");
	LCD12864_SetWindow(1, 0);
	WriteString("1.");
	LCD12864_WriteData(bxh[0] / 100 + 0x30);
	LCD12864_WriteData(bxh[0] / 10 % 10 + 0x30);
	LCD12864_WriteData(bxh[0] % 10 + 0x30);
	LCD12864_WriteData(' ');
	LCD12864_SetWindow(2, 0);
	WriteString("2.");
	LCD12864_WriteData(bxh[1] / 100 + 0x30);
	LCD12864_WriteData(bxh[1] / 10 % 10 + 0x30);
	LCD12864_WriteData(bxh[1] % 10 + 0x30);
	LCD12864_WriteData(' ');
	LCD12864_SetWindow(3, 0);
	WriteString("3.");
	LCD12864_WriteData(bxh[2] / 100 + 0x30);
	LCD12864_WriteData(bxh[2] / 10 % 10 + 0x30);
	LCD12864_WriteData(bxh[2] % 10 + 0x30);
	LCD12864_WriteData(' ');
	while(1){
		if(OK == 0){
			LCD12864_WriteCmd(0x01);
			menu = 1;
			return;
		}
	}
}

void DisplayMenu(void){
	EA = 0;
	// LCD12864_WriteCmd(0x01);
	if(menu == 1){
		vitri = 1;
		menu = 0;
	}
	LCD12864_SetWindow(0, 3);
	WriteString("Menu");
	LCD12864_SetWindow(1, 1);
	WriteString("Bat dau");
	LCD12864_SetWindow(2, 1);
	WriteString("BXH");
	LCD12864_SetWindow(3, 1);
	WriteString("Do kho");
	LCD12864_SetWindow(vitri, 0);
	LCD12864_WriteData('>');

	if(NUT_LEN == 0){
		Delay10ms(2);
		while(NUT_LEN == 0);
		LCD12864_SetWindow(vitri, 0);
		LCD12864_WriteData(' ');
		vitri = vitri == 1 ? 1 : vitri - 1;
		LCD12864_SetWindow(vitri, 0);
		LCD12864_WriteData('>');
	}
	if(NUT_XUONG == 0){
		Delay10ms(2);
		while(NUT_XUONG == 0);
		LCD12864_SetWindow(vitri, 0);
		LCD12864_WriteData(' ');
		vitri = vitri == 3 ? 3 : vitri + 1;
		LCD12864_SetWindow(vitri, 0);
		LCD12864_WriteData('>');
	}
	if(OK == 0){
		Delay10ms(2);
		while(OK == 0);
		if(vitri == 1){
			start = 1;
			EA = 1;
			return;
		}
		else if(vitri == 2){
			DisplayBXH();
		}
		else{
			DisplayDoKho();
			P1 = dokho;
		}
	}
}



void main()
{
	LCD12864_Init();
  btn_setup();
	SapXepBXH();
	OK = 1;
	NUT_LEN = 1;
	NUT_XUONG = 1;
//	At24c02Write(0, 0xFF);
//	At24c02Write(1, 0xFF);
//	At24c02Write(2, 0xFF);
//	At24c02Write(3, 0xFF);
//	At24c02Write(4, 0xFF);
//	At24c02Write(5, 0xFF);
	while(1)
	{
		if(start == 0){
			DisplayMenu();
			if(start == 1){
				SpamObstacle();
			}
		}
		else{
			if(update){
				if(gameover){
					EndGame();
				}
				else{
					if(vitri == 4){
						hang4[0] = 'I';
					}
					else if(vitri == 3){
						hang3[0] = 'I';
					}
					else if(vitri == 2){
						hang2[0] = 'I';
					}
					else{
						hang1[0] = 'I';
					}

					LCD12864_SetWindow(0, 0);
					LCD12864_WriteStr(hang1);
					LCD12864_SetWindow(1, 0);
					LCD12864_WriteStr(hang2);
					LCD12864_SetWindow(2, 0);
					LCD12864_WriteStr(hang3);
					LCD12864_SetWindow(3, 0);
					LCD12864_WriteStr(hang4);
				}
				update = 0;
			}		
			if(count >= 5 && gameover != 1){
				SpamObstacle();
				count = 0;
			}
			if(count1 >= dokho && gameover != 1){
				MoveObstacle();
				count1 = 0;
			}
			count1 ++;
			delay_ms(50);
			// delay1MS();
		}
	}
}


void Delay10ms(unsigned int c)   
{
    unsigned char a, b;

    for (;c>0;c--)
		{
			for (b=38;b>0;b--)
			{
				for (a=130;a>0;a--);
			}      
		}
        
}