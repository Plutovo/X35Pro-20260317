//
#include "DATA_FRAME.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include "TextExCtrl.h"
//
/***************************************************************************************************
*
*       Private config defaults
*
****************************************************************************************************
*/
/* Define colors */
#ifndef   DATA_FRAME_BKCOLOR_DEFAULT
#define DATA_FRAME_BKCOLOR_DEFAULT   GUI_YAN_BLUE
#endif
#ifndef   DATA_FRAME_FRAMECOLOR_DEFAULT
#define DATA_FRAME_FRAMECOLOR_DEFAULT  0x404040
#endif
#ifndef   DATA_FRAME_DATACOLOR_DEFAULT
#define DATA_FRAME_DATACOLOR_DEFAULT  GUI_BLUE
#endif
#ifndef   DATA_FRAME_TEXTCOLOR_DEFAULT
#define DATA_FRAME_TEXTCOLOR_DEFAULT  GUI_BLACK
#endif
#ifndef   DATA_FRAME_DATA_DEFAULT
#define DATA_FRAME_DATA_DEFAULT           0
#endif
#ifndef   DATA_FRAME_DATAFONT_DEFAULT
#define DATA_FRAME_DATAFONT_DEFAULT       &GUI_Font_16
#endif
#ifndef   DATA_FRAME_TEXTFONT_DEFAULT
#define DATA_FRAME_TEXTFONT_DEFAULT       &GUI_Font_16
#endif
#ifndef   DATA_FRAME_HEADFONT_DEFAULT
#define DATA_FRAME_HEADFONT_DEFAULT       &GUI_Font_16
#endif
#ifndef   DATA_FRAME_UNITFONT_DEFAULT
#define DATA_FRAME_UNITFONT_DEFAULT       &GUI_Font_15//GUI_Font13B_1
#endif
#ifndef   DATA_FRAME_RTDATAFONT_DEFAULT
#define DATA_FRAME_RTDATAFONT_DEFAULT     &GUI_Font_24
#endif
extern char* _Float2String(float fData,int floatLen);


/***************************************************************************************************
*
*       Static routines
*
***************************************************************************************************/

