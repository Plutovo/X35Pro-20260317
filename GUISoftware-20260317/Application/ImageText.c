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
File        : ImageText.c
Purpose     : Implementation of IMAGETEXT widget
---------------------------END-OF-HEADER------------------------------
*/
#include <stdlib.h>
#include <string.h>
#include "ImageText_Private.h"
//#include "Skinning_lib.h"
#if GUI_WINSUPPORT
/***************************************************************************************************
*
*       Private config defaults
*
**********************************************************************
*/
/* Define default fonts */
#ifndef   IMAGETEXT_FONT_DEFAULT
#define IMAGETEXT_FONT_DEFAULT &GUI_Font_20//GUI_Font_16//GUI_Font_20
#endif
/* Define colors */
#ifndef   IMAGETEXT_BKCOLOR0_DEFAULT
#define IMAGETEXT_BKCOLOR0_DEFAULT   0x383434 // WINDOW_BK_COLOR
#endif
#ifndef   IMAGETEXT_TEXTCOLOR0_DEFAULT
#define IMAGETEXT_TEXTCOLOR0_DEFAULT GUI_WHITE
#endif
#ifndef   IMAGETEXT_FRAMECOLOR0_DEFAULT
#define IMAGETEXT_FRAMECOLOR0_DEFAULT 0x1b1818 // SPLIT_LINE_COLOR
#endif
#ifndef   IMAGETEXT_ALIGN_DEFAULT
#define IMAGETEXT_ALIGN_DEFAULT GUI_TA_HCENTER | GUI_TA_VCENTER
#endif
#ifndef IMAGETEXT_WRAPMODE_DEFAULT   //add 20190428 是否自动换行
#define IMAGETEXT_WRAPMODE_DEFAULT GUI_WRAPMODE_WORD
#endif
/***************************************************************************************************
*
*       Static data
*
**********************************************************************
*/
IMAGETEXT_PROPS IMAGETEXT__DefaultProps =
{
    IMAGETEXT_BKCOLOR0_DEFAULT,
	IMAGETEXT_TEXTCOLOR0_DEFAULT,
	IMAGETEXT_FRAMECOLOR0_DEFAULT,
    IMAGETEXT_FONT_DEFAULT,
    IMAGETEXT_ALIGN_DEFAULT,
	IMAGETEXT_WRAPMODE_DEFAULT
};
/***************************************************************************************************
*
*       Static routines
*
**********************************************************************
*/
/***************************************************************************************************
*
*       _Paint
*/
static void _Paint(IMAGETEXT_Obj* pObj, IMAGETEXT_Handle hObj)
{
    const char* s = NULL;
    GUI_RECT rClient;
	GUI_RECT r;
	int i;
    GUI_SetFont(pObj->Props.pFont);
    GUI_DEBUG_LOG("IMAGETEXT: Paint(..)\n");
    if (pObj->hpText)
    {
        s = (const char*) GUI_ALLOC_h2p(pObj->hpText);
    }
    GUI_GetClientRect(&rClient);
    LCD_SetBkColor (pObj->Props.aBkColor);
    GUI_Clear();
	r = rClient;
	GUI_SetTextMode(GUI_TM_TRANS);

    /* Draw bitmap.
       If we have only one, we will use it.
       If we have to we will use the second one (Index 1) for the pressed state
    */
	if (pObj->nStyle==1)
	{
		int y0,x0,Size,Rad;
		Rad = 9;
		x0 = r.x1-Rad*2;
		switch (pObj->nState)
		{
		default:
		case 0:
			GUI_SetColor(GUI_WHITE);
			GUI_DrawHLine((r.y1+r.y0)/2,r.x1-16,r.x1-8);
			GUI_DrawHLine((r.y1+r.y0)/2,r.x1-2,r.x1+6);
			break;
		case 1:
			GUI_SetColor(GUI_GREEN);
			GUI_DrawCircle(x0+Rad,r.y0+6+Rad,Rad);
			Size=2*Rad-5;
			for (i = 5; i < Size; i++)
			{
				GUI_DrawHLine(r.y0+6 + i,x0 + i,   x0+ i + 1);
				GUI_DrawHLine(r.y0+6 + i,r.x1 - i - 1,  r.x1 - i);
			}
			break;
		case 2:
			GUI_SetColor(GUI_RED);
			GUI_DrawCircle(x0+Rad,r.y0+6+Rad,Rad);
			Size=2*Rad-5;
			for (i = 5; i < Size; i++)
			{
				GUI_DrawHLine(r.y0+6 + i,x0 + i,   x0+ i + 1);
				GUI_DrawHLine(r.y0+6 + i,r.x1 - i - 1,  r.x1 - i);
			}
			break;
		}
	}
    else if (pObj->nStyle == 2)
    {
		GUI_SetColor(pObj->Props.aFrameColor);
		GUI_DrawHLine(r.y0,r.x0,r.x1);
		GUI_DrawHLine(r.y1,r.x0,r.x1);
		r.x0 += 1;
		r.x1 -= 1;
		r.y0 += 1;
		r.y1 -= 1;
    }
    else if (pObj->nStyle == 3)
    {
		LCD_SetColor(pObj->Props.aFrameColor);
		FillBevel(rClient.x0+8,rClient.y0+8,rClient.x1-8,rClient.y1-8,8);
    }
    else
	{
		GUI_DRAW__Draw(pObj->ahDrawObj, 0, 0);
		if (pObj->ahDrawObj)
		{
			if (pObj->BitmapTextFlag == 2)
			{
				r.x0 += GUI_DRAW__GetXSize(pObj->ahDrawObj)+5;
			}
		}
	}

	LCD_SetColor   (pObj->Props.aTextColor);
 	GUI_DispStringInRectWrap(s, &r, pObj->Props.Align, pObj->Props.WrapMode);
    /* Draw focus */
    /*if (State & BUTTON_STATE_FOCUS) {
      LCD_SetColor(GUI_BLUE);
      GUI_DrawFocusRect(&rClient, 3);
    GUI_DrawFocusRect(&rClient, 2);
    }*/
    WM_SetUserClipRect(NULL);
}
/***************************************************************************************************
*
*       _Delete
*
* Delete attached objects (if any)
*/
static void _Delete(IMAGETEXT_Obj* pObj)
{
    GUI_ALLOC_FreePtr(&pObj->hpText);
    GUI_ALLOC_FreePtr(&pObj->ahDrawObj);
}
/***************************************************************************************************
*
*       _OnTouch
*/
static void _OnTouch(IMAGETEXT_Handle hObj, IMAGETEXT_Obj* pObj, WM_MESSAGE*pMsg)
{
}
/***************************************************************************************************
*
*       IMAGETEXT_Callback
*/
void IMAGETEXT_Callback(WM_MESSAGE *pMsg)
{
    IMAGETEXT_Handle hObj = pMsg->hWin;
    IMAGETEXT_Obj* pObj = IMAGETEXT_H2P(hObj);
    /* Let widget handle the standard messages */
    if (WIDGET_HandleActive(hObj, pMsg) == 0)
    {
        return;
    }
    switch (pMsg->MsgId)
    {
    case WM_TOUCH:
        _OnTouch(hObj, pObj, pMsg);
        return;      /* Message handled. Do not call WM_DefaultProc, because the window may have been destroyed */
    case WM_PAINT:
        GUI_DEBUG_LOG("IMAGETEXT: _IMAGETEXT_Callback(WM_PAINT)\n");
        _Paint(pObj, hObj);
        return;
    case WM_DELETE:
        GUI_DEBUG_LOG("IMAGETEXT: _IMAGETEXT_Callback(WM_DELETE)\n");
        _Delete(pObj);
        break;       /* No return here ... WM_DefaultProc needs to be called */
    }
    WM_DefaultProc(pMsg);
}
/***************************************************************************************************
*
*       Exported routines:  Create
*
**********************************************************************
*/
/***************************************************************************************************
*
*       IMAGETEXT_CreateEx
*/
IMAGETEXT_Handle IMAGETEXT_CreateEx(int x0, int y0, int xsize, int ysize, WM_HWIN hParent, int WinFlags, int ExFlags, int Id)
{
    IMAGETEXT_Handle hObj;
    GUI_USE_PARA(ExFlags);
    /* Create the window */
    WM_LOCK();
    hObj = WM_CreateWindowAsChild(x0, y0, xsize, ysize, hParent, WinFlags, IMAGETEXT_Callback,
                                  sizeof(IMAGETEXT_Obj) - sizeof(WM_Obj));
    if (hObj)
    {
        IMAGETEXT_Obj* pObj = IMAGETEXT_H2P(hObj);
        /* init widget specific variables */
        WIDGET__Init(&pObj->Widget, Id, WIDGET_STATE_FOCUSSABLE);
        /* init member variables */
        IMAGETEXT_INIT_ID(pObj);
        pObj->Props = IMAGETEXT__DefaultProps;
		pObj->BitmapTextFlag = 0;
        //pObj->Button_Use_3D = 1;
		pObj->nStyle = 0;
    }
    else
    {
        GUI_DEBUG_ERROROUT_IF(hObj==0, "ImageText_Create failed")
    }
    WM_UNLOCK();
    return hObj;
}
/***************************************************************************************************
*
*       BUTTON_Create
*/
IMAGETEXT_Handle IMAGETEXT_Create(int x0, int y0, int xsize, int ysize, int Id, int Flags)
{
    return IMAGETEXT_CreateEx(x0, y0, xsize, ysize, WM_HMEM_NULL, Flags, 0, Id);
}
/***************************************************************************************************
*
*       IMAGETEXT_CreateAsChild
*/
IMAGETEXT_Handle IMAGETEXT_CreateAsChild (int x0, int y0, int xsize, int ysize, WM_HWIN hParent, int Id, int Flags)
{
    return IMAGETEXT_CreateEx(x0, y0, xsize, ysize, hParent, Flags, 0, Id);
}
/***************************************************************************************************
*
*       IMAGETEXT_CreateIndirect
*/
IMAGETEXT_Handle IMAGETEXT_CreateIndirect(const GUI_WIDGET_CREATE_INFO* pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK* cb)
{
    IMAGETEXT_Handle  hThis;
    GUI_USE_PARA(cb);
    hThis = IMAGETEXT_CreateEx(pCreateInfo->x0 + x0, pCreateInfo->y0 + y0,
                               pCreateInfo->xSize, pCreateInfo->ySize,
                               hWinParent, 0, pCreateInfo->Flags, pCreateInfo->Id);
    IMAGETEXT_SetText(hThis, pCreateInfo->pName);
    return hThis;
}
/***************************************************************************************************
*
*       Exported routines:  Various methods
*
**********************************************************************
*/
/***************************************************************************************************
*
*       IMAGETEXT_SetText
*/
void IMAGETEXT_SetText(IMAGETEXT_Handle hObj, const char* s)
{
    if (hObj)
    {
        IMAGETEXT_Obj* pObj;
        WM_LOCK();
        pObj = IMAGETEXT_H2P(hObj);
        if (GUI__SetText(&pObj->hpText, s))
        {
            IMAGETEXT_Invalidate(hObj);
        }
        WM_UNLOCK();
    }
}
/***************************************************************************************************
*
*       IMAGETEXT_SetFont
*/
void IMAGETEXT_SetFont(IMAGETEXT_Handle hObj, const GUI_FONT GUI_UNI_PTR * pfont)
{
    if (hObj)
    {
        IMAGETEXT_Obj* pObj;
        WM_LOCK();
        pObj = IMAGETEXT_H2P(hObj);
        IMAGETEXT_ASSERT_IS_VALID_PTR(pObj);
        pObj->Props.pFont = pfont;
        IMAGETEXT_Invalidate(hObj);
        WM_UNLOCK();
    }
}
/***************************************************************************************************
*
*       IMAGETEXT_SetBkColor
*/
void IMAGETEXT_SetBkColor(IMAGETEXT_Handle hObj, GUI_COLOR Color)
{
    if (hObj)
    {
        IMAGETEXT_Obj* pObj;
        WM_LOCK();
        pObj = IMAGETEXT_H2P(hObj);
        IMAGETEXT_ASSERT_IS_VALID_PTR(pObj);
        pObj->Props.aBkColor = Color;
        IMAGETEXT_Invalidate(hObj);
        WM_UNLOCK();
    }
}

