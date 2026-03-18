#include "HomePrompt_cfg.h"
#include "HomePrompt.h"
#include "static_menu_creat.h"

extern GUI_TIMER_TIME SystemClock;
extern unsigned char StandbyState;
extern MENU_HWIN Menu_hWin;
extern WM_HWIN AlarmInfoWin;//屏幕上方报警字符区域

char HomePrompt_String[HPBufSize] AHB_RAM  = "";
const char *Prompt_Display[OS_CFG_PRIO_MAX];
static HomePrompt_typedef HomePmt_Type;
static HomePrompt_Tmrtypedef HomePmt_TmrType;
static OS_PRIO HighestPrio = 0;
static u32 prio_table[OS_PRIO_TBL_SIZE(OS_CFG_PRIO_MAX)] = {0};
/*
*{@ home prompt display
*/
void HomePrompt_Init(void)
{
	//OS_PrioInit();
	HomePmt_Type.Disp1st_Flag = 0;
	HomePmt_Type.Disp2nd_Flag = 0;
	HomePmt_Type.HPstring = "";
	HomePmt_Type.DispStr_UpdateFlag = 0;
	HomePmt_TmrType.Tmr_StartTime = 0;
	HomePmt_TmrType.Tmr_StartFlag = 0;
	HomePmt_TmrType.Tmr_Flag = 0;
	HomePmt_TmrType.Tmr_FinishFlag = 0;
	HomePmt_TmrType.Tmr_Delay = HomePrompt_TempDelay;
	HomePrompt_StrInit();
	OS_PrioInsert(prio_table, HomePrompt_OSIdle_Prio);
}

void HomePrompt_StrInit(void)
{
// 	Prompt_Display[HomePrompt_NULL_Prio]		= *Global_DS[DISP_EMPTY];
// 	Prompt_Display[HomePrompt_EGMFail_Prio]		= *Global_DS[DISP_EMERGENCY_ON];
// 	Prompt_Display[HomePrompt_NoFresh_Prio]		= *Global_DS[DISP_NO_FG_GAS];
// 	Prompt_Display[HomePrompt_O2Sub_Prio]		= *Global_DS[DISP_OXYGEN_DEC];
// 	Prompt_Display[HomePrompt_O2Add_Prio]		= *Global_DS[DISP_OXYGEN_ADD];
// 	Prompt_Display[HomePrompt_Dragger_Prio]		= *Global_DS[DISP_DRAGGER_ACCURACY];
// 	Prompt_Display[HomePrompt_MasimoFail_Prio]	= *Global_DS[DISP_SPO2_PULSE_SEARCH];
// 	Prompt_Display[HomePrompt_EmergencyO2_Prio] = *Global_DS[DISP_EMERGENCY_OPEN];
// 	Prompt_Display[HomePrompt_O2CONLOW_Prio]	= *Global_DS[DISP_FG_LOW_O2CONCEN];

	Prompt_Display[HomePrompt_NULL_Prio]		= Sys_Lib_GetDispStr(DP_EMPTY);
	Prompt_Display[HomePrompt_Dragger_Prio]		= Sys_Lib_GetDispStr(DP_UNACCURACY);
}

