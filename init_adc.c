#include <c8051f340.h>
#include <stddef.h>                  // Used for NULL pointer definition


//********************************************************//
//***************Constants Definitions********************//
//********************************************************//
#define uchar unsigned char
#define uint  unsigned int
#define ulong unsigned long

//-------------------------------------------------------//
//***************Constants Definitions[End]**************//
//-------------------------------------------------------//


//*******************************************************//
//*********************引脚定义**************************//
//*******************************************************//
sbit ADC_CS1    = P2^4;		//output
sbit ADC_CS2    = P2^7;		//output
sbit ADC_SCLK   = P2^5; 	//output 
sbit ADC_SDIO	= P2^6;		//output(write),input(read)

sbit DAC_CS1    = P3^0;		//output
sbit DAC_CS2    = P3^4;		//output
sbit DAC_SCLK   = P3^1; 	//output 
sbit DAC_SDIO	= P3^2;		//output
sbit DAC_SDO	= P3^3;		//input

sbit CLK_RESET	= P1^6;		//output
sbit CLK_CS     = P1^2;		//output
sbit CLK_SCLK   = P1^3; 	//output 
sbit CLK_SDIO	= P1^4;		//output
sbit CLK_SDO	= P1^5;		//input


sbit PLL1_LD    = P2^2;		//input
sbit PLL2_LD	= P2^3;		//input
sbit PLL_bak	= P3^5;		//output

sbit BCM_RESET	= P1^0;		//output
sbit BCM_MDIO1	= P0^5;	 	//output
sbit BCM_MDIO2	= P0^4;		//output
sbit BCM_MDIO3	= P0^7;		//output
sbit BCM_MDIO4	= P0^6;		//output
sbit BCM_MDC1	= P0^3;		//output
sbit BCM_MDC2	= P0^2;		//output
sbit BCM_MDC3	= P0^1;		//output
sbit BCM_MDC4	= P0^0;		//output

sbit FPGA_Program	= P1^1;	//output
//-------------------------------------------------------//
//********************引脚定义[End]**********************//
//-------------------------------------------------------//



//*******************************************************//
//******* [BEGIN] Function Information [BEGIN] **********//
//*******************************************************//


void Init_Device(void);			 // Initialize Ports Pins and Enable Crossbar
void delay(uint cnt);			 // 延时

void AD_SPI(uchar add, uchar num);	 // write AD register
void CLK_SPI(uint add, uchar num);	 // write CLK register
void DA_SPI(uchar add, uchar num);	 // write DA register

void ini9627(uchar number);			 // 初始化ADC,0:power down;1:normal
void ini9522(uchar number);			 // 初始化CLK,
void ini9779(uchar number);			 // 初始化DAC,0:power down;1:normal

uchar read_DA(uchar num,uchar add);		 //read DAC
uchar read_AD(uchar num,uchar add);		 //read ADC
uchar read_CLK(uint add);		 //read CLK
//-------------------------------------------------------//
//*************Function Information [ END ]**************//
//-------------------------------------------------------//


