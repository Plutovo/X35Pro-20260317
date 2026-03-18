/*
****************************************************************************************************
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
----------------------------------------------------------------------------------------------------
File        : DataBar.c
Purpose     : Implementation of progress bar
---------------------------END-OF-HEADER------------------------------------------------------------
*/

#include <stdlib.h>
#include <string.h>
#include "GUI_Protected.h"
#include "DATABAR.h"
#include "WIDGET.h"
//
#if GUI_WINSUPPORT
/***************************************************************************************************
*
*       Private config defaults
*
***************************************************************************************************/
#ifndef DATABAR_DEFAULT_FONT
#define DATABAR_DEFAULT_FONT &GUI_Font13HB_1
#endif
#ifndef DATABAR_DEFAULT_BARCOLOR0
#define DATABAR_DEFAULT_BARCOLOR0 0x555555
#endif
#ifndef DATABAR_DEFAULT_BARCOLOR1
#define DATABAR_DEFAULT_BARCOLOR1 GUI_BLUE
#endif
#ifndef DATABAR_DEFAULT_TEXTCOLOR0
#define DATABAR_DEFAULT_TEXTCOLOR0 GUI_BLACK
#endif
#ifndef DATABAR_DEFAULT_TEXTCOLOR1
#define DATABAR_DEFAULT_TEXTCOLOR1 GUI_BLUE
#endif
#ifndef DATABAR_DEFAULT_BKCOLOR
#define DATABAR_DEFAULT_BKCOLOR GUI_YAN_BLUE
#endif

/***************************************************************************************************
*
*       Object definition
*
***************************************************************************************************/
typedef struct
{
    WIDGET Widget;
    const GUI_FONT GUI_UNI_PTR * pFont;
    const GUI_FONT GUI_UNI_PTR * pFont1;
    GUI_COLOR BarColor[2];
    GUI_COLOR TextColor[2];
    GUI_COLOR BkColor;
    char HV;                      //横向显示，纵向显示        ('H' or 'V')   -- or |
    char Type;

    //I16 XOff, YOff;
    float Base,Min, Max;      //min must low or equal to zero
    float v;
    char* Unit;
    char* Name;

    /*  I16 Options; */
#if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
    int DebugId;
#endif
} DATABAR_Obj;

/***************************************************************************************************
*
*       Macros for internal use
*
***************************************************************************************************/
#define Invalidate(h) WM_InvalidateWindow(h)
#if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
#define OBJECT_ID 0x4569   /* Magic nubmer, should be unique if possible */
#define INIT_ID(p)   p->DebugId = OBJECT_ID
#define DEINIT_ID(p) p->DebugId = OBJECT_ID+1
#else
#define INIT_ID(p)
#define DEINIT_ID(p)
#endif

/***************************************************************************************************
*
*       Static routines
*
***************************************************************************************************/
#if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
DATABAR_Obj* DATABAR_h2p(DATABAR_Handle h)
{
    DATABAR_Obj* p = (DATABAR_Obj*)GUI_ALLOC_h2p(h);
    if (p)
    {
        if (p->DebugId != OBJECT_ID)
        {
            GUI_DEBUG_ERROROUT("DATABAR.C: Wrong handle type or Object not init'ed");
            return 0;
        }
    }
    return p;
}
#define DATABAR_H2P(h) DATABAR_h2p(h)
#else
#define DATABAR_H2P(h) (DATABAR_Obj*) GUI_ALLOC_h2p(h)
#endif

/***************************************************************************************************
*
*       _Value2X
*/
static int _Value2X(const DATABAR_Obj* pObj, int v)
{
    int EffectSize = pObj->Widget.pEffect->EffectSize;
    int xSize = pObj->Widget.Win.Rect.x1 - pObj->Widget.Win.Rect.x0 + 1;
    int Min   = pObj->Min;
    int Max   = pObj->Max;
    if (v < Min)
    {
        v = Min;
    }
    if (v > Max)
    {
        v = Max;
    }
    return EffectSize + ((xSize - 2 * EffectSize) * (I32)(v - Min)) / (Max - Min);
}

/***************************************************************************************************
*
*       _Value2Y
*/
static int _Value2Y(const DATABAR_Obj* pObj, int v)
{
    int EffectSize = pObj->Widget.pEffect->EffectSize;
    int ySize = pObj->Widget.Win.Rect.y1 - pObj->Widget.Win.Rect.y0 + 1;
    int Min   = pObj->Min;
    int Max   = pObj->Max;
    if (v < Min)
    {
        v = Min;
    }
    if (v > Max)
    {
        v = Max;
    }
    return EffectSize + ((ySize - 2 * EffectSize) * (I32)(v - Min)) / (Max - Min);
}

