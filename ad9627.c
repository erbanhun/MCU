#include "C8051F340.h"
#define uchar unsigned char
#define uint  unsigned int
#define ulong unsigned long

sbit ADC_CS1    = P1^0;		//output
sbit ADC_CS2    = P1^1;		//output
sbit ADC_CS3    = P1^2;		//output
sbit ADC_CS4    = P1^3;		//output
sbit ADC_SCLK   = P1^4; 	//output 
sbit ADC_SDIO	= P1^5;		//output(write),input(read)

extern void delay(uint cnt);			 // 延时
void AD_SPI(uchar add, uchar num);	 // write AD register
void ini9627(uchar number);			 // 初始化ADC,0:power down;1:normal
uchar read_AD(uchar num,uchar add);		 //read ADC

/*---------------------------------------
purpose:初始化ADC
parameters:  0:power down;1:normal
modify:
----------------------------------------*/
void ini9627(uchar number)
{
	delay(5);
    //-------ad set-------//
	if(number==0)
	{
		AD_SPI(0x08,0x01);			//power down AD
		AD_SPI(0xff,0x01);			//update AD register
	}
	else
	{
		AD_SPI(0x08,0x00);			//AD normal
		AD_SPI(0x14,0x40);			//LVDS output,offset(unsigned) binary  40
		AD_SPI(0xff,0x01);			//update AD register
	}
}
/*---------------------------------------
purpose:在ADC指定的地址写寄存器的值
parameters:
modify:
----------------------------------------*/
void AD_SPI(uchar add, uchar num)
{	
	idata uchar i=0;
	idata ulong reg=0;

	P2MDOUT  = 0xf3;					//P2.6(SDIO):output
	ADC_SDIO = 0;

	ADC_CS1=1;
	ADC_CS2=1;
	ADC_CS3=1;
	ADC_CS4=1;
	reg=add;
	reg=reg<<8;
	reg=reg+num;
	ADC_CS1=0;
	ADC_CS2=0;
	ADC_CS3=0;
	ADC_CS4=0;
	do
	{
		if((reg&0x800000) == 0x800000)
			ADC_SDIO = 1;
		else
			ADC_SDIO = 0;
		ADC_SCLK = 0;
		delay(10);						//最小50ns
		ADC_SCLK = 1;
		delay(10);
		ADC_SCLK = 0;
		reg=reg<<1;
		i++;
	}while(i<24);
	ADC_CS1=1;
	ADC_CS2=1;
	ADC_CS3=1;
	ADC_CS4=1;
}

/*---------------------------------------
purpose:read ADC
parameters:
modify:
----------------------------------------*/									 
uchar read_AD(uchar num,uchar add)
{	
	idata uchar i=0;
	idata uint reg=0;
	idata uchar da_temp,temp;
		
	ADC_CS1=1;
	ADC_CS2=1;
	ADC_CS3=1;
	ADC_CS4=1;
	reg=add+0x8000;						//16bit instruction

	P2MDOUT  = 0xf3;					//P2.6(SDIO):output
	ADC_SDIO = 0;

	switch(num)
	{
		case 1:	ADC_CS1=0; break;
		case 2:	ADC_CS2=0; break;
		case 3:	ADC_CS3=0; break;
		case 4:	ADC_CS4=0; break;
		default: break;
	}

	do									
	{
		if((reg&0x8000) == 0x8000)
			ADC_SDIO = 1;
		else
			ADC_SDIO = 0;
		ADC_SCLK = 0;
		delay(10);						//最小50ns
		ADC_SCLK = 1;
		delay(10);
		ADC_SCLK = 0;
		reg=reg<<1;
		i++;
	}while(i<16);
	i=0;

	P2MDOUT  = 0xb3;					//P2.6(SDIO):input
	ADC_SDIO = 1;
	do
	{
		ADC_SCLK = 0;
		delay(10);						//最小50ns
		ADC_SCLK = 1;
		da_temp=da_temp<<1;
		temp=ADC_SDIO;
		da_temp=temp+da_temp;
		delay(10);
		ADC_SCLK = 0;
		i++;
	}while(i<8);
	ADC_CS1=1;
	ADC_CS2=1;
	ADC_CS3=1;
	ADC_CS4=1;

	return da_temp;
}