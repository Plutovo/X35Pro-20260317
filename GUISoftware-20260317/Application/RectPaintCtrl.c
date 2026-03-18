/*************************************************************
标题:  RectPaintCtrl.c
功能:  矩形区域内的消息显示，分别在左上角、右上角、右下角显示数据的属性，在中间区域显示数据
说明:
当前版本： 0.1.0
修改记录：
2007.08.1   yanglin, created
**************************************************************/
#include <stdlib.h>
#include <string.h>
#include "RectPaintCtrl.h"
#include "WIDGET.h"
#include "GUI_Protected.h"
#include "Skinning_Lib.H"
//#include "ValuesInfoManage.h"
extern GUI_CONST_STORAGE GUI_BITMAP bmlimit_icon;
#if GUI_WINSUPPORT
#ifndef RECTPAINTCTRL_BKCOLOR_DEFAULT
#define RECTPAINTCTRL_BKCOLOR_DEFAULT             0x333333//0xD05356//0xCFB68B//0x6E6450
#endif
#define RECTPAINTCTRL_PROCOLOR_DEFAULT              GUI_BLACK
#define RECTPAINTCTRL_DEFAULT_TEXTCOLORTOPBOT       GUI_WHITE
#define RECTPAINTCTRL_DEFAULT_TEXTCOLORCEN          GUI_YELLOW//GUI_YAN_BLUE //cpp md 20130328
#define RECTPAINTCTRL_DEFAULT_TEXTTOPL              "TEXT"
#define RECTPAINTCTRL_DEFAULT_TEXTTOPR              "text"
#define RECTPAINTCTRL_DEFAULT_TEXTCENTERL           "--"
#define RECTPAINTCTRL_DEFAULT_TEXTCENTERR           ""
///Font
#define RECTPAINTCTRL_DEFAULT_FONTTOPL              &GUI_Font_16
#define RECTPAINTCTRL_DEFAULT_FONTBOTR              &GUI_Font_15//&GUI_Font8_1
#define RECTPAINTCTRL_DEFAULT_FONTCENL              &GUI_Font32B_ASCII//&GUI_Font_24//&GUI_FontD32 //cpp md 20130328
#define RECTPAINTCTRL_DEFAULT_FONTCENR              &GUI_FontD32 //cpp md 20130328
#define RECTPAINTCTRL_DEFAULT_SIGNBOT               0
#define RECTPAINTCTRL_DEFAULT_TEXTBOTRT             "50"
#define RECTPAINTCTRL_DEFAULT_TEXTBOTRB             "10"
typedef struct
{
    WIDGET Widget;
    WM_HMEM hpTextTopL;                               ///顶左显示内容Buffer句柄
    WM_HMEM hpTextTopR;                               ///顶右显示内容Buffer句柄
    WM_HMEM hpTextCenterL;                            ///中间左显示内容Buffer句柄
    WM_HMEM hpTextCenterR;                            ///中间右显示内容Buffer句柄
    WM_HMEM hpTextBotRT;
    WM_HMEM hpTextBotRB;
    GUI_POINT CapturePoint;
    RectPaintCtrlInfo RBCInfo;
    int IndexL;                                       ///在ValuesInfoMansge中的一种 左
    int IndexR;                                       ///在ValuesInfoMansge中的一种 右
} RectPaintCtrl_Obj;
static const RectPaintCtrlInfo RPCInfo_DEFAULT =
{
    RECTPAINTCTRL_BKCOLOR_DEFAULT,
    RECTPAINTCTRL_PROCOLOR_DEFAULT,
    RECTPAINTCTRL_DEFAULT_TEXTCOLORTOPBOT,
    RECTPAINTCTRL_DEFAULT_TEXTCOLORCEN,
    RECTPAINTCTRL_DEFAULT_TEXTTOPL,
    RECTPAINTCTRL_DEFAULT_TEXTTOPR,
    RECTPAINTCTRL_DEFAULT_TEXTCENTERL,
    RECTPAINTCTRL_DEFAULT_TEXTCENTERR,
    RECTPAINTCTRL_DEFAULT_FONTTOPL,
    RECTPAINTCTRL_DEFAULT_FONTBOTR,
    RECTPAINTCTRL_DEFAULT_FONTCENL,
    RECTPAINTCTRL_DEFAULT_FONTCENR,
    RECTPAINTCTRL_DEFAULT_SIGNBOT,
    RECTPAINTCTRL_DEFAULT_TEXTBOTRT,
    RECTPAINTCTRL_DEFAULT_TEXTBOTRB,
};
#define RectPaintCtrl_H2P(h) (RectPaintCtrl_Obj*) GUI_ALLOC_h2p(h)
#define RectPaintCtrl_Invalidate(hObj) WM_InvalidateWindow(hObj)
//
static void _DrawPaintText(RectPaintCtrl_Obj* pObj)
{
    GUI_RECT r;
    int i=0,j=0;
    GUI_GetClientRect(&r);
    //TextTopL显示在控件左上角的位置
    GUI_SetTextMode(GUI_TM_TRANS);
    GUI_SetColor(pObj->RBCInfo.TextColorTopBot);
    GUI_SetFont(pObj->RBCInfo.pFontTopL);
    GUI_SetTextAlign(GUI_TA_LEFT|GUI_TA_TOP);
    GUI_DispStringAt(pObj->RBCInfo.TextTopL,r.x0+4,r.y0+2);     //TopL

    
    GUI_SetTextAlign(GUI_TA_RIGHT|GUI_TA_TOP);

	if(pObj->RBCInfo.Rect_DispStyle == RECT_DISPSTYLE_SINGLE) //ylk add 20181123 
	{
		GUI_DispStringAt(pObj->RBCInfo.TextTopR,r.x1-2,r.y0+2);     //TopR
		GUI_SetFont(pObj->RBCInfo.pFontBotR);
	}
	else
	{
 		GUI_DispStringAt(pObj->RBCInfo.TextTopR,r.x1/3*2+50,r.y0+2); //????位置未定
 		GUI_SetFont(pObj->RBCInfo.pFontTopL);
	}

    //TextBotR显示在控件右下角的位置
	if(pObj->RBCInfo.Rect_DispStyle == RECT_DISPSTYLE_SINGLE) //ylk add 20181123 两个参数时右下角无内容
	{
		if(pObj->RBCInfo.SignBot)
		{
			//GUI_DrawBitmap(&bmlimit_icon,(r.x1-5-(pObj->RBCInfo.pFontBotR->YDist)*5),r.y1-18); //20140520
			GUI_SetFont(pObj->RBCInfo.pFontBotR);
			GUI_SetTextAlign(GUI_TA_RIGHT|GUI_TA_BOTTOM);
			GUI_DispStringAt(pObj->RBCInfo.TextBotRT,r.x1-2,r.y1-17); //Bottom R Top //13->14 cpp md 20130402
			GUI_SetTextAlign(GUI_TA_RIGHT|GUI_TA_BOTTOM);
			GUI_DispStringAt(pObj->RBCInfo.TextBotRB,r.x1-2,r.y1-5); //Bottom R B
		}	
	}

    //TextCenter显示在控件中间的位置
    GUI_SetColor(pObj->RBCInfo.TextColorCen);
    GUI_SetFont(pObj->RBCInfo.pFontCenL);
	if(pObj->RBCInfo.Rect_DispStyle == RECT_DISPSTYLE_SINGLE)//ylk modify 20181123
	{
		GUI_SetTextAlign(GUI_TA_HCENTER|GUI_TA_VCENTER);
		GUI_DispStringAt(pObj->RBCInfo.TextCenterL, (r.x0+r.x1)/2, (r.y0+r.y1)/2+7);  //20140520	
	}
	else//ylk add 20181123
	{
		GUI_SetTextAlign(GUI_TA_HCENTER|GUI_TA_VCENTER);
		GUI_DispStringAt(pObj->RBCInfo.TextCenterL, (r.x0+r.x1)/3-10,  (r.y0+r.y1)/2+9);
		GUI_SetTextAlign(GUI_TA_HCENTER|GUI_TA_VCENTER);
		GUI_DispStringAt(pObj->RBCInfo.TextCenterR, (r.x0+r.x1)/3*2+15,(r.y0+r.y1)/2+9);
	}
    
}
static void _Paint(RectPaintCtrl_Obj* pObj)
{
    /* Draw Frame */
    //int xsize = pObj->Widget.Win.Rect.x1 - pObj->Widget.Win.Rect.x0;
    //int ysize = pObj->Widget.Win.Rect.y1 - pObj->Widget.Win.Rect.y0;
    //GUI_SetColor  (pObj->RBCInfo.BkColor);
    //GUI_FillRect  (0, 0, xsize, ysize);
    //GUI_SetColor  (GUI_WHITE);
    //GUI_DrawRect  (0, 0, xsize, ysize);
    GUI_RECT rClient;
    GUI_GetClientRect(&rClient);
    LCD_SetColor(pObj->RBCInfo.ProColor);
    GUI_Clear();
    //LCD_SetColor(GUI_WHITE);  //sun del 20140520
    //Arc(rClient.x0 + 6, rClient.y0 + 6, rClient.x1 - 6, rClient.y1 - 6,5,6,0xE1);  //sun del 20140520
    LCD_SetColor(GUI_BLACK);
    Arc(rClient.x0 + 6, rClient.y0 + 6, rClient.x1 - 6, rClient.y1 - 6,5,6,0x1E);
    Bevel(rClient.x0 + 6, rClient.y0 + 6, rClient.x1 - 6, rClient.y1 - 6,6);
    //LCD_SetColor (pObj->Props.aBkColor[ColorIndex]);
    GUI_SetColor  (pObj->RBCInfo.BkColor);
    FillBevel(rClient.x0 +6,rClient.y0+6,rClient.x1-6,rClient.y1-6,4);
    if (pObj->hpTextBotRB)
    {
        pObj->RBCInfo.TextBotRB = (char*) GUI_ALLOC_h2p(pObj->hpTextBotRB);
    }
    if (pObj->hpTextBotRT)
    {
        pObj->RBCInfo.TextBotRT = (char*) GUI_ALLOC_h2p(pObj->hpTextBotRT);
    }
    if (pObj->hpTextCenterL)
    {
        pObj->RBCInfo.TextCenterL = (char*) GUI_ALLOC_h2p(pObj->hpTextCenterL);
    }
    if (pObj->hpTextCenterR)
    {
        pObj->RBCInfo.TextCenterR = (char*) GUI_ALLOC_h2p(pObj->hpTextCenterR);
    }
    if (pObj->hpTextTopL)
    {
        pObj->RBCInfo.TextTopL = (char*) GUI_ALLOC_h2p(pObj->hpTextTopL);
    }
    if (pObj->hpTextTopR)
    {
        pObj->RBCInfo.TextTopR = (char*) GUI_ALLOC_h2p(pObj->hpTextTopR);
    }
    _DrawPaintText(pObj);
    //WIDGET_EFFECT_3D_DrawUp();
}
/***************************************************************************************************
*/
static void _UpdateRectPaintCenterData(RectPaintCtrl_Obj* pObj)
{
    GUI_RECT r;
    GUI_GetClientRect(&r);
    WM_SetUserClipRect(&r);
    GUI_SetColor(pObj->RBCInfo.BkColor);//Data所在框的背景颜色
	//GUI_SetColor(GUI_RED);

	if(pObj->RBCInfo.Rect_DispStyle == RECT_DISPSTYLE_SINGLE)
	{
		GUI_FillRect(((r.x0+r.x1)/2-(pObj->RBCInfo.pFontCenL->YDist)-2),//20140520 +2
					((r.y0+r.y1)/2-(pObj->RBCInfo.pFontCenL->YSize)/2 + 10),
					((r.x0+r.x1)/2+(pObj->RBCInfo.pFontCenL->YDist)) + 16,//20140520
					((r.y0+r.y1)/2+(pObj->RBCInfo.pFontCenL->YSize)/2));
 	 	GUI_SetColor(pObj->RBCInfo.TextColorCen);//Data字体颜色
		GUI_SetTextMode(GUI_TM_TRANS);
		GUI_SetFont(pObj->RBCInfo.pFontCenL);
		GUI_SetTextAlign(GUI_TA_HCENTER|GUI_TA_VCENTER);
		GUI_DispStringAt(pObj->RBCInfo.TextCenterL, (r.x0+r.x1)/2, (r.y0+r.y1)/2+7); //20140520
		WM_SetUserClipRect(NULL);//ylk add 20190124
	}
	else
	{
		GUI_FillRect(((r.x0+r.x1)/2-(pObj->RBCInfo.pFontCenL->YDist)-30),//ylk modify -10
			((r.y0+r.y1)/2-(pObj->RBCInfo.pFontCenL->YSize)/2+10),
			((r.x0+r.x1)/2+(pObj->RBCInfo.pFontCenL->YDist)-35),
			((r.y0+r.y1)/2+(pObj->RBCInfo.pFontCenL->YSize)/2+3));
		GUI_FillRect(((r.x0+r.x1)/2-(pObj->RBCInfo.pFontCenL->YDist)+40),//ylk modify
			((r.y0+r.y1)/2-(pObj->RBCInfo.pFontCenL->YSize)/2+12),
			((r.x0+r.x1)/2+(pObj->RBCInfo.pFontCenL->YDist)+40),
			((r.y0+r.y1)/2+(pObj->RBCInfo.pFontCenL->YSize)/2+3));
		GUI_SetColor(pObj->RBCInfo.TextColorCen);
		GUI_SetTextMode(GUI_TM_TRANS);
		GUI_SetFont(pObj->RBCInfo.pFontCenL);
		GUI_SetTextAlign(GUI_TA_HCENTER|GUI_TA_VCENTER);
		GUI_DispStringAt(pObj->RBCInfo.TextCenterL, (r.x0+r.x1)/3-10, (r.y0+r.y1)/2+9);//ylk modify

		GUI_SetColor(pObj->RBCInfo.TextColorCen);
		GUI_SetTextMode(GUI_TM_TRANS);
		GUI_SetFont(pObj->RBCInfo.pFontCenL);
		GUI_SetTextAlign(GUI_TA_HCENTER|GUI_TA_VCENTER);
		GUI_DispStringAt(pObj->RBCInfo.TextCenterR, (r.x0+r.x1)/3*2+15, (r.y0+r.y1)/2+9);//ylk modify
		WM_SetUserClipRect(NULL);
	}
}
/***************************************************************************************************
*/
static void _UpdateRectPaintBottomData(RectPaintCtrl_Obj* pObj)
{
    GUI_RECT r;
    GUI_GetClientRect(&r);
    WM_SetUserClipRect(&r);
    if(pObj->RBCInfo.SignBot)
    {
        GUI_GetClientRect(&r);
        GUI_SetColor(pObj->RBCInfo.BkColor);
        GUI_SetTextMode(GUI_TM_TRANS);
        GUI_SetFont(pObj->RBCInfo.pFontBotR);
        GUI_FillRect((r.x1 - 5 - (pObj->RBCInfo.pFontBotR->YDist)*2),
                     (r.y1 - 15 -(pObj->RBCInfo.pFontBotR->YSize)),(r.x1-2),(r.y1-20)); //15->14 cpp md 20130402
        GUI_FillRect((r.x1 - 5 - (pObj->RBCInfo.pFontBotR->YDist)*2),
                     (r.y1 - 8 -(pObj->RBCInfo.pFontBotR->YSize)),(r.x1-2),(r.y1-5));
        GUI_SetTextAlign(GUI_TA_RIGHT|GUI_TA_BOTTOM);
        GUI_SetColor(pObj->RBCInfo.TextColorTopBot);
		GUI_DispStringAt(pObj->RBCInfo.TextBotRT,r.x1-2,r.y1-17); //Bottom R Top //13->14 cpp md 20130402
        GUI_SetTextAlign(GUI_TA_RIGHT|GUI_TA_BOTTOM);
        GUI_DispStringAt(pObj->RBCInfo.TextBotRB,r.x1-2,r.y1-5); //Bottom R B
    }
    WM_SetUserClipRect(NULL);
}
/***************************************************************************************************
*    20130412 
*/
static void _UpdateRectPaintTopRText(RectPaintCtrl_Obj* pObj)
{
    GUI_RECT r;
    GUI_GetClientRect(&r);
    WM_SetUserClipRect(&r);
    GUI_SetColor(pObj->RBCInfo.BkColor);
    GUI_SetTextMode(GUI_TM_TRANS);
    GUI_SetFont(pObj->RBCInfo.pFontBotR);
    GUI_FillRect(((r.x0+r.x1)/2-(pObj->RBCInfo.pFontBotR->YDist)),
                 ((r.y0+r.y1)/2-(pObj->RBCInfo.pFontBotR->YSize)/2),
                 ((r.x0+r.x1)/2+(pObj->RBCInfo.pFontBotR->YDist)),
                 ((r.y0+r.y1)/2+(pObj->RBCInfo.pFontBotR->YSize)/2));
    GUI_SetTextAlign(GUI_TA_RIGHT|GUI_TA_TOP);
	GUI_SetColor(pObj->RBCInfo.TextColorTopBot);

	if(pObj->RBCInfo.Rect_DispStyle == RECT_DISPSTYLE_SINGLE)//ylk add 20181123
	{
		GUI_DispStringAt(pObj->RBCInfo.TextTopR,r.x1-2,r.y0+2);     //TopR
	}
	else
	{
		GUI_DispStringAt(pObj->RBCInfo.TextTopR,r.x1/3*2+70,r.y0+2);//ylk add
	}

    WM_SetUserClipRect(NULL);
}
/***************************************************************************************************
*/
static void _RectPaintCtrl_Callback (WM_MESSAGE *pMsg)
{
    RectPaintCtrl_Handle hWin = (RectPaintCtrl_Handle)(pMsg->hWin);
    RectPaintCtrl_Obj* pObj = RectPaintCtrl_H2P(hWin);
    if (WIDGET_HandleActive(hWin, pMsg) == 0)
    {
        return;
    }
    //GUI_RECT* pRect = (GUI_RECT*)(pMsg->Data.p);
    switch (pMsg->MsgId)
    {
    case WM_PAINT:
        _Paint(pObj);
        break;
    case WM_TOUCH:
        break;
    case WM_DELETE:
        GUI_ALLOC_FreePtr(&pObj->hpTextBotRB);
        GUI_ALLOC_FreePtr(&pObj->hpTextBotRT);
        GUI_ALLOC_FreePtr(&pObj->hpTextCenterL);
        GUI_ALLOC_FreePtr(&pObj->hpTextCenterR);
        GUI_ALLOC_FreePtr(&pObj->hpTextTopL);
        GUI_ALLOC_FreePtr(&pObj->hpTextTopR);
        break;
    case WM_NOTIFICATION_VALUE_CHANGED:
        switch (pMsg->Data.v)
        {
        case CENTER_DATA_CHANGED:
            _UpdateRectPaintCenterData(pObj);
            break;
        case BOTTOM_DATA_CHANGED:
			if(pObj->RBCInfo.Rect_DispStyle == RECT_DISPSTYLE_SINGLE) //ylk add 20181207
			{
				_UpdateRectPaintBottomData(pObj);
			}
            break;
        case TOP_R_TEXT_CHANGED:
			if(pObj->RBCInfo.Rect_DispStyle == RECT_DISPSTYLE_SINGLE) //ylk add 20181207
			{
				_UpdateRectPaintTopRText(pObj);
			}
            break;
        default:break;
        }
        break;
    }
    /* Let widget handle the standard messages */
    WM_DefaultProc(pMsg);
}
/***************************************************************************************************
*/
RectPaintCtrl_Handle RectPaintCtrl_CreateAsChild(
    int x0, int y0, int xsize, int ysize, WM_HWIN hParent,int Id,
    WM_CALLBACK* cb, int Flags)
{
    RectPaintCtrl_Handle hObj;
    /* Create the window */
    WM_LOCK();
    hObj = WM_CreateWindowAsChild(x0, y0, xsize/*+2*HBorder*/, ysize/*+TBorder+BBorder*/, hParent,
                                  Flags, _RectPaintCtrl_Callback, sizeof(RectPaintCtrl_Obj) - sizeof(WM_Obj));
    if (hObj)
    {
        WM_HMEM hMem1 = 0;
        WM_HMEM hMem2 = 0;
        WM_HMEM hMem3 = 0;
        WM_HMEM hMem4 = 0;
        WM_HMEM hMem5 = 0;
        WM_HMEM hMem6 = 0;
        RectPaintCtrl_Obj* pObj = RectPaintCtrl_H2P(hObj);
        /* init widget specific variables */
        WIDGET__Init(&pObj->Widget,Id, WIDGET_STATE_FOCUSSABLE );
        pObj->RBCInfo = RPCInfo_DEFAULT;
        pObj->IndexL = -1;
        pObj->IndexR = -1;
        //sizeof(pObj->RBCInfo.TextBotRT) ???
        //TextBotRT
        hMem1 = GUI_ALLOC_AllocZero(sizeof(pObj->RBCInfo.TextBotRT) + 1);
        if (hMem1)
        {
            strcpy((char*) GUI_ALLOC_h2p(hMem1), pObj->RBCInfo.TextBotRT);
        }
        pObj->hpTextBotRT = hMem1;        //20140103  djq add   pObj->hpTextBotRB = hMem1
        //TextBotRB
        hMem2 = GUI_ALLOC_AllocZero(sizeof(pObj->RBCInfo.TextBotRB) + 1);
        if (hMem2)
        {
            strcpy((char*) GUI_ALLOC_h2p(hMem2), pObj->RBCInfo.TextBotRB);
        }
        pObj->hpTextBotRB = hMem2;
        //TextCenterL
        hMem3 = GUI_ALLOC_AllocZero(sizeof(pObj->RBCInfo.TextCenterL) + 1);
        if (hMem3)
        {
            strcpy((char*) GUI_ALLOC_h2p(hMem3), pObj->RBCInfo.TextCenterL);
        }
        pObj->hpTextCenterL = hMem3;
        //TextTopL
        hMem5 = GUI_ALLOC_AllocZero(sizeof(pObj->RBCInfo.TextTopL) + 1);
        if (hMem5)
        {
            strcpy((char*) GUI_ALLOC_h2p(hMem5), pObj->RBCInfo.TextTopL);
        }
        pObj->hpTextTopL = hMem5;
        //TextTopR
        hMem6 = GUI_ALLOC_AllocZero(sizeof(pObj->RBCInfo.TextTopR) + 1);
        if (hMem6)
        {
            strcpy((char*) GUI_ALLOC_h2p(hMem6), pObj->RBCInfo.TextTopR);
        }
        pObj->hpTextTopR = hMem6;
        //TextCenterR
        hMem4 = GUI_ALLOC_AllocZero(sizeof(pObj->RBCInfo.TextCenterR) + 1);
        if (hMem4)
        {
            strcpy((char*) GUI_ALLOC_h2p(hMem4), pObj->RBCInfo.TextCenterR);
        }
        pObj->hpTextCenterR = hMem4;
    }
    WM_UNLOCK();
    return hObj;
}
RectPaintCtrl_Handle RectPaintCtrl_Create(WM_CALLBACK* cb,
        int Flags,
        int x0, int y0, int xsize, int ysize,int Id)
{
    return RectPaintCtrl_CreateAsChild(x0, y0, xsize, ysize, WM_HWIN_NULL,Id, cb, Flags);
}
RectPaintCtrl_Handle RectPaintCtrl_CreateIndirect(const GUI_WIDGET_CREATE_INFO* pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK* pCallback)
{
    RectPaintCtrl_Handle hObj;
    hObj = RectPaintCtrl_CreateAsChild(
               pCreateInfo->x0 + x0, pCreateInfo->y0 + y0, pCreateInfo->xSize, pCreateInfo->ySize, hWinParent, pCreateInfo->Id,
               pCallback, 0);
    return hObj;
}
void RectPaintCtrl_SetBkColor(RectPaintCtrl_Handle hObj,GUI_COLOR BkColor)
{
    if (hObj)
    {
        RectPaintCtrl_Obj* pObj;
        WM_LOCK();
        pObj = RectPaintCtrl_H2P(hObj);
        pObj->RBCInfo.BkColor = BkColor;
        RectPaintCtrl_Invalidate(hObj);
        WM_UNLOCK();
    }
}
void RectPaintCtrl_SetColor(RectPaintCtrl_Handle hObj,GUI_COLOR TextColorTopBot,GUI_COLOR TextColorCen)
{
    if (hObj)
    {
        RectPaintCtrl_Obj* pObj;
        WM_LOCK();
        pObj = RectPaintCtrl_H2P(hObj);
        pObj->RBCInfo.TextColorTopBot = TextColorTopBot;
        pObj->RBCInfo.TextColorCen = TextColorCen;
        RectPaintCtrl_Invalidate(hObj);
        WM_UNLOCK();
    }
}
void RectPaintCtrl_SetMaxMinText(RectPaintCtrl_Handle hObj,int Sign,char* MinText,char* MaxText)
{
    if (hObj)
    {
        RectPaintCtrl_Obj* pObj;
        WM_LOCK();
        pObj = RectPaintCtrl_H2P(hObj);
        pObj->RBCInfo.SignBot = Sign;
        if (GUI__SetText(&pObj->hpTextBotRT, MaxText))
        {
            pObj->RBCInfo.TextBotRT = (char*) GUI_ALLOC_h2p(pObj->hpTextBotRT);
        }
        if (GUI__SetText(&pObj->hpTextBotRB, MinText))
        {
            pObj->RBCInfo.TextBotRB = (char*) GUI_ALLOC_h2p(pObj->hpTextBotRB);
        }
        RectPaintCtrl_Invalidate(hObj);
        WM_UNLOCK();
    }
}
void RectPaintCtrl_SetMinText(RectPaintCtrl_Handle hObj,char* MinText)
{
    if (hObj)
    {
        RectPaintCtrl_Obj* pObj;
        WM_LOCK();
        pObj = RectPaintCtrl_H2P(hObj);
        if (GUI__SetText(&pObj->hpTextBotRB, MinText))
        {
            pObj->RBCInfo.TextBotRB = (char*) GUI_ALLOC_h2p(pObj->hpTextBotRB);
        }
        RectPaintCtrl_Invalidate(hObj);
        WM_UNLOCK();
    }
}
void RectPaintCtrl_SetMaxText(RectPaintCtrl_Handle hObj,char* MaxText)
{
    if (hObj)
    {
        RectPaintCtrl_Obj* pObj;
        WM_LOCK();
        pObj = RectPaintCtrl_H2P(hObj);
        if (GUI__SetText(&pObj->hpTextBotRT, MaxText))
        {
            pObj->RBCInfo.TextBotRT = (char*) GUI_ALLOC_h2p(pObj->hpTextBotRT);
        }
        RectPaintCtrl_Invalidate(hObj);
        WM_UNLOCK();
    }
}
void RectPaintCtrl_SetMaxData(RectPaintCtrl_Handle hObj,int data)
{
    char MaxText[20] = {0};
    RectPaintCtrl_Obj* pObj;
    if (hObj)
    {
        sprintf(MaxText,"%d",data);
        WM_LOCK();
        pObj = RectPaintCtrl_H2P(hObj);
        if (GUI__SetText(&pObj->hpTextBotRT, MaxText))
        {
            pObj->RBCInfo.TextBotRT = (char*) GUI_ALLOC_h2p(pObj->hpTextBotRT);
        }
        //RectPaintCtrl_Invalidate(hObj);
        WM_UNLOCK();
    }
}
void RectPaintCtrl_SetMinData(RectPaintCtrl_Handle hObj,int data)
{
    char MinText[20] = {0};
    RectPaintCtrl_Obj* pObj;
    if (hObj)
    {
        sprintf(MinText,"%d",data);
        WM_LOCK();
        pObj = RectPaintCtrl_H2P(hObj);
        if (GUI__SetText(&pObj->hpTextBotRB, MinText))
        {
            pObj->RBCInfo.TextBotRB = (char*) GUI_ALLOC_h2p(pObj->hpTextBotRB);
        }
        //RectPaintCtrl_Invalidate(hObj);
        WM_UNLOCK();
    }
}
void RectPaintCtrl_SetSignBottom(RectPaintCtrl_Handle hObj,int sign)
{
    if (hObj)
    {
        RectPaintCtrl_Obj* pObj;
        WM_LOCK();
        pObj = RectPaintCtrl_H2P(hObj);
        pObj->RBCInfo.SignBot = sign;
        //RectPaintCtrl_Invalidate(hObj);
        WM_UNLOCK();
    }
}
void RectPaintCtrl_SetCenterText(RectPaintCtrl_Handle hObj,char* TextCenterL,char* TextCenterR)
{
    if (hObj)
    {
        RectPaintCtrl_Obj* pObj;
        WM_LOCK();
        pObj = RectPaintCtrl_H2P(hObj);
        if (GUI__SetText(&pObj->hpTextCenterL, TextCenterL))
        {
            pObj->RBCInfo.TextCenterL = (char*) GUI_ALLOC_h2p(pObj->hpTextCenterL);
        }
        if (GUI__SetText(&pObj->hpTextCenterR, TextCenterR))
        {
            pObj->RBCInfo.TextCenterR = (char*) GUI_ALLOC_h2p(pObj->hpTextCenterR);
        }
        //	pObj->RBCInfo.TextCenterL = TextCenterL;
        //	pObj->RBCInfo.TextCenterR = TextCenterR;
        //RectPaintCtrl_Invalidate(hObj);
        WM_UNLOCK();
    }
}
void RectPaintCtrl_SetCenterTextL(RectPaintCtrl_Handle hObj,char* TextCenterL)
{
    if (hObj)
    {
        RectPaintCtrl_Obj* pObj;
        WM_LOCK();
        pObj = RectPaintCtrl_H2P(hObj);
        if (GUI__SetText(&pObj->hpTextCenterL, TextCenterL))
        {
            pObj->RBCInfo.TextCenterL = (char*) GUI_ALLOC_h2p(pObj->hpTextCenterL);
        }
        //RectPaintCtrl_Invalidate(hObj);
        WM_UNLOCK();
    }
}
void RectPaintCtrl_SetCenterTextR(RectPaintCtrl_Handle hObj,char* TextCenterR)
{
    if (hObj)
    {
        RectPaintCtrl_Obj* pObj;
        WM_LOCK();
        pObj = RectPaintCtrl_H2P(hObj);
        if (GUI__SetText(&pObj->hpTextCenterR, TextCenterR))
        {
            pObj->RBCInfo.TextCenterR = (char*) GUI_ALLOC_h2p(pObj->hpTextCenterR);
        }
        //RectPaintCtrl_Invalidate(hObj);
        WM_UNLOCK();
    }
}
void RectPaintCtrl_SetTopText(RectPaintCtrl_Handle hObj,char* TextTopL,char* TextTopR)
{
    unsigned int i;
    if (hObj)
    {
        RectPaintCtrl_Obj* pObj;
        WM_LOCK();
        pObj = RectPaintCtrl_H2P(hObj);
        if (GUI__SetText(&pObj->hpTextTopL, TextTopL))
        {
            pObj->RBCInfo.TextTopL = (char*) GUI_ALLOC_h2p(pObj->hpTextTopL);
        }
        if (GUI__SetText(&pObj->hpTextTopR, TextTopR))
        {
            pObj->RBCInfo.TextTopR = (char*) GUI_ALLOC_h2p(pObj->hpTextTopR);
        }
        //
        pObj->IndexL = -1;
        pObj->IndexR = -1;
        for(i=0; i<=ValuesInfo_end; i++)
        {
            if(!strcmp(pObj->RBCInfo.TextTopL,ValuesInfo[i]))
            {
                pObj->IndexL = i;
                break;
            }
        }
        for(i=0; i<=ValuesInfo_end; i++)
        {
            if(!strcmp(pObj->RBCInfo.TextTopR,ValuesInfo[i]))
            {
                pObj->IndexR = i;
                break;
            }
        }
        //pObj->RBCInfo.TextTopL = TextTopL;
        //pObj->RBCInfo.TextTopR = TextTopR;
        RectPaintCtrl_Invalidate(hObj);
        WM_UNLOCK();
    }
}
void RectPaintCtrl_SetTopTextL(RectPaintCtrl_Handle hObj,char* TextTopL)
{
	unsigned int i;
	if (hObj)
	{
		RectPaintCtrl_Obj* pObj;
		WM_LOCK();
		pObj = RectPaintCtrl_H2P(hObj);
		if (GUI__SetText(&pObj->hpTextTopL, TextTopL))
		{
			pObj->RBCInfo.TextTopL = (char*) GUI_ALLOC_h2p(pObj->hpTextTopL);
		}
		//
		pObj->IndexL = -1;
		for(i=0; i<=ValuesInfo_end; i++)
		{
			if(!strcmp(pObj->RBCInfo.TextTopL,ValuesInfo[i]))
			{
				pObj->IndexL = i;
				break;
			}
		}
		RectPaintCtrl_Invalidate(hObj);
		WM_UNLOCK();
	}
}

