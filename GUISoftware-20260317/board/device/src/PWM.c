#include "includes.h"
#include "PWM.h"

void PWM0_Init(void)
{
    PWM0TCR_bit.CR = 1;/* Counter Reset */
    PWM0PR = 0x00;            /* count frequency:Fpclk */
    PWM0MCR_bit.PWMMR0I = 1;  /* interrupt on PWMMR06, reset on PWMMR06, reset TC if PWM06 matches*/
    PWM0MCR_bit.PWMMR0R = 1;
} 
//2016.5.18 zy 修改：PWM计数值以适应频率
void PWM_Set(Int16U cycle)
{
   // PWM0MR0 = 18000000/cycle;     /* set PWM cycle */
   // PWM0MR6 = 9000000/cycle;
    PWM0MR0 = Chip_Clock_GetPeripheralClockRate() / cycle;     /* set PWM cycle */
    PWM0MR6 = Chip_Clock_GetPeripheralClockRate() / 2 / cycle;	
    PWM0LER_bit.EM0L = 1;
    PWM0LER_bit.EM6L = 1;
}

void PWM_Start(void)
{
    PWM0PCR_bit.PWMENA6 = 1;
    PWM0TCR = 0x09;
}

void PWM_Stop(void)
{
    PWM0PCR = 0;
    PWM0TCR = 0x02;     /* Stop all PWMs */
}
