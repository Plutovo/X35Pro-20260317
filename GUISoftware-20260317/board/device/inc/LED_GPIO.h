#ifndef __LED_GPIO_H
#define __LED_GPIO_H

void GPIO_Init(void);
void LED_SWITCH(unsigned char num,unsigned char status);
void LED_On(void);
void LED_Off(void);
#ifdef IAR_LPC_1788_STK
	#define IO_OD 1<<10
	#define IO_SLEW_FAST 1<<9
	#define IO_INV_E 1<<6
	#define IO_HYS 1<<5
	#define IO_NP 0<<3
	#define IO_PD 1<<3
	#define IO_PP 2<<3
	#define IO_RP 3<<3

	#define IO_DIGITAL 1<<7
	#define IO_NO_FILTER  1<<8
	#define IO_DACEN		1<<16
#endif

#endif