/***************************************************************************************************
*
*       _Paint
*/
static void _Paint(DATA_FRAME_Obj* pObj, DATA_FRAME_Handle hObj)
{
    GUI_RECT r;
    char * pTemp;
    int  base = 0;
    GUI_GetClientRect(&r);
    //int xsize = pObj->Widget.Win.Rect.x1 - pObj->Widget.Win.Rect.x0;
    //int ysize = pObj->Widget.Win.Rect.y1 - pObj->Widget.Win.Rect.y0;
    GUI_SetColor  (pObj->BkColor);
    GUI_FillRect  (r.x0, r.y0, r.x1, r.y1);
    GUI_SetColor  (pObj->FrameColor);
    GUI_DrawRect(r.x0+2,r.y0+8,r.x1-2,r.y1-2);
    //GUI_DrawRect(r.x0+3,r.y0+3,r.x1-3,r.y1-3);
    GUI_SetColor (0xe0e0e0);
    GUI_DrawVLine(r.x0+3,r.y0+9,r.y1-3);
    GUI_DrawVLine(r.x1-1,r.y0+9,r.y1-1);
    GUI_DrawHLine(r.y0+9,r.x0+3,r.x1-3);
    GUI_DrawHLine(r.y1-1,r.x0+3,r.x1-1);
    GUI_SetTextMode(GUI_TM_NORMAL);
    GUI_SetColor (pObj->TextColor);
    GUI_SetBkColor (pObj->BkColor);
    GUI_SetFont(pObj->pHeadFont);
    GUI_SetTextAlign(GUI_TA_LEFT|GUI_TA_TOP);
    GUI_DispStringAt(pObj->HeadText,r.x0+12,r.y0);
    if(pObj->DisplayFlag)
    {
        GUI_SetColor (pObj->DataColor);
        GUI_SetFont(pObj->pRealtimeDataFont);
        pTemp = _Float2String(pObj->RT_Value,1);
        GUI_DispStringAt(pTemp,r.x0+60,r.y0+40);
        GUI_SetColor (pObj->TextColor);
        GUI_SetFont(pObj->pUnitFont);
        GUI_DispStringAt(pObj->RT_Unit,r.x0+130,r.y0+40);
        base = 40;
    }
    GUI_SetColor (pObj->TextColor);
    GUI_SetFont(pObj->pTextFont);
    if(pObj->ParameterNum)
    {
        switch(pObj->ParameterNum)
        {
        case 1:
            GUI_DispStringAt(pObj->data0_text,r.x0+30,r.y0+40+base);
            pTemp = _Float2String(pObj->data0,1);
            GUI_DispStringAt(pTemp,r.x0+(r.x1-r.x0)*7/12,r.y0+40+base);
            GUI_DispStringAt(pObj->data0_Unit,r.x0+(r.x1-r.x0)*9/12,r.y0+40+base);
            break;
        case 2:
            GUI_DispStringAt(pObj->data0_text,r.x0+30,r.y0+40+base);
            pTemp = _Float2String(pObj->data0,1);
            GUI_DispStringAt(pTemp,r.x0+(r.x1-r.x0)*7/12,r.y0+40+base);
            GUI_DispStringAt(pObj->data0_Unit,r.x0+(r.x1-r.x0)*9/12,r.y0+40+base);
            GUI_DispStringAt(pObj->data1_text,r.x0+30,r.y0+65+base);
            pTemp = _Float2String(pObj->data1,1);
            GUI_DispStringAt(pTemp,r.x0+(r.x1-r.x0)*7/12,r.y0+65+base);
            GUI_DispStringAt(pObj->data1_Unit,r.x0+(r.x1-r.x0)*9/12,r.y0+65+base);
            break;
        case 3:
            GUI_DispStringAt(pObj->data0_text,r.x0+30,r.y0+40+base);
            pTemp = _Float2String(pObj->data0,1);
            GUI_DispStringAt(pTemp,r.x0+(r.x1-r.x0)*7/12,r.y0+40+base);
            GUI_DispStringAt(pObj->data0_Unit,r.x0+(r.x1-r.x0)*9/12,r.y0+40+base);
            GUI_DispStringAt(pObj->data1_text,r.x0+30,r.y0+65+base);
            pTemp = _Float2String(pObj->data1,1);
            GUI_DispStringAt(pTemp,r.x0+(r.x1-r.x0)*7/12,r.y0+65+base);
            GUI_DispStringAt(pObj->data1_Unit,r.x0+(r.x1-r.x0)*9/12,r.y0+65+base);
            GUI_DispStringAt(pObj->data2_text,r.x0+30,r.y0+90+base);
            pTemp = _Float2String(pObj->data2,1);
            GUI_DispStringAt(pTemp,r.x0+(r.x1-r.x0)*7/12,r.y0+90+base);
            GUI_DispStringAt(pObj->data2_Unit,r.x0+(r.x1-r.x0)*9/12,r.y0+90+base);
            break;
        case 4:
            GUI_DispStringAt(pObj->data0_text,r.x0+30,r.y0+40+base);
            pTemp = _Float2String(pObj->data0,1);
            GUI_DispStringAt(pTemp,r.x0+(r.x1-r.x0)*7/12,r.y0+40+base);
            GUI_DispStringAt(pObj->data0_Unit,r.x0+(r.x1-r.x0)*9/12,r.y0+40+base);
            GUI_DispStringAt(pObj->data1_text,r.x0+30,r.y0+65+base);
            pTemp = _Float2String(pObj->data1,1);
            GUI_DispStringAt(pTemp,r.x0+(r.x1-r.x0)*7/12,r.y0+65+base);
            GUI_DispStringAt(pObj->data1_Unit,r.x0+(r.x1-r.x0)*9/12,r.y0+65+base);
            GUI_DispStringAt(pObj->data2_text,r.x0+30,r.y0+90+base);
            pTemp = _Float2String(pObj->data2,1);
            GUI_DispStringAt(pTemp,r.x0+(r.x1-r.x0)*7/12,r.y0+90+base);
            GUI_DispStringAt(pObj->data2_Unit,r.x0+(r.x1-r.x0)*9/12,r.y0+90+base);
            GUI_DispStringAt(pObj->data3_text,r.x0+30,r.y0+115+base);
            pTemp = _Float2String(pObj->data3,1);
            GUI_DispStringAt(pTemp,r.x0+(r.x1-r.x0)*7/12,r.y0+115+base);
            GUI_DispStringAt(pObj->data3_Unit,r.x0+(r.x1-r.x0)*9/12,r.y0+115+base);
            break;
        case 5:
            GUI_DispStringAt(pObj->data0_text,r.x0+30,r.y0+40+base);
            pTemp = _Float2String(pObj->data0,1);
            GUI_DispStringAt(pTemp,r.x0+(r.x1-r.x0)*7/12,r.y0+40+base);
            GUI_DispStringAt(pObj->data0_Unit,r.x0+(r.x1-r.x0)*9/12,r.y0+40+base);
            GUI_DispStringAt(pObj->data1_text,r.x0+30,r.y0+65+base);
            pTemp = _Float2String(pObj->data1,1);
            GUI_DispStringAt(pTemp,r.x0+(r.x1-r.x0)*7/12,r.y0+65+base);
            GUI_DispStringAt(pObj->data1_Unit,r.x0+(r.x1-r.x0)*9/12,r.y0+65+base);
            GUI_DispStringAt(pObj->data2_text,r.x0+30,r.y0+90+base);
            pTemp = _Float2String(pObj->data2,1);
            GUI_DispStringAt(pTemp,r.x0+(r.x1-r.x0)*7/12,r.y0+90+base);
            GUI_DispStringAt(pObj->data2_Unit,r.x0+(r.x1-r.x0)*9/12,r.y0+90+base);
            GUI_DispStringAt(pObj->data3_text,r.x0+30,r.y0+115+base);
            pTemp = _Float2String(pObj->data3,1);
            GUI_DispStringAt(pTemp,r.x0+(r.x1-r.x0)*7/12,r.y0+115+base);
            GUI_DispStringAt(pObj->data3_Unit,r.x0+(r.x1-r.x0)*9/12,r.y0+115+base);
            GUI_DispStringAt(pObj->data4_text,r.x0+30,r.y0+140+base);
            pTemp = _Float2String(pObj->data4,1);
            GUI_DispStringAt(pTemp,r.x0+(r.x1-r.x0)*7/12,r.y0+140+base);
            GUI_DispStringAt(pObj->data4_Unit,r.x0+(r.x1-r.x0)*9/12,r.y0+140+base);
            break;
        case 6:
            GUI_DispStringAt(pObj->data0_text,r.x0+30,r.y0+40+base);
            pTemp = _Float2String(pObj->data0,1);
            GUI_DispStringAt(pTemp,r.x0+(r.x1-r.x0)*7/12,r.y0+40+base);
            GUI_DispStringAt(pObj->data0_Unit,r.x0+(r.x1-r.x0)*9/12,r.y0+40+base);
            GUI_DispStringAt(pObj->data1_text,r.x0+30,r.y0+65+base);
            pTemp = _Float2String(pObj->data1,1);
            GUI_DispStringAt(pTemp,r.x0+(r.x1-r.x0)*7/12,r.y0+65+base);
            GUI_DispStringAt(pObj->data1_Unit,r.x0+(r.x1-r.x0)*9/12,r.y0+65+base);
            GUI_DispStringAt(pObj->data2_text,r.x0+30,r.y0+90+base);
            pTemp = _Float2String(pObj->data2,1);
            GUI_DispStringAt(pTemp,r.x0+(r.x1-r.x0)*7/12,r.y0+90+base);
            GUI_DispStringAt(pObj->data2_Unit,r.x0+(r.x1-r.x0)*9/12,r.y0+90+base);
            GUI_DispStringAt(pObj->data3_text,r.x0+30,r.y0+115+base);
            pTemp = _Float2String(pObj->data3,1);
            GUI_DispStringAt(pTemp,r.x0+(r.x1-r.x0)*7/12,r.y0+115+base);
            GUI_DispStringAt(pObj->data3_Unit,r.x0+(r.x1-r.x0)*9/12,r.y0+115+base);
            GUI_DispStringAt(pObj->data4_text,r.x0+30,r.y0+140+base);
            pTemp = _Float2String(pObj->data4,1);
            GUI_DispStringAt(pTemp,r.x0+(r.x1-r.x0)*7/12,r.y0+140+base);
            GUI_DispStringAt(pObj->data4_Unit,r.x0+(r.x1-r.x0)*9/12,r.y0+140+base);
            GUI_DispStringAt(pObj->data5_text,r.x0+30,r.y0+165+base);
            pTemp = _Float2String(pObj->data5,1);
            GUI_DispStringAt(pTemp,r.x0+(r.x1-r.x0)*7/12,r.y0+165+base);
            GUI_DispStringAt(pObj->data5_Unit,r.x0+(r.x1-r.x0)*9/12,r.y0+165+base);
            break;
        }
    }
    WM_SetUserClipRect(0);
}