static void HomePrompt_Dis(HomePrompt_typedef *HomePmt)
{
	static u8 old_StandbyState = 0;

	if(HomePmt == NULL)
	{
		return;
	}
//	if(HomePmt_TmrType.Tmr_FinishFlag != 0)//计时完成 移除临时提示信息优先级
//	{
//		HomePmt_TmrType.Tmr_StartTime = 0;
//		HomePmt_TmrType.Tmr_StartFlag = 0;
//		HomePmt_TmrType.Tmr_Flag = 0;
//		HomePmt_TmrType.Tmr_FinishFlag = 0;
//		HomePmt_TmrType.Tmr_Delay = HomePrompt_TempDelay;
//		HomePmt_Type.Disp1st_Flag = 0;
//		HomePmt_Type.Disp2nd_Flag = 0;
//		HomePmt_Type.DispStr_UpdateFlag = 1;
//		strcpy(HomePrompt_String,Prompt_Display[HomePrompt_NULL_Prio]);
// 		OS_PrioRemove(HomePrompt_Temp_Prio);
//	}

	HighestPrio = OS_PrioGetHighest(prio_table);

	if(HighestPrio >= HomePrompt_4everPos && HighestPrio < HomePrompt_OSIdle_Prio)//永久提示信息
	{
		strcpy(HomePrompt_String,Prompt_Display[HighestPrio]);
	}
	//else if(HighestPrio == HomePrompt_Temp_Prio) //临时提示信息
	//{
	//	HomePrompt_Tmr(SystemClock);

	//	if(HomePmt_TmrType.Tmr_FinishFlag == 0)
	//	{
	//		if(HomePmt->Disp2nd_Flag)
	//		{
	//			sprintf(HomePrompt_String,"%s\n%s",System_Display[HomePmt->Disp1st_Flag],System_Display[HomePmt->Disp2nd_Flag]);
	//		}
	//		else
	//		{
	//			sprintf(HomePrompt_String,"%s",System_Display[HomePmt->Disp1st_Flag]);
	//		}
	//	}
	//}
	//else if(HighestPrio == HomePrompt_MinFG_Prio)
	//{
	//	if(HomePmt->HPstring != NULL)
	//	{
	//		strcpy(HomePrompt_String,HomePmt->HPstring);
	//	}
	//}
	else
	{
		strcpy(HomePrompt_String,Prompt_Display[HomePrompt_NULL_Prio]);
	}
	//显示
	if(HomePmt->DispStr_UpdateFlag || old_StandbyState != StandbyState)
	{
		HomePmt->DispStr_UpdateFlag = 0;
		old_StandbyState = StandbyState;
// 		if((StandbyState==0)&&(WM_IsWindow(Menu_hWin.HomeWin.ParentWin)))//工作界面
// 		{
// 			TEXT_SetText(Menu_hWin.HomeWin.tObj,HomePrompt_String);
//  			if(HighestPrio == HomePrompt_MinFG_Prio)
//  			{
//  				if(TEXT_GetTextColor(Menu_hWin.HomeWin.ParentWin) != FGBTN_LimitColor)
//  				{
//  					TEXT_SetTextColor(Menu_hWin.HomeWin.tObj,FGBTN_LimitColor);
//  				}
//  			}
//  			else
// 			{
// 				if(TEXT_GetTextColor(Menu_hWin.HomeWin.tObj) != GUI_WHITE)
// 				{	
// 					TEXT_SetTextColor(Menu_hWin.HomeWin.tObj,GUI_WHITE);
// 				}
// 			}
// 		}
// 		else//待机界面
// 		{
// 			if((StandbyState==0xff)&&(WM_IsWindow(Menu_hWin.Swin.PromptWin)))
//  			if(WM_IsWindow(Menu_hWin.Swin.PromptWin))
//  			{
//  				WM_SendMessageNoPara(Menu_hWin.Swin.PromptWin,WM_USER);
//  			}
			if(WM_IsWindow(AlarmInfoWin))
			{
				WM_SendMessageNoPara(AlarmInfoWin,WM_USER);
			}
		//}
	}
}

void HomePrompt_Display(void)
{
	HomePrompt_Dis(&HomePmt_Type);
}

// void HomePrompt_SetStringPmt(const char *s)
// {
// 	if(s == NULL)
// 	{
// 		return;
// 	}
// 	HomePmt_Type.HPstring = (char *)s;
// 	HomePmt_Type.DispStr_UpdateFlag = 1;
// 	HomePmt_TmrType.Tmr_StartTime = 0;
// 	HomePmt_TmrType.Tmr_StartFlag = 0;
// 	HomePmt_TmrType.Tmr_Flag = 0;
// 	HomePmt_TmrType.Tmr_FinishFlag = 1;
// 	HomePmt_TmrType.Tmr_Delay = HomePrompt_TempDelay;
// 	OS_PrioInsert(HomePrompt_MinFG_Prio);
// }

// void HomePrompt_ClrSrtingPmt(void)
// {
// 	HomePmt_Type.DispStr_UpdateFlag = 1;
// 	OS_PrioRemove(HomePrompt_MinFG_Prio);
// }

// void HomePrompt_SetTempPmt(OS_PRIO Disp1st,OS_PRIO Disp2nd,u32 t)
// {
// 	HomePmt_TmrType.Tmr_StartFlag = 1;
// 	HomePmt_TmrType.Tmr_StartTime = 0;
// 	HomePmt_TmrType.Tmr_Flag = 0;
// 	HomePmt_TmrType.Tmr_FinishFlag = 0;
// 	HomePmt_Type.Disp1st_Flag = Disp1st;
// 	HomePmt_Type.Disp2nd_Flag = Disp2nd;
// 	HomePmt_Type.DispStr_UpdateFlag = 1;
// 	HomePrompt_TmrSetDelay(t);
// 	OS_PrioInsert(HomePrompt_Temp_Prio);
// }

