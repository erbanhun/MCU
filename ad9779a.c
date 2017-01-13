#include "C8051F340.h"
#define uchar unsigned char
#define uint  unsigned int
#define ulong unsigned long

sbit DAC_CS1    = P3^0;		//output
sbit DAC_CS2    = P3^1;		//output
sbit DAC_CS3    = P3^2;		//output
sbit DAC_CS4    = P3^3;		//output
sbit DAC_SCLK   = P3^4; 	//output 
sbit DAC_SDIO	= P3^5;		//output
sbit DAC_SDO	= P3^6;		//input

extern void delay(uint cnt);			 // 延时
void DA_SPI(uchar add, uchar num);	 // write DA register
void ini9779(uchar number);			 // 初始化DAC,0:power down;1:normal
uchar read_DA(uchar num,uchar add);		 //read DAC
/*---------------------------------------
purpose:初始化DAC
parameters:  0:power down;1:normal
modify:
----------------------------------------*/
void ini9779(uchar number)
{
	delay(5);
    //-------da set-------//
	if(number==0)
		DA_SPI(0x00,0x10);			//power down DA
	else
	{
		DA_SPI(0x00,0x00);			//DA normal
		DA_SPI(0x02,0x88);			//unsigned binary  88

 		DA_SPI(0x01,0xc0); 			//Interpolation filter *8
		DA_SPI(0x08,0x17);
		DA_SPI(0x09,0xd2);
		DA_SPI(0x0a,0x0f);

		DA_SPI(0x0b,0xb5);
		DA_SPI(0x0c,0x03);
		DA_SPI(0x0f,0xb5);
		DA_SPI(0x10,0x03);
	}
}

/*---------------------------------------
purpose:在DAC指定的地址写寄存器的值
parameters:
modify:
----------------------------------------*/
void DA_SPI(uchar add, uchar num)
{	
	idata uchar i=0;
	idata uint  reg=0;

	DAC_SDIO = 0;
	DAC_CS1=1;
	DAC_CS2=1;
	DAC_CS3=1;
	DAC_CS4=1;
	reg=add;
	reg=reg<<8;
	reg=reg+num;
	DAC_CS1=0;
	DAC_CS2=0;
	DAC_CS3=0;
	DAC_CS4=0;
	do
	{
		if((reg&0x8000) == 0x8000)
			DAC_SDIO = 1;
		else
			DAC_SDIO = 0;
		DAC_SCLK = 0;
		delay(10);						//最小50ns
		DAC_SCLK = 1;
		delay(10);
		DAC_SCLK = 0;
		reg=reg<<1;
		i++;
	}while(i<16);
	DAC_CS1=1;
	DAC_CS2=1;
	DAC_CS3=1;
	DAC_CS4=1;
}
/*---------------------------------------
purpose:read AD9779
parameters:
modify:
----------------------------------------*/									 
uchar read_DA(uchar num,uchar add)
{	
	idata uchar i=0;
	idata uchar reg=0;
	idata uchar da_temp,temp;	

	DAC_SDIO = 0;		
	DAC_CS1=1;
	DAC_CS2=1;
	DAC_CS3=1;
	DAC_CS4=1;
	reg=add+0x80;			//8bit instruction
	switch(num)
	{
		case 1:	DAC_CS1=0; break;
		case 2:	DAC_CS2=0; break;
		case 3:	DAC_CS3=0; break;
		case 4:	DAC_CS4=0; break;
		default: break;
	}
		
	do
	{
		if((reg&0x80) == 0x80)
			DAC_SDIO = 1;
		else
			DAC_SDIO = 0;
		DAC_SCLK = 0;
		delay(10);						//最小50ns
		DAC_SCLK = 1;
		delay(10);
		DAC_SCLK = 0;
		reg=reg<<1;
		i++;
	}while(i<8);
	i=0;
	do
	{
		DAC_SCLK = 0;
		delay(10);						//最小50ns
		DAC_SCLK = 1;
		da_temp=da_temp<<1;
		temp=DAC_SDO;
		da_temp=temp+da_temp;
		delay(10);
		DAC_SCLK = 0;
		i++;
	}while(i<8);
	DAC_CS1=1;
	DAC_CS2=1;
	DAC_CS3=1;
	DAC_CS4=1;

	return da_temp;
}