/***************************************************************************************************
*
*       _DrawPart
*/
static void _DrawPart(const DATABAR_Obj* pObj, int Index,
                      int xText, int yText, const char* pText)
{
    LCD_SetBkColor(pObj->BarColor[Index]);
    LCD_SetColor(pObj->TextColor[Index]);
    GUI_Clear();
    GUI_GotoXY(xText, yText);
    GUI_DispString(pText);
}

/***************************************************************************************************
*
*       _Paint
*/
static void _Paint(DATABAR_Handle hObj)
{
    DATABAR_Obj* pObj;
    GUI_RECT r, rInside, rClient, rText;
    const char* pText;
// char ac[5];
    float num;
    int LableStep,num1 =1,num2 = 0;
    float data = 1,data1;
    pObj = DATABAR_H2P(hObj);
    //GUI_GetClientRect(&r);
    WM_GetClientRect(&r);
    GUI_SetBkColor(pObj->BkColor);
    GUI_Clear();
    rClient.x0 = r.x0 + 25;
    rClient.x1 = r.x1 - 5;
    rClient.y0 = r.y0 + (r.y1 - r.y0)/10;
    rClient.y1 = r.y1 - (r.y1 - r.y0)/8;
    WIDGET__EFFECT_DrawDownRect(&pObj->Widget, &rClient);
    GUI_SetFont(pObj->pFont1);
    LCD_SetColor(pObj->TextColor[0]);
    GUI_SetTextAlign(GUI_TA_CENTER);
    GUI_DispStringAt(pObj->Name,rClient.x0+(rClient.x1 - rClient.x0)/2,
                     rClient.y1 + 8);
    LCD_SetColor(pObj->TextColor[1]);
    GUI_SetTextAlign(GUI_TA_CENTER);
    GUI_GotoXY(rClient.x0+(rClient.x1 - rClient.x0)/2,rClient.y0-20);
    if(pObj->v<10)
    {
        GUI_DispFloat(pObj->v,3);
    }
    else
    {
        GUI_DispFloat(pObj->v,2);
    }
    GUI_SetFont(pObj->pFont);
    GUI_SetTextAlign(GUI_TA_CENTER);
    GUI_DispStringAt(pObj->Unit,rClient.x0+(rClient.x1 - rClient.x0)/2,
                     rClient.y1 + 25);
    num = (float)((pObj->Max - pObj->Min)/pObj->Base);
    while((num/2)!=pObj->Base)
    {
        num /= 2;
        num1++;
    }
    LableStep = (rClient.y1 - rClient.y0)*0.95/num1;
    GUI_SetColor(pObj->BarColor[0]);
    GUI_DrawHLine(rClient.y1-2,rClient.x0-5,rClient.x0-1);                  //change
    GUI_DispDecAt(0,rClient.x0-20,rClient.y1-5,1);
    while((num1>0))
    {
        num1--;
        num2++;
        GUI_DrawHLine(rClient.y1-LableStep*num2-2,rClient.x0-5,rClient.x0-1);      //change
        data1 = data*pObj->Base;
        data = data * 2;
        GUI_GotoXY(rClient.x0-22,rClient.y1-LableStep*num2-5);
        if(data1<10)
        {
            GUI_DispFloat(data1,3);
        }
        else
        {
            GUI_DispFloat(data1,2);
        }
    }
    GUI_SetColor(pObj->BarColor[1]);
    if(pObj->v > pObj->Max)
    {
        GUI_FillRect(rClient.x0+1,rClient.y1-LableStep*num2+1,rClient.x1-1,rClient.y1-1);
    }
    else
    {
        num = 1;
        num1 = 1;
        while(pObj->v > num*pObj->Base)
        {
            num1++;
            num *= 2;
        }
        data = (pObj->v - num*pObj->Base/2)*LableStep/num*pObj->Base/2;
        data1 = data + (num1-1)*LableStep;
        GUI_FillRect(rClient.x0+2,rClient.y1 - data1 -2,rClient.x1-2,rClient.y1-2);
    }
    /* Draw left bar */
    /* r    = rInside;

     if(pObj->HV == 'H'){
    xPos  = _Value2X(pObj, pObj->v);
    r.x1 = xPos - 1;
     }
     else if(pObj->HV == 'V'){
    yPos  = _Value2Y(pObj, pObj->v);
    r.y0 = rInside.y1 - yPos;
     }
     WM_SetUserClipArea(&r);
     _DrawPart(pObj, 0, rText.x0, rText.y0, pText);
     /* Draw right bar */
    /* r    = rInside;

     if(pObj->HV == 'H'){
    r.x0 = xPos;
     }
     else if(pObj->HV == 'V'){
    r.y1 = rInside.y1 - yPos - 1;
     }
     WM_SetUserClipArea(&r);
     _DrawPart(pObj, 1, rText.x0, rText.y0, pText);
     WM_SetUserClipArea(NULL);
     GUI_SetTextMode(tm);


     WM_GetInsideRectEx(hObj,&rClient);
     GUI_SetColor(pObj->BarColor[1]);
     GUI_DrawRectEx(&rClient);*/
}

