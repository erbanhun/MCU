#include "C8051F340.h"
#define uchar unsigned char
#define uint  unsigned int
#define ulong unsigned long

sbit LMK_RESET  = P2^3;		//output
sbit LMK_SYNC   = P2^4;		//input
sbit LMK_SCK    = P2^5;		//output
sbit LMK_SDIO   = P2^6;		//output
sbit LMK_CSB    = P2^7;		//output

sbit STATUS_LD1 = P1^6; // input
sbit STATUS_LD2 = P1^7; // input
sbit CLKIN_SEL0 = P2^1; // input
sbit CLKIN_SEL1 = P2^2; // input
extern void delay(uint cnt);			 // 延时


void iniLMK04828(void);			 // 初始化LMK04828
uchar read_CLK04828(uint add);   // 通过SPI读取寄存器数值
void CLK_SPI04828(uint add, uchar num);

/*---------------------------------------
purpose:初始化CLK
input:10M, output:2211.84M

----------------------------------------*/
//========================== Dec.1
void iniLMK04828(void)
{		
	idata uchar cnt = 0;
	idata uchar a=0;

		LMK_RESET  =0;	
		delay(5);
		LMK_RESET  =1;	
    	delay(5);
		LMK_RESET  =0;	
		delay(5);

//a=read_CLK04828(0x13b);
//CLK_SPI04828(0x13b,0x12);
//a=read_CLK04828(0x13b);

//Programming registers in numeric order from 0x000 to 0x165.
CLK_SPI04828(0X0000,0X80);// Programming register 0x000 with RESET = 1.
CLK_SPI04828(0X0002,0X00);// normal:0;powerdown:1
//(0x100 - 0x138) Device Clock and SYSREF Clock Output Controls
// DCLK0
CLK_SPI04828(0X0100,0X18);	//0 122.88MHz,1/24,LVDS
CLK_SPI04828(0X0103,0X00);
CLK_SPI04828(0X0104,0X22);	// SDCLKoutX outputs:SYSREF output
CLK_SPI04828(0X0106,0X70);	// sdclkout_pd=0 normal 
CLK_SPI04828(0X0107,0X11);	// format: lvds,lvds

//DCLK2
CLK_SPI04828(0X0108,0X18); 	//2  122.88MHz,1/24,LVDS
CLK_SPI04828(0X010b,0X00);
CLK_SPI04828(0X010c,0X22);  // SDCLKoutX outputs:SYSREF output
//CLK_SPI04828(0X010d,0X11);
CLK_SPI04828(0X010e,0X70);
CLK_SPI04828(0X010f,0X11);	// format: lvds,lvds

//DCLK4
CLK_SPI04828(0X0110,0X18);	 //4 122.88MHz,1/24,LVDS
CLK_SPI04828(0X0113,0X00);
CLK_SPI04828(0X0114,0X22);	// SDCLKoutX outputs:SYSREF output
CLK_SPI04828(0X0116,0X70);
//CLK_SPI04828(0X0116,0X78);
CLK_SPI04828(0X0117,0X11);	// format: lvds,lvds

//DCLK6
CLK_SPI04828(0X0118,0X18);	//6,7	 2211.84MHZ,sin(LVPECL)
CLK_SPI04828(0X011b,0X00);
CLK_SPI04828(0X011c,0X22);	// SDCLKoutX outputs:SYSREF output
//CLK_SPI04828(0X011e,0X70);
CLK_SPI04828(0X011e,0X70);	
CLK_SPI04828(0X011f,0X55);	// format: LVPECL 1600 mV,LVPECL 1600 mV

//DCLK8
CLK_SPI04828(0X0120,0X00);	//8,9 PowerDown
CLK_SPI04828(0X0123,0X00);
CLK_SPI04828(0X0124,0X20);	// SDCLKoutX outputs:SYSREF output
//CLK_SPI04828(0X0126,0X70); //sdclk pd
CLK_SPI04828(0X0126,0X78); 
CLK_SPI04828(0X0127,0X11);

//DCLK10							 
CLK_SPI04828(0X0128,0X08); //10 11 PowerDown
CLK_SPI04828(0X012b,0x00);
CLK_SPI04828(0X012c,0X20);	// SDCLKoutX outputs:SYSREF output
//CLK_SPI04828(0X012e,0X70); //sdclk pd
CLK_SPI04828(0X012e,0X78); 
CLK_SPI04828(0X012f,0X11);

//DCLK12
CLK_SPI04828(0X0130,0X0C);	// 12	245.76MHz ,1/12,LVDS
CLK_SPI04828(0X0133,0X00);
CLK_SPI04828(0X0134,0X22);	// SDCLKoutX outputs:SYSREF output
CLK_SPI04828(0X0136,0X70);
CLK_SPI04828(0X0137,0X55);	// format: LVPECL 1600 mV,LVPECL 1600 mV

/////////////////////

CLK_SPI04828(0X0138,0X21);	   //vco=vco1	,LVDS

//CLK_SPI04828(0X013E,0X03); //no set in continus modes -----//
//CLK_SPI04828(0X013F,0X14); //??delay 0 mode fb_mux=sysref div
CLK_SPI04828(0X013F,0X00);  // PLL2 N Divider=0,PLL1 N Delay=0,  FB_MUX_EN=0
	
//CLK_SPI04828(0X0140,0X09);	 //?? 

//CLK_SPI04828(0X0141,0X00);
//CLK_SPI04828(0X0142,0X00);

//CLK_SPI04828(0X0143,0X91);	//SYNC_EN=1,SYSREF_CLR=1->0
//CLK_SPI04828(0X0143,0X11);  //SYNC_EN=1,SYSREF_CLR=0
	
//CLK_SPI04828(0X0144,0X00);	  
CLK_SPI04828(0X0145,0X7F);  // alwyas 127
CLK_SPI04828(0X0146,0X02);  //CLKin1_TYPE=1
CLK_SPI04828(0X0147,0X1B);	//CLKin_SEL_MODE:CLKin1 Manual , CLKin1_OUT_MUX :PLL1,CLKin0_OUT_MUX:off
CLK_SPI04828(0X0148,0X13);	//CLKin_SEL0_MUX:CLKin0  , CLKin_SEL0_TYPE : output(push pull)
CLK_SPI04828(0X0149,0X53);  //SDIO pin :Output(open drain). CLKin_SEL1_MUX : CLKin1 , CLKin_SEL1:output(push pull)
CLK_SPI04828(0X014A,0X02);  // RESET:Input /w pull-down resistor
CLK_SPI04828(0X014B,0X16);	// TRACK_EN:ENABLE ,HOLDOVER_FORCE: Disabled ,MAN_DAC_EN: Manual,
CLK_SPI04828(0X014C,0X00);	// MAN_DAC = 0x100
CLK_SPI04828(0X014D,0X00);	// DAC_TRIP_LOW = Vcc/64
CLK_SPI04828(0X014E,0XC0);  // DAC_CLK_MULT:16384 , DAC_TRIP_HIGH: Vcc/64
CLK_SPI04828(0X014F,0X7F);	// DAC_CLK_CNTR = 127
CLK_SPI04828(0X0150,0X03);	// HOLDOVER_EN: Enable ,HOLDOVER_HITLESS_SWITCH:Hitless switching 
CLK_SPI04828(0X0151,0X02);  // HOLDOVER_DLD_CNT=512
CLK_SPI04828(0X0152,0X00);  
CLK_SPI04828(0X0153,0X00);  //  CLKin0_R=120	
CLK_SPI04828(0X0154,0X78);	   
CLK_SPI04828(0X0155,0X00);  // CLKin1_R=125
CLK_SPI04828(0X0156,0X7D);  
CLK_SPI04828(0X0157,0X00);	//  CLKin2_R=150
CLK_SPI04828(0X0158,0X96);	   
CLK_SPI04828(0X0159,0X06);  // PLL1_N=1536
CLK_SPI04828(0X015A,0X00);
CLK_SPI04828(0X015B,0XD4);	// PLL1_WND_SIZE:43ns ,PLL1 CPout1 is active, Positive Slope VCO/VCXO,PLL1_CP_GAIN=450uA
CLK_SPI04828(0X015C,0X20);	// PLL1_DLD_CNT=8192
CLK_SPI04828(0X015D,0X00);
CLK_SPI04828(0X015E,0X00);  // PLL1_R_DLY=0ps ,PLL1_N_DLY=0ps

//a=read_CLK04828(0x15F);
CLK_SPI04828(0X015F,0X0B);	// PLL1_LD_MUX:PLL1 DLD, PLL1_LD_TYPE:Output (push-pull)
//a=read_CLK04828(0x15F);

CLK_SPI04828(0X0160,0X00);	// PLL2_R=1
CLK_SPI04828(0X0161,0X01);
CLK_SPI04828(0X0162,0X44);  // OSCin_FREQ:>63 MHz to 127 MHz , PLL2_P=2
CLK_SPI04828(0X0163,0X00);	// PLL2_N_CAL=12
CLK_SPI04828(0X0164,0X00);	   
CLK_SPI04828(0X0165,0X0C);

CLK_SPI04828(0X0171,0XAA);	// Always program to 170
CLK_SPI04828(0X0172,0X02);  // Always program to 2 


CLK_SPI04828(0X017C,0X15); //OPT_REG_1: LMK04821
CLK_SPI04828(0X017D,0X33); //OPT_REG_2: LMK04828
	
CLK_SPI04828(0X0166,0X00);	// PLL2_N=12
CLK_SPI04828(0X0167,0X00);
CLK_SPI04828(0X0168,0X0C);
CLK_SPI04828(0X0169,0X59);	// PLL2_WND_SIZE=3.7ns ,PLL2_CP_GAIN=3200uA, PLL2_CP_POL:Negative, PLL2_CP_TRI:Disable ,
//CLK_SPI04828(0X016A,0X20);	   //vco=clkin1	,LVDS
CLK_SPI04828(0X016B,0X00);
CLK_SPI04828(0X016C,0X00);  // PLL2_LF_R4=200 ,PLL2_LF_R3=200, PLL2_LF_C4=15pf , PLL2_LF_C3=15pf
CLK_SPI04828(0X016D,0X00);

//a=read_CLK04828(0x16E);
CLK_SPI04828(0X016E,0X13); // PLL2_LD_MUX:PLL2 DLD , PLL2_LD_TYPE:Output (push-pull)
a=read_CLK04828(0x16E); 

CLK_SPI04828(0X0173,0X00);	// PLL2_PRE_PD:PowerDown ,PLL2_PD:PowerDown,


CLK_SPI04828(0X1FFD,0X0);	// SPI_LOCK=83
CLK_SPI04828(0X1FFE,0X0);
CLK_SPI04828(0X1FFF,0X53);	
//----------------------
//step 1: Program registers 0x000 to 0x1fff as desired. Key to prepare for SYSREF operations
CLK_SPI04828(0X0143,0X11);	 //sync_pol=0,sync_mode=1;
CLK_SPI04828(0X0139,0X00);	 //sysref_mux=0
CLK_SPI04828(0X013a,0X01);   //sys=div384 = 7.68
CLK_SPI04828(0X013b,0X80);	 //sys=div384 = 7.68
CLK_SPI04828(0X0140,0X08);	 //?SYSREF_PLSR_PD=0,sysref_pd=0,sysref_ddly_pd=0,	 ------x09
CLK_SPI04828(0X0143,0X91);	 //sysref_clr=1;
//------------------
//step 2: Establish deterministic phase relationships between SYSREF and Device Clock for JESD204B
CLK_SPI04828(0X0144,0X00);	 //sysref_dis=0;

CLK_SPI04828(0X0143,0XB1);	 //sysref_pol=1then sysref_pol=0;
CLK_SPI04828(0X0143,0X91);	 //sysref_pol=1then sysref_pol=0;
//-------------------
//step 3: dividers are synchronized, disable SYNC from resetting these dividers.
CLK_SPI04828(0X0144,0Xff);	 //sysref_dis=1;
//step 4: Release reset of local SYSREF digital delay.
CLK_SPI04828(0X0143,0X11);	 //sysref_clr=0;

CLK_SPI04828(0X016a,0X60); //??20
//step 5: Set SYSREF operation.
//  配置 SYNC模式：
//1.Continues SYSREF: SYSREF-MUX=3 
//2.External SYSREF request:SYNC_MODE=0，SYSREF_MUX=2，SYSREF_REQ_EN=1,FPGA JESD204 需要有lmk_sync作为产生SYSREF的条件，且极性与ad的sync相反
//3.JESD204B Pulse on SPI programing：SYNC_MODE=3，SYSREF_MUX=2，且在main 函数中，要在配置完成后，不断判断FPGA回来的sync信号的高低，以确定是否要产生脉冲
/////////////////////
CLK_SPI04828(0X0143,0X10);	  // sync_mode;----0x13   SYNC_POL:normal
CLK_SPI04828(0X0139,0X02);	  // sysref_mux  ---0x02

CLK_SPI04828(0X013E,0X03);    // SYSREF_PULSE_CNT=8
//CLK_SPI04828(0X0143,0X91); //?
//------------------------
//no set in continus modes -----//
//CLK_SPI04828(0X0002,0X00);

//=================
/*
//CLK_SPI04828(0X0139,0X03);	  //sysref_mux=0
//CLK_SPI04828(0X013a,0X01);
//CLK_SPI04828(0X013b,0X80);	   //sys=div192 00C0

//CLK_SPI04828(0X0140,0X0F);	  //pd mode  sysref_pd=0,sysref_ddly_pd=0,	 sysref_plsr_pd=0
//CLK_SPI04828(0X0143,0X11);	 //sync_en=1;sysref_clr=1;

//CLK_SPI04828(0X0144,0X00);	 //sysref_dis=0;



CLK_SPI04828(0X0145,0X7F);
CLK_SPI04828(0X0147,0X1A);	 //sync_mode=2;
CLK_SPI04828(0X014B,0X16);	  //sysref_mux=2


CLK_SPI04828(0X014E,0XC0);
CLK_SPI04828(0X0155,0X01);// clk1in
CLK_SPI04828(0X0156,0XF4);
CLK_SPI04828(0X0159,0X18);
CLK_SPI04828(0X015A,0X00);	 //RESET=INPUT	1
CLK_SPI04828(0X015F,0X0B);
CLK_SPI04828(0X0161,0X01);
CLK_SPI04828(0X0162,0X24);
CLK_SPI04828(0X017C,0X15);
CLK_SPI04828(0X017D,0X33);
CLK_SPI04828(0X016E,0X13);


//CLK_SPI04828(0X016a,0X40);
CLK_SPI04828(0X1FFD,0X0);
CLK_SPI04828(0X1FFE,0X0);
CLK_SPI04828(0X1FFF,0X53);		

*/

	}

