/*************************************************************
标题:  RectInfoCtrl.c
功能:  矩形区域内的消息显示，一般为三行，左边为描述项，右边（中）为数据
说明:

当前版本： 0.1.0
修改记录：
2007.08.1   yanglin, created
**************************************************************/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "RectInfoCtrl.h"
#include "WIDGET.h"
#include "GUI_Protected.h"
#include "DispKeyLib.h"

#if GUI_WINSUPPORT
#ifndef RECTINFOCTRL_BKCOLOR_DEFAULT
#define RECTINFOCTRL_BKCOLOR_DEFAULT 0x6E6450//0xFEEDDA
#endif
/* Default for top frame size */
#ifndef RECTINFOCTRL_WIGTH_DEFAULT
#define RECTINFOCTRL_WIGTH_DEFAULT 12
#endif
#ifndef RECTINFOCTRL_DEFAULT_FONT
#define RECTINFOCTRL_DEFAULT_FONT                 &GUI_Font_16
#endif
#ifndef VIEWCOLOR_BKBUTTON
#define VIEWCOLOR_BKBUTTON                      0xDCE2BF
#endif
#define RECTINFOCTRL_DEFAULT_TEXTCOLOR              GUI_YELLOW//GUI_BLACK
#define RECTINFOCTRL_DEFAULT_DATACOLOR              GUI_YELLOW//GUI_BLACK//GUI_BLUE
#define RECTINFOCTRL_DEFAULT_SIGNDEFAULT            10
#define RECTINFOCTRL_DEFAULT_FONT_INDEX             0
#define RECTINFOCTRL_SMALL_FONT_INDEX               1
#define RECTINFOCTRL_DEFAULT_VIEWUNIT               'F'
#define TEXTEXCTRL_DEFAULT_INVALIDATION             0xffff
#define RECTINFOCTRL_DATA_FONT                      &GUI_Font_24
typedef struct
{
    WIDGET Widget;
    WM_HMEM hpText[3];
    GUI_POINT CapturePoint;
    RectInfoCtrlInfo RICInfo;
    const GUI_FONT GUI_UNI_PTR* pFont;                //字体大小
    const GUI_FONT GUI_UNI_PTR* pDataFont[3];         //数据大小
    int SizeOffset;                                   //使显示的信息整体向下移动一个偏移量
    char Mode;            //T:显示(默认)  F:不显示
    //使每行的数据将加入新的字符描述项，并改变显示格局，左边、右边都显示字符，而中间区域将显示浮点数　　eg. VT    0.520 L   /  Tinsp   1.70 sec.
    char sUnit;           //F:不显示(默认)    T:显示   V:显示并隐藏数据显示部分
} RectInfoCtrl_Obj;
//static const GUI_FONT* _pDefaultFont = RECTINFOCTRL_DEFAULT_FONT;
//static GUI_COLOR _DefaultColor = RectInfoCtrl_COLOR_DEFAULT;
static const RectInfoCtrlInfo RPCInfo_DEFAULT =
{
    RECTINFOCTRL_BKCOLOR_DEFAULT,
    RECTINFOCTRL_DEFAULT_TEXTCOLOR,
    RECTINFOCTRL_DEFAULT_DATACOLOR,
    "Ppeak",
    "Pmean",
    "PEEP",
    -1,-1,-1,
    "(cmH2O)",
    "(cmH2O)",
    "(cmH2O)",
    (float)10,
    (float)3,
    (float)8,
    0,
    0,
    0,
    RECTINFOCTRL_DEFAULT_SIGNDEFAULT
};
static const char *WaveValuesName[10] =
{
    "ETCO2","INCO2","PEAK","PLAT","PEEP","MV","VT","F","SPO2","PR"
};
#define RectInfoCtrl_H2P(h) (RectInfoCtrl_Obj*) GUI_ALLOC_h2p(h)
#define RectInfoCtrl_Invalidate(hObj) WM_InvalidateWindow(hObj)
///_Float2String
///数据无效，则显示“--”
//将整形数据，转换为实际数据，_Float2String
char* _Float2String(float fData,int floatLen)
{
    static char DataTemp[10] = "--";
    if((fData!=0)&(fData<32768))
    {
        if(floatLen==0)
        {
            if(fData>=TEXTEXCTRL_DEFAULT_INVALIDATION)
            {
                sprintf(DataTemp,"--");
            }
            else
            {
                sprintf(DataTemp,"%.0f",fData);
            }
        }
        else if(floatLen==1)
        {
            if((fData*10)>=TEXTEXCTRL_DEFAULT_INVALIDATION)
            {
                sprintf(DataTemp,"--");
            }
            else
            {
                sprintf(DataTemp,"%.1f",fData);
            }
        }
        else if(floatLen==2)
        {
            if((fData*100)>=TEXTEXCTRL_DEFAULT_INVALIDATION)
            {
                sprintf(DataTemp,"--");
            }
            else
            {
                sprintf(DataTemp,"%.2f",fData);
            }
        }
        else if(floatLen==3)
        {
            if((fData*1000)>=TEXTEXCTRL_DEFAULT_INVALIDATION)
            {
                sprintf(DataTemp,"--");
            }
            else
            {
                sprintf(DataTemp,"%.3f",fData);
            }
        }
        else if(floatLen==4)
        {
            if((fData*10000)>=TEXTEXCTRL_DEFAULT_INVALIDATION)
            {
                sprintf(DataTemp,"--");
            }
            else
            {
                sprintf(DataTemp,"%.4f",fData);
            }
        }
    }
    else if(fData==0)
    {
        sprintf(DataTemp,"0");
    }
    else
    {
        sprintf(DataTemp,"--");
    }
    return DataTemp;
}