/***************************************************************************************************
*
*       _Delete
*/
static void _Delete(DATABAR_Handle hObj)
{
    //_FreeText(hObj);
    DEINIT_ID(DATABAR_H2P(hObj));
}

/***************************************************************************************************
*
*       _Callback
*/
static void _DATABAR_Callback(WM_MESSAGE*pMsg)
{
    DATABAR_Handle hObj = (DATABAR_Handle)pMsg->hWin;
    /* Let widget handle the standard messages */
    if (WIDGET_HandleActive(hObj, pMsg) == 0)
    {
        return;
    }
    switch (pMsg->MsgId)
    {
    case WM_PAINT:
        _Paint(hObj);
        return;
    case WM_DELETE:
        _Delete(hObj);
        break;
    }
    WM_DefaultProc(pMsg);
}

/***************************************************************************************************
*
*       Exported routines:  Create
*
***************************************************************************************************/

/***************************************************************************************************
*
*       DATABAR_CreateEx
*/
DATABAR_Handle DATABAR_CreateEx(int x0, int y0, int xsize, int ysize, WM_HWIN hParent,
                                int WinFlags, int ExFlags, int Id)
{
    DATABAR_Handle hObj;
    GUI_USE_PARA(ExFlags);
    hObj = WM_CreateWindowAsChild(x0, y0, xsize, ysize, hParent, WinFlags, _DATABAR_Callback,
                                  sizeof(DATABAR_Obj) - sizeof(WM_Obj));
    if (hObj)
    {
        DATABAR_Obj* pObj;
        WM_LOCK();
        pObj = (DATABAR_Obj*) GUI_ALLOC_h2p(hObj);
        /* init widget specific variables */
        WIDGET__Init(&pObj->Widget, Id, 0);
        //WIDGET_SetEffect(hObj, &WIDGET_Effect_None); /* Standard effect for DATABAR: None */
        INIT_ID(pObj);
        /* init member variables */
        pObj->pFont        = GUI_DEFAULT_FONT;
        pObj->pFont1        = DATABAR_DEFAULT_FONT;
        pObj->BarColor[0]  = DATABAR_DEFAULT_BARCOLOR0;
        pObj->BarColor[1]  = DATABAR_DEFAULT_BARCOLOR1;
        pObj->TextColor[0] = DATABAR_DEFAULT_TEXTCOLOR0;
        pObj->TextColor[1] = DATABAR_DEFAULT_TEXTCOLOR1;
        pObj->BkColor      = DATABAR_DEFAULT_BKCOLOR;
        pObj->Base     = 0.5;
        pObj->Type         = 'I';
        pObj->Max          = 16;
        pObj->Min          = 0;
        pObj->Name         = "O2";
        pObj->Unit         = "L/min";
        pObj->HV           = 'H';
        pObj->v            = 10;
        WM_UNLOCK();
    }
    return hObj;
}

/***************************************************************************************************
*
*       DATABAR_CreateIndirect
*/
DATABAR_Handle  DATABAR_CreateIndirect(const GUI_WIDGET_CREATE_INFO* pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK* cb)
{
    DATABAR_Handle  hThis;
    GUI_USE_PARA(cb);
    hThis = DATABAR_CreateEx(pCreateInfo->x0 + x0, pCreateInfo->y0 + y0, 
                             pCreateInfo->xSize, pCreateInfo->ySize,
                             hWinParent, 0, pCreateInfo->Flags, pCreateInfo->Id);
    return hThis;
}

/***************************************************************************************************
*
*       Exported routines:  Various methods
*
***************************************************************************************************/

