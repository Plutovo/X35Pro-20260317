#ifndef _ALARMDISPSTRLIB_H
#define _ALARMDISPSTRLIB_H

#include "MainTask.h"

#define ALARM_MAXNUM  65
#define ALARM_AA_MAXNUM  30

extern const char* AlarmContent[ALARM_MAXNUM];
extern const char* AlarmContent1[ALARM_MAXNUM];
extern const char* AlarmContent2[ALARM_AA_MAXNUM];

void InitAlarmInfoRegister(void);

#endif