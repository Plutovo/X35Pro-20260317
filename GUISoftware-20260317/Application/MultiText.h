/*--------------------------------------------------------------------
File        : MonitorFrame.h
Purpose     : MonitorFrame header file
--------------------END-OF-HEADER-------------------------------------*/

#ifndef MULTIPLETEXT_H
#define MULTIPLETEXT_H

#include "WM.h"
#include "DIALOG_Intern.h"      /* Req. for Create indirect data structure */
#include "WIDGET.h"
#if GUI_WINSUPPORT

#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

/************************************************************
*
*       Types
*
*************************************************************
*/
typedef WM_HWIN MULTITEXT_Handle;

/************************************************************
*
*       States
*/

#define MULTITEXT_STATE_FOCUS      WIDGET_STATE_FOCUS
#define MULTITEXT_STATE_PRESSED    WIDGET_STATE_USER0

#define MULTITEXT_CF_FOCUSSABLE	   WIDGET_STATE_FOCUSSABLE

#define MULTITEXT_CF_MultiStyle				(1<<1)
#define MULTITEXT_CF_MultiSeleNumLess		(1<<2)
#define MULTITEXT_CF_MultiSeleNumMore		(1<<3)

#define MULTITEXT_CF_MultiDropDownStyle		(1<<4) 	

typedef enum{
	MultiText_Name = 0,
	MultiText_Name1,
	MultiText_Name2,
	MultiText_Name3,
	MultiText_Unit,
	MultiText_Unit1,
	MultiText_Data,
	MultiText_Data1,
	MultiText_Data2,
	MultiText_Data3,
	MultiText_MaxValue,
	MultiText_MaxValue1,
	MultiText_MinValue,
	MultiText_MinValue1
}MULTITEXTTYPE_t;
/*********************************************************************
*
*       Standard member functions
*
**********************************************************************
*/

void MultiText_SetFrameColor(MULTITEXT_Handle hObj, GUI_COLOR Color);
void MultiText_SetLeftFrameSize(MULTITEXT_Handle hObj, int size);
void MultiText_SetRightFrameSize(MULTITEXT_Handle hObj, int size);
void MultiText_SetTopFrameSize(MULTITEXT_Handle hObj, int size);
void MultiText_SetBotFrameSize(MULTITEXT_Handle hObj, int size);
void MultiText_SetBKColor(MULTITEXT_Handle hObj, GUI_COLOR color);
void MultiText_SetWinBkColor(MULTITEXT_Handle hObj, GUI_COLOR color);
void MultiText_SetTextRect(MULTITEXT_Handle hObj, int x0, int xsize, int y0, int ysize, MULTITEXTTYPE_t index);
void MultiText_SetTextColor(MULTITEXT_Handle hObj, GUI_COLOR color, MULTITEXTTYPE_t index);
void MultiText_SetTextFont(MULTITEXT_Handle hObj, const GUI_FONT GUI_UNI_PTR * pFont, MULTITEXTTYPE_t index);
void MultiText_SetParaMinValue(MULTITEXT_Handle hObj, char * str);
void MultiText_SetOtherParaMinValue(MULTITEXT_Handle hObj, char * str);
void MultiText_SetParaMaxValue(MULTITEXT_Handle hObj, char * str);
void MultiText_SetOtherParaMaxValue(MULTITEXT_Handle hObj, char * str);
void MultiText_SetParaUnit(MULTITEXT_Handle hObj, char* str);
void MultiText_SetOtherParaUnit(MULTITEXT_Handle hObj, char* str);
void MultiText_SetParaName(MULTITEXT_Handle hObj, char* str);
void MultiText_SetParaData(MULTITEXT_Handle hObj, char * str);
void MultiText_SetSeleParaFlag(MULTITEXT_Handle hObj, int flag);
void MultiText_SetOtherParaData(MULTITEXT_Handle hObj, char * str);
void MultiText_SetOtherParaName(MULTITEXT_Handle hObj, char* str);
void MultiText_SetTextWrapMode(MULTITEXT_Handle hObj, GUI_WRAPMODE WrapMode, MULTITEXTTYPE_t index);
void MultiText_SetTextAlign(MULTITEXT_Handle hObj, int Align, MULTITEXTTYPE_t index);
void MultiText_SetOtherParaData1(MULTITEXT_Handle hObj, char * str);
void MultiText_SetParaData1(MULTITEXT_Handle hObj, char * str);
void MultiText_SetPressureData(MULTITEXT_Handle hObj, int pressure);
void MultiText_SetOtherParaName1(MULTITEXT_Handle hObj, char* str);
void MultiText_SetParaName1(MULTITEXT_Handle hObj, char* str);
void MultiText_EX_SetBitmap(MULTITEXT_Handle hObj, const GUI_BITMAP* pBitmap);
void MultiText_EX_SetBitmapEx(MULTITEXT_Handle hObj, const GUI_BITMAP* pBitmap, int x, int y);

void MultiText_SetsCircle(MULTITEXT_Handle hObj, int Circle);
void MultiText_SetsStyte(MULTITEXT_Handle hObj, int Style);

void MultiText_SetTimeStart(MULTITEXT_Handle hObj);
void MultiText_SetTimerStop(MULTITEXT_Handle hObj);
void MultiText_SetTime(MULTITEXT_Handle hObj,int times,int Period,int Flag);
int MultiText_GetSec(MULTITEXT_Handle hObj);
int MultiText_GetMin(MULTITEXT_Handle hObj);
int MultiText_GetHour(MULTITEXT_Handle hObj);
void MultiText_SetSec(MULTITEXT_Handle hObj, int sec);
void MultiText_SetMin(MULTITEXT_Handle hObj, int min);
void MultiText_SetHour(MULTITEXT_Handle hObj, int hour);
void MultiText_SetTimerFlag(MULTITEXT_Handle hObj, int flag);

void MultiText_SetTitleButtonColor(MULTITEXT_Handle hObj, int index, GUI_COLOR color);
WM_HWIN MultiText_GetHandle(MULTITEXT_Handle hObj);

MULTITEXT_Handle MultiText_CreateIndirect(const GUI_WIDGET_CREATE_INFO* pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK* cb);
MULTITEXT_Handle MULTITEXT_Create(int x0, int y0, int xsize, int ysize, int Id, int Flags);

#define MULTITEXT_EnableMemdev(hObj)	WM_EnableMemdev(hObj)
#define MULTITEXT_DisableMemdev(hObj)	WM_DisableMemdev(hObj)
#define MULTITEXT_Delete(hObj)			WM_DeleteWindow(hObj)
#define MULTITEXT_Paint(hObj)			WM_Paint(hObj)
#define MULTITEXT_Invalidate(hObj)		WM_InvalidateWindow(hObj)


#endif   

#if defined(__cplusplus)
}
#endif

#endif   /* MultiText_H */
