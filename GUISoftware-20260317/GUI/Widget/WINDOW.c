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
File        : WINDOW.c
Purpose     : Window routines
---------------------------END-OF-HEADER------------------------------
*/
#include <stdlib.h>
#include <string.h>
#include "DIALOG.h"
#include "Skinning_lib.h"
#if GUI_WINSUPPORT
/***************************************************************************************************
*
*       Private config defaults
*
**********************************************************************
*/
#ifndef WINDOW_BKCOLOR_DEFAULT
#define WINDOW_BKCOLOR_DEFAULT GUI_YAN_BLUE//0xFFE2BF
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
    GUI_COLOR BkColor;
	GUI_COLOR FrameColor;
    int Use_3D;
	GUI_WINFRAME GUI_WinFrame;
	GUI_ReduceRect GUI_WinRect;
	int Arc_r;
} WINDOW_OBJ;
#define WINDOW_H2P(h) (WINDOW_OBJ*)WM_H2P(h)
/***************************************************************************************************
*
*       Static data
*
**********************************************************************
*/
GUI_COLOR WINDOW__DefaultBkColor = WINDOW_BKCOLOR_DEFAULT;
/***************************************************************************************************
*
*       Static routines
*
**********************************************************************
*/
/***************************************************************************************************
*
*       _OnChildHasFocus
*/
static void _OnChildHasFocus(WM_HWIN hWin, WINDOW_OBJ* pObj, const WM_MESSAGE* pMsg)
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
static void _cb(WM_MESSAGE* pMsg)
{
    WM_HWIN hObj;
    WINDOW_OBJ* pObj;
    WM_CALLBACK* cb;
    GUI_RECT r;
    hObj = pMsg->hWin;
    pObj = WINDOW_H2P(hObj);
    GUI_GetClientRect(&r);
    cb   = pObj->cb;
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
        LCD_SetBkColor(pObj->BkColor);
        GUI_Clear();

		if(pObj->GUI_WinFrame == GUI_FRAMERECT)//带边框
		{
			GUI_SetColor(pObj->FrameColor);
			GUI_DrawHLine(r.y0 + pObj->GUI_WinRect.Reducey0,r.x0 + pObj->GUI_WinRect.Reducex0,r.x1 - pObj->GUI_WinRect.Reducex1);//水平线
			GUI_DrawHLine(r.y1 - pObj->GUI_WinRect.Reducey1,r.x0 + pObj->GUI_WinRect.Reducex0,r.x1 - pObj->GUI_WinRect.Reducex1);//水平线
			GUI_DrawVLine(r.x0 + pObj->GUI_WinRect.Reducex0,r.y0 + pObj->GUI_WinRect.Reducey0,r.y1 - pObj->GUI_WinRect.Reducey1);//竖直线
			GUI_DrawVLine(r.x1 - pObj->GUI_WinRect.Reducex1,r.y0 + pObj->GUI_WinRect.Reducey0,r.y1 - pObj->GUI_WinRect.Reducey1);//竖直线
		}
		else if(pObj->GUI_WinFrame == GUI_FRAMEARCRECT)//带圆角边框
		{
			GUI_SetColor(pObj->FrameColor);
#if 0
			Bevel(pObj->GUI_WinRect.x0, pObj->GUI_WinRect.y0, \
				pObj->GUI_WinRect.x1, pObj->GUI_WinRect.y1,pObj->Arc_r);
#else
			GUI_DrawArc(r.x0 + pObj->GUI_WinRect.Reducex0 + pObj->Arc_r,r.y0 + pObj->GUI_WinRect.Reducey0 + pObj->Arc_r,pObj->Arc_r,pObj->Arc_r, 90,180);
			GUI_DrawArc(r.x0 + pObj->GUI_WinRect.Reducex0 + pObj->Arc_r,r.y1 - pObj->GUI_WinRect.Reducey1 - pObj->Arc_r,pObj->Arc_r,pObj->Arc_r,180,270);
			GUI_DrawArc(r.x1 - pObj->GUI_WinRect.Reducex1 - pObj->Arc_r,r.y0 + pObj->GUI_WinRect.Reducey0 + pObj->Arc_r,pObj->Arc_r,pObj->Arc_r,  0, 90);
			GUI_DrawArc(r.x1 - pObj->GUI_WinRect.Reducex1 - pObj->Arc_r,r.y1 - pObj->GUI_WinRect.Reducey1 - pObj->Arc_r,pObj->Arc_r,pObj->Arc_r,-90,  0);

			GUI_DrawHLine(r.y0 + pObj->GUI_WinRect.Reducey0, \
				r.x0 + pObj->GUI_WinRect.Reducex0 + pObj->Arc_r, r.x1 - pObj->GUI_WinRect.Reducex1 - pObj->Arc_r);//水平线
			GUI_DrawHLine(r.y1 - pObj->GUI_WinRect.Reducey1, \
				r.x0 + pObj->GUI_WinRect.Reducex0 + pObj->Arc_r, r.x1 - pObj->GUI_WinRect.Reducex1 - pObj->Arc_r);//水平线
			GUI_DrawVLine(r.x0 + pObj->GUI_WinRect.Reducex0, \
				r.y0 + pObj->GUI_WinRect.Reducey0 + pObj->Arc_r, r.y1 - pObj->GUI_WinRect.Reducey1 - pObj->Arc_r);//竖直线
			GUI_DrawVLine(r.x1 - pObj->GUI_WinRect.Reducex1, \
				r.y0 + pObj->GUI_WinRect.Reducey0 + pObj->Arc_r, r.y1 - pObj->GUI_WinRect.Reducey1 - pObj->Arc_r);//竖直线
#endif
		}

        if(pObj->Use_3D)
        {
            _WIDGET_EFFECT_3D_DrawDown();
        }
        //FillBevel(r.x0 +8,r.y0+8,r.x1-8,r.y1-8,8);
        break;
    case WM_GET_BKCOLOR:
        pMsg->Data.Color = pObj->BkColor;
        return;                       /* Message handled */
    }
    if (cb)
    {
        (*cb)(pMsg);
    }
    else
    {
        WM_DefaultProc(pMsg);
    }
}
/***************************************************************************************************
*
*       Exported routines
*
**********************************************************************
*/
/***************************************************************************************************
*
*       WINDOW_CreateIndirect
*/
WM_HWIN WINDOW_CreateIndirect(const GUI_WIDGET_CREATE_INFO* pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK* cb)
{
    WM_HWIN hObj;
    hObj = WM_CreateWindowAsChild(
               pCreateInfo->x0 + x0, pCreateInfo->y0 + y0, pCreateInfo->xSize, pCreateInfo->ySize, hWinParent,
               pCreateInfo->Flags, _cb, sizeof(WINDOW_OBJ) - sizeof(WM_Obj));
    if (hObj)
    {
        WINDOW_OBJ* pObj;
        WM_LOCK();
        pObj = WINDOW_H2P(hObj);
        WIDGET__Init(&pObj->Widget, pCreateInfo->Id, WIDGET_STATE_FOCUSSABLE);
        pObj->cb             = cb;
        pObj->hFocussedChild = 0;
        pObj->BkColor = WINDOW__DefaultBkColor;
		pObj->FrameColor = GUI_BLACK;
		pObj->Use_3D = 0;
		pObj->GUI_WinFrame = GUI_NOFRAME;
		pObj->GUI_WinRect.Reducex0 = 0;
		pObj->GUI_WinRect.Reducex1 = 0;
		pObj->GUI_WinRect.Reducey0 = 0;
		pObj->GUI_WinRect.Reducey1 = 0;
		pObj->Arc_r = 0;
		pObj->Arc_r = 0;
        WM_UNLOCK();
    }
    return hObj;
}
/***************************************************************************************************
*
*       Window_SetBkColor
*/
void WINDOW_SetBkColor(WM_HWIN hObj, GUI_COLOR Color)
{
    if (hObj)
    {
        WINDOW_OBJ* pObj;
        WM_LOCK();
        pObj = WINDOW_H2P(hObj);
        pObj->BkColor = Color;
        WM_InvalidateWindow(hObj);
        WM_UNLOCK();
    }
}
/***************************************************************************************************
*
*       Window_SetUse_3D
*/
void WINDOW_SetUse_3D(WM_HWIN hObj, int flg)
{
    if (hObj)
    {
        WINDOW_OBJ* pObj;
        WM_LOCK();
        pObj = WINDOW_H2P(hObj);
        pObj->Use_3D = flg;
        WM_InvalidateWindow(hObj);
        WM_UNLOCK();
    }
}

