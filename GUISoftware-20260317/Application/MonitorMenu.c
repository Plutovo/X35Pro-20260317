#include "MonitorMenu.h"
#include "CO2_IRMA.h"
#include "AA_Module.h"

static MONITOR_RANK_Typedef Monitor_Rank = {0};
static CUSTOMIZE_TIMER_Obj Customize_Timer;

extern unsigned char SpO2ModuleExist;
extern unsigned char SpO2_Config_On;
extern unsigned char Masimo_SpO2_Config_On;
extern MENU_HWIN Menu_hWin;

void InitMonitorRank(MONITOR_RANK_Typedef *Rank)
{
	if(Rank == NULL)	return;
	Rank->Monitor_Robj_DispFlag[0] = DP_PPEAK;
	Rank->Monitor_Robj_DispFlag[1] = DP_PEEP;
	Rank->Monitor_Robj_DispFlag[2] = DP_MV;
	Rank->Monitor_Robj_DispFlag[3] = DP_VTE;

	Rank->Monitor_AA_Robj_DispFlag[0] = DP_MAC;
	Rank->Monitor_AA_Robj_DispFlag[1] = DP_AA1;
	Rank->Monitor_AA_Robj_DispFlag[2] = DP_AA2;
	Rank->Monitor_AA_Robj_DispFlag[3] = DP_N2O;
	Rank->Monitor_AA_Robj_DispFlag[4] = DP_CO2;//  [2/16/2022 yaoyao]CO2永远在左侧

	switch(Rank->RankType)
	{
	case RANk_SPO2:
 		Rank->Monitor_Robj_DispFlag[4] = DP_FIO2;
		//  [2/16/2022 yaoyao modify ]将血氧参数移到右侧，CO2永远在左侧
// 		Rank->Monitor_Robj_DispFlag[5] = DP_ETCO2; 
// 		Rank->Monitor_AA_Robj_DispFlag[4] = DP_SPO2;
		Rank->Monitor_Robj_DispFlag[5] = DP_PULSE;
		Rank->Monitor_Robj_DispFlag[6] = DP_SPO2;

// 		Rank->Monitor_AA_Robj_DispFlag[4] = DP_CO2;

		break;
	case RANK_NONE:
	default:
		Rank->Monitor_Robj_DispFlag[4] = DP_F;
		Rank->Monitor_Robj_DispFlag[5] = DP_FIO2;
		Rank->Monitor_Robj_DispFlag[6] = DP_PMEAN;//  [2/16/2022 yaoyao add]右侧监测参数改为7个

// 		Rank->Monitor_AA_Robj_DispFlag[4] = DP_CO2;
		break;
	}
}

unsigned char InitMonitorRank_Auto(unsigned char flag)
{
 	static unsigned int Old_EtCO2ModuleExist = 0, Old_SpO2ModuleExist = 0;
 	static unsigned char Old_axid = 0;
 	unsigned char ret = 0;
	static unsigned int Old_SpO2_Config_On = 0, Old_Masimo_SpO2_Config_On = 0;
 	if((Old_EtCO2ModuleExist != AA_Module_Flag.EtCO2ModuleExist) 
 		|| (Old_SpO2ModuleExist != SpO2ModuleExist) || flag) 
	{
 		Old_EtCO2ModuleExist = AA_Module_Flag.EtCO2ModuleExist;
		Old_SpO2ModuleExist  = SpO2ModuleExist;
 		ret = 1;
 
		if(SpO2_Config_On || Masimo_SpO2_Config_On)
		{
			Monitor_Rank.RankType = RANk_SPO2;
			InitMonitorRank(&Monitor_Rank);
		}
		else
		{
			Monitor_Rank.RankType = RANK_NONE;
			InitMonitorRank(&Monitor_Rank);
		}						
 	}
 	return ret;
}