void IMAGETEXT_SetFrameColor(IMAGETEXT_Handle hObj, GUI_COLOR Color)
{
	if (hObj)
	{
		IMAGETEXT_Obj* pObj;
		WM_LOCK();
		pObj = IMAGETEXT_H2P(hObj);
		IMAGETEXT_ASSERT_IS_VALID_PTR(pObj);
		pObj->Props.aFrameColor = Color;
		IMAGETEXT_Invalidate(hObj);
		WM_UNLOCK();
	}
}
/***************************************************************************************************
*
*       IMAGETEXT_SetTextColor
*/
void IMAGETEXT_SetTextColor(IMAGETEXT_Handle hObj, GUI_COLOR Color)
{
    if (hObj)
    {
        IMAGETEXT_Obj* pObj;
        WM_LOCK();
        pObj = IMAGETEXT_H2P(hObj);
        IMAGETEXT_ASSERT_IS_VALID_PTR(pObj);
        pObj->Props.aTextColor = Color;
        IMAGETEXT_Invalidate(hObj);
        WM_UNLOCK();
    }
}
/***************************************************************************************************
*
*       IMAGETEXT_GetBkColor
*/
GUI_COLOR IMAGETEXT_GetBkColor(IMAGETEXT_Handle hObj)
{
    GUI_COLOR Color = 0;
    if (hObj)
    {
        IMAGETEXT_Obj * pObj;
        WM_LOCK();
        pObj = IMAGETEXT_H2P(hObj);
        Color = pObj->Props.aBkColor;
        WM_UNLOCK();
    }
    return Color;
}
/***************************************************************************************************
*
*       IMAGETEXT_GetFont
*/
const GUI_FONT GUI_UNI_PTR * IMAGETEXT_GetFont(IMAGETEXT_Handle hObj)
{
    const GUI_FONT GUI_UNI_PTR * pFont = 0;
    if (hObj)
    {
        IMAGETEXT_Obj * pObj;
        WM_LOCK();
        pObj = IMAGETEXT_H2P(hObj);
        pFont = pObj->Props.pFont;
        WM_UNLOCK();
    }
    return pFont;
}
/***************************************************************************************************
*
*       IMAGETEXT_GetText
*/
void IMAGETEXT_GetText(IMAGETEXT_Handle hObj, char * pBuffer, int MaxLen)
{
    if (hObj)
    {
        IMAGETEXT_Obj * pObj;
        WM_LOCK();
        pObj = IMAGETEXT_H2P(hObj);
        if (pObj->hpText)
        {
            const char * pText = (const char*) GUI_ALLOC_h2p(pObj->hpText);
            int Len = strlen(pText);
            if (Len > (MaxLen - 1))
            {
                Len = MaxLen - 1;
            }
            memcpy((void *)pBuffer, (const void *)pText, Len);
            *(pBuffer + Len) = 0;
        }
        else
        {
            *pBuffer = 0;     /* Empty string */
        }
        WM_UNLOCK();
    }
}
/***************************************************************************************************
*
*       IMAGETEXT_GetTextColor
*/
GUI_COLOR IMAGETEXT_GetTextColor(IMAGETEXT_Handle hObj)
{
    GUI_COLOR Color = 0;
    if (hObj)
    {
        IMAGETEXT_Obj * pObj;
        WM_LOCK();
        pObj = IMAGETEXT_H2P(hObj);
        Color = pObj->Props.aTextColor;
        WM_UNLOCK();
    }
    return Color;
}
/***************************************************************************************************
*
*       IMAGETEXT_SetTextAlign
*/
void IMAGETEXT_SetTextAlign(IMAGETEXT_Handle hObj, int Align)
{
    if (hObj)
    {
        IMAGETEXT_Obj* pObj;
        WM_LOCK();
        pObj = IMAGETEXT_H2P(hObj);
        IMAGETEXT_ASSERT_IS_VALID_PTR(pObj);
        pObj->Props.Align = Align;
        IMAGETEXT_Invalidate(hObj);
        WM_UNLOCK();
    }
}
void IMAGETEXT_DeleteBitmap(IMAGETEXT_Handle hObj)
{
	if (hObj)
	{
		IMAGETEXT_Obj* pObj;
		WM_LOCK();
		pObj = IMAGETEXT_H2P(hObj);
		IMAGETEXT_ASSERT_IS_VALID_PTR(pObj);
		GUI_ALLOC_FreePtr(&pObj->ahDrawObj);
		IMAGETEXT_Invalidate(hObj);
		WM_UNLOCK();
	}
}
void IMAGETEXT_SetBitmapTextFlag(IMAGETEXT_Handle hObj, unsigned char Flg)
{
	if (hObj)
	{
		IMAGETEXT_Obj* pObj;
		WM_LOCK();
		pObj = IMAGETEXT_H2P(hObj);
		IMAGETEXT_ASSERT_IS_VALID_PTR(pObj);
		pObj->BitmapTextFlag = Flg;
		IMAGETEXT_Invalidate(hObj);
		WM_UNLOCK();
	}
}
/***************************************************************************************************
*
*       IMAGETEXT_SetBitmapEx
*/
void IMAGETEXT_SetBitmapEx(IMAGETEXT_Handle hObj, const GUI_BITMAP * pBitmap, int x, int y)
{
    IMAGETEXT__SetDrawObj(hObj, GUI_DRAW_BITMAP_Create(pBitmap, x, y));
}
/***************************************************************************************************
*
*       IMAGETEXT_SetBitmap
*/
void IMAGETEXT_SetBitmap(IMAGETEXT_Handle hObj, const GUI_BITMAP* pBitmap)
{
    IMAGETEXT_SetBitmapEx(hObj, pBitmap, 0, 0);
}
/***************************************************************************************************
*
*       IMAGETEXT_SetDrawObj
*/
void IMAGETEXT__SetDrawObj(IMAGETEXT_Handle hObj, GUI_DRAW_HANDLE hDrawObj)
{
    if (hObj)
    {
        IMAGETEXT_Obj* pObj;
        WM_LOCK();
        pObj = IMAGETEXT_H2P(hObj);
        IMAGETEXT_ASSERT_IS_VALID_PTR(pObj);
        GUI_ALLOC_FreePtr(&pObj->ahDrawObj);
        pObj->ahDrawObj = hDrawObj;
        IMAGETEXT_Invalidate(hObj);
        WM_UNLOCK();
    }
}
/*********************************************************************
*
*       IMAGETEXT_SetWrapMode
*/
void IMAGETEXT_SetWrapMode(IMAGETEXT_Handle hObj,GUI_WRAPMODE Mode)
{
	if (hObj)
	{
		IMAGETEXT_Obj* pObj;
		WM_LOCK();
		pObj = IMAGETEXT_H2P(hObj);
		IMAGETEXT_ASSERT_IS_VALID_PTR(pObj);
		pObj->Props.WrapMode = Mode;
		IMAGETEXT_Invalidate(hObj);
		WM_UNLOCK();
	}
}
/***************************************************************************************************
*
*       IMAGETEXT_SetBMPEx
*/
void IMAGETEXT_SetBMPEx(IMAGETEXT_Handle hObj, const void * pBitmap, int x, int y)
{
    IMAGETEXT__SetDrawObj(hObj, GUI_DRAW_BMP_Create(pBitmap, x, y));
}
/***************************************************************************************************
*
*       IMAGETEXT_SetBMP
*/
void IMAGETEXT_SetBMP(IMAGETEXT_Handle hObj, const void * pBitmap)
{
    IMAGETEXT_SetBMPEx(hObj, pBitmap, 0, 0);
}
/***************************************************************************************************
*
*       IMAGETEXT_SetSelfDrawEx
*/
void IMAGETEXT_SetSelfDrawEx(IMAGETEXT_Handle hObj, GUI_DRAW_SELF_CB* pDraw, int x, int y)
{
    IMAGETEXT__SetDrawObj(hObj, GUI_DRAW_SELF_Create(pDraw, x, y));
}
/***************************************************************************************************
*
*       IMAGETEXT_SetSelfDraw
*/
void IMAGETEXT_SetSelfDraw(IMAGETEXT_Handle hObj, GUI_DRAW_SELF_CB* pDraw)
{
    IMAGETEXT_SetSelfDrawEx(hObj, pDraw, 0, 0);
}

//////////////////////////////////////////////////////////////////////////

void IMAGETEXT_SetStyle(IMAGETEXT_Handle hObj, int style)
{
	if (hObj)
	{
		IMAGETEXT_Obj* pObj;
		WM_LOCK();
		pObj = IMAGETEXT_H2P(hObj);
		IMAGETEXT_ASSERT_IS_VALID_PTR(pObj);
		pObj->nStyle = style;
		IMAGETEXT_Invalidate(hObj);
		WM_UNLOCK();
	}
}
void IMAGETEXT_SetState(IMAGETEXT_Handle hObj, int state)
{
	if (hObj)
	{
		IMAGETEXT_Obj* pObj;
		WM_LOCK();
		pObj = IMAGETEXT_H2P(hObj);
		IMAGETEXT_ASSERT_IS_VALID_PTR(pObj);
		pObj->nState = state;
		IMAGETEXT_Invalidate(hObj);
		WM_UNLOCK();
	}
}


#else                            /* Avoid problems with empty object modules */
void IMAGETEXT_C(void) {}
#endif /* GUI_WINSUPPORT */
