#include "C8051F340.h"
#define uchar unsigned char
#define uint  unsigned int
#define ulong unsigned long

//sbit BCM_RESET	= P1^0;		//output
sbit BCM_MDIO1	= P0^5;	 	//output
sbit BCM_MDIO2	= P0^4;		//output
sbit BCM_MDIO3	= P0^7;		//output
sbit BCM_MDIO4	= P0^6;		//output
sbit BCM_MDC1	= P0^3;		//output
sbit BCM_MDC2	= P0^2;		//output
sbit BCM_MDC3	= P0^1;		//output
sbit BCM_MDC4	= P0^0;		//output

//Start of Frame 2 bits
#define ST       0x01
//Operation Code 2 bits
#define OP_READ  0x02
#define OP_WRITE 0x01
//PHY Address 5 bits
#define PHYAD1   0x1C
#define PHYAD2   0x1D
#define PHYAD3   0x1E
#define PHYAD4   0x1F
//Register Address 5bits
//Turnaround 2bits
#define TA		 0x2


/////////BCM5464 Register Map-START////////////////
#define	MII_Control   					0x00
#define	MII_Status    					0x01
#define PHY_ID1   						0x02
#define PHY_ID2   						0x03
#define Auto_Nego_Advertisement 		0x04
#define Auto_Nego_Link_Partner  		0x05
#define Auto_Nego_Expansion     		0x06
#define Next_Page_Transmit      		0x07
#define Link_Partner_Received_Next_Page 0x08
#define Control     					0x09
#define Status       					0x0A
//0Ch-0Eh Reserved
#define IEEE_Extended_Status  			0x0f
#define PHY_Extended_Control 			0x10
#define PHY_Extended_Status  			0x11
#define Receive_Error_Counter 			0x12
#define False_Carrier_Sense_Counter 	0x13
#define Receiver_NOT_OK_Counter 		0x14
//15h-16h Reserved
#define Expansion_Reg_Access   			0x17
//0x18 Shadow Value
#define Auxiliary_Control       		0x18
#define Auxiliary_Status_Summary 		0x19
#define Interrupt_Status         		0x1A
#define Interrupt_Mask           		0x1B
//0x1C	Shadow Value
#define  Spare_Led_Control       		0x1C
#define MS_Seed_HCD_Status       		0x1D
#define Test          		     		0x1E
//1Fh Reserved
/////////BCM5464 Register Map-END////////////////

//
void bcm5464_Write1(uint dat,uchar REGAD);
void bcm5464_Write2(uint dat,uchar REGAD);
void bcm5464_Write3(uint dat,uchar REGAD);
void bcm5464_Write4(uint dat,uchar REGAD);
uint bcm5464_Read1(uchar REGAD);
extern void delay(uint cnt);			 // 延时

