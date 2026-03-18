/*
*********************************************************************************************************
*                                                uC/GUI
*                        Universal graphic software for embedded applications
*
*                       (c) Copyright 2002, Micrium Inc., Weston, FL
*                       (c) Copyright 2002, SEGGER Microcontroller Systeme GmbH
*
*              礐/GUI is protected by international copyright laws. Knowledge of the
*              source code may not be used to write a similar product. This file may
*              only be used in accordance with a license and should not be redistributed
*              in any way. We appreciate your understanding and fairness.
*
----------------------------------------------------------------------
File        : Dialog.h
Purpose     : Dialog box include
----------------------------------------------------------------------
Open items:
None
--------------------END-OF-HEADER-------------------------------------
*/
#ifndef DIALOG_H
#define DIALOG_H
//
#include "WM.h"
#include "BUTTON.h"
#include "CHECKBOX.h"
#include "DROPDOWN.h"
#include "EDIT.h"
#include "FRAMEWIN.h"
#include "LISTBOX.h"
#include "MULTIEDIT.h"
#include "RADIO.h"
#include "SLIDER.h"
#include "TEXT.h"
//
#if GUI_WINSUPPORT
#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif
//
typedef enum {GUI_NOFRAME=0, GUI_FRAMERECT, GUI_FRAMEARCRECT} GUI_WINFRAME;
typedef struct //相对于当前窗口缩小的距离
{
	I16 Reducex0;
	I16 Reducex1;
	I16 Reducey0;
	I16 Reducey1;
}GUI_ReduceRect;

WM_HWIN WINDOW_CreateIndirect(const GUI_WIDGET_CREATE_INFO* pCreateInfo, WM_HWIN hWinParent, 
                              int x0, int y0, WM_CALLBACK* cb);

void    WINDOW_SetDefaultBkColor(GUI_COLOR Color);
void    WINDOW_SetBkColor(WM_HWIN hObj, GUI_COLOR Color);
void	WINDOW_SetUse_3D(WM_HWIN hObj, int flg);
void	WINDOW_SetFrameStyle(WM_HWIN hObj,GUI_WINFRAME WinFrame);
void	WINDOW_SetFrameColor(WM_HWIN hObj, GUI_COLOR color);
void WINDOW_SetFrameReduceRect(WM_HWIN hObj, GUI_ReduceRect Rect, int Arc);
#if defined(__cplusplus)
}
#endif
#endif
#endif
