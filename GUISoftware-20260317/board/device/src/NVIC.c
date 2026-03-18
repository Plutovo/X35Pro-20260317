#include "includes.h"
#include "NVIC.h"
/*
WDT_IRQHandler
TIMER0_IRQHandler
TIMER1_IRQHandler
TIMER2_IRQHandler
TIMER3_IRQHandler
UART0_IRQHandler
UART1_IRQHandler
UART2_IRQHandler
UART3_IRQHandler
PWM1_IRQHandler
I2C0_IRQHandler
I2C1_IRQHandler
I2C2_IRQHandler
SPIFI_IRQHandler      
SSP0_IRQHandler
SSP1_IRQHandler
PLL0_IRQHandler
RTC_IRQHandler
EINT0_IRQHandler
EINT1_IRQHandler
EINT2_IRQHandler
EINT3_IRQHandler
ADC_IRQHandler
BOD_IRQHandler
USB_IRQHandler
CAN_IRQHandler
DMA_IRQHandler
I2S_IRQHandler
ENET_IRQHandler
MCI_IRQHandler      
MCPWM_IRQHandler
QEI_IRQHandler
PLL1_IRQHandler
USBActivity_IRQHandler
CANActivity_IRQHandler
UART4_IRQHandler
SSP2_IRQHandler
LCD_IRQHandler
GPIO_IRQHandler
PWM0_IRQHandler
EEPROM_IRQHandler
*/
void ExtUsartIntHandler(void);
void Timer0IntrHandler (void);
void Timer1IntrHandler (void);
void Timer2IntrHandler (void);
void RTCIntrHandler(void);


//void GPIO_IRQHandler(void);
void TIMER0_IRQHandler(void);
void TIMER1_IRQHandler (void);
void TIMER2_IRQHandler (void);
void UART0_IRQHandler (void);
void UART1_IRQHandler (void);
void UART2_IRQHandler (void);
void UART3_IRQHandler (void);
void UART4_IRQHandler (void);
void PWM0_IRQHandler(void);
void RTC_IRQHandler(void);
extern int OS_TimeMS;

void interrupt_init(void)
{    	 
#ifdef IAR_LPC_1788_STK
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); //0~15
	
	NVIC_SetPriority(TIMER0_IRQn,0);
	NVIC_EnableIRQ(TIMER0_IRQn);

	NVIC_SetPriority(TIMER1_IRQn,5);
	NVIC_EnableIRQ(TIMER1_IRQn);

	NVIC_SetPriority(TIMER2_IRQn,28);
	NVIC_EnableIRQ(TIMER2_IRQn);
	
	NVIC_SetPriority(UART0_IRQn,0);
	NVIC_EnableIRQ(UART0_IRQn);
	
	NVIC_SetPriority(UART1_IRQn,7);
	NVIC_EnableIRQ(UART1_IRQn);	
	
	NVIC_SetPriority(UART2_IRQn,2);
	NVIC_EnableIRQ(UART2_IRQn);
	
	NVIC_SetPriority(UART3_IRQn,3);
	NVIC_EnableIRQ(UART3_IRQn);                                   

	NVIC_SetPriority(GPIO_IRQn,4);
	NVIC_EnableIRQ(GPIO_IRQn);		
	
	NVIC_SetPriority(UART4_IRQn,6);
	NVIC_EnableIRQ(UART4_IRQn);	
	
	NVIC_SetPriority(RTC_IRQn,4);
	NVIC_EnableIRQ(RTC_IRQn);	
	
#endif
}

void GPIO_IRQHandler(void)
{
  ExtUsartIntHandler();	
}
void TIMER0_IRQHandler(void)
{
  Timer0IntrHandler();	
}	
void TIMER1_IRQHandler(void)
{
  Timer1IntrHandler();
}	
void TIMER2_IRQHandler(void)
{
  Timer2IntrHandler();	
}	
void RTC_IRQHandler(void)
{
  RTCIntrHandler();	
}	
void UART0_IRQHandler(void)
{
  Uart0IntHandler();  	
}	
void UART1_IRQHandler(void)
{
  Uart1IntHandler();	
}	
void UART2_IRQHandler(void)
{
  Uart2IntHandler();	
}	
void UART3_IRQHandler(void)
{
  Uart3IntHandler();	
}	
void UART4_IRQHandler(void)
{
  Uart4IntHandler();	
}	

#ifdef IAR_LPC_1788_STK

	void NVIC_PriorityGroupConfig(unsigned int NVIC_PriorityGroup)
	{
		AIRCR = AIRCR_VECTKEY_MASK | NVIC_PriorityGroup;
	}
	
	void NVIC_SetPriority(int IRQn, uint32_t priority)
	{
		if(IRQn < 16) 
			*(volatile unsigned int *)((unsigned int)&SHPR0 + (IRQn / 4 )) = priority << ((IRQn % 4) * 8);		
		else 
		{
			IRQn=IRQn-16;

			*(volatile unsigned int *)((unsigned int)&IP0 + (IRQn / 4 ) * 4) = priority << ((IRQn % 4) * 8);
		}		
	}

	void NVIC_EnableIRQ(int IRQn)
	{
		IRQn=IRQn - 16;
		*(volatile unsigned int *)((unsigned int)&SETENA0 + (IRQn / 32 ) * 4) = 1 << (IRQn % 32 );
	}          

#endif
