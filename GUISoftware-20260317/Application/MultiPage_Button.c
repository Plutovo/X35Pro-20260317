/*************************************************************
标题:  MultiPage_Button.c
功能:  多页属性页的按键
说明:  水平方向模式，在按键的下方是直角
       垂直方向模式，在按键的左侧是直角
当前版本： 0.1.0
修改记录：
2007.08.1   yanglin, created
**************************************************************/
#include <stdlib.h>
#include <string.h>
#include "MultiPage_Button.h"
#include "WIDGET.h"
#include "GUI_Protected.h"
#if GUI_WINSUPPORT
#ifndef MULTIPAGE_BUTTON_COLOR_DEFAULT
#define MULTIPAGE_BUTTON_COLOR_DEFAULT 0xffffff
#endif
#ifndef MULTIPAGE_BUTTON_PROCOLOR_DEFAULT           //有焦点
#define MULTIPAGE_BUTTON_PROCOLOR_DEFAULT GUI_WHITE
#endif
#define MULTIPAGE_BUTTON_DEFAULT_PROCOLOR       0xDCE2BF
#define MULTIPAGE_BUTTON_DEFAULT_PROLINECOLOR   0xc0c0c0
#define MULTIPAGE_BUTTON_DEFAULT_TEXTCOLOR      GUI_BLACK

//#define MULTIPAGE_BUTTON_DEFAULT_TEXTFONT       &GUI_Font13B_ASCII //GUI_Font13B_1 //cpp md 20130328

#define MULTIPAGE_BUTTON_DEFAULT_TEXTFONT       &GUI_Font_15 //????????????????