/***************************************************************************************************
*
*       DATABAR_SetValue
*/
void DATABAR_SetValue(DATABAR_Handle hObj, int v)
{
    if (hObj)
    {
        DATABAR_Obj* pObj;
        WM_LOCK();
        pObj= DATABAR_H2P(hObj);
        /* Put v into legal range */
        if (v < pObj->Min)
        {
            v = pObj->Min;
        }
        if (v > pObj->Max)
        {
            v = pObj->Max;
        }
        pObj->v = v;
        WM_InvalidateWindow(hObj);
        WM_UNLOCK();
    }
}

/***************************************************************************************************
*
*       DATABAR_SetFont
*/
void DATABAR_SetFont(DATABAR_Handle hObj, const GUI_FONT GUI_UNI_PTR * pfont)
{
    DATABAR_Obj* pObj;
    if (hObj)
    {
        WM_LOCK();
        pObj = DATABAR_H2P(hObj);
        pObj->pFont = pfont;
        WM_InvalidateWindow(hObj);
        WM_UNLOCK();
    }
}

/***************************************************************************************************
*
*       DATABAR_SetBarColor
*/
void DATABAR_SetBarColor(DATABAR_Handle hObj, unsigned int Index, GUI_COLOR color)
{
    DATABAR_Obj* pObj;
    if (hObj)
    {
        WM_LOCK();
        pObj = DATABAR_H2P(hObj);
        if (Index < GUI_COUNTOF(pObj->BarColor))
        {
            pObj->BarColor[Index] = color;
            WM_InvalidateWindow(hObj);
        }
        WM_UNLOCK();
    }
}

/***************************************************************************************************
*
*       DATABAR_SetTextColor
*/
void DATABAR_SetTextColor(DATABAR_Handle hObj, unsigned int Index, GUI_COLOR color)
{
    DATABAR_Obj* pObj;
    if (hObj)
    {
        WM_LOCK();
        pObj = DATABAR_H2P(hObj);
        if (Index < GUI_COUNTOF(pObj->TextColor))
        {
            pObj->TextColor[Index] = color;
            WM_InvalidateWindow(hObj);
        }
        WM_UNLOCK();
    }
}

/***************************************************************************************************
*
*       DATABAR_SetText
*/
void DATABAR_SetName(DATABAR_Handle hObj, char* s)
{
    if (hObj)
    {
        DATABAR_Obj* pObj;
        WM_LOCK();
        pObj = DATABAR_H2P(hObj);
        pObj->Name = s;
        WM_InvalidateWindow(hObj);
        WM_UNLOCK();
    }
}

/***************************************************************************************************
*
*       DATABAR_SetTextPos
*
void DATABAR_SetTextPos(DATABAR_Handle hObj, int XOff, int YOff) {
  DATABAR_Obj* pObj;
  if (hObj) {
    WM_LOCK();
    pObj = DATABAR_H2P(hObj);
    pObj->XOff = XOff;
    pObj->YOff = YOff;
    WM_InvalidateWindow(hObj);
    WM_UNLOCK();
  }
}

/***************************************************************************************************
*
*       DATABAR_SetMinMax
*/
void DATABAR_SetMinMax(DATABAR_Handle hObj, int Min, int Max)
{
    DATABAR_Obj* pObj;
    if (hObj)
    {
        WM_LOCK();
        pObj = DATABAR_H2P(hObj);
        if (Max > Min)
        {
            if ((Max != pObj->Max) || (Min != pObj->Min))
            {
                pObj->Min = Min;
                pObj->Max = Max;
                WM_InvalidateWindow(hObj);
            }
        }
        WM_UNLOCK();
    }
}

/***************************************************************************************************
*
*       DATABAR_SetBkColor
*/
void DATABAR_SetBkColor(DATABAR_Handle hObj, GUI_COLOR color)
{
    DATABAR_Obj* pObj;
    if (hObj)
    {
        WM_LOCK();
        pObj = DATABAR_H2P(hObj);
        pObj->BkColor = color;
        WM_InvalidateWindow(hObj);
        WM_UNLOCK();
    }
}

/***************************************************************************************************
*
*       DATABAR_SetHV
*/
void DATABAR_SetHV(DATABAR_Handle hObj, char Sign)
{
    DATABAR_Obj* pObj;
    if (hObj)
    {
        WM_LOCK();
        pObj = DATABAR_H2P(hObj);
        pObj->HV = Sign;
        WM_InvalidateWindow(hObj);
        WM_UNLOCK();
    }
}

#else
void WIDGET_DATABAR(void) {} /* avoid empty object files */
#endif /* GUI_WINSUPPORT */


