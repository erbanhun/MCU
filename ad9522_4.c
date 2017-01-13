#include "C8051F340.h"
#define uchar unsigned char
#define uint  unsigned int
#define ulong unsigned long

sbit CLK_CS     = P2^4;		//output
sbit CLK_SCLK   = P2^5; 	//output 
sbit CLK_SDIO	= P2^6;		//output
sbit CLK_SDO	= P2^7;		//input

extern void delay(uint cnt);			 // 延时
void CLK_SPI(uint add, uchar num);	 // write CLK register
void ini9522(void);			 // 初始化CLK
uchar read_CLK(uint add);		 //read CLK

/*---------------------------------------
purpose:初始化CLK
parameters:  0:both pwn; 1:AD_clk normal,DA_clk pwn; 2:AD_clk pwn,DA_clk normal; 3:both normal
			 input:10M, output:32.72M
modify:
----------------------------------------*/
void ini9522(void)
{
	delay(5);
/*
	CLK_SPI(0x00,0x81);			//
	CLK_SPI(0x10,0x7c);			//normal
	CLK_SPI(0x11,0x0a);			//R=10
	CLK_SPI(0x12,0x00);		   	//R
	CLK_SPI(0x13,0x00);			//A=0
	CLK_SPI(0x14,0x30);	   		//B=48(0x30)
	CLK_SPI(0x15,0x00);			//B
	CLK_SPI(0x1C,0xA2);			//REF				
	CLK_SPI(0xf9,0x63);		    //power down
	CLK_SPI(0xfa,0x63);		    //power down
	CLK_SPI(0x190,0xcb);		//  /25
	CLK_SPI(0x193,0xcb);		//  /25
	CLK_SPI(0x196,0xcb);		//  /25
	CLK_SPI(0x199,0x22);		//  /6
	CLK_SPI(0x19b,0x04);		//power down out9,out10,out11
	CLK_SPI(0x1e1,0x02);		//  vco divider input
	CLK_SPI(0x18,0x06);			//vco
	CLK_SPI(0x232,0x01);		//update
	CLK_SPI(0x18,0x07);			//vco
	CLK_SPI(0x232,0x01);		//update
*/

	CLK_SPI(0x00,0x81);			//
	CLK_SPI(0x10,0x7c);			//normal
	CLK_SPI(0x11,0xfa);			//R=250
	CLK_SPI(0x12,0x00);		   	//R
	CLK_SPI(0x13,0x00);			//A=0
	CLK_SPI(0x14,0x80);	   		//B=1152(0x480)
	CLK_SPI(0x15,0x04);			//B
	CLK_SPI(0x1C,0xA2);			//REF				
	CLK_SPI(0xf9,0x63);		    //power down
	CLK_SPI(0xfa,0x63);		    //power down
	CLK_SPI(0x190,0xbb);		//  /24
	CLK_SPI(0x193,0xbb);		//  /24
	CLK_SPI(0x196,0xbb);		//  /24
	CLK_SPI(0x199,0x22);		//  /6
	CLK_SPI(0x1e1,0x02);		//  vco divider input
	CLK_SPI(0x18,0x06);			//vco
	CLK_SPI(0x232,0x01);		//update
	CLK_SPI(0x18,0x07);			//vco
	CLK_SPI(0x232,0x01);		//update

}
/*---------------------------------------
purpose:在CLK指定的地址写寄存器的值
parameters:
modify:
----------------------------------------*/
void CLK_SPI(uint add, uchar num)
{	
	idata uchar i=0;
	idata ulong reg=0;

	CLK_SDIO = 0;
	CLK_CS=1;
	reg=add;
	reg=reg<<8;
	reg=reg+num;
	CLK_CS=0;
	do
	{
		if((reg&0x800000) == 0x800000)
			CLK_SDIO = 1;
		else
			CLK_SDIO = 0;
		CLK_SCLK = 0;
		delay(10);						//最小50ns
		CLK_SCLK = 1;
		delay(10);
		CLK_SCLK = 0;
		reg=reg<<1;
		i++;
	}while(i<24);
	CLK_CS=1;
}
/*---------------------------------------
purpose:read CLK
parameters:
modify:
----------------------------------------*/									 
uchar read_CLK(uint add)
{	
	idata uchar i=0;
	idata uint reg=0;
	idata uchar da_temp,temp;
		
	CLK_CS=1;
	reg=add+0x8000;						//16bit instruction
	CLK_SDIO = 0;
	CLK_CS=0;
	do									
	{
		if((reg&0x8000) == 0x8000)
			CLK_SDIO = 1;
		else
			CLK_SDIO = 0;
		CLK_SCLK = 0;
		delay(10);						//最小50ns
		CLK_SCLK = 1;
		delay(10);
		CLK_SCLK = 0;
		reg=reg<<1;
		i++;
	}while(i<16);

	i=0;
	do
	{
		CLK_SCLK = 0;
		delay(10);						//最小50ns
		CLK_SCLK = 1;
		da_temp=da_temp<<1;
		temp=CLK_SDO;
		da_temp=temp+da_temp;
		delay(10);
		CLK_SCLK = 0;
		i++;
	}while(i<8);
	CLK_CS=1;
	return da_temp;
}