/***************************************************************************************************
*
*
*/
static void DATA_FRAME_Callback(WM_MESSAGE *pMsg)
{
    DATA_FRAME_Handle hObj = pMsg->hWin;
    DATA_FRAME_Obj* pObj = DATA_FRAME_H2P(hObj);
    /* Let widget handle the standard messages */
    if (WIDGET_HandleActive(hObj, pMsg) == 0)
    {
        return;
    }
    switch (pMsg->MsgId)
    {
    case WM_TOUCH:
        return;      /* Message handled. Do not call WM_DefaultProc, 
                        because the window may have been destroyed */
    case WM_PAINT:
        _Paint(pObj, hObj);
        return;
    case WM_DELETE:
        break;       /* No return here ... WM_DefaultProc needs to be called */
    }
    WM_DefaultProc(pMsg);
}

/***************************************************************************************************
*
*       Exported routines:  Create
*
***************************************************************************************************/
DATA_FRAME_Handle DATA_FRAME_Create(int x0, int y0, int xsize, int ysize, int Id, int Flags)
{
    return DATA_FRAME_CreateEx(x0, y0, xsize, ysize, WM_HMEM_NULL, Flags, 0, Id);
}

/***************************************************************************************************
*
*
*/
DATA_FRAME_Handle DATA_FRAME_CreateEx(int x0, int y0, int xsize, int ysize, 
                                      WM_HWIN hParent, int Id,
                                      WM_CALLBACK* cb,int WinFlags)
{
    DATA_FRAME_Handle hObj;
    //GUI_USE_PARA(ExFlags);
    /* Create the window */
    WM_LOCK();
    hObj = WM_CreateWindowAsChild(x0, y0, xsize, ysize, hParent, WinFlags, DATA_FRAME_Callback,
                                  sizeof(DATA_FRAME_Obj) - sizeof(WM_Obj));
    if (hObj)
    {
        DATA_FRAME_Obj* pObj = DATA_FRAME_H2P(hObj);
        /* init widget specific variables */
        WIDGET__Init(&pObj->Widget, Id, WIDGET_STATE_FOCUSSABLE);
        pObj->BkColor = DATA_FRAME_BKCOLOR_DEFAULT;
        pObj->FrameColor = DATA_FRAME_FRAMECOLOR_DEFAULT;
        pObj->DataColor = DATA_FRAME_DATACOLOR_DEFAULT;
        pObj->TextColor = DATA_FRAME_TEXTCOLOR_DEFAULT;
        pObj->pDataFont = DATA_FRAME_DATAFONT_DEFAULT;
        pObj->pHeadFont = DATA_FRAME_HEADFONT_DEFAULT;
        pObj->pTextFont = DATA_FRAME_TEXTFONT_DEFAULT;
        pObj->pRealtimeDataFont = DATA_FRAME_RTDATAFONT_DEFAULT;
        pObj->pUnitFont = DATA_FRAME_UNITFONT_DEFAULT;
        pObj->RT_Value = DATA_FRAME_DATA_DEFAULT;
        pObj->ParameterNum = 0;
        pObj->data0 = 0;
        pObj->data0_text = "Min:";
        pObj->DisplayFlag = 0;
        pObj->HeadText = "Trigger Type";
        //pObj->data0_Unit = "L/min";
    }
    else
    {
        GUI_DEBUG_ERROROUT_IF(hObj==0, "DATA_FRAME_Create failed")
    }
    WM_UNLOCK();
    return hObj;
}

