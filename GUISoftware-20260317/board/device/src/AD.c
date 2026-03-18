#include "includes.h"
#include "AD.h"

void AD_Init(void)
{
  Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_ADC);
  AD0CR_bit.CLKDIV = Chip_Clock_GetPeripheralClockRate()/1000000/12.4;/*1788 AD Frequency should be less or equal 12.4MHz */
  //AD0CR_bit.BURST=1;
  AD0CR_bit.PDN = 1;
  AD0CR_bit.SEL = 0x80;//  20130130
  AD0CR_bit.START = 1;
  //ADINTEN_bit.ADGINTEN = 0;
  //AD0CR_bit.BURST = 1;
}

void AD_Channel_Start(char channel)
{
	 AD0CR_bit.SEL |= 1<<channel;
	 AD0CR_bit.START = 1;
}
void AD_READ(char channel,unsigned short int *adv)
{
#ifdef IAP_LPC_2478_STK
  switch(channel)
  {
  case 0:
		*adv=(ADDR0>>6) & 0x3ff;
    break;
  case 1:
		*adv=(ADDR1>>6) & 0x3ff;
    break;
  case 2:
		*adv=(ADDR2>>6) & 0x3ff;
    break;
  case 3:'
		*adv=(ADDR3>>6) & 0x3ff;
    break;
  case 4:
		*adv=(ADDR4>>6) & 0x3ff;
    break;  
  case 5:
		*adv=(ADDR5>>6) & 0x3ff;
    break;
  case 6:
		*adv=(ADDR6>>6) & 0x3ff;
    break;
  case 7:
      *adv=(ADDR7>>6) & 0x3ff;
    break;    
  }
#endif
	
#ifdef IAR_LPC_1788_STK
  switch(channel)
  {
  case 0:
		*adv=(ADDR0>>6) & 0xfff;
    break;
  case 1:
		*adv=(ADDR1>>6) & 0xfff;
    break;
  case 2:
		*adv=(ADDR2>>6) & 0xfff;
    break;
  case 3:
		*adv=(ADDR3>>6) & 0xfff;
    break;
  case 4:
		*adv=(ADDR4>>6) & 0xfff;
    break;  
  case 5:
		*adv=(ADDR5>>6) & 0xfff;
    break;
  case 6:
		*adv=(ADDR6>>6) & 0xfff;
    break;
  case 7:
      *adv=(ADDR7>>6) & 0xfff;
    break;    
  }
#endif
}