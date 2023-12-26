#include"lcd12864.h"


// Ham Delay 1ms 
void LCD12864_Delay1ms(uint c) 
{
  uchar a,b;
	for(; c>0; c--)
	{
	    for(b=199; b>0; b--)
		{
	        for(a=1; a>0; a--);
		}
	}
}


// Ham kiem tra LCD co dang ban ko
uchar LCD12864_Busy(void)
{
	uchar i = 0;

	LCD12864_RS = 0; // chan RS = 0 -> giao tiep voi thanh ghi lenh
	LCD12864_RW = 1; // chan RW = 1 -> che do doc

	LCD12864_EN = 1; // chan EN len muc 1 
	LCD12864_Delay1ms(1); // delay LCD 1 ms

	while((LCD12864_DATAPORT & 0x80) == 0x80) // kiem tra bit thu 8 cua du lieu co = 1 ko
	{
		i++;
		if(i > 100)
		{
			LCD12864_EN = 0; // chan EN xuong muc 0 
			return 0; // tra ve 0 (LCD ban)
		}
	}

	LCD12864_EN = 0;// chan EN xuong muc 0

	return 1; // LCD ranh
}


void LCD12864_WriteCmd(uchar cmd)
{
	uchar i;
	i = 0;
	while( LCD12864_Busy() == 0) // Kiem tra LCD co ban ko
	{
		LCD12864_Delay1ms(1);
		i++;
		if( i>100)
		{
			return;	 // neu LCD ban thi thoat khoi ham
		}	
	}
	
	LCD12864_RS = 0; // chan RS = 0 -> giao tiep voi thanh ghi lenh
	LCD12864_RW = 0; // chan RW = 0 -> che do ghi    
	LCD12864_EN = 0; // chan EN xuong muc 0    

	LCD12864_DATAPORT = cmd; // thuc hien viec ghi lenh

	LCD12864_EN = 1; // chan EN len muc 1 de kich xung
	LCD12864_Delay1ms(1); // delay LCD 1ms
	LCD12864_EN = 0;  // chan EN xuong lai muc 0  					
}


// Ham gui du lieu (hien thi len LCD)
void LCD12864_WriteData(uchar dat)
{
	uchar i;
	i = 0;
	while( LCD12864_Busy() == 0) // Kiem tra LCD co ban ko
	{
		LCD12864_Delay1ms(1);
		i++;
		if( i>100)
		{
			return;	  
		}	
	}

	LCD12864_RS = 1; // chan RS = 1 -> giao tiep voi thanh ghi du lieu (hien thi len LCD)
	LCD12864_RW = 0; // chan RW = 0 -> che do ghi      
	LCD12864_EN = 0; // chan EN xuong muc 0     

	LCD12864_DATAPORT = dat;  // ghi du lieu len LCD

	LCD12864_EN = 1;	// chan EN len muc 1 de kich xung	   
	LCD12864_Delay1ms(1); // delay LCD 1ms
	LCD12864_EN = 0;  // chan EN xuong lai muc 0  							
}


#ifdef LCD12864_PICTURE

// Ham doc du lieu
uchar LCD12864_ReadData(void)
{
	uchar i, readValue;

	i = 0;
	while( LCD12864_Busy() == 0) // Kiem tra LCD co ban ko
	{
		LCD12864_Delay1ms(1);
		i++;
		if( i>100)
		{
			return 0;	   
		}	
	}

	LCD12864_RS = 1;   // chan RS = 1 -> giao tiep voi thanh ghi du lieu 
	LCD12864_RW = 1;   // chan RW = 1 -> che do doc  
	LCD12864_EN = 0;   // chan EN xuong muc 0
	LCD12864_Delay1ms(1);   // delay LCD 1ms

	LCD12864_EN = 1; // chan EN len muc 1 (kich xung)
	LCD12864_Delay1ms(1); // delay LCD 1ms
	readValue = LCD12864_DATAPORT; // Lay du lieu vua doc duoc luu vao bien readValue
	LCD12864_EN = 0; // chan EN xuong muc 0

	return readValue; // tra ve gia tri vua doc duoc
}

#endif


// Ham khoi tao
void LCD12864_Init()
{
	LCD12864_PSB = 1;	  // chon che do truyen song song 8/4 bit
	LCD12864_RST = 1;	  // Dua chan RST len muc 1 (ko reset)
	
	// gui lenh Function set 0x30 (0011 0000)-> chon che do giao tiep 8 bit
	LCD12864_WriteCmd(0x30); 
	// gui lenh Display control 0x0C (0000 1100) -> display on, cursor off (tat con tro), character blink off (ko chop ki tu)	
	LCD12864_WriteCmd(0x0c);
	// gui lenh Display clear 0x01 (0000 0001) -> xoa man hinh
	LCD12864_WriteCmd(0x01);
	// gui lenh Entry Mode set 0x06 (0000 0110) -> bit thu 2 = 1 -> khi ghi xong 1 ki tu con tro tu dong dich phai
	LCD12864_WriteCmd(0x06); 
}


#ifdef LCD12864_PICTURE