/***************************************************************************************************
*
*
*/
DATA_FRAME_Handle DATA_FRAME_CreateIndirect(const GUI_WIDGET_CREATE_INFO* pCreateInfo, 
                                            WM_HWIN hWinParent, int x0, int y0, 
                                            WM_CALLBACK* pCallback)
{
    DATA_FRAME_Handle hObj;
    hObj = DATA_FRAME_CreateEx(pCreateInfo->x0 + x0, pCreateInfo->y0 + y0, 
                               pCreateInfo->xSize, pCreateInfo->ySize, 
                               hWinParent, pCreateInfo->Id,
                               pCallback, 0);
    return hObj;
}

/***************************************************************************************************
*
*       POWER_INFO_SetBkColor
*/
void DATA_FRAME_SetBkColor(DATA_FRAME_Handle hObj,GUI_COLOR Color)
{
    if (hObj)
    {
        DATA_FRAME_Obj* pObj;
        WM_LOCK();
        pObj = DATA_FRAME_H2P(hObj);
        pObj->BkColor = Color;
        DATA_FRAME_Invalidate(hObj);
        WM_UNLOCK();
    }
}

/***************************************************************************************************
*
*       DATA_FRAME_SetHeadText
*/
void DATA_FRAME_SetHeadText(DATA_FRAME_Handle hObj,char * text)
{
    if (hObj)
    {
        DATA_FRAME_Obj* pObj;
        WM_LOCK();
        pObj = DATA_FRAME_H2P(hObj);
        pObj->HeadText = text;
        DATA_FRAME_Invalidate(hObj);
        WM_UNLOCK();
    }
}

