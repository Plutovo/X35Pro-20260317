#include "includes.h"
#include "LED_GPIO.h"

//
void GPIO_Init()
{       
    volatile int i  ;
	volatile unsigned int *addr;
	    
    Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_GPIO);
    
#ifdef  IAR_LPC_1788_STK
	//LED
	IOCON_P1_07 |= IO_PP;
	FIO1DIR |= 1<<7;

  //ALARM
	IOCON_P0_22 |= IO_PP;
	FIO0DIR |= 1<<22;

	//ADC
	IOCON_P0_13 = 3;
	FIO0DIR &= ~(1<<13);

	//PWM
	IOCON_P1_11=0x23;
	
	//UART3 
	IOCON_P0_00 |= 2 | IO_PD;
	IOCON_P0_01 |= 2 | IO_PD;
	//UART0 
	IOCON_P0_02 |= 1 | IO_PD;
	IOCON_P0_03 |= 1 | IO_PD;	
	//UART2 
	IOCON_P0_10 |= 1 | IO_PD;
	IOCON_P0_11 |= 1 | IO_PD;
	//UART1 
	IOCON_P0_15 |= 1 | IO_PD;
	IOCON_P0_16 |= 1 | IO_PD;			
//UART4 
    IOCON_P0_22 |= 3 | (1<<3);//uart4
    IOCON_P5_03 |= 4; 
    
	//IIC	
	IOCON_P0_27 = 0;	
	IOCON_P0_28 = 0;
	FIO0DIR |= 1<<27;
	FIO0DIR |= 1<<28;
	
	IOCON_P1_16 |= IO_PP;
	FIO1DIR |= 1<<16;
	FIO1PIN &= ~(1<<16);

	//LCD
	for(addr = &(IOCON_P1_20);	addr <= &(IOCON_P1_29);	addr++)
		*addr = 7;
	for(addr = &(IOCON_P2_02);	addr <= &(IOCON_P2_09);	addr++)
		*addr = 7;
	IOCON_P2_12 = 5;
	IOCON_P2_13 = 7;
	
	//AC_Battery
	IOCON_P0_30 = 0;
	IOCON_P0_31 = 0;
	FIO0DIR &= ~(1<<30 | 1ul<<31) ;	

#endif
}
//2016.5.16  zy IO1CLR/IO1SET¸ÄÎªFIO1CLR/FIO1SET
void LED_On(void)
{
    FIO1CLR_bit.P1_7 = 1;
}

void LED_Off(void)
{
    FIO1SET_bit.P1_7 = 1;
}