char __Get_MonitorRank_Rapid(MONITOR_RANK_Typedef *Rank,unsigned short ParaFlag)
{
	char cnt = 0;
	unsigned char Disp_Num = 0;

	if(Rank == NULL)	return -1;
	Disp_Num = sizeof(Rank->Monitor_Robj_DispFlag);//

	for(cnt=0;cnt < Disp_Num;cnt++)
	{
		if(Rank->Monitor_Robj_DispFlag[cnt] == ParaFlag)
		{
			break;
		}
	}
	if(cnt == Disp_Num)
	{
		cnt = -1;
	}
	return cnt;
}

char Get_MonitorRank_Rapid(unsigned short ParaFlag)
{
	char cnt;
	cnt = __Get_MonitorRank_Rapid(&Monitor_Rank,ParaFlag);
	return cnt;
}

char Get_MonitorRank_AA(unsigned short ParaFlag)
{
	char cnt = 0;

	for(cnt = 0;cnt < 5;cnt++)//+1
	{
		if(Monitor_Rank.Monitor_AA_Robj_DispFlag[cnt] == ParaFlag)//传入的参数是将要显示的AA参数
		{
			break;
		}
	}
	if(cnt == 5)
	{
		cnt = -1;
	}
	return cnt;
}
//*******************************************************************************
void _Timer_Callback(GUI_TIMER_MESSAGE* pTM)
{
 	char buf[20] = {0};

	Customize_Timer.Sec++;
	if(Customize_Timer.Sec == 60)
	{
		Customize_Timer.Sec = 0;
		Customize_Timer.Min++;
	}
	else if(Customize_Timer.Min == 60)
	{
		Customize_Timer.Min = 0;
		Customize_Timer.Hour++;
	}
	else if(Customize_Timer.Hour == 99)
	{
		Customize_Timer.Hour = 0;
	}
	//
	if(Customize_Timer.RefreshFlag)
	{
		sprintf(buf, "%02d.%02d.%02d", Customize_Timer.Hour, Customize_Timer.Min, Customize_Timer.Sec);
		if(Customize_Timer.hWin != 0)
		{
			MultiText_ParaUpdate(Customize_Timer.hWin, buf);
		}
	}
	GUI_TIMER_Restart(Customize_Timer.hTimer);
}
//
void SetTimerStart(int TimerPeriod)
{	
	Customize_Timer.hTimer = GUI_TIMER_Create(_Timer_Callback,0,0,1);
	GUI_TIMER_SetPeriod(Customize_Timer.hTimer, TimerPeriod);
	GUI_TIMER_Restart(Customize_Timer.hTimer);	
}
//
void SetTimerStop()
{
	if(Customize_Timer.hTimer)
	{
		GUI_TIMER_Delete(Customize_Timer.hTimer);
		Customize_Timer.hTimer = 0;
	}
}
//
int GetTimerHandle()
{
	return Customize_Timer.hTimer;
}
void SetTimerHandle(WM_HWIN handle)
{
	Customize_Timer.hWin = handle;
}
//
void SetTimerStartFlag(int flag)
{
	Customize_Timer.StartFlag = flag;
}
int GetTimerStartFlag()
{
	return Customize_Timer.StartFlag;
}
//
void SetTimerClearFlag(int flag)
{
	Customize_Timer.ClearFlag = flag;
}
int GetTimerClearFlag()
{
	return Customize_Timer.ClearFlag;
}
//
void SetBitmapFlag(int flag)
{
	Customize_Timer.BitmapFlag = flag;
}
int GetBitmapFlag()
{
	return Customize_Timer.BitmapFlag;
}
//
void SetRefreshFlag(int flag)
{
	Customize_Timer.RefreshFlag = flag;
}
int GetRefreshFlag()
{
	return Customize_Timer.RefreshFlag;
}
//
void SetTimerSec(int sec)
{
	Customize_Timer.Sec = sec;
}
int GetTimerSec()
{
	return Customize_Timer.Sec;
}
void SetTimerMin(int min)
{
	Customize_Timer.Min = min;
}
int GetTimerMin()
{
	return Customize_Timer.Min;
}
void SetTimerHour(int hour)
{
	Customize_Timer.Hour = hour;
}
int GetTimerHour()
{
	return Customize_Timer.Hour;
}