static void _DrawPaintText(RectInfoCtrl_Obj* pObj)
{
    GUI_RECT r;
    char *strTemp = NULL;
    int sign = 0;
    int i,j,k=0;
    GUI_GetClientRect(&r);
    //pObj->RICInfo.SignDefault==-1为自定义部分，通过标识的输入...
    if(pObj->RICInfo.SignDefault==-1)
    {
        GUI_SetTextMode(GUI_TM_TRANS);
        if(pObj->RICInfo.strText[0]!=NULL)
        {
            GUI_SetFont(pObj->pFont);
            GUI_SetColor(pObj->RICInfo.TextColor);
            GUI_DispStringAt(pObj->RICInfo.strText[0],r.x0+4,r.y1/7);//r.y0+6
            strTemp = _Float2String(pObj->RICInfo.dataText[0],pObj->RICInfo.floatLen[0]);
            //在显示区右侧，将添加的两组信息表现出来
            if(pObj->sUnit=='F')
            {
                GUI_SetFont(pObj->pDataFont[0]);
                GUI_SetColor(pObj->RICInfo.DataColor);
                GUI_SetTextAlign(GUI_TA_RIGHT);
                GUI_DispStringAt(strTemp,r.x1-6,r.y1/7+pObj->SizeOffset);
            }
            else if(pObj->sUnit == 'T')
            {
                GUI_SetTextAlign(GUI_TA_LEFT);
                GUI_DispStringAt(pObj->RICInfo.strTextR[0],r.x1*2/3+4,r.y1/7+pObj->SizeOffset);
                GUI_SetFont(pObj->pDataFont[0]);
                GUI_SetColor(pObj->RICInfo.DataColor);
                GUI_SetTextAlign(GUI_TA_RIGHT);
                GUI_DispStringAt(strTemp,r.x1*2/3,r.y1/7+pObj->SizeOffset);
            }
            else if(pObj->sUnit == 'V')
            {
                GUI_SetTextAlign(GUI_TA_RIGHT);
                GUI_DispStringAt(pObj->RICInfo.strTextR[0],r.x1-6,r.y1/7+pObj->SizeOffset);
            }
        }
        if(pObj->RICInfo.strText[1]!=NULL)
        {
            GUI_SetFont(pObj->pFont);
            GUI_SetColor(pObj->RICInfo.TextColor);
            GUI_DispStringAt(pObj->RICInfo.strText[1],r.x0+4,r.y1*3/7);//r.y0+6+r.y1/4
            ///显示有效数据
            strTemp = _Float2String(pObj->RICInfo.dataText[1],pObj->RICInfo.floatLen[1]);
            //在显示区右侧，将添加的两组信息表现出来
            if(pObj->sUnit=='F')
            {
                GUI_SetFont(pObj->pDataFont[1]);
                GUI_SetTextAlign(GUI_TA_RIGHT);
                GUI_SetColor(pObj->RICInfo.DataColor);
                GUI_DispStringAt(strTemp,r.x1-6,r.y1*3/7+pObj->SizeOffset);
            }
            else
            {
                GUI_SetTextAlign(GUI_TA_LEFT);
                GUI_DispStringAt(pObj->RICInfo.strTextR[1],r.x1*2/3+4,r.y1*3/7+pObj->SizeOffset);
                if(pObj->sUnit != 'V')
                {
                    GUI_SetFont(pObj->pDataFont[1]);
                    GUI_SetColor(pObj->RICInfo.DataColor);
                    GUI_SetTextAlign(GUI_TA_RIGHT);
                    GUI_DispStringAt(strTemp,r.x1*2/3,r.y1*3/7+pObj->SizeOffset);
                }
            }
        }
        if(pObj->RICInfo.strText[2]!=NULL)
        {
            GUI_SetFont(pObj->pFont);
            GUI_SetColor(pObj->RICInfo.TextColor);
            GUI_DispStringAt(pObj->RICInfo.strText[2],r.x0+4,r.y1*5/7);//r.y0+6+r.y1*2/4
            ///显示有效数据
            strTemp = _Float2String(pObj->RICInfo.dataText[2],pObj->RICInfo.floatLen[2]);
            //在显示区右侧，将添加的两组信息表现出来
            if(pObj->sUnit=='F')
            {
                GUI_SetFont(pObj->pDataFont[2]);
                GUI_SetTextAlign(GUI_TA_RIGHT);
                GUI_SetColor(pObj->RICInfo.DataColor);
                GUI_DispStringAt(strTemp,r.x1-6,r.y1*5/7+pObj->SizeOffset);
            }
            else
            {
                GUI_SetTextAlign(GUI_TA_LEFT);
                GUI_DispStringAt(pObj->RICInfo.strTextR[2],r.x1*2/3+4,r.y1*5/7+pObj->SizeOffset);
                if(pObj->sUnit != 'V')
                {
                    GUI_SetFont(pObj->pDataFont[2]);
                    GUI_SetColor(pObj->RICInfo.DataColor);
                    GUI_SetTextAlign(GUI_TA_RIGHT);
                    GUI_DispStringAt(strTemp,r.x1*2/3,r.y1*5/7+pObj->SizeOffset);
                }
            }
        }
    }
    else if(pObj->RICInfo.SignDefault==0)
    {
        if(pObj->Mode == 'T')
        {
            GUI_SetColor(VIEWCOLOR_BKBUTTON);
            GUI_DrawCircle(r.x0+10,r.y0+12,4);
            GUI_SetColor(pObj->RICInfo.TextColor);
            GUI_SetTextMode(GUI_TM_TRANS);
            GUI_DispStringAt("Apnea-veat.Off",r.x0+20,r.y0+8);
        }
        else
        {
            GUI_SetColor(GUI_WHITE);
            GUI_FillRect  (0, 0, r.x1, r.y1);
        }
    }
    else if((pObj->RICInfo.SignDefault>0)&&(pObj->RICInfo.SignDefault<=7))
    {
        sign = pObj->RICInfo.SignDefault;
        GUI_SetColor(pObj->RICInfo.TextColor);
        GUI_SetTextMode(GUI_TM_TRANS);
        for(i=0,j=0; i<3; i++)
        {
            if(sign>>i&1)
            {
                j++;
            }
        }
        for(i=0; i<3; i++)
        {
            if(sign>>i&1)
            {
                GUI_SetFont(pObj->pFont);
                GUI_DispStringAt(pObj->RICInfo.strText[i],r.x0+4,r.y0+4+r.y1*k/j);
                GUI_DispStringAt(pObj->RICInfo.strTextR[i],r.x0+4,r.y0+20+r.y1*k/j);
                strTemp=_Float2String(pObj->RICInfo.dataText[i],0);
                GUI_SetFont(pObj->pDataFont[i]);
                GUI_DispStringAt(strTemp,r.x1-60,r.y0+10+r.y1*k/j);
                k++;
            }
        }
    }
    else if(pObj->RICInfo.SignDefault==8)           //Psupp.按键对应的显示框
    {
        GUI_SetColor(pObj->RICInfo.TextColor);
        GUI_SetTextMode(GUI_TM_TRANS);
        GUI_DispStringAt("Psupp.",r.x0+4,r.y0+6);
        sprintf(strTemp,"%.0f",pObj->RICInfo.dataText[0]);
        GUI_SetTextAlign(GUI_TA_RIGHT);
        GUI_DispStringAt(strTemp,r.x1-6,r.y0+6);
    }
    else if(pObj->RICInfo.SignDefault==9)           //FlowAssist按键对应的显示框
    {
        GUI_SetColor(pObj->RICInfo.TextColor);
        GUI_SetTextMode(GUI_TM_TRANS);
        GUI_DispStringAt("Compensated",r.x0+4,r.y0+6);
        GUI_DispStringAt("R",r.x0+4,r.y0+6+15);
        sprintf(strTemp,"%.1f",pObj->RICInfo.dataText[0]);
        GUI_SetTextAlign(GUI_TA_RIGHT);
        GUI_DispStringAt(strTemp,r.x1-6-56,r.y0+6+15);
        GUI_SetTextAlign(GUI_TA_RIGHT);
        GUI_DispStringAt("cmH2O/L/s",r.x1-6,r.y0+6+15);
    }
    else if(pObj->RICInfo.SignDefault==10)          //VolAssist按键对应的显示框
    {
        GUI_SetColor(pObj->RICInfo.TextColor);
        GUI_SetTextMode(GUI_TM_TRANS);
        GUI_DispStringAt("Compensated",r.x0+4,r.y0+6);
        GUI_DispStringAt("C",r.x0+4,r.y0+6+15);
        if(pObj->RICInfo.dataText[0]<=100)
        {
            sprintf(strTemp,"%.1f",pObj->RICInfo.dataText[0]);
        }
        else if((pObj->RICInfo.dataText[0]>100)&&(pObj->RICInfo.dataText[0]<=1000))
        {
            sprintf(strTemp,"%.0f",pObj->RICInfo.dataText[0]);
        }
        else if(pObj->RICInfo.dataText[0]>1000)
        {
            strcpy(strTemp,"--");
        }
        GUI_SetTextAlign(GUI_TA_RIGHT);
        GUI_DispStringAt(strTemp,r.x1-6-50,r.y0+6+15);
        GUI_SetTextAlign(GUI_TA_RIGHT);
        GUI_DispStringAt("mL/cmH2O",r.x1-6,r.y0+6+15);
    }
}
static void _UpdateDataText(RectInfoCtrl_Obj* pObj)
{
    GUI_RECT r;
    char *strTemp = NULL;
    int sign = 0;
    int i,j,k=0;
    GUI_GetClientRect(&r);
    GUI_SetTextAlign(GUI_TA_LEFT);
    //GUI_SetFont(pObj->RICInfo.BkColor);
    sign = pObj->RICInfo.SignDefault;
    for(i=0,j=0; i<3; i++)
    {
        if(sign>>i&1)
        {
            j++;
        }
    }
    for(i=0; i<3; i++)
    {
        if(sign>>i&1)
        {
            GUI_SetBkColor(pObj->RICInfo.BkColor);
            GUI_ClearRect(r.x1-60,r.y0+10+r.y1*k/j,r.x1,r.y0+10+r.y1*k/j+32);
            strTemp=_Float2String(pObj->RICInfo.dataText[i],0);
            GUI_SetFont(pObj->pDataFont[i]);
            GUI_SetColor(pObj->RICInfo.DataColor);
            GUI_DispStringAt(strTemp,r.x1-60,r.y0+10+r.y1*k/j);
            k++;
        }
    }
}
static void _Paint(RectInfoCtrl_Obj* pObj)
{
    /* Draw Frame */
    int xsize = pObj->Widget.Win.Rect.x1 - pObj->Widget.Win.Rect.x0;
    int ysize = pObj->Widget.Win.Rect.y1 - pObj->Widget.Win.Rect.y0;
    GUI_SetColor  (pObj->RICInfo.BkColor);
    GUI_FillRect  (0, 0, xsize, ysize);
    /*  GUI_RECT rClient;
      GUI_GetClientRect(&rClient);
      LCD_SetColor(GUI_WHITE);
      Arc(rClient.x0 + 8, rClient.y0 + 8, rClient.x1 - 8, rClient.y1 - 8,6,8,0xE1);
      LCD_SetColor(0x222222);
      Arc(rClient.x0 + 8, rClient.y0 + 8, rClient.x1 - 8, rClient.y1 - 8,6,8,0x1E);

      Bevel(rClient.x0 + 8, rClient.y0 + 8, rClient.x1 - 8, rClient.y1 - 8,8);
      //LCD_SetColor (pObj->Props.aBkColor[ColorIndex]);
      GUI_SetColor  (pObj->RICInfo.BkColor);
      FillBevel(rClient.x0 +8,rClient.y0+8,rClient.x1-8,rClient.y1-8,6);*/
    _DrawPaintText(pObj);
    //WIDGET_EFFECT_3D_DrawUp();
}
/***************************************************************************************************
*/
void _RectInfoCtrl_Callback (WM_MESSAGE* pMsg)
{
    RectInfoCtrl_Handle hWin = (RectInfoCtrl_Handle)(pMsg->hWin);
    RectInfoCtrl_Obj* pObj = RectInfoCtrl_H2P(hWin);
    //GUI_RECT* pRect = (GUI_RECT*)(pMsg->Data.p);
    switch (pMsg->MsgId)
    {
    case WM_PAINT:
        _Paint(pObj);
        break;
    case WM_TOUCH:
        return;
    case WM_DELETE:
        GUI_ALLOC_FreePtr(&pObj->hpText[0]);
        GUI_ALLOC_FreePtr(&pObj->hpText[1]);
        GUI_ALLOC_FreePtr(&pObj->hpText[2]);
        break;
    case WM_NOTIFICATION_VALUE_CHANGED:
        _UpdateDataText(pObj);
        break;
    }
    /* Let widget handle the standard messages */
    if (WIDGET_HandleActive(hWin, pMsg) == 0)
    {
        return;
    }
    WM_DefaultProc(pMsg);
}
RectInfoCtrl_Handle RectInfoCtrl_CreateAsChild(
    int x0, int y0, int xsize, int ysize, WM_HWIN hParent,int Id,
    WM_CALLBACK* cb, int Flags)
{
    RectInfoCtrl_Handle hObj;
    /* Create the window */
    WM_LOCK();
    hObj = WM_CreateWindowAsChild(x0, y0, xsize/*+2*HBorder*/, ysize/*+TBorder+BBorder*/, hParent,
                                  Flags, _RectInfoCtrl_Callback, sizeof(RectInfoCtrl_Obj) - sizeof(WM_Obj));
    if (hObj)
    {
        WM_HMEM hMem = 0;
        RectInfoCtrl_Obj* pObj = RectInfoCtrl_H2P(hObj);
        /* init widget specific variables */
        WIDGET__Init(&pObj->Widget,Id, WIDGET_STATE_FOCUSSABLE );
        pObj->RICInfo = RPCInfo_DEFAULT;
        pObj->Mode = 'T';           //T:显示    F:不显示
        pObj->pFont = RECTINFOCTRL_DEFAULT_FONT;
        pObj->pDataFont[0] = RECTINFOCTRL_DATA_FONT;
        pObj->pDataFont[1] = RECTINFOCTRL_DATA_FONT;
        pObj->pDataFont[2] = RECTINFOCTRL_DATA_FONT;
        pObj->SizeOffset = 0;
        pObj->sUnit = RECTINFOCTRL_DEFAULT_VIEWUNIT;
        hMem = GUI_ALLOC_AllocZero(40 + 1);
        pObj->hpText[0] = hMem;
        hMem = GUI_ALLOC_AllocZero(40 + 1);
        pObj->hpText[1] = hMem;
        hMem = GUI_ALLOC_AllocZero(40 + 1);
        pObj->hpText[2] = hMem;
    }
    WM_UNLOCK();
    return hObj;
}
RectInfoCtrl_Handle RectInfoCtrl_Create(WM_CALLBACK* cb,
                                        int Flags,
                                        int x0, int y0, int xsize, int ysize,int Id)
{
    return RectInfoCtrl_CreateAsChild(x0, y0, xsize, ysize, WM_HWIN_NULL,Id, cb, Flags);
}
RectInfoCtrl_Handle RectInfoCtrl_CreateIndirect(const GUI_WIDGET_CREATE_INFO* pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK* pCallback)
{
    RectInfoCtrl_Handle hObj;
    hObj = RectInfoCtrl_CreateAsChild(
               pCreateInfo->x0 + x0, pCreateInfo->y0 + y0, pCreateInfo->xSize, pCreateInfo->ySize, hWinParent, pCreateInfo->Id,
               pCallback, 0);
    return hObj;
}
void RectInfoCtrl_SetBkColor(RectInfoCtrl_Handle hObj,GUI_COLOR BkColor)
{
    if (hObj)
    {
        RectInfoCtrl_Obj* pObj;
        WM_LOCK();
        pObj = RectInfoCtrl_H2P(hObj);
        pObj->RICInfo.BkColor = BkColor;
        RectInfoCtrl_Invalidate(hObj);
        WM_UNLOCK();
    }
}
void RectInfoCtrl_SetSignDefault(RectInfoCtrl_Handle hObj,int SignDefault)
{
    if (hObj)
    {
        RectInfoCtrl_Obj* pObj;
        WM_LOCK();
        pObj = RectInfoCtrl_H2P(hObj);
        pObj->RICInfo.SignDefault = SignDefault;
        RectInfoCtrl_Invalidate(hObj);
        WM_UNLOCK();
    }
}
void RectInfoCtrl_SetMode(RectInfoCtrl_Handle hObj,char Mode)
{
    if (hObj)
    {
        RectInfoCtrl_Obj* pObj;
        WM_LOCK();
        pObj = RectInfoCtrl_H2P(hObj);
        pObj->Mode = Mode;
        RectInfoCtrl_Invalidate(hObj);
        WM_UNLOCK();
    }
}
void RectInfoCtrl_SetDataEx(RectInfoCtrl_Handle hObj,int Index,float Data)
{
    if (hObj)
    {
        RectInfoCtrl_Obj* pObj;
        WM_LOCK();
        pObj = RectInfoCtrl_H2P(hObj);
        pObj->RICInfo.dataText[Index] = Data;
        RectInfoCtrl_Invalidate(hObj);
        WM_UNLOCK();
    }
}
void RectInfoCtrl_SetFloatDataEx(RectInfoCtrl_Handle hObj,int Index,float Data,int FloatLen)
{
    if (hObj)
    {
        RectInfoCtrl_Obj* pObj;
        WM_LOCK();
        pObj = RectInfoCtrl_H2P(hObj);
        pObj->RICInfo.dataText[Index] = Data;
        pObj->RICInfo.floatLen[Index] = FloatLen;
        RectInfoCtrl_Invalidate(hObj);
        WM_UNLOCK();
    }
}
void RectInfoCtrl_SetTextEx(RectInfoCtrl_Handle hObj,int Index,const char* strText)
{
    if (hObj)
    {
        RectInfoCtrl_Obj* pObj;
        WM_LOCK();
        pObj = RectInfoCtrl_H2P(hObj);
        pObj->RICInfo.strText[Index] = strText;
        RectInfoCtrl_Invalidate(hObj);
        WM_UNLOCK();
    }
}
void RectInfoCtrl_SetTextIdEx(RectInfoCtrl_Handle hObj,int Index,const char* strText,int strEnum)
{
    if (hObj)
    {
        RectInfoCtrl_Obj* pObj;
        WM_LOCK();
        pObj = RectInfoCtrl_H2P(hObj);
        pObj->RICInfo.strEnum[Index] = strEnum;
        pObj->RICInfo.strText[Index] = strText;
        RectInfoCtrl_Invalidate(hObj);
        WM_UNLOCK();
    }
}
int RectInfoCtrl_GetTextId(RectInfoCtrl_Handle hObj,int Index)
{
    int ret;
    if (hObj)
    {
        RectInfoCtrl_Obj* pObj;
        WM_LOCK();
        pObj = RectInfoCtrl_H2P(hObj);
        ret = pObj->RICInfo.strEnum[Index];
        WM_UNLOCK();
    }
    return ret;
}
void RectInfoCtrl_SetFontEx(RectInfoCtrl_Handle hObj,int Index, const GUI_FONT GUI_UNI_PTR * pFont)
{
    if (hObj)
    {
        RectInfoCtrl_Obj* pObj;
        WM_LOCK();
        pObj = RectInfoCtrl_H2P(hObj);
        pObj->pDataFont[Index] = pFont;
        RectInfoCtrl_Invalidate(hObj);
        WM_UNLOCK();
    }
}
/***************************************************************************************************
*
*       RectInfoCtrl_SetFont
*/
void RectInfoCtrl_SetFont(RectInfoCtrl_Handle hObj, const GUI_FONT GUI_UNI_PTR * pFont)
{
    if (hObj)
    {
        RectInfoCtrl_Obj* pObj;
        WM_LOCK();
        pObj = RectInfoCtrl_H2P(hObj);
        pObj->pFont = pFont;
        RectInfoCtrl_Invalidate(hObj);
        WM_UNLOCK();
    }
}
/***************************************************************************************************
*
*       RectInfoCtrl_SetTextColor
*/
void RectInfoCtrl_SetTextColor(RectInfoCtrl_Handle hObj, GUI_COLOR Color)
{
    if (hObj)
    {
        RectInfoCtrl_Obj* pObj;
        WM_LOCK();
        pObj = RectInfoCtrl_H2P(hObj);
        pObj->RICInfo.TextColor = Color;
        RectInfoCtrl_Invalidate(hObj);
        WM_UNLOCK();
    }
}
/***************************************************************************************************
*
*       RectInfoCtrl_SetDataColor
*/
void RectInfoCtrl_SetDataColor(RectInfoCtrl_Handle hObj, GUI_COLOR Color)
{
    if (hObj)
    {
        RectInfoCtrl_Obj* pObj;
        WM_LOCK();
        pObj = RectInfoCtrl_H2P(hObj);
        pObj->RICInfo.DataColor = Color;
        RectInfoCtrl_Invalidate(hObj);
        WM_UNLOCK();
    }
}
/***************************************************************************************************
*
*       RectInfoCtrl_SetDataSizeOffset
*/
void RectInfoCtrl_SetDataSizeOffset(RectInfoCtrl_Handle hObj, int Offset)
{
    if (hObj)
    {
        RectInfoCtrl_Obj* pObj;
        WM_LOCK();
        pObj = RectInfoCtrl_H2P(hObj);
        pObj->SizeOffset = Offset;
        RectInfoCtrl_Invalidate(hObj);
        WM_UNLOCK();
    }
}
/***************************************************************************************************
*
*       RectInfoCtrl_SetViewUnit
*       功能：使显示更丰富，
*       使每行的数据将加入新的字符描述项，并改变显示格局，左边、右边都显示字符，而中间区域将显示浮点数　　
*       eg. VT    0.520 L   /  Tinsp   1.70 sec.
*       F:不显示(默认)  T:显示   V:显示并隐藏数据显示部分
*/
void RectInfoCtrl_SetViewUnit(RectInfoCtrl_Handle hObj, char sUnit)
{
    if (hObj)
    {
        RectInfoCtrl_Obj* pObj;
        WM_LOCK();
        pObj = RectInfoCtrl_H2P(hObj);
        pObj->sUnit = sUnit;
        RectInfoCtrl_Invalidate(hObj);
        WM_UNLOCK();
    }
}
void RectInfoCtrl_SetUnit(RectInfoCtrl_Handle hObj,int Index,const char* strTextR)
{
    if (hObj)
    {
        RectInfoCtrl_Obj* pObj;
        WM_LOCK();
        pObj = RectInfoCtrl_H2P(hObj);
        if (GUI__SetText(&pObj->hpText[Index], strTextR))
        {
            pObj->RICInfo.strTextR[Index]  = (char*) GUI_ALLOC_h2p(pObj->hpText[Index]);
        }
        //pObj->RICInfo.strTextR[Index] = strTextR;
        RectInfoCtrl_Invalidate(hObj);
        WM_UNLOCK();
    }
}
#else
void WIDGET_viewWin(void) {} /* avoid empty object files */
#endif /* GUI_WINSUPPORT */