///SetFont
void RectPaintCtrl_SetFont(RectPaintCtrl_Handle hObj,const GUI_FONT GUI_UNI_PTR * pFont,int Index)
{
    if (hObj)
    {
        RectPaintCtrl_Obj* pObj;
        WM_LOCK();
        pObj = RectPaintCtrl_H2P(hObj);
        switch(Index)
        {
        case 0:                     ///左上角字体大小
            pObj->RBCInfo.pFontTopL = pFont;
            break;
        case 1:                     ///右下角字体大小
            pObj->RBCInfo.pFontBotR = pFont;
            break;
        case 2:                     ///中间的位置左字体大小
            pObj->RBCInfo.pFontCenL = pFont;
            break;
        case 3:                     ///中间的位置右字体大小
            pObj->RBCInfo.pFontCenR = pFont;
            break;
        }
        RectPaintCtrl_Invalidate(hObj);
        WM_UNLOCK();
    }
}
int RectPaintCtrl_GetIndexL(RectPaintCtrl_Handle hObj)
{
    int Index = -1;
    if (hObj)
    {
        RectPaintCtrl_Obj* pObj;
        WM_LOCK();
        pObj = RectPaintCtrl_H2P(hObj);
        Index = pObj->IndexL;
        WM_UNLOCK();
    }
    return Index;
}
int RectPaintCtrl_GetIndexR(RectPaintCtrl_Handle hObj)
{
    int Index = -1;
    if (hObj)
    {
        RectPaintCtrl_Obj* pObj;
        WM_LOCK();
        pObj = RectPaintCtrl_H2P(hObj);
        Index = pObj->IndexR;
        WM_UNLOCK();
    }
    return Index;
}
/*Set Display Style*/
void RectPaintCtrl_SetDispStyle(RectPaintCtrl_Handle hObj,RECT_DISPSTYLE DispStyle)
{
    if (hObj)
    {
        RectPaintCtrl_Obj* pObj;
        WM_LOCK();
        pObj = RectPaintCtrl_H2P(hObj);
        pObj->RBCInfo.Rect_DispStyle = DispStyle;
        WM_UNLOCK();
    }
}
#else
void WIDGET_viewWin(void) {} /* avoid empty object files */
#endif /* GUI_WINSUPPORT */