//*******************************************************//
//*******************************************************//
//*******************************************************//
/*---------------------------------------
purpose:主程序
parameters:
modify:
----------------------------------------*/	
void main()
{
	uchar clk;

	PCA0MD &= ~0x40;   			  // WDTE = 0 (clear watchdog timer enable) 	                                        
	Init_Device();

	ini9627(0);
	ini9779(0);
	clk=read_CLK(0x19);
	ini9522(2);	

	clk=read_CLK(0x32);
	clk=read_CLK(0x34);


	P4=0x00;
	P4=0xff;

    while (1);
}
/*---------------------------------------
purpose:初始化MCU
parameters:
modify:
----------------------------------------*/
void Init_Device(void)
{
   OSCICN = 0x83;                      //Configure internal oscillator for its highest frequency

   P0MDIN   = 0xff;					   //digital port
   P0MDOUT  = 0xff;                    //output
   P0       = 0x00;					   //
                                       
   P1MDIN   = 0xff;					   //digital port
   P1MDOUT  = 0xdf;                    //P1.5:input
   P1       = 0x20;

   P2MDIN   = 0xff;					   //digital port
   P2MDOUT  = 0xf3;                    //P2.2,P2.3:input
   P2       = 0x0c;

   P3MDIN   = 0xff;					   //digital port
   P3MDOUT  = 0xf7;                    //P3.3:input
   P3       = 0x08;                    

   P4MDIN   = 0xff;					   //digital port
   P4MDOUT  = 0xff;                    //output
   P4       = 0x00;                    

   XBR0     = 0x00;                    // No digital peripherals selected
   XBR1     = 0x40;                    // Enable crossbar and weak pull-ups
}
/*---------------------------------------
purpose:延时
parameters:
modify:
----------------------------------------*/
void delay(uint cnt)
{
	idata uint k;
	k=cnt;
	while(k>0)k--;
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
	reg=add;
	reg=reg<<8;
	reg=reg+num;
	ADC_CS1=0;
	ADC_CS2=0;
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

	CLK_RESET= 1;						//reset ESC
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
	reg=add;
	reg=reg<<8;
	reg=reg+num;
	DAC_CS1=0;
	DAC_CS2=0;
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
	reg=add+0x8000;						//16bit instruction

	P2MDOUT  = 0xf3;					//P2.6(SDIO):output
	ADC_SDIO = 0;

	if(num==1)
		ADC_CS1=0;
	else
		ADC_CS2=0;
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

	return da_temp;
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
	
	CLK_RESET=1;	
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
	reg=add+0x80;			//8bit instruction
	if(num==1)
		DAC_CS1=0;
	else
		DAC_CS2=0;
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

	return da_temp;
}
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
		AD_SPI(0x14,0x00);			//cmos output,offset(unsigned) binary
		AD_SPI(0xff,0x01);			//update AD register
	}
}
/*---------------------------------------
purpose:初始化CLK
parameters:  0:both pwn; 1:AD_clk normal,DA_clk pwn; 2:AD_clk pwn,DA_clk normal; 3:both normal
			 input:10M, output:32.72M
modify:
----------------------------------------*/
void ini9552(uchar number)
{
	delay(5);
    //-------clk set-------//
	if(number==0)
	{
		CLK_SPI(0x32,0xe8);			//power down DA clock
		CLK_SPI(0x34,0xe8);			//power down AD clock
		CLK_SPI(0x05,0x01);			//update CLK register
	}
	else if(number==1)
	{
		CLK_SPI(0x11,0xb4);
		CLK_SPI(0x14,0x0e);
		CLK_SPI(0x15,0x00);

		CLK_SPI(0x17,0x00);
		CLK_SPI(0x18,0x32);
		CLK_SPI(0x19,0x80);			
		CLK_SPI(0x1d,0x04);

		CLK_SPI(0x32,0xe8);			//power down DA clock
		CLK_SPI(0x34,0xa8);			//AD clock normal----
		CLK_SPI(0x05,0x01);			//update CLK register
	}	
	else if(number==2)
	{
		CLK_SPI(0x11,0xb4);
		CLK_SPI(0x14,0x0e);
		CLK_SPI(0x15,0x00);

		CLK_SPI(0x17,0x00);
		CLK_SPI(0x18,0x32);
		CLK_SPI(0x19,0x80);
		CLK_SPI(0x1d,0x04);

		CLK_SPI(0x32,0xa8);			//DA clock normal----
		CLK_SPI(0x34,0xe8);			//power down AD clock
		CLK_SPI(0x05,0x01);			//update CLK register
	}
	else if(number==3)
	{
		CLK_SPI(0x32,0xa8);			//
		CLK_SPI(0x34,0xa8);			//
		CLK_SPI(0x05,0x01);			//update CLK register
	}
}
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
		DA_SPI(0x02,0x80);			//unsigned binary
	}
}