// void HomePrompt_ClrTempPmt(void)
// {
// 	HomePmt_Type.Disp1st_Flag = 0;
// 	HomePmt_Type.Disp2nd_Flag = 0;
// 	HomePmt_TmrType.Tmr_StartTime = 0;
// 	HomePmt_TmrType.Tmr_Flag = 0;
// 	HomePmt_TmrType.Tmr_FinishFlag = 0;
// 	HomePmt_Type.DispStr_UpdateFlag = 1;
// 	OS_PrioRemove(HomePrompt_Temp_Prio);
// }

void HomePrompt_Set4everPmt(OS_PRIO Prio)
{
    if(OS_PrioLookup(prio_table, Prio) == 0)
    {
        HomePmt_Type.DispStr_UpdateFlag = 1;
        OS_PrioInsert(prio_table, Prio);
    }
}

void HomePrompt_Clr4everPmt(OS_PRIO Prio)
{
    if(OS_PrioLookup(prio_table, Prio) == 1)
    {
        HomePmt_Type.DispStr_UpdateFlag = 1;
        OS_PrioRemove(prio_table, Prio);
    }
}

void HomePrompt_ClrAllPmt(void)
{
	HomePmt_Type.Disp1st_Flag = 0;
	HomePmt_Type.Disp2nd_Flag = 0;
	HomePmt_Type.HPstring = "";
	HomePmt_Type.DispStr_UpdateFlag = 1;
	//OS_PrioInit();
	OS_PrioInsert(prio_table, HomePrompt_OSIdle_Prio);//空闲优先级不能删除
}

void HomePrompt_UpdateStr(void)
{	
	HomePmt_Type.DispStr_UpdateFlag = 1;
}

OS_PRIO HomePrompt_GetHighestPrio(void)
{
	return HighestPrio;
}

/*
*{@ end 
*/



/*
*{@ temp prompt timer
*/
static void HomePrompt_Tmr(int t)
{
	if(HomePmt_TmrType.Tmr_StartFlag)
	{
		if(!HomePmt_TmrType.Tmr_Flag)
		{
			HomePmt_TmrType.Tmr_StartTime = t;
			HomePmt_TmrType.Tmr_Flag = 1;
		}
		else
		{
			if(t >= HomePmt_TmrType.Tmr_StartTime)
			{
				if((t-HomePmt_TmrType.Tmr_StartTime) >= HomePmt_TmrType.Tmr_Delay)
				{
					HomePmt_TmrType.Tmr_StartTime = t;
					HomePmt_TmrType.Tmr_FinishFlag = 1;
				}
			}
			else
			{
				if(HomePmt_TmrType.Tmr_StartTime-(2147483647+t) >= HomePmt_TmrType.Tmr_Delay)
				{
					HomePmt_TmrType.Tmr_StartTime = t;
					HomePmt_TmrType.Tmr_FinishFlag = 1;
				}
			}
		}
	}
}


void HomePrompt_TmrStart(void)
{
	HomePmt_TmrType.Tmr_StartFlag = 1;
}

void HomePrompt_TmrStop(void)
{
	HomePmt_TmrType.Tmr_StartFlag = 0;
}

void HomePrompt_TmrClr(void)
{
	HomePmt_TmrType.Tmr_StartTime = 0;
	HomePmt_TmrType.Tmr_StartFlag = 0;
	HomePmt_TmrType.Tmr_Flag = 0;
	HomePmt_TmrType.Tmr_FinishFlag = 0;
	HomePmt_TmrType.Tmr_Delay = HomePrompt_TempDelay;
}

u8 HomePrompt_TmrGetFinishFlag(void)
{
	return HomePmt_TmrType.Tmr_FinishFlag;
}

void HomePrompt_TmrSetFinishFlag(u8 data)
{
	HomePmt_TmrType.Tmr_FinishFlag = data;
}

u32 HomePrompt_TmrGetDelay(void)
{
	return (u32)(HomePmt_TmrType.Tmr_Delay/1000);
}

void HomePrompt_TmrSetDelay(u32 data)
{
	HomePmt_TmrType.Tmr_Delay = data*1000;//
}
/*
*@end
*/


/*
*@{ Set Fresh gas O2 and total flow button color
*/
#if 0
void HomePrompt_SetO2ConBtnColor(GUI_COLOR color)
{
	if(ButtonExCtrl_GetButtonExCtrlColorEx(Menu_hWin.FreshgasWin.Robj[0],1) != color)
	{
		ButtonExCtrl_SetButtonExCtrlColorEx(Menu_hWin.FreshgasWin.Robj[0], color,1);//center color 
		ButtonExCtrl_SetButtonExCtrlColorEx(Menu_hWin.FreshgasWin.Robj[0], color,4);//Upper edge color
		ButtonExCtrl_SetButtonExCtrlColorEx(Menu_hWin.FreshgasWin.Robj[0], color,5);//Lower edge color
		if(!STANDBYSTATE(StandbyState))
		{
			ButtonExCtrl_SetTextColor(Menu_hWin.FreshgasWin.Robj[0],GUI_BLACK,0);// text color
		}
	}
}