// Ham xoa man hinh
void LCD12864_ClearScreen(void)
{
	uchar i,j;
	
	// gui lenh Set Graphic RAM Address 0x34 (0011 0100) -> chon che do giao tiep 8 bit, bit thu 3 = 1 -> extended instruction
	LCD12864_WriteCmd(0x34);	  

	for(i=0;i<32;i++) 	  
	{
		LCD12864_WriteCmd(0x80+i); // Dia chi chieu doc GDRAM (xem datasheet)
		LCD12864_WriteCmd(0x80);	 // Dia chi chieu ngang GDRAM (xem datasheet)
		for(j=0;j<32;j++)		  // ghi 2 lan 16 (ngang)
		{						  
			LCD12864_WriteData(0x00);	  // ghi vao thanh ghi du lieu 0x00 (xoa cac gia tri truoc)
		}
	}
	
	// gui lenh Extended Function Set 0x36 (0011 0110) -> chon che do giao tiep 8 bit, bit thu 3 = 1 -> extended instruction
	// bit thu 2 -> graphic display ON
	LCD12864_WriteCmd(0x36);
	// gui lenh Function set 0x30 (0011 0000)-> chon che do giao tiep 8 bit, bit thu 3 = 1 -> basic instruction
	LCD12864_WriteCmd(0x30);	 
}

#endif


// Ham set vi tri cho con tro
void LCD12864_SetWindow(uchar x, uchar y)
{
	uchar pos; 

	if(x == 0)	   // Hang dau tien
	{
		x = 0x80; // Dia chi tung hang co trong datasheet
	}
		
	else if(x == 1)  // Hang thu hai
	{
		x = 0x90;	 // Dia chi tung hang co trong datasheet
	}
	
	else if(x == 2)  // Hang thu ba
	{
		x = 0x88; // Dia chi tung hang co trong datasheet
	}
	
	else if(x == 3) // Hang thu tu
	{
		x = 0x98; // Dia chi tung hang co trong datasheet
	}
	
	pos = x + y; // Gia tri cua vi tri = gia tri cua hang + cot (trong datasheet)
	LCD12864_WriteCmd(pos); // Gui lenh di chuyen con tro den vi tri da cai dat
}


#ifdef LCD12864_PICTURE

// Ham ve hinh
void LCD12864_DrowPic(uchar *a)	   
{

	unsigned char i,j;
	
	// Xoa man hinh
	LCD12864_ClearScreen();
	
	// gui lenh Set Graphic RAM Address 0x34 (0011 0100) -> chon che do giao tiep 8 bit, bit thu 3 = 1 -> extended instruction
	LCD12864_WriteCmd(0x34);    
	
	// Ve 1 nua ben tren
	for(i=0;i<32;i++)			  
	{
		LCD12864_WriteCmd(0x80+i);		  
		LCD12864_WriteCmd(0x80);		  
		for(j=0; j<16; j++)		  
		{						 
			LCD12864_WriteData(*a); 	      
			a++;
		}
	}
	// Ve 1 nua ben duoi
	for(i=0; i<32; i++)			  
	{
		
		LCD12864_WriteCmd(0x80+i); // Dia chi chieu doc GDRAM (xem datasheet)
		LCD12864_WriteCmd(0x88);	 // Dia chi chieu ngang GDRAM (xem datasheet)
		for(j=0; j<16; j++)		  
		{						  
			LCD12864_WriteData(*a); 	      
			a++;		
		}
	}	
	
	// gui lenh Extended Function Set 0x36 (0011 0110) -> chon che do giao tiep 8 bit, bit thu 3 = 1 -> extended instruction
	// bit thu 2 -> graphic display ON
	LCD12864_WriteCmd(0x36);	   
	// gui lenh Function set 0x30 (0011 0000)-> chon che do giao tiep 8 bit, bit thu 3 = 1 -> basic instruction
	LCD12864_WriteCmd(0x30);  	   
}


// Ham ve 1 diem anh tren LCD (x = 0~127; y = 0~63)
void LCD12864_DrowPoint(uchar x, uchar y)
{
	uint bt = 0, readValue = 0;
	uchar x_adr, y_adr, h_bit, l_bit;

	y_adr = 0x80 + y % 32;		
	// Lenh dieu khien Set Graphic RAM Address chi co the dieu chinh dc 32 gia tri cho chieu doc (xem datasheet)
	
	if(y>31) // Neu qua 31 thi xuong hang 2
	{
		x_adr = 0x88 + x / 16;
		// Lenh dieu khien Set Graphic RAM Address chi co the dieu chinh dc 16 gia tri cho chieu ngang (xem datasheet)
	}
	
	else // Ko thi hang dau tien
	{
		x_adr = 0x80 + x / 16;
	}
	
	bt = 0x8000 >> (x % 16); // Vi tri cua diem se duoc ghi vao
	
	// gui lenh Set Graphic RAM Address 0x34 (0011 0100) -> chon che do giao tiep 8 bit, bit thu 3 = 1 -> extended instruction
	LCD12864_WriteCmd(0x34);	  
	
	// Doc gia tri tai dia chi da thiet lap
	LCD12864_WriteCmd(y_adr);	  
	LCD12864_WriteCmd(x_adr);

  LCD12864_ReadData();		  //Dummy read
	readValue = LCD12864_ReadData();
	P1 = readValue;
	readValue <<= 8;
	readValue |= LCD12864_ReadData();


	bt = bt | readValue; // them 1 diem trong khi giu nguyen cac diem con lai
	h_bit = bt >> 8;
//	P1 = h_bit;
	l_bit = bt & 0x00FF;
//	P3 = l_bit;

	// Ghi gia tri vao vi tri da thiet lap
	LCD12864_WriteCmd(y_adr);	   
	LCD12864_WriteCmd(x_adr);
	LCD12864_WriteData(h_bit);
	LCD12864_WriteData(l_bit);
	
	// gui lenh Extended Function Set 0x36 (0011 0110) -> chon che do giao tiep 8 bit, bit thu 3 = 1 -> extended instruction
	// bit thu 2 -> graphic display ON
	LCD12864_WriteCmd(0x36);	   
	// gui lenh Function set 0x30 (0011 0000)-> chon che do giao tiep 8 bit, bit thu 3 = 0 -> basic instruction
	LCD12864_WriteCmd(0x30);	   
}
#endif
