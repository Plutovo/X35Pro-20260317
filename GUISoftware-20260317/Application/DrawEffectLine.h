/*
*********************************************************************************************************
*                                                uC/GUI
*                        Universal graphic software for embedded applications
*
*                       (c) Copyright 2002, Micrium Inc., Weston, FL
*                       (c) Copyright 2002, SEGGER Microcontroller Systeme GmbH
*
*              µC/GUI is protected by international copyright laws. Knowledge of the
*              source code may not be used to write a similar product. This file may
*              only be used in accordance with a license and should not be redistributed
*              in any way. We appreciate your understanding and fairness.
*
----------------------------------------------------------------------
File        : DrawEffectLine.c
Purpose     : Draw Effect Line
----------------------------------------------------------------------
*/
#ifndef DRAWEFFECTLINE_H
#define DRAWEFFECTLINE_H
#ifdef  GUI_WINSUPPORT
#include "GUI.h"
#include "WM.h"
#define WINDOW_BKCOLOR_DEFAULT						0xFAEFE6
typedef WM_HMEM DrawEffectLine_Handle;
typedef struct
{
    WIDGET Widget;
    GUI_POINT CapturePoint;
    GUI_COLOR Color;
} DrawEffectLine_Obj;
#define DrawEffectLine_H2P(h) (DrawEffectLine_Obj*) GUI_ALLOC_h2p(h)
#define DrawEffectLine_Invalidate(h)   WM_InvalidateWindow(h)
void DrawEffectLine_SetColor(DrawEffectLine_Handle hObj,GUI_COLOR Color);
DrawEffectLine_Handle DrawEffectLine_CreateIndirect(const GUI_WIDGET_CREATE_INFO *pCreateInfo,WM_HWIN hWinParent,int x0, int y0,WM_CALLBACK *pCallBack);
#endif
#endif