/*---------------------------------------
purpose: 写BCM5464 PORT1 寄存器值
parameters:
modify:
----------------------------------------*/
void bcm5464_Write1(uint dat,uchar REGAD)
{
	idata uint tmp2;
	ulong sedata,tmp;//32bits
    uchar i;
	tmp=ST;
	tmp=tmp<<30;
 	sedata = tmp;
	tmp=OP_WRITE;
	tmp=tmp<<28;
	sedata=sedata+tmp;
	tmp=PHYAD1;
	tmp=tmp<<23;
	sedata=sedata+tmp;
	tmp=REGAD;
	tmp=tmp<<18;
	sedata=sedata+tmp;
	tmp=TA;
	tmp=tmp<<16;
	sedata=sedata+tmp;
	tmp2=dat;
	sedata=sedata+tmp2;

    BCM_MDC1  = 0;
	BCM_MDIO1 = 0;
    for(i = 0;i < 64;i++)
    {
        if(i<32)
		{
			BCM_MDIO1 = 1;
		}
		else
		{
			BCM_MDIO1 = ((sedata & 0x80000000)?1:0); // 输出数据
			sedata <<= 1;
		}
		delay(5);
        BCM_MDC1  = 1;        // 上升沿数据锁存
        delay(5);
		BCM_MDC1  = 0;
        
    }
}
/*---------------------------------------
purpose: 读BCM5464 PORT1 寄存器值
parameters:
modify:
----------------------------------------*/
uint bcm5464_Read1(uchar REGAD)
{
	ulong sedata,tmp;//32bits
	uint read_dat;
	uint read_bit;
    uchar i;
 	sedata = ST;
	sedata=sedata<<30;
	tmp=OP_READ;
	tmp=tmp<<28;
	sedata = sedata+tmp;
	tmp=PHYAD1;
	tmp=tmp<<23;
	sedata = sedata+tmp;
	tmp=REGAD;
	tmp=tmp<<18;
	sedata = sedata+tmp;
    BCM_MDC1  = 0;
    for(i = 0;i < 46;i++)
    {
        if(i<32)
		{
			BCM_MDIO1 = 1;
		}
		else
		{
			BCM_MDIO1 = ((sedata & 0x80000000)?1:0); // 输出数据
			sedata <<= 1;
		}
        delay(5);
		BCM_MDC1  = 1;        // 上升沿数据锁存
		delay(5);
        BCM_MDC1  = 0;
    }

    BCM_MDC1  = 0;
	P0MDOUT	  = 0xDF;  //set P0^5 input
	BCM_MDIO1 = 1;
	read_dat  = 0;
	read_bit=0;
	for(i = 0;i < 17;i++)
    {	
		delay(5);
        BCM_MDC1  = 1;        // 上升沿数据锁存
		delay(5);
		BCM_MDC1  = 0;       
		
		{
			read_dat=read_dat<<1;
			read_bit=BCM_MDIO1;
        	read_dat=read_dat+read_bit;
			
		}

    }
	P0MDOUT  = 0xff;
	return read_dat;
}
void bcm5464_Write2(uint dat,uchar REGAD)
{
	idata uint tmp2;
	ulong sedata,tmp;//32bits
    uchar i;
	tmp=ST;
	tmp=tmp<<30;
 	sedata = tmp;
	tmp=OP_WRITE;
	tmp=tmp<<28;
	sedata=sedata+tmp;
	tmp=PHYAD2;
	tmp=tmp<<23;
	sedata=sedata+tmp;
	tmp=REGAD;
	tmp=tmp<<18;
	sedata=sedata+tmp;
	tmp=TA;
	tmp=tmp<<16;
	sedata=sedata+tmp;
	tmp2=dat;
	sedata=sedata+tmp2;

    BCM_MDC2  = 0;
	BCM_MDIO2 = 0;
    for(i = 0;i < 64;i++)
    {
        if(i<32)
		{
			BCM_MDIO2 = 1;
		}
		else
		{
			BCM_MDIO2 = ((sedata & 0x80000000)?1:0); // 输出数据
			sedata <<= 1;
		}
		delay(5);
        BCM_MDC2  = 1;        // 上升沿数据锁存
        delay(5);
		BCM_MDC2  = 0;
        
    }
}
void bcm5464_Write3(uint dat,uchar REGAD)
{
	idata uint tmp2;
	ulong sedata,tmp;//32bits
    uchar i;
	tmp=ST;
	tmp=tmp<<30;
 	sedata = tmp;
	tmp=OP_WRITE;
	tmp=tmp<<28;
	sedata=sedata+tmp;
	tmp=PHYAD3;
	tmp=tmp<<23;
	sedata=sedata+tmp;
	tmp=REGAD;
	tmp=tmp<<18;
	sedata=sedata+tmp;
	tmp=TA;
	tmp=tmp<<16;
	sedata=sedata+tmp;
	tmp2=dat;
	sedata=sedata+tmp2;

    BCM_MDC3  = 0;
	BCM_MDIO3 = 0;
    for(i = 0;i < 64;i++)
    {
        if(i<32)
		{
			BCM_MDIO3 = 1;
		}
		else
		{
			BCM_MDIO3 = ((sedata & 0x80000000)?1:0); // 输出数据
			sedata <<= 1;
		}
		delay(5);
        BCM_MDC3  = 1;        // 上升沿数据锁存
        delay(5);
		BCM_MDC3  = 0;
        
    }
}
void bcm5464_Write4(uint dat,uchar REGAD)
{
	idata uint tmp2;
	ulong sedata,tmp;//32bits
    uchar i;
	tmp=ST;
	tmp=tmp<<30;
 	sedata = tmp;
	tmp=OP_WRITE;
	tmp=tmp<<28;
	sedata=sedata+tmp;
	tmp=PHYAD4;
	tmp=tmp<<23;
	sedata=sedata+tmp;
	tmp=REGAD;
	tmp=tmp<<18;
	sedata=sedata+tmp;
	tmp=TA;
	tmp=tmp<<16;
	sedata=sedata+tmp;
	tmp2=dat;
	sedata=sedata+tmp2;

    BCM_MDC4  = 0;
	BCM_MDIO4 = 0;
    for(i = 0;i < 64;i++)
    {
        if(i<32)
		{
			BCM_MDIO4 = 1;
		}
		else
		{
			BCM_MDIO4 = ((sedata & 0x80000000)?1:0); // 输出数据
			sedata <<= 1;
		}
		delay(5);
        BCM_MDC4  = 1;        // 上升沿数据锁存
        delay(5);
		BCM_MDC4  = 0;
        
    }
}