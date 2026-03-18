#ifndef __AD_H
#define __AD_H

void AD_Init(void);
void AD_READ(char channel,unsigned short int *adv);	
void AD_Channel_Start(char channel);
#endif