#define MULTIPAGE_BUTTON_DEFAULT_TEXT           "TEXT"
#define MULTIPAGE_BUTTON_DEFAULT_INVALIDATECOLOR        0xc0c0c0
#define MULTIPAGE_BUTTON_DEFAULT_FONT_INDEX     0
#define MULTIPAGE_BUTTON_SMALL_FONT_INDEX       1
#define MULTIPAGE_BUTTON_DEFAULT_YES            1
#define MULTIPAGE_BUTTON_DEFAULT_NO             0
typedef struct
{
    WIDGET Widget;
    GUI_POINT CapturePoint;
    MultiPage_ButtonInfo MPBInfo;
    GUI_COLOR ProColor1;              ///没点击控件时的颜色
    GUI_COLOR ProColor2;              ///点击控件后的颜色
    GUI_COLOR ProColor3;              ///回车键选中的控件，控件上的颜色
    int Mode;                         ///mode  0：水平方向模式，在按键的下方是直角
    ///     1：垂直方向模式，在按键的左侧是直角
    int KeyEnterSign;                 ///确定是否响应回车键消息   0:不响应    1:响应
    int nInvalidate;                  ///使控件无效，当显示，显示颜色为灰色，并且不能操作　0:有效（默认）　1:无效
} MultiPage_Button_Obj;
static  int keySign = 0;            ///只响应一次左键弹起消息
static const MultiPage_ButtonInfo MPBInfo_DEFAULT =
{
    MULTIPAGE_BUTTON_COLOR_DEFAULT,
    MULTIPAGE_BUTTON_DEFAULT_PROCOLOR,
    MULTIPAGE_BUTTON_DEFAULT_PROLINECOLOR,
    MULTIPAGE_BUTTON_DEFAULT_TEXTCOLOR,
    MULTIPAGE_BUTTON_DEFAULT_TEXTFONT,
    MULTIPAGE_BUTTON_DEFAULT_TEXT
};
#define MultiPage_Button_H2P(h) (MultiPage_Button_Obj*) GUI_ALLOC_h2p(h)
#define MultiPage_Button_Invalidate(hObj) WM_InvalidateWindow(hObj)
static void _OnTouch(MultiPage_Button_Handle hWin, MultiPage_Button_Obj* pObj, WM_MESSAGE* pMsg)
{
    //GUI_TOUCH_tState* pState;
    WM_MESSAGE Msg;
    const GUI_PID_STATE* pState = (const GUI_PID_STATE*)pMsg->Data.p;
    if(!WM_IsWindow(hWin))
    {
        return;
    }
    pState = (GUI_PID_STATE*)pMsg->Data.p;
    if (pMsg->Data.p)
    {
        if (pState->Pressed)
        {
            WM_SetFocus(hWin);
            WM_BringToTop(hWin);
            keySign = 1;
            if (!WM_HasCaptured(hWin))
            {
                WM_SetCapture(hWin, 1);
                pObj->CapturePoint.x = pState->x;
                pObj->CapturePoint.y = pState->y;
            }
        }
        else
        {
            if ((pObj->Widget.State & BUTTON_STATE_PRESSED) == 0)
            {
                if(keySign==1)
                {
                    keySign++;
                    //当有按键按下，颜色将改变
                    if((pObj->MPBInfo.ProColor)!=(pObj->ProColor3))
                    {
                        if((pObj->MPBInfo.ProColor)==(pObj->ProColor2))
                        {
                            pObj->MPBInfo.ProColor = pObj->ProColor1;
                        }
                        else
                        {
                            pObj->MPBInfo.ProColor = pObj->ProColor2;
                        }
                    }
                    WM_InvalidateWindow(hWin);
                    if(pObj->KeyEnterSign == MULTIPAGE_BUTTON_DEFAULT_NO)
                    {
                        if(pObj->nInvalidate == 0)                  ///当按键为有效时，才发送消息
                        {
                            Msg.MsgId   = WM_NOTIFY_PARENT;
                            Msg.Data.v  = WM_NOTIFICATION_RELEASED;
                            WM_SendToParent(hWin, &Msg);
                        }
                    }
                    else                                            ///当有控件响应回车键，有单击控件将发送WM_NOTIFICATION_CLICKED的消息
                    {
                        if(pObj->nInvalidate == 0)                  ///当按键为有效时，才发送消息
                        {
                            Msg.MsgId   = WM_NOTIFY_PARENT;
                            Msg.Data.v  = WM_NOTIFICATION_USER_MBCLICKED;       //WM_NOTIFICATION_CLICKED;
                            WM_SendToParent(hWin, &Msg);
                        }
                    }
                }
            }
        }
    }
}
static void _OnKeyEnter(MultiPage_Button_Handle hWin, MultiPage_Button_Obj* pObj, WM_MESSAGE* pMsg)
{
    WM_MESSAGE Msg;
    if(!WM_IsWindow(hWin))
    {
        return;
    }
    if((pObj->MPBInfo.ProColor)!=(pObj->ProColor3))
    {
        if(pObj->nInvalidate == 0)                  //当按键为有效时，才发送消息
        {
            Msg.MsgId   = WM_NOTIFY_PARENT;
            Msg.Data.v  = WM_NOTIFICATION_RELEASED;
            WM_SendToParent(hWin, &Msg);
            pObj->MPBInfo.ProColor = pObj->ProColor3;
            WM_InvalidateWindow(hWin);
        }
    }
}
static void _DrawPaintCircle(MultiPage_Button_Obj* pObj)
{
    GUI_RECT r;
    int i=0;
    int NewFontSizeY;
    char *s= {0},*s2= {0};
    char s1[20]= {0};
    char s3[20]= {0};
    GUI_GetClientRect(&r);
    if(pObj->nInvalidate == 0)
    {
        GUI_SetColor(pObj->MPBInfo.ProColor);
    }
    else if(pObj->nInvalidate == 1)
    {
        GUI_SetColor(MULTIPAGE_BUTTON_DEFAULT_INVALIDATECOLOR);
    }
    if(pObj->Mode==0)               //mode  0：水平方向模式，在按键的下方是直角
    {
        GUI_DrawHLine(r.y0+1,r.x0+3,r.x1-3);
        GUI_DrawHLine(r.y0+2,r.x0+2,r.x1-2);
        GUI_DrawHLine(r.y0+3,r.x0+1,r.x1-1);
        GUI_DrawHLine(r.y0,r.x0+4,r.x1-4);      //Add...
        GUI_FillRect(r.x0,r.y0+4,r.x1,r.y1);
        if((pObj->MPBInfo.ProColor!=pObj->ProColor1)||(pObj->ProColor1 == pObj->ProColor2))     //画控件周围的线
        {
            GUI_SetColor(pObj->MPBInfo.ProLineColor);
            GUI_DrawHLine(r.y0,r.x0+4,r.x1-4);
            GUI_DrawVLine(r.x0,r.y0+4,r.y1);
            GUI_DrawVLine(r.x1,r.y0+4,r.y1);
            GUI_DrawPoint(r.x0+2,r.y0+2);
            GUI_DrawPoint(r.x1-2,r.y0+2);
        }
    }
    else if(pObj->Mode==1)          //1：垂直方向模式，在按键的左侧是直角
    {
        GUI_DrawVLine(r.x1-1,r.y0+3,r.y1-3);
        GUI_DrawVLine(r.x1-2,r.y0+2,r.y1-2);
        GUI_DrawVLine(r.x1-3,r.y0+1,r.y1-1);
        GUI_FillRect(r.x0,r.y0,r.x1-4,r.y1);
        if(pObj->MPBInfo.ProColor!=pObj->ProColor1)
        {
            GUI_SetColor(pObj->MPBInfo.ProLineColor);
            GUI_DrawVLine(r.x1,r.y0+4,r.y1-4);
            GUI_DrawHLine(r.y0,r.x0,r.x1-4);
            GUI_DrawHLine(r.y1,r.x0,r.x1-4);
            GUI_DrawPoint(r.x1-2,r.y0+2);
            GUI_DrawPoint(r.x1-2,r.y1-2);
        }
    }
    //TEXT显示在控件居中的位置
    GUI_SetTextMode(GUI_TM_TRANS);
    GUI_SetColor(pObj->MPBInfo.TextColor);
    GUI_SetFont(pObj->MPBInfo.pTextFont);
    //GUI_DispStringInRect(pObj->MPBInfo.Text, &r, GUI_TA_HCENTER|GUI_TA_VCENTER);
    ///解决换行的问题！
    s = pObj->MPBInfo.Text;
    for(i=0; (s[i]!='\n')&&(s[i]!=0); i++)
    {
        s1[i]=s[i];
    }
    s2 = strchr(pObj->MPBInfo.Text,'\n');
    if(s2)          ///有'\n'
    {
        for(i=1; s2[i]!=0; i++)             ///去除'\n'
        {
            s3[i-1]=s2[i];
        }
        NewFontSizeY = GUI_GetFontDistY();
        GUI_SetTextAlign(GUI_TA_HCENTER|GUI_TA_VCENTER);
        GUI_DispStringAt(s1, (r.x0+r.x1)/2, (r.y0+r.y1)/2-NewFontSizeY/2);
        GUI_SetTextAlign(GUI_TA_HCENTER|GUI_TA_VCENTER);
        GUI_DispStringAt(s3, (r.x0+r.x1)/2, (r.y0+r.y1)/2+NewFontSizeY/2);
    }
    else            ///没有'\n'
    {
        GUI_SetTextAlign(GUI_TA_HCENTER|GUI_TA_VCENTER);
        GUI_DispStringAt(pObj->MPBInfo.Text, (r.x0+r.x1)/2, (r.y0+r.y1)/2);
    }
}
static void _Paint(MultiPage_Button_Obj* pObj)
{
    /* Draw Frame */
    int xsize = pObj->Widget.Win.Rect.x1 - pObj->Widget.Win.Rect.x0;
    int ysize = pObj->Widget.Win.Rect.y1 - pObj->Widget.Win.Rect.y0;
    GUI_SetColor  (pObj->MPBInfo.BkColor);
    GUI_FillRect  (0, 0, xsize, ysize);
    _DrawPaintCircle(pObj);
    //WIDGET_EFFECT_3D_DrawUp();
}
/***************************************************************************************************
*/
static void _MultiPage_Button_Callback (WM_MESSAGE *pMsg)
{
    MultiPage_Button_Handle hWin = (MultiPage_Button_Handle)(pMsg->hWin);
    MultiPage_Button_Obj* pObj = MultiPage_Button_H2P(hWin);
    //GUI_RECT* pRect = (GUI_RECT*)(pMsg->Data.p);
    int PressedCnt;
    int Key;
    switch (pMsg->MsgId)
    {
    case WM_PAINT:
        _Paint(pObj);
        break;
    case WM_TOUCH:
        _OnTouch(hWin, pObj, pMsg);
        return;
    case WM_SIZE:
        break;
    case WM_KEY:
        PressedCnt = ((WM_KEY_INFO*)(pMsg->Data.p))->PressedCnt;
        Key = ((WM_KEY_INFO*)(pMsg->Data.p))->Key;
        if (PressedCnt > 0)     /* Key pressed? */
        {
            switch (Key)
            {
            case GUI_KEY_ENTER:
                //只有在只响应回车键消息和按键为选中状态时，才去处理_OnKeyEnter
                if(pObj->KeyEnterSign==MULTIPAGE_BUTTON_DEFAULT_YES)
                {
                    if((pObj->MPBInfo.ProColor)==(pObj->ProColor2))
                    {
                        _OnKeyEnter(hWin, pObj, pMsg);
                    }
                }
                return;
            case GUI_KEY_DOWN:
                break;
            case GUI_KEY_UP:
                break;
            }
        }
        break;
    }
    /* Let widget handle the standard messages */
    if (WIDGET_HandleActive(hWin, pMsg) == 0)
    {
        return;
    }
    WM_DefaultProc(pMsg);
}
MultiPage_Button_Handle MultiPage_Button_CreateAsChild(
    int x0, int y0, int xsize, int ysize, WM_HWIN hParent,int Id,
    WM_CALLBACK* cb, int Flags)
{
    MultiPage_Button_Handle hObj;
    /* Create the window */
    WM_LOCK();
    hObj = WM_CreateWindowAsChild(x0, y0, xsize/*+2*HBorder*/, ysize/*+TBorder+BBorder*/, hParent,
                                  Flags, _MultiPage_Button_Callback, sizeof(MultiPage_Button_Obj) - sizeof(WM_Obj));
    if (hObj)
    {
        MultiPage_Button_Obj* pObj = MultiPage_Button_H2P(hObj);
        /* init widget specific variables */
        WIDGET__Init(&pObj->Widget,Id, WIDGET_STATE_FOCUSSABLE );
        //pObj->Widget.Id     = Id;
        pObj->MPBInfo = MPBInfo_DEFAULT;
        pObj->Mode = MULTIPAGE_BUTTON_DEFAULT_NO;
        pObj->ProColor1 = pObj->MPBInfo.ProColor;
        pObj->ProColor2 = MULTIPAGE_BUTTON_PROCOLOR_DEFAULT;
        pObj->ProColor3 = MULTIPAGE_BUTTON_PROCOLOR_DEFAULT;
        pObj->KeyEnterSign = MULTIPAGE_BUTTON_DEFAULT_NO;
        pObj->nInvalidate = MULTIPAGE_BUTTON_DEFAULT_NO;
    }
    WM_UNLOCK();
    return hObj;
}
MultiPage_Button_Handle MultiPage_Button_Create(WM_CALLBACK* cb,
        int Flags,
        int x0, int y0, int xsize, int ysize,int Id)
{
    return MultiPage_Button_CreateAsChild(x0, y0, xsize, ysize, WM_HWIN_NULL,Id, cb, Flags);
}
MultiPage_Button_Handle MultiPage_Button_CreateIndirect(const GUI_WIDGET_CREATE_INFO* pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK* pCallback)
{
    MultiPage_Button_Handle hObj;
    hObj = MultiPage_Button_CreateAsChild(
               pCreateInfo->x0 + x0, pCreateInfo->y0 + y0, pCreateInfo->xSize, pCreateInfo->ySize, hWinParent, pCreateInfo->Id,
               pCallback, 0);
    return hObj;
}
void MultiPage_Button_SetMode(MultiPage_Button_Handle hObj,int mode)
{
    if (hObj)
    {
        MultiPage_Button_Obj* pObj;
        WM_LOCK();
        pObj = MultiPage_Button_H2P(hObj);
        pObj->Mode = mode;
        MultiPage_Button_Invalidate(hObj);
        WM_UNLOCK();
    }
}
void MultiPage_Button_SetText(MultiPage_Button_Handle hObj,char* Text)
{
    if (hObj)
    {
        MultiPage_Button_Obj* pObj;
        WM_LOCK();
        pObj = MultiPage_Button_H2P(hObj);
        pObj->MPBInfo.Text = Text;
        MultiPage_Button_Invalidate(hObj);
        WM_UNLOCK();
    }
}
void MultiPage_Button_SetTextColor(MultiPage_Button_Handle hObj,GUI_COLOR TextColor)
{
    if (hObj)
    {
        MultiPage_Button_Obj* pObj;
        WM_LOCK();
        pObj = MultiPage_Button_H2P(hObj);
        pObj->MPBInfo.TextColor = TextColor;
        MultiPage_Button_Invalidate(hObj);
        WM_UNLOCK();
    }
}
void MultiPage_Button_SetProColor(MultiPage_Button_Handle hObj,GUI_COLOR ProColor)
{
    if (hObj)
    {
        MultiPage_Button_Obj* pObj;
        WM_LOCK();
        pObj = MultiPage_Button_H2P(hObj);
        pObj->MPBInfo.ProColor = ProColor;
        MultiPage_Button_Invalidate(hObj);
        WM_UNLOCK();
    }
}
void MultiPage_Button_SetColor(MultiPage_Button_Handle hObj,GUI_COLOR BkColor,GUI_COLOR ProColor1,GUI_COLOR ProColor2,GUI_COLOR ProLineColor,GUI_COLOR TextColor)
{
    if (hObj)
    {
        MultiPage_Button_Obj* pObj;
        WM_LOCK();
        pObj = MultiPage_Button_H2P(hObj);
        pObj->ProColor1 = ProColor1;
        pObj->ProColor2 = ProColor2;
        pObj->MPBInfo.BkColor = BkColor;
        pObj->MPBInfo.ProLineColor = ProLineColor;
        pObj->MPBInfo.TextColor = TextColor;
        pObj->MPBInfo.ProColor = ProColor1;
        MultiPage_Button_Invalidate(hObj);
        WM_UNLOCK();
    }
}
void MultiPage_Button_SetFocus(MultiPage_Button_Handle hObj)
{
    if (hObj)
    {
        MultiPage_Button_Obj* pObj;
        WM_LOCK();
        pObj = MultiPage_Button_H2P(hObj);
        pObj->MPBInfo.ProColor = pObj->ProColor2;
        MultiPage_Button_Invalidate(hObj);
        WM_UNLOCK();
    }
}
void MultiPage_Button_SetFont(MultiPage_Button_Handle hObj,const GUI_FONT GUI_UNI_PTR *pTextFont)
{
    if (hObj)
    {
        MultiPage_Button_Obj* pObj;
        WM_LOCK();
        pObj = MultiPage_Button_H2P(hObj);
        pObj->MPBInfo.pTextFont = pTextFont;
        MultiPage_Button_Invalidate(hObj);
        WM_UNLOCK();
    }
}
void MultiPage_Button_SetInvalidate(MultiPage_Button_Handle hObj,int nInvalidate)
{
    if (hObj)
    {
        MultiPage_Button_Obj* pObj;
        WM_LOCK();
        pObj = MultiPage_Button_H2P(hObj);
        pObj->nInvalidate = nInvalidate;
        pObj->MPBInfo.ProColor = pObj->ProColor1;           ///Add by yanglin at 2007-12-29
        MultiPage_Button_Invalidate(hObj);
        WM_UNLOCK();
    }
}
void MultiPage_Button_SetEnterFocus(MultiPage_Button_Handle hObj)
{
    if (hObj)
    {
        MultiPage_Button_Obj* pObj;
        WM_LOCK();
        pObj = MultiPage_Button_H2P(hObj);
        if(pObj->KeyEnterSign == MULTIPAGE_BUTTON_DEFAULT_NO)
        {
            pObj->MPBInfo.ProColor = pObj->ProColor2;
        }
        else
        {
            pObj->MPBInfo.ProColor = pObj->ProColor3;
        }
        MultiPage_Button_Invalidate(hObj);
        WM_UNLOCK();
    }
}
void MultiPage_Button_SetKeyEnterState(MultiPage_Button_Handle hObj,GUI_COLOR color)
{
    if (hObj)
    {
        MultiPage_Button_Obj* pObj;
        WM_LOCK();
        pObj = MultiPage_Button_H2P(hObj);
        pObj->ProColor3 = color;
        MultiPage_Button_Invalidate(hObj);
        WM_UNLOCK();
    }
}
void MultiPage_Button_SetKeyEnterSign(MultiPage_Button_Handle hObj,int KeySign)
{
    if (hObj)
    {
        MultiPage_Button_Obj* pObj;
        WM_LOCK();
        pObj = MultiPage_Button_H2P(hObj);
        pObj->KeyEnterSign = KeySign;
        MultiPage_Button_Invalidate(hObj);
        WM_UNLOCK();
    }
}
void MultiPage_Button_KillFocus(MultiPage_Button_Handle hObj)
{
    if (hObj)
    {
        MultiPage_Button_Obj* pObj;
        WM_LOCK();
        pObj = MultiPage_Button_H2P(hObj);
        pObj->MPBInfo.ProColor = pObj->ProColor1;
        MultiPage_Button_Invalidate(hObj);
        WM_UNLOCK();
    }
}
int MultiPage_Button_GetFocus(MultiPage_Button_Handle hObj)
{
    int bFocus = -1;
    if (hObj)
    {
        MultiPage_Button_Obj* pObj;
        WM_LOCK();
        pObj = MultiPage_Button_H2P(hObj);
        if(pObj->MPBInfo.ProColor == pObj->ProColor1)
        {
            bFocus = 1;
        }
        else if(pObj->MPBInfo.ProColor == pObj->ProColor2)
        {
            bFocus = 0;
        }
        else if(pObj->MPBInfo.ProColor == pObj->ProColor3)
        {
            bFocus = 2;
        }
        else
        {
            bFocus = -1;
        }
        WM_UNLOCK();
    }
    return bFocus;
}
#else
void WIDGET_viewWin(void) {} /* avoid empty object files */
#endif /* GUI_WINSUPPORT */