//==========================
 
/*---------------------------------------
purpose:在CLK指定的地址写寄存器的值
parameters:
modify:
----------------------------------------*/
void CLK_SPI04828(uint add, uchar num)
{	
	idata uchar i=0;
	idata ulong reg=0;

	LMK_SDIO = 0;
	LMK_CSB=1;
	reg=add;
	reg=reg<<8;
	reg=reg+num;
	LMK_CSB=0;
	do
	{
		if((reg&0x800000) == 0x800000)
			LMK_SDIO = 1;
		else
			LMK_SDIO = 0;
		LMK_SCK = 0;
		delay(10);						//最小50ns
		LMK_SCK= 1;
		delay(10);
		LMK_SCK = 0;
		reg=reg<<1;
		i++;
	}while(i<24);
	LMK_CSB=1;
}
/*---------------------------------------
purpose:read CLK
parameters:
modify:
----------------------------------------*/									 
uchar read_CLK04828(uint add)
{	
	idata uchar i=0;
	idata uint reg=0;
	idata uchar da_temp,temp;
		
	LMK_CSB=1;
	reg=add+0x8000;						//16bit instruction
	LMK_SDIO = 0;
	LMK_CSB=0;
	do									
	{
		if((reg&0x8000) == 0x8000)
			LMK_SDIO = 1;
		else
			LMK_SDIO = 0;
		LMK_SCK = 0;
		delay(10);						//最小50ns
		LMK_SCK = 1;
		delay(10);
		LMK_SCK = 0;
		reg=reg<<1;
		i++;
	}while(i<16);

	i=0;

   P2MDOUT  = 0xb8;                    //output,p2.6 input
   P2       = 0x40;

	do
	{
		LMK_SCK = 0;
		delay(10);						//最小50ns
		LMK_SCK = 1;
		da_temp=da_temp<<1;
		temp=LMK_SDIO;
		da_temp=temp+da_temp;
		delay(10);
		LMK_SCK = 0;
		i++;
	}while(i<8);
	LMK_CSB=1;

   P2MDOUT  = 0xf8;                    //output
   P2       = 0x00;
	return da_temp;
}