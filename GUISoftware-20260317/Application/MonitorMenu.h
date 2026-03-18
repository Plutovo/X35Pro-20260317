#ifndef MONITORMENU_H
#define MONITORMENU_H
#include <stdio.h>
#include "GUI.h"
#include "WM.h"
typedef enum
{
	RANK_NONE,
	RANK_CO2,
	RANK_AA,
	RANk_SPO2,
	RANK_CO2SPO2,
	RANK_AASPO2
}MONITOR_RANK_Typeenum;

typedef struct
{
    MONITOR_RANK_Typeenum  RankType;
    unsigned short Monitor_Robj_DispFlag[7];//  [2/16/2022 yaoyao midify 6->7]
	unsigned short Monitor_AA_Robj_DispFlag[5];
} MONITOR_RANK_Typedef;

typedef struct
{
	GUI_TIMER_HANDLE hTimer;
	WM_HWIN hWin;
	int ClearFlag;
	int StartFlag;
	int BitmapFlag;
	int RefreshFlag;
	int Sec;
	int Min;
	int Hour;
}CUSTOMIZE_TIMER_Obj;

void InitMonitorRank(MONITOR_RANK_Typedef *Rank);
unsigned char InitMonitorRank_Auto(unsigned char flag);
char __Get_MonitorRank_Rapid(MONITOR_RANK_Typedef *Rank,unsigned short ParaFlag);
char Get_MonitorRank_Rapid(unsigned short ParaFlag);
char Get_MonitorRank_AA(unsigned short ParaFlag);
void SetTimerClearFlag(int flag);
void SetTimerStartFlag(int flag);
void SetTimerSec(int sec);
void SetTimerMin(int min);
void SetTimerHour(int hour);
void SetBitmapFlag(int flag);

int GetTimerClearFlag();
int GetTimerStartFlag();
int GetTimerSec();
int GetTimerMin();
int GetTimerHour();
int GetBitmapFlag();

void SetTimerStart(int TimerPeriod);
void SetTimerStop();
int GetTimerHandle();
void SetTimerHandle(WM_HWIN handle);
void SetRefreshFlag(int flag);



#endif

