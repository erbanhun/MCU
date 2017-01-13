#include "c8051f340.h"
#include <stddef.h>                  // Used for NULL pointer definition


//********************************************************//
//***************Constants Definitions********************//
//********************************************************//
#define uchar unsigned char
#define uint  unsigned int
#define ulong unsigned long

//*******************************************************//
//*********************���Ŷ���**************************//
//*******************************************************//



//*******************************************************//
//******* [BEGIN] Function Information [BEGIN] **********//
//*******************************************************//

void Init_Device(void);			 // Initialize Ports Pins and Enable Crossbar
void delay(uint cnt);			 // ��ʱ


extern void CLK_SPI2581(uchar add, ulong num);	 // write CLK register
extern void iniLMX2581(void);			 // ��ʼ��LMX2581
extern void iniLMK04828(void);			 // ��ʼ��LMK04828
extern void iniAD6676(void);

extern uchar read_CLK04828(uint add);
extern void CLK_SPI04828(uint add, uchar num);

extern void inidac38j84(uchar number);			 // ��ʼ��DAC,0:power down;1:normal
extern  uchar read_AD0(ulong add);
extern void iniAD12J2700(uchar number);			 // ��ʼ��ADC,0:power down;1:normal
extern void DA_SPI(uchar add, ulong num);	 // write DA register
extern uint read_DA(uint add);		 //read DAC
 sbit LMK_SYNC   = P2^4;		//inuput
/*---------------------------------------
purpose:������
parameters:
modify:
----------------------------------------*/	
void main()
{
uchar num=0;

	PCA0MD &= ~0x40;   			  // WDTE = 0 (clear watchdog timer enable) 	                                        
	Init_Device();

	//delay(60000);

	iniLMK04828();
	iniAD6676();

	delay(100);
   num=0;
    while (1)
	{	
		
	}
}
/*---------------------------------------
purpose:��ʼ��MCU
parameters:
modify:
----------------------------------------*/
void Init_Device(void)
{
   OSCICN = 0x83;                      //Configure internal oscillator for its highest frequency

   P0MDIN   = 0xff;					   //digital port
   P0MDOUT  = 0x5f;                    //p0.7 P0.5 input
   P0       = 0xa0;					   //
                                       
   P1MDIN   = 0xff;					   //digital port
   P1MDOUT  = 0x0f;                    //p1.4-7 input 
   P1       = 0x08;

   P2MDIN   = 0xff;					   //digital port
   P2MDOUT  = 0xe8;                    //output  p2.1-2 input
   P2       = 0x08;

   P3MDIN   = 0xff;					   //digital port
   P3MDOUT  = 0x3f;                    //P3.7,P3.6:input
   P3       = 0xc0;                    

   P4MDIN   = 0xff;					   //digital port
   P4MDOUT  = 0xff;                    //output
   P4       = 0x00;                    

   XBR0     = 0x00;                    // No digital peripherals selected
   XBR1     = 0x40;                    // Enable crossbar and weak pull-ups
}
/*---------------------------------------
purpose:��ʱ
parameters:
modify:
----------------------------------------*/
void delay(uint cnt)
{
	idata uint k,j;
	k=cnt;
	//while(k>0)k--;
	for(k=cnt;k>0;k--) // k=40--1ms
		for(j=100;j>0;j--);
}