/***************************************************************************************************
*
*       DATA_FRAME_SetDispFlag
*/
void DATA_FRAME_SetDispFlag(DATA_FRAME_Handle hObj,int flag)
{
    if (hObj)
    {
        DATA_FRAME_Obj* pObj;
        WM_LOCK();
        pObj = DATA_FRAME_H2P(hObj);
        pObj->DisplayFlag = flag;
        DATA_FRAME_Invalidate(hObj);
        WM_UNLOCK();
    }
}

/***************************************************************************************************
*
*       DATA_FRAME_SetUnitText
*/
void DATA_FRAME_SetUnitText(DATA_FRAME_Handle hObj,char * text)
{
    if (hObj)
    {
        DATA_FRAME_Obj* pObj;
        WM_LOCK();
        pObj = DATA_FRAME_H2P(hObj);
        pObj->RT_Unit = text;
        DATA_FRAME_Invalidate(hObj);
        WM_UNLOCK();
    }
}

/***************************************************************************************************
*
*       DATA_FRAME_SetParameterNum
*/
void DATA_FRAME_SetParameterNum(DATA_FRAME_Handle hObj,int num)
{
    if (hObj)
    {
        DATA_FRAME_Obj* pObj;
        WM_LOCK();
        pObj = DATA_FRAME_H2P(hObj);
        pObj->ParameterNum = num;
        DATA_FRAME_Invalidate(hObj);
        WM_UNLOCK();
    }
}

/***************************************************************************************************
*
*       DATA_FRAME_SetParamterText
*/
void DATA_FRAME_SetParamterText(DATA_FRAME_Handle hObj,char * text,int num)
{
    if (hObj)
    {
        DATA_FRAME_Obj* pObj;
        WM_LOCK();
        pObj = DATA_FRAME_H2P(hObj);
        switch(num)
        {
        case 1:
            pObj->data0_text = text;
            break;
        case 2:
            pObj->data1_text = text;
            break;
        case 3:
            pObj->data2_text = text;
            break;
        case 4:
            pObj->data3_text = text;
            break;
        case 5:
            pObj->data4_text = text;
            break;
        case 6:
            pObj->data5_text = text;
            break;
        case 7:
            pObj->data6_text = text;
            break;
        default:
            pObj->data0_text = text;
        }
        DATA_FRAME_Invalidate(hObj);
        WM_UNLOCK();
    }
}

/***************************************************************************************************
*
*       DATA_FRAME_SetParamterUnit
*/
void DATA_FRAME_SetParamterUnit(DATA_FRAME_Handle hObj,char * text,int num)
{
    if (hObj)
    {
        DATA_FRAME_Obj* pObj;
        WM_LOCK();
        pObj = DATA_FRAME_H2P(hObj);
        switch(num)
        {
        case 1:
            pObj->data0_Unit = text;
            break;
        case 2:
            pObj->data1_Unit = text;
            break;
        case 3:
            pObj->data2_Unit = text;
            break;
        case 4:
            pObj->data3_Unit = text;
            break;
        case 5:
            pObj->data4_Unit = text;
            break;
        case 6:
            pObj->data5_Unit = text;
            break;
        case 7:
            pObj->data6_Unit = text;
            break;
        default:
            pObj->data0_Unit = text;
        }
        DATA_FRAME_Invalidate(hObj);
        WM_UNLOCK();
    }
}

/***************************************************************************************************
*
*       DATA_FRAME_SetParamterData
*/
void DATA_FRAME_SetParamterData(DATA_FRAME_Handle hObj,int data,int num)
{
    if (hObj)
    {
        DATA_FRAME_Obj* pObj;
        WM_LOCK();
        pObj = DATA_FRAME_H2P(hObj);
        switch(num)
        {
        case 1:
            pObj->data0 = data;
            break;
        case 2:
            pObj->data1 = data;
            break;
        case 3:
            pObj->data2 = data;
            break;
        case 4:
            pObj->data3 = data;
            break;
        case 5:
            pObj->data4 = data;
            break;
        case 6:
            pObj->data5 = data;
            break;
        case 7:
            pObj->data6 = data;
            break;
        default:
            pObj->data0 = data;
        }
        DATA_FRAME_Invalidate(hObj);
        WM_UNLOCK();
    }
}
//
//
//


