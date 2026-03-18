#include "includes.h"
#include "TIM.h"


void TIM_Init(unsigned int T0_Freq,unsigned int T1_Freq,unsigned int T2_Freq)
{
  
  Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_TIMER0);
  Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_TIMER1);
  Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_TIMER2);
  
  T0TCR_bit.CE = 0;     // counting  disable
  T0TCR_bit.CR = 1;     // set reset
  T0TCR_bit.CR = 0;     // release reset
  T0CTCR_bit.CTM = 0;   // Timer Mode: every rising PCLK edge
  T0MCR_bit.MR0I = 1;   // Enable Interrupt on MR0
  T0MCR_bit.MR0R = 1;   // Enable reset on MR0
  T0MCR_bit.MR0S = 0;   // Disable stop on MR0
  T0PR = 0;
  T0MR0 = Chip_Clock_GetPeripheralClockRate() / T0_Freq;
  T0IR_bit.MR0INT = 1;  
  T0TCR_bit.CE = 1;   


  T1TCR_bit.CE = 0;     // counting  disable
  T1TCR_bit.CR = 1;     // set reset
  T1TCR_bit.CR = 0;     // release reset
  T1CTCR_bit.CTM = 0;   // Timer Mode: every rising PCLK edge
  T1MCR_bit.MR0I = 1;   // Enable Interrupt on MR0
  T1MCR_bit.MR0R = 1;   // Enable reset on MR0
  T1MCR_bit.MR0S = 0;   // Disable stop on MR0
  T1PR = 0;
  T1MR0 = Chip_Clock_GetPeripheralClockRate() / T1_Freq;
  T1IR_bit.MR0INT = 1;  
  T1TCR_bit.CE = 1;    
	
  T2TCR_bit.CE = 0;
  T2TCR_bit.CR = 1;
  T2TCR_bit.CR = 0;
  T2CTCR_bit.CTM = 0;   
  T2MCR_bit.MR0I = 1;
  T2MCR_bit.MR0R = 1;
  T2MCR_bit.MR0S = 0;
	T2PR = 0;
  T2MR0 = Chip_Clock_GetPeripheralClockRate() / T2_Freq;
  T2IR_bit.MR0INT = 1;  
  T2TCR_bit.CE = 1;	     
}
