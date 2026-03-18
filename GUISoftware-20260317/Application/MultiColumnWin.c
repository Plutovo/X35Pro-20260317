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
File        : WINDOW.c
Purpose     : Window routines
---------------------------END-OF-HEADER------------------------------
*/
#include <stdlib.h>
#include <string.h>
#include "DIALOG.h"
#include "Skinning_lib.h"
#include "MultiColumnWin.h"
#if GUI_WINSUPPORT
/***************************************************************************************************
*
*       Private config defaults
*
**********************************************************************
*/
#ifndef MultiColumnWin_BKCOLOR_DEFAULT
#define MultiColumnWin_BKCOLOR_DEFAULT GUI_YAN_BLUE
#endif
/***************************************************************************************************
*
*       Object definition
*
**********************************************************************
*/
typedef struct
{
    WIDGET Widget;
    WM_CALLBACK* cb;
    WM_HWIN hFocussedChild;
    WM_DIALOG_STATUS* pDialogStatus;
    int Column1;
    int Column2;
} MultiColumnWin_Obj;
#define WINDOW_H2P(h) (MultiColumnWin_Obj*)WM_H2P(h)
/***************************************************************************************************
*
*       Static data
*
**********************************************************************
*/
GUI_COLOR MultiColumnWin_DefaultBkColor = MultiColumnWin_BKCOLOR_DEFAULT;
/***************************************************************************************************
*
*       Static routines
*
**********************************************************************
*/
static void _Paint(MultiColumnWin_Obj *pObj)
{
    GUI_RECT rClient;
    GUI_GetClientRect(&rClient);
    LCD_SetBkColor(MultiColumnWin_BKCOLOR_DEFAULT);
    GUI_Clear();
    LCD_SetColor(0x555555);
    Arc(rClient.x0, rClient.y0, rClient.x1, rClient.y0 + pObj->Column1,-2,0,0xE1);
    LCD_SetColor(GUI_WHITE);
    Arc(rClient.x0, rClient.y0, rClient.x1, rClient.y0 + pObj->Column1,-2,0,0x1E);
    Bevel(rClient.x0, rClient.y0, rClient.x1, rClient.y0 + pObj->Column1,0);
    LCD_SetColor(0x555555);
    Arc(rClient.x0, rClient.y0 + pObj->Column1, rClient.x1, rClient.y0 + pObj->Column1 + pObj->Column2,-2,0,0xE1);
    LCD_SetColor(GUI_WHITE);
    Arc(rClient.x0, rClient.y0 + pObj->Column1, rClient.x1, rClient.y0 + pObj->Column1 + pObj->Column2,-2,0,0x1E);
    Bevel(rClient.x0, rClient.y0 + pObj->Column1, rClient.x1, rClient.y0 + pObj->Column1 + pObj->Column2,0);
    LCD_SetColor(0x555555);
    Arc(rClient.x0, rClient.y0 + pObj->Column1 + pObj->Column2, rClient.x1, rClient.y1,-2,0,0xE1);
    LCD_SetColor(GUI_WHITE);
    Arc(rClient.x0, rClient.y0 + pObj->Column1 + pObj->Column2, rClient.x1, rClient.y1,-2,0,0x1E);
    Bevel(rClient.x0, rClient.y0 + pObj->Column1 + pObj->Column2, rClient.x1, rClient.y1,0);
}
/***************************************************************************************************
*
*       _OnChildHasFocus
*/
static void _OnChildHasFocus(WM_HWIN hWin, MultiColumnWin_Obj* pObj, const WM_MESSAGE* pMsg)
{
    if (pMsg->Data.p)
    {
        const WM_NOTIFY_CHILD_HAS_FOCUS_INFO * pInfo = (const WM_NOTIFY_CHILD_HAS_FOCUS_INFO *)pMsg->Data.p;
        int IsDesc = WM__IsAncestorOrSelf(pInfo->hNew, hWin);
        if (!IsDesc)    /* A child has received the focus, Framewindow needs to be activated */
        {
            /* Remember the child which had the focus so we can reactive this child */
            if (WM__IsAncestor(pInfo->hOld, hWin))
            {
                pObj->hFocussedChild = pInfo->hOld;
            }
        }
    }
}
/***************************************************************************************************
*
*       _cb
*/
static void _MultiColumnWin_CallBack(WM_MESSAGE* pMsg)
{
    WM_HWIN hObj;
    MultiColumnWin_Obj* pObj;
    GUI_RECT r;
    hObj = pMsg->hWin;
    pObj = WINDOW_H2P(hObj);
    GUI_GetClientRect(&r);
    switch (pMsg->MsgId)
    {
    case WM_HANDLE_DIALOG_STATUS:
        if (pMsg->Data.p)                             /* set pointer to Dialog status */
        {
            pObj->pDialogStatus = (WM_DIALOG_STATUS*)pMsg->Data.p;
        }
        else                                          /* return pointer to Dialog status */
        {
            pMsg->Data.p = pObj->pDialogStatus;
        }
        return;
    case WM_SET_FOCUS:
        if (pMsg->Data.v)     /* Focus received */
        {
            if (pObj->hFocussedChild && (pObj->hFocussedChild != hObj))
            {
                WM_SetFocus(pObj->hFocussedChild);
            }
            else
            {
                pObj->hFocussedChild = WM_SetFocusOnNextChild(hObj);
            }
            pMsg->Data.v = 0;   /* Focus change accepted */
        }
        return;
    case WM_GET_ACCEPT_FOCUS:
        WIDGET_HandleActive(hObj, pMsg);
        return;
    case WM_NOTIFY_CHILD_HAS_FOCUS:
        _OnChildHasFocus(hObj, pObj, pMsg);
        return;
    case WM_KEY:
        if (((const WM_KEY_INFO*)(pMsg->Data.p))->PressedCnt > 0)
        {
            int Key = ((const WM_KEY_INFO*)(pMsg->Data.p))->Key;
            switch (Key)
            {
            case GUI_KEY_TAB:
                pObj->hFocussedChild = WM_SetFocusOnNextChild(hObj);
                break;                    /* Send to parent by not doing anything */
            }
        }
        break;
    case WM_PAINT:
        //LCD_SetBkColor(WINDOW__DefaultBkColor);
        //FillBevel(r.x0 +8,r.y0+8,r.x1-8,r.y1-8,8);
        //GUI_Clear();
        _Paint(pObj);
        break;
    case WM_GET_BKCOLOR:
        pMsg->Data.Color = MultiColumnWin_BKCOLOR_DEFAULT;
        return;                       /* Message handled */
    }
    if (WIDGET_HandleActive(hObj, pMsg) == 0)
    {
        return;
    }
    WM_DefaultProc(pMsg);
}
/***************************************************************************************************
*
*       Exported routines
*
**********************************************************************
*/
WM_HWIN MultiColumnWin_CreateEx(int x0, int y0, int xsize, int ysize, WM_HWIN hParent,
                                int WinFlags, int ExFlags, int Id)
{
    WM_HWIN hObj;
    /* Create the window */
    WM_LOCK();
    hObj = WM_CreateWindowAsChild(x0, y0, xsize, ysize, hParent,
                                  WinFlags, _MultiColumnWin_CallBack, sizeof(MultiColumnWin_Obj) - sizeof(WM_Obj));
    if (hObj)
    {
        MultiColumnWin_Obj* pObj;
        //WM_LOCK();
        pObj = WINDOW_H2P(hObj);
        WIDGET__Init(&pObj->Widget, Id, WIDGET_STATE_FOCUSSABLE);
        //pObj->cb             = _cb;
        pObj->Column1 = 100;
        pObj->Column2 = 100;
        //WM_UNLOCK();
    }
    WM_UNLOCK();
    return hObj;
}
/*****************************************************************************************************/
void MultiColumnWin_SetColumn(WM_HWIN hObj,int column1,int column2)
{
    if (hObj)
    {
        MultiColumnWin_Obj* pObj;
        WM_LOCK();
        pObj = WINDOW_H2P(hObj);
        pObj->Column1 = column1;
        pObj->Column2 = column2;
        WM_InvalidateWindow(hObj);
        WM_UNLOCK();
    }
}
/***************************************************************************************************
*
*       WINDOW_CreateIndirect
*/
WM_HWIN MultiColumnWin_CreateIndirect(const GUI_WIDGET_CREATE_INFO* pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK* cb)
{
    WM_HWIN hObj;
    hObj = MultiColumnWin_CreateEx(pCreateInfo->x0 + x0, pCreateInfo->y0 + y0, pCreateInfo->xSize, pCreateInfo->ySize,
                                   hWinParent, 0, pCreateInfo->Flags, pCreateInfo->Id);
    return hObj;
}
#else
void MultiColumnWin_c(void);
void MultiColumnWin_c(void) {} /* avoid empty object files */
#endif /* GUI_WINSUPPORT */
