#ifndef _HOMEPROMPT_
#define _HOMEPROMPT_
#include "GUIType.h"
#include "os_prio.h"

typedef struct 
{
	OS_PRIO Disp1st_Flag;
	OS_PRIO Disp2nd_Flag;
	u8 DispStr_UpdateFlag;
	char *HPstring;
}HomePrompt_typedef;

typedef struct  
{
	u32 Tmr_Delay;		//延时时间
	u32 Tmr_StartTime;	//开始时间
	u8  Tmr_StartFlag;	//计数/暂停标志
	u8  Tmr_Flag;
	u8  Tmr_FinishFlag;	//计数完成标志
}HomePrompt_Tmrtypedef;

extern char HomePrompt_String[HPBufSize];
static void HomePrompt_Tmr(int t);
void HomePrompt_TmrStart(void);
void HomePrompt_TmrStop(void);
void HomePrompt_TmrClr(void);
u8	 HomePrompt_TmrGetFinishFlag(void);
void HomePrompt_TmrSetFinishFlag(u8 data);
u32  HomePrompt_TmrGetDelay(void);
void HomePrompt_TmrSetDelay(u32 data);

void HomePrompt_Init(void);
void HomePrompt_StrInit(void);
static void HomePrompt_Dis(HomePrompt_typedef *HomePmt);
void HomePrompt_Display(void);
void HomePrompt_SetStringPmt(const char *s);
void HomePrompt_ClrSrtingPmt(void);
void HomePrompt_SetTempPmt(OS_PRIO Disp1st,OS_PRIO Disp2nd,u32 t);
void HomePrompt_ClrTempPmt(void);
void HomePrompt_Set4everPmt(OS_PRIO Prio);
void HomePrompt_Clr4everPmt(OS_PRIO Prio);
void HomePrompt_ClrAllPmt(void);
void HomePrompt_UpdateStr(void);
OS_PRIO HomePrompt_GetHighestPrio(void);

void HomePrompt_SetO2ConBtnColor(GUI_COLOR color);
void HomePrompt_ClrO2ConBtnColor(void);
void HomePrompt_SetTotalFlowBtnColor(GUI_COLOR color);
void HomePrompt_ClrTotalFlowBtnColor(void);
#endif
