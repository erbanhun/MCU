#include "C8051F340.h"
#define uchar unsigned char
#define uint  unsigned int
#define ulong unsigned long

sbit AD_SCK    = P1^1;		//output
sbit AD_SDIO   = P1^2;		//output
sbit AD_CSB    = P1^3;		//output
sbit AD_AGC1   = P1^4;		//input
sbit AD_AGC2   = P1^5;		//input


extern void delay(uint cnt);			 // 延时


void iniAD6676(void);			 // 初始化LMK04828
uchar read_AD6676(uint add);	// 读取寄存器数值
void CLK_SPIAD6676(uint add, uchar num); //写寄存器数值

void iniAD6676(void)
{
	idata uchar a,b,c;
	uint		i=0;
	a=0;b=0;c=0;

// reset spi
//CLK_SPIAD6676(0x000,0x99); 
CLK_SPIAD6676(0x000,0x81); // 3-wire spi
delay(80);		//2ms for SPI initialization

// set ext RF clk init -TABLE28
/*CLK_SPIAD6676(0x2A5,0x05); 
CLK_SPIAD6676(0x2A0,0xC0); 
*/

// SPI CLK SYN Initialization
CLK_SPIAD6676(0x2A1,0x60); //Set the integer-N value: 96
CLK_SPIAD6676(0x2A2,0x00); //
CLK_SPIAD6676(0x2A5,0x08); //Reset VCO calibration. 
CLK_SPIAD6676(0x2AA,0x37 ); //VCO Configuration
CLK_SPIAD6676(0x2AC,0x18 ); //Charge Pump Current 
CLK_SPIAD6676(0x2B7,0xF0); //VCO Configuration
CLK_SPIAD6676(0x2BB,0x8D ); //reference divider to DIV=4
CLK_SPIAD6676(0x2A0,0x7D); //Enable CLKSYN and the ADC clock. 
CLK_SPIAD6676(0x2AB,0xC5); //Start VCO calibration.
delay(1);
a = read_AD6676(0x2bc);
/*a = a&0x02;
if(a==0x00){ 
	//printf("VCO cal.\n");
	delay(1);
	a = read_AD6676(0x2bc);
	}
*/

//
CLK_SPIAD6676(0x1E7,0x00); // LVCOMS input for SYNC
CLK_SPIAD6676(0x1C0,0x00); //  DID=0
CLK_SPIAD6676(0x1C1,0x00); // BID
CLK_SPIAD6676(0x1C3,0x01); // SCR=0 L=2
CLK_SPIAD6676(0x1C4,0x01); // F=2
CLK_SPIAD6676(0x1C5,0x1f); // K=32
//
CLK_SPIAD6676(0x1EC,0xBD); // 
CLK_SPIAD6676(0x100,0x85); // Fadc=2949MHZ
CLK_SPIAD6676(0x101,0x0B); //
CLK_SPIAD6676(0x102,0x5E); // IF=350M
CLK_SPIAD6676(0x103,0x01); //
CLK_SPIAD6676(0x104,0xA0); // BW 0 =160M
CLK_SPIAD6676(0x105,0x00); // BW 1 = 0
CLK_SPIAD6676(0x106,0x13); // Lext = 19nH
CLK_SPIAD6676(0x107,0x0A); // MRGN_L=10M
CLK_SPIAD6676(0x108,0x01); // MRGN_u=10M
CLK_SPIAD6676(0x109,0x01); // MRGN_IF=1M
CLK_SPIAD6676(0x10A,0x40); // IDAC1=4mA

for(i=0;i<2;i++){
	CLK_SPIAD6676(0x140,0x01); // 
	CLK_SPIAD6676(0x116,0x0A); // INIT RESON1 CAL
	
	delay(12000); // 2949M set 300MS
	a = 0x00;
	a = read_AD6676(0x117);
	a = a & 0x01;
	if(a==0x00){
		CLK_SPIAD6676(0x11A,0x01); //
		CLK_SPIAD6676(0x11A,0x00); //
		}
	else break;
	}
CLK_SPIAD6676(0x140,0x04); // dec_mode=4 - 12
//CLK_SPIAD6676(0x116,0x17); 

//delay(??) // 300ms
for(i=0;i<2;i++){
	CLK_SPIAD6676(0x116,0x17); // cal & init ADC / init JESD
	delay(12000); // 2949M set 300MS
	a = 0x00;
	a = read_AD6676(0x117);
	a = a & 0x01;
	if(a==0x00){
		CLK_SPIAD6676(0x11A,0x01); //
		CLK_SPIAD6676(0x11A,0x00); //
		}
	else break;
	}	
// SPI AGC init 
CLK_SPIAD6676(0x181,0x00); //00db
CLK_SPIAD6676(0x182,0x06); //06db
CLK_SPIAD6676(0x19E,0x13); //
CLK_SPIAD6676(0x19B,0x04); //
CLK_SPIAD6676(0x19C,0x06); //
CLK_SPIAD6676(0x193,0x00); //
CLK_SPIAD6676(0x194,0x0D); //
CLK_SPIAD6676(0x197,0x00); //
CLK_SPIAD6676(0x198,0x09); //
CLK_SPIAD6676(0x19A,0x02); //
// SPI SHUFFLER INIT
CLK_SPIAD6676(0x342,0x0F5); //
CLK_SPIAD6676(0x343,0xFF); //
// PlL lock??

c = read_AD6676(0x2DC);
b = read_AD6676(0x2BC);
c = read_AD6676(0x141);
b = read_AD6676(0x142);
delay(1);

}

void CLK_SPIAD6676(uint add, uchar num)
{	
	idata uchar i=0;
	idata ulong reg=0;

	AD_SDIO = 0;
	AD_CSB=1;
	reg=add;
	reg=reg<<8;
	reg=reg+num;
	AD_CSB=0;
	do
	{
		if((reg&0x800000) == 0x800000)
			AD_SDIO = 1;
		else
			AD_SDIO = 0;
		AD_SCK = 0;
		delay(10);						//最小50ns
		AD_SCK= 1;
		delay(10);
		AD_SCK = 0;
		reg=reg<<1;
		i++;
	}while(i<24);
	AD_CSB=1;
}
/*---------------------------------------
purpose:read CLK
parameters:
modify:
----------------------------------------*/									 
uchar read_AD6676(uint add)
{	
	idata uchar i=0;
	idata uint reg=0;
	idata uchar da_temp,temp;
		
	AD_CSB=1;
	reg=add+0x8000;						//16bit instruction
	AD_SDIO = 0;
	AD_CSB=0;
	do									
	{
		if((reg&0x8000) == 0x8000)
			AD_SDIO = 1;
		else
			AD_SDIO = 0;
		AD_SCK = 0;
		delay(10);						//最小50ns
		AD_SCK = 1;
		delay(10);
		AD_SCK = 0;
		reg=reg<<1;
		i++;
	}while(i<16);

	i=0;

   P1MDOUT  = 0x0B;                    //output,p2.6 input
   P1       = 0x04;

	do
	{
		AD_SCK = 0;
		delay(10);						//最小50ns
		AD_SCK = 1;
		da_temp=da_temp<<1;
		temp=AD_SDIO;
		da_temp=temp+da_temp;
		//delay(10);
		AD_SCK = 0;
		i++;
	}while(i<8);
	AD_CSB=1;

   P1MDOUT  = 0x0f;                    //output
   P1       = 0x00;
	return da_temp;
}
