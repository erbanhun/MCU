#include "C8051F340.h"
#define uchar unsigned char
#define uint  unsigned int
#define ulong unsigned long

sbit AD0_SCSB    = P0^6;		//output
sbit AD0_SDO    = P0^7;		//input
sbit AD0_SDI    = P1^0;		//output
sbit AD0_SCLK    = P1^1;		//output
sbit AD1_SCSB    = P1^2;		//output
sbit AD1_SDO    = P1^3;		//input
sbit AD1_SDI    = P1^4;		//output
sbit AD1_SCLK    = P1^5;		//output
sbit LMK_SYNC   = P2^3;		//output

extern void delay(uint cnt);			 // 延时
void AD_SPI(ulong add, uchar num);	 // write AD 

void iniAD12J2700(uchar number);			 // 初始化ADC,0:power down;1:normal
uchar read_AD0(ulong add);		 //read ADC0
uchar read_AD1(ulong add);		 //read ADC1

/*---------------------------------------
purpose:初始化ADC
parameters:  0:power down;1:normal
modify:
----------------------------------------*/
void iniAD12J2700(uchar number)
{ idata uchar i=0;
	delay(5);
//	P0MDOUT=0XFE;
//	P1MDOUT=0XDF;
    //-------ad set-------//
	if(number==0)
	{
		AD_SPI(0x201,0x0e);			//power down AD
		AD_SPI(0x200,0x10);			//update AD register
	}
	else
	{	AD_SPI(0x000,0xbc);							 //power up reset
	  AD_SPI(0x033,0xc7);
		AD_SPI(0x201,0x5e);	//JESD_EN=0	   0-scrmber enable 10111 k-1=23 1 ddr fb=2fs 0	jesd_en disabled
		AD_SPI(0x200,0x32);//program DECIMATE,SCRAM_EN,KM1,DDR  decimate by 4   0x32
	  AD_SPI(0x203,0x00);	// did
		AD_SPI(0x057,0x13);
		AD_SPI(0x066,0x03);	//configre the calibration settings
		AD_SPI(0x050,0x0e);//initiate a calibration CAL_SFT=1
		AD_SPI(0x058,0x02);//enable test  0x02
		AD_SPI(0x202,0x80);//1 p54=1,0 sync_se 00 reserved 0000(test disabled) 0001 PRBS 7  	AD_SPI(0x058,0x04);//enable test

		AD_SPI(0x20c,0x01);//1 : Use the NCO_SEL register
		AD_SPI(0x20d,0x00);//choose preset 0
		//AD_SPI(0x20e,0xc0);
		//AD_SPI(0x20f,0x06);//NCO_RDIV=1728;fstep=10khz
		AD_SPI(0x210,0x00);
		AD_SPI(0x211,0x00);
		AD_SPI(0x212,0xc0);
		AD_SPI(0x213,0x3e);	 //fnco =2754.0-2211.84M*/ 3ec00000
		AD_SPI(0x201,0x5f);//JESD_EN=1
	//apply one SYSREF rising edge
	//assert SYNC~
	//de-assert SYNC~ ,ILA PROCESS BEGINS
	//NORMAL DATA OUTPUT
        read_AD1(0x201);
		     read_AD1(0x213);
			   read_AD0(0x201);
			     read_AD0(0x213);
	}
}
/*---------------------------------------
purpose:在ADC指定的地址写寄存器的值
parameters:
modify:
----------------------------------------*/

void AD_SPI(ulong add, uchar num)
{	
	idata uchar i=0;
	idata ulong reg=0;
	AD0_SCSB=1;
	AD1_SCSB=1;
	reg=add;
	reg=reg<<8;
	reg=reg+num;
	AD0_SCSB=0;
	AD1_SCSB=0;

	do
	{
		if((reg&0x800000) == 0x800000)
		{	AD0_SDI = 1;
			AD1_SDI = 1; }
		else
		{	AD0_SDI = 0;
			AD1_SDI = 0;  }
		AD0_SCLK = 0;
		AD1_SCLK = 0;
		delay(10);						//最小50ns
		AD0_SCLK = 1;
		AD1_SCLK = 1;
		delay(10);
		AD0_SCLK = 0;
		AD1_SCLK = 0;
		reg=reg<<1;
		i++;
	}while(i<24);
	AD0_SCSB=1;
	AD1_SCSB=1;

}

/*---------------------------------------
purpose:read ADC0
parameters:
modify:
----------------------------------------*/									 
uchar read_AD0(ulong add)
{	
	idata uchar i=0;
	idata ulong reg=0;
	idata uchar da_temp,temp;
		
	AD0_SCSB=1;

	reg=add+0x8000;						//16bit instruction
	AD0_SCSB=0;

	do									
	{
		if((reg&0x8000) == 0x8000)
			AD0_SDI = 1;
		else
			AD0_SDI = 0;
		AD0_SCLK = 0;
		delay(10);						//最小50ns
		AD0_SCLK = 1;
		delay(10);
		AD0_SCLK = 0;
		reg=reg<<1;
		i++;
	}while(i<16);
	i=0;

	AD0_SDO=1;
	do
	{
		AD0_SCLK = 0;
		delay(10);						//最小50ns
		AD0_SCLK = 1;
		da_temp=da_temp<<1;
		temp=AD0_SDO;
		da_temp=temp+da_temp;
		delay(10);
		AD0_SCLK = 0;
		i++;
	}while(i<8);
	AD0_SCSB=1;


	return da_temp;
}

/*---------------------------------------
purpose:read ADC1
parameters:
modify:
----------------------------------------*/									 
uchar read_AD1(ulong add)
{	
	idata uchar i=0;
	idata ulong reg=0;
	idata uchar da_temp,temp;
		
	AD1_SCSB=1;

	reg=add+0x8000;						//16bit instruction

	AD1_SCSB=0;
	do									
	{
		if((reg&0x8000) == 0x8000)
			AD1_SDI = 1;
		else
			AD1_SDI = 0;
		AD1_SCLK = 0;
		delay(10);						//最小50ns
		AD1_SCLK = 1;
		delay(10);
		AD1_SCLK = 0;
		reg=reg<<1;
		i++;
	}while(i<16);
	i=0;

	AD1_SDO=1;
	do
	{
		AD1_SCLK = 0;
		delay(10);						//最小50ns
		AD1_SCLK = 1;
		da_temp=da_temp<<1;
		temp=AD1_SDO;
		da_temp=temp+da_temp;
		delay(10);
		AD1_SCLK = 0;
		i++;
	}while(i<8);
	AD1_SCSB=1;


	return da_temp;
}