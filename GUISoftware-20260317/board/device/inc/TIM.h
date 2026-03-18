#ifndef __TIM_H
#define __TIM_H

void TIM_Init(unsigned int T0_Freq,unsigned int T1_Freq,unsigned int T2_Freq);
void Delay(uint32_t delaydata);
void TIM0_Interrupt(void);
void TIM1_Interrupt(void);

extern volatile int TEST_MS;
extern unsigned int Avoid_Time_Runover  ;
extern unsigned short int Battery_now;
#endif


