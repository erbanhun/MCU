															#include "C8051F340.h"
#define uchar unsigned char
#define uint  unsigned int
#define ulong unsigned long

sbit DAC_RESET  = P0^1;		//output
sbit DAC_TRSTB  = P0^0;		//output
sbit DAC_SDENB  = P0^3;		//output
sbit DAC_SCLK   = P0^2; 	//output 
sbit DAC_SDIO	= P0^4;		//output
sbit DAC_SDO	= P0^5;		//input
 

 sbit LMK_SYNC   = P2^3;	
extern void delay(uint cnt);			 // 延时
void DA_SPI(uchar add, ulong num);	 // write DA register
void inidac38j84(uchar number);			 // 初始化DAC,0:power down;1:normal
uint read_DA(uint add);		 //read DAC
/*---------------------------------------
purpose:初始化DAC
parameters:  0:power down;1:normal
modify:
----------------------------------------*/
void inidac38j84(uchar number)
{
	    delay(5);
		DAC_TRSTB=0;
	
//-------da set-------//
	if(number==1)
	{	
		DAC_RESET=1;
		delay(5);
		DAC_RESET=0;
		delay(5);
    DAC_RESET=1;
		delay(5);

		
	/*	DA_SPI(0x00,0x0118);	
		DA_SPI(0x01,0x0003);	
		DA_SPI(0x02,0x0002);	//	
		DA_SPI(0x1a,0x0020);	//
			DA_SPI(0x24,0x0020);	//
			DA_SPI(0x25,0x4000);	//
		DA_SPI(0x31,0x1000);	//
			DA_SPI(0x3b,0x0000);	//
		DA_SPI(0x3c,0x8028);	//
		DA_SPI(0x3d,0x0088);	//
		DA_SPI(0x3e,0x0128);	//
		DA_SPI(0x4a,0xff1e);
DA_SPI(0x4b,0x1200);			
DA_SPI(0x4c,0x1307);
DA_SPI(0x4d,0x0300);
DA_SPI(0x4e,0x0f4f);
DA_SPI(0x4f,0x1c61);

DA_SPI(0x51,0x00dc);

DA_SPI(0x5c,0x1133);
DA_SPI(0x1b,0x0200);//dtest
 //DA_SPI(0x51,0x0000);		//0 1 2 3 4ok 5ok 6ok 7ok
   // DA_SPI(0x52,0x0000);	

	DA_SPI(0x4a,0xff01); 
	
	
	
	DA_SPI(0x03,0xf301); */
	
	  DA_SPI(0x02,0x2082);			   //4xian spi	

		DA_SPI(0x1a,0x0020);			//dac_pll DISabled	  --pll-sleep=1
		DA_SPI(0x31,0x1000);			 //	--pll-reset=1pll_ena=0
	
		DA_SPI(0x3b,0x0800);			//the SERDES settings	   serdes_ clk_sel=0, serdes_refclk_div= 2 0800

		DA_SPI(0x3C,0x8028);			//100 10(LB) 0(SLEEPY)0(RANGE) 00010100(MPY=5) 0
		DA_SPI(0x3D,0x0088);//61
		DA_SPI(0x3E,0x0128);//62  00000 001(TERM) 0 01(RATE) 010 (BUSWIDTH)20bit 011(16bit) 00	  552.96/1*5*2  0128
		DA_SPI(0x3F,0x0000);//63
	
		DA_SPI(0x24,0x0010);  //cdrvser_sysref_mode =2
		DA_SPI(0x25,0x4000);	//clkjesd_div=4  4000
		DA_SPI(0x00,0x0118);  //interp=2.
		
		
 	  DA_SPI(0x4a,0xFF1E); 	//lane_ena  74
		DA_SPI(0x4b,0x1300); //F=1    75
		DA_SPI(0x4c,0x1307); //K=20,L=8    76
		DA_SPI(0x4d,0x0300); //M=4,S=1    77
		DA_SPI(0x4e,0x0f4f);  //SCR_DIS,HD=1   78
   //DA_SPI(0x51,0x0000);	//0 1 2 3 4ok 5ok 6ok 7ok
  // DA_SPI(0x52,0x0000);	

	  DA_SPI(0x4f,0x1cc1);

		DA_SPI(0x5C,0x1111);		//SYSREF MODE  92
	  DA_SPI(0x61,0x0001);		//SYNC MODE  97
		
		read_DA(0x6C);		 //read SERDES PLL alarms  default 0x0000
		DA_SPI(0x6C,0x0000);	
		DA_SPI(0x4a,0xff01);     		 //Set init_state to “0000” and jesd_reset_n to “1” to start the JESD204B link initialization
	//Start the SYSREF generation
		 delay(100);
		//DA_SPI(0x1b,0x0200);//dtest
	//	     	//sif_txenable to “1”.
		
DA_SPI(0x03,0xf301); 
read_DA(0x03);
	}
}

/*---------------------------------------
purpose:在DAC指定的地址写寄存器的值
parameters:
modify:
----------------------------------------*/
void DA_SPI(uchar add, ulong num)
{	
	idata uchar i=0;
	idata ulong  reg=0;

	//DAC_SDIO = 0;
	DAC_SDENB=1;
	reg=add;
	reg=reg<<16;
	reg=reg+num;
	DAC_SDENB=0;

	do
	{
		if((reg&0x800000) == 0x800000)
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
	}while(i<24);
	DAC_SDENB=1;

}
/*---------------------------------------
purpose:read AD
parameters:
modify:
----------------------------------------*/									 
uint read_DA(uint add)	  
{	
	idata uchar i=0;
	idata uint reg=0;
	idata uint da_temp,temp;	

	//DAC_SDIO = 0;		
	DAC_SDENB=1;

	reg=add+0x80;			//8bit 
	DAC_SDENB=0;
		
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

	DAC_SDO=1;
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
	}while(i<16);
	DAC_SDENB=1;

	return da_temp;
}