void HomePrompt_SetTotalFlowBtnColor(GUI_COLOR color)
{
	if(ButtonExCtrl_GetButtonExCtrlColorEx(Menu_hWin.FreshgasWin.Robj[1],1) != color)
	{
		ButtonExCtrl_SetButtonExCtrlColorEx(Menu_hWin.FreshgasWin.Robj[1], color,1);
		ButtonExCtrl_SetButtonExCtrlColorEx(Menu_hWin.FreshgasWin.Robj[1], color,4);
		ButtonExCtrl_SetButtonExCtrlColorEx(Menu_hWin.FreshgasWin.Robj[1], color,5);
		if(!STANDBYSTATE(StandbyState))
		{
			ButtonExCtrl_SetTextColor(Menu_hWin.FreshgasWin.Robj[1],GUI_BLACK,0);
		}
	}
}

void HomePrompt_ClrO2ConBtnColor(void)
{
	if(STANDBYSTATE(StandbyState))
	{
		if(ButtonExCtrl_GetButtonExCtrlColorEx(Menu_hWin.FreshgasWin.Robj[0],1) != FGBTN_StandbyColor)
		{
			ButtonExCtrl_SetButtonExCtrlColorEx(Menu_hWin.FreshgasWin.Robj[0], FGBTN_StandbyColor, 1);
			ButtonExCtrl_SetButtonExCtrlColorEx(Menu_hWin.FreshgasWin.Robj[0], GUI_WHITE,4);
			ButtonExCtrl_SetButtonExCtrlColorEx(Menu_hWin.FreshgasWin.Robj[0], FGBTN_StandbyColor,5);
			ButtonExCtrl_SetTextColor(Menu_hWin.FreshgasWin.Robj[0],GUI_BLACK,0);
		}
	}
	else
	{
		if(ButtonExCtrl_GetButtonExCtrlColorEx(Menu_hWin.FreshgasWin.Robj[0],1) != FGBTN_WorkColor)
		{
			ButtonExCtrl_SetButtonExCtrlColorEx(Menu_hWin.FreshgasWin.Robj[0], FGBTN_WorkColor, 1);
			ButtonExCtrl_SetButtonExCtrlColorEx(Menu_hWin.FreshgasWin.Robj[0], FGBTN_UpperColor,4);
			ButtonExCtrl_SetButtonExCtrlColorEx(Menu_hWin.FreshgasWin.Robj[0], FGBTN_WorkColor,5);
			ButtonExCtrl_SetTextColor(Menu_hWin.FreshgasWin.Robj[0],GUI_WHITE,0);
		}
	}
}

void HomePrompt_ClrTotalFlowBtnColor(void)
{
	if(STANDBYSTATE(StandbyState))
	{
		if(ButtonExCtrl_GetButtonExCtrlColorEx(Menu_hWin.FreshgasWin.Robj[1],1) != FGBTN_StandbyColor)
		{
			ButtonExCtrl_SetButtonExCtrlColorEx(Menu_hWin.FreshgasWin.Robj[1], FGBTN_StandbyColor, 1);
			ButtonExCtrl_SetButtonExCtrlColorEx(Menu_hWin.FreshgasWin.Robj[1], GUI_WHITE,4);
			ButtonExCtrl_SetButtonExCtrlColorEx(Menu_hWin.FreshgasWin.Robj[1], FGBTN_StandbyColor,5);
			ButtonExCtrl_SetTextColor(Menu_hWin.FreshgasWin.Robj[1],GUI_BLACK,0);
		}
	}
	else
	{
		if(ButtonExCtrl_GetButtonExCtrlColorEx(Menu_hWin.FreshgasWin.Robj[1],1) != FGBTN_WorkColor)
		{
			ButtonExCtrl_SetButtonExCtrlColorEx(Menu_hWin.FreshgasWin.Robj[1], FGBTN_WorkColor, 1);
			ButtonExCtrl_SetButtonExCtrlColorEx(Menu_hWin.FreshgasWin.Robj[1], FGBTN_UpperColor,4);
			ButtonExCtrl_SetButtonExCtrlColorEx(Menu_hWin.FreshgasWin.Robj[1], FGBTN_WorkColor,5);
			ButtonExCtrl_SetTextColor(Menu_hWin.FreshgasWin.Robj[1],GUI_WHITE,0);
		}
	}
}
#endif
/*
*@end
*/




