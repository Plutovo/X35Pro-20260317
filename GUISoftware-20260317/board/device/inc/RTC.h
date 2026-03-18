#ifndef __RTC_H
#define __RTC_H
#include "board.h"
#include "Static_Menu_Creat.h"

void RTC_Init(void);
void RTCStart(void);
void RTCStop(void);
void RTCSetTime(RTC_TIMER Time);
RTC_TIMER RTCReadTime(void);

	

#endif
