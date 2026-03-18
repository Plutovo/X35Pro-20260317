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
#include "WM.H"
#include "Dialog.h"
#include "GUI_Protected.h"
#include "DrawEffectLine.h"
void DrawEffectLine_SetColor(DrawEffectLine_Handle hObj,GUI_COLOR Color)
{
    if(hObj)
    {
        DrawEffectLine_Obj *pObj;
        WM_LOCK();
        pObj = DrawEffectLine_H2P(hObj);
        pObj->Color = Color;
        DrawEffectLine_Invalidate(hObj);
        WM_LOCK();
    }
}
static void PaintLine(DrawEffectLine_Handle hObj)
{
    GUI_RECT rClient;
    DrawEffectLine_Obj *pObj  = DrawEffectLine_H2P(hObj);;
    GUI_GetClientRect(&rClient);
    WM_SetUserClipRect(&rClient);
    GUI_SetColor(WINDOW_BKCOLOR_DEFAULT);
    GUI_FillRect  (rClient.x0, rClient.y0, rClient.x1, rClient.y1);
    LCD_SetColor(pObj->Color);
    GUI_DrawHLine(0, 0, rClient.x1 - rClient.x0  + 1);
    /*  LCD_SetColor(0x0);
      GUI_DrawHLine(1, 0, rClient.x1 - rClient.x0  + 1);
      */
}
static void DrawEffectLine_CallBack(WM_MESSAGE *pMsg)
{
    DrawEffectLine_Handle hObj = (DrawEffectLine_Handle)(pMsg->hWin);
    if(WIDGET_HandleActive(hObj,pMsg) == 0)
    {
        return;
    }
    switch(pMsg->MsgId)
    {
    case WM_PAINT:
        PaintLine(hObj);
        break;
    case WM_TOUCH:
        return;
    case WM_SIZE:
        return;
    }
    WM_DefaultProc(pMsg);
}
DrawEffectLine_Handle DrawEffectLine_CreateAsChild(int x0, int y0, int xsize, int ysize, WM_HWIN hParent,int Id,WM_CALLBACK *cb,int Flags)
{
    DrawEffectLine_Handle hObj;
    WM_LOCK();
    hObj = WM_CreateWindowAsChild(x0,y0,xsize, ysize,hParent,Flags,DrawEffectLine_CallBack,sizeof(DrawEffectLine_Obj)-sizeof(WM_Obj));
    if(hObj)
    {
        DrawEffectLine_Obj *pObj = DrawEffectLine_H2P(hObj);
        WIDGET__Init(&pObj->Widget,Id,WIDGET_STATE_FOCUSSABLE);
        pObj->Color = GUI_BLACK;
    }
    WM_UNLOCK();
    return hObj;
}
DrawEffectLine_Handle DrawEffectLine_CreateIndirect(const GUI_WIDGET_CREATE_INFO *pCreateInfo,WM_HWIN hWinParent,int x0, int y0,WM_CALLBACK *pCallBack)
{
    DrawEffectLine_Handle hObj;
    hObj = DrawEffectLine_CreateAsChild(pCreateInfo->x0 + x0, pCreateInfo->y0 + y0, pCreateInfo->xSize, pCreateInfo->ySize,hWinParent,pCreateInfo->Id,pCallBack,0);
    return hObj;
}