/*********************************************************************
*
*       WINDOW_SetFrameStyle
*/
void WINDOW_SetFrameStyle(WM_HWIN hObj,GUI_WINFRAME WinFrame)
{
	if (hObj)
	{
		WINDOW_OBJ* pObj;
		WM_LOCK();
		pObj = WINDOW_H2P(hObj);
		pObj->GUI_WinFrame = WinFrame;
		WM_InvalidateWindow(hObj);
		WM_UNLOCK();
	}
}
/*********************************************************************
*
*       WINDOW_SetFrameColor
*/
void WINDOW_SetFrameColor(WM_HWIN hObj, GUI_COLOR color)
{
	if (hObj)
	{
		WINDOW_OBJ* pObj;
		WM_LOCK();
		pObj = WINDOW_H2P(hObj);
		pObj->FrameColor = color;
		WM_InvalidateWindow(hObj);
		WM_UNLOCK();
	}
}
/*********************************************************************
*
*       WINDOW_SetFrameReduceRect
*/
void WINDOW_SetFrameReduceRect(WM_HWIN hObj, GUI_ReduceRect Rect, int Arc)
{
	if (hObj)
	{
		WINDOW_OBJ* pObj;
		WM_LOCK();
		pObj = WINDOW_H2P(hObj);
		pObj->GUI_WinRect = Rect;
		pObj->Arc_r = Arc;
		WM_InvalidateWindow(hObj);
		WM_UNLOCK();
	}
}

#else
void WINDOW_c(void);
void WINDOW_c(void) {} /* avoid empty object files */
#endif /* GUI_WINSUPPORT */
