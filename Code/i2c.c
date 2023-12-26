#include"i2c.h"


void Delay10us()
{
	unsigned char a,b;
	for(b=1;b>0;b--)
		for(a=2;a>0;a--);

}


void I2cStart()
{
	SDA=1;
	Delay10us();
	SCL=1;
	Delay10us();
	SDA=0;
	Delay10us();
	SCL=0;			
	Delay10us();		
}


void I2cStop()
{
	SDA=0;
	Delay10us();
	SCL=1;
	Delay10us();
	SDA=1;
	Delay10us();		
}


unsigned char I2cSendByte(unsigned char dat)
{
	unsigned char a=0,b=0;
	for(a=0;a<8;a++)
	{
		SDA=dat>>7;	 
		dat=dat<<1;
		Delay10us();
		SCL=1;
		Delay10us();
		SCL=0;
		Delay10us();		
	}
	SDA=1;
	Delay10us();
	SCL=1;
	while(SDA)
	{
		b++;
		if(b>200)	 
		{
			SCL=0;
			Delay10us();
			return 0;
		}
	}
	SCL=0;
	Delay10us();
 	return 1;		
}



unsigned char I2cReadByte()
{
	unsigned char a=0,dat=0;
	SDA=1;			
	Delay10us();
	for(a=0;a<8;a++)
	{
		SCL=1;
		Delay10us();
		dat<<=1;
		dat|=SDA;
		Delay10us();
		SCL=0;
		Delay10us();
	}
	return dat;		
}

//void I2cReadRespon()
//{
//	SDA=0;
//	Delay10us();
//	SDA=1;
//	Delay10us();
//}
