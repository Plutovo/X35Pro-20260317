/*--------------------------------------------------------------------
File        : MonitorFrame_Private.h
Purpose     : MonitorFrame private header file
--------------------END-OF-HEADER-------------------------------------*/
			  
#ifndef MULTIPLETEXT_PRIVATE_H
#define MULTIPLETEXT_PRIVATE_H

#include "WM.h"
#include "MultiText.h"

#if GUI_WINSUPPORT

/*********************************************************************
*
*       Object definition
*
**********************************************************************
*/
typedef struct
{
	GUI_COLOR FrameColor;
	char MultiFrameSizeLeft;
	char MultiFrameSizeRight;
	char MultiFrameSizeTop;
	char MultiFrameSizeBot;
}MULTITEXTFRAME_PROPS;

typedef struct
{
	GUI_COLOR		 Color;//颜色
	const GUI_FONT * Font;//字体 
	GUI_RECT         Rect;//显示位置
	int              Align;//对齐方式
	GUI_WRAPMODE	 WrapMode;
}MULTITEXT_PROPS;

// typedef struct
// {
// 	int Times;
// /*	int CurrentTimes;*/
// 	int TimerFlag;
// 	int TimerPeriod;
// 	int sec;
// 	int min;
// 	int hour;
// 	int flag;
// }Timer_PROPS;

typedef struct
{
	WIDGET Widget;

	GUI_COLOR BkColor;
	GUI_COLOR WinBkColor;

	MULTITEXTFRAME_PROPS MultiTextFrame;

	MULTITEXT_PROPS ParaName;
	MULTITEXT_PROPS ParaName1;
	MULTITEXT_PROPS ParaName2;
	MULTITEXT_PROPS ParaName3;
	MULTITEXT_PROPS ParaData;
	MULTITEXT_PROPS ParaData1;
	MULTITEXT_PROPS ParaData2;
	MULTITEXT_PROPS ParaData3;
	MULTITEXT_PROPS ParaUnit;
	MULTITEXT_PROPS ParaUnit1;
	MULTITEXT_PROPS ParaMaxValue;
	MULTITEXT_PROPS ParaMinValue;
	MULTITEXT_PROPS ParaMaxValue1;
	MULTITEXT_PROPS ParaMinValue1;

	char hpParaName[50];
	char hpParaName1[10];
	char hpParaName2[10];
	char hpParaName3[10];
	char hpParaUnit[10];
	char hpParaUnit1[10];
	char hpParaData[10];
	char hpParaData1[10];
	char hpParaData2[10];
	char hpParaData3[10];
	char hpParaMaxValue[10];
	char hpParaMinValue[10];
	char hpParaMaxValue1[10];
	char hpParaMinValue1[10];

	WM_HWIN hSelectWin;
	WM_HMEM ahDrawObj;
	int SeleParaFlag;//0：无选择按钮 1：有

	int index;
	int nStyle;//0 1:粗圆角边框 2:细圆角边框
	int nCircle;//0 1:压力表
	int nPressure;

// 	Timer_PROPS hTimerProps;
// 	GUI_TIMER_HANDLE hTimer;

#if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
	int DebugId;	
#endif

}MultiText_Obj;
/*********************************************************************
*
*       Macros for internal use
*
**********************************************************************
*/
#define MULTITEXTID 0x42555464

#if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
	MultiText_Obj * MULTITEXT_h2p(MULTITEXT_Handle h);
	#define MULTITEXT_H2P(h) MULTITEXT_h2p(h)
	#define MULTITEXT_INIT_ID(p) p->DebugId = MULTITEXTID
#else
	#define MULTITEXT_H2P(h) (MultiText_Obj *)GUI_ALLOC_h2p(h)
	#define MULTITEXT_INIT_ID(p)
#endif







#endif   /* GUI_WINSUPPORT */
#endif
    