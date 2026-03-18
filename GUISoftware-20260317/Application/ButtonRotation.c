//
/***************************************************************************************************
标题:  ButtonRotation.c
功能:  复杂的控件，能通过上下、左右键调整显示内容，并有一个小的线条位置的改变
说明:   控制部分的数据为10的FloatLen次方倍的数据，与显示的数据不同，要保证传进的数据为整形
当前版本： 0.1.0
修改记录：
2008.09.07   xumiaohai, created
***************************************************************************************************/
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include "ButtonRotation.h"
#include "WIDGET.h"
#include "GUI_Protected.h"
#include "MainTask.h"
//djq add 20170407

//
#ifndef SIZE_OF_ARRAY
#define SIZE_OF_ARRAY(Array) (sizeof(Array) / sizeof(Array[0]))
#endif

#if GUI_WINSUPPORT

#ifndef GUI_YELLOW_EX
#define GUI_YELLOW_EX                   0x00DDDD
#endif
#ifndef VIEWCOLOR_BLACK
#define VIEWCOLOR_BLACK                 0x404040
#endif
#ifndef VIEWCOLOR_PROBUTTON
#define VIEWCOLOR_PROBUTTON             0x4080FF
#endif
//
#define BUTTONEXCTRL_DEFAULT_BKCOLOR        GUI_YAN_BLUE
#define BUTTONEXCTRL_DEFAULT_PRCOLOR        0xD05356
#define BUTTONEXCTRL_DEFAULT_TEXTCOLOR      GUI_YELLOW
#define BUTTONEXCTRL_DEFAULT_TEXTFONT       &GUI_Font_16
#define BUTTONEXCTRL_DEFAULT_UNITFONT       &GUI_Font_15
#define BUTTONEXCTRL_DEFAULT_TEXT           "36"
#define BUTTONEXCTRL_DEFAULT_TEXT1          ""
#define BUTTONEXCTRL_DEFAULT_TEXT2          ""
#define BUTTONEXCTRL_DEFAULT_MIN            0
#define BUTTONEXCTRL_DEFAULT_MAX            100
#define BUTTONEXCTRL_DEFAULT_DEFAULTSIZE    20
#define BUTTONEXCTRL_DEFAULT_STEP           1
#define BUTTONEXCTRL_DEFAULT_FLOATLEN       0
//extern const GUI_BITMAP bmArcL;           ///弧线图标
static  int keySign = 0;                    //只响应一次左键弹起消息
//
// static const int IE_RatioTable[] = 
// {40,35,30,25,20,15,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100};
extern const int IE_RatioTable[];
//
static const ButtonRotationInfo BECInfo_DEFAULT =
{
    BUTTONEXCTRL_DEFAULT_BKCOLOR,         //背景色
    BUTTONEXCTRL_DEFAULT_PRCOLOR,         //前景色
    VIEWCOLOR_BLACK,                      //标签的颜色
    VIEWCOLOR_PROBUTTON,                  //颜色画圆弧、圆的外边缘
    BUTTONEXCTRL_DEFAULT_TEXTCOLOR,       //字体颜色
    BUTTONEXCTRL_DEFAULT_TEXTFONT,        //字体大小
    BUTTONEXCTRL_DEFAULT_UNITFONT,
    BUTTONEXCTRL_DEFAULT_TEXT,            //显示内容在中间
    BUTTONEXCTRL_DEFAULT_TEXT1,           //显示内容在左方
    BUTTONEXCTRL_DEFAULT_TEXT2            //显示单位
};
//
static const ButtonRotationDataInfo BDInfo_DEFAULT =
{
    BUTTONEXCTRL_DEFAULT_MIN,            //最小值
    BUTTONEXCTRL_DEFAULT_MAX,            //最大值
    BUTTONEXCTRL_DEFAULT_DEFAULTSIZE,    //默认值　小的三角形指向的值
    BUTTONEXCTRL_DEFAULT_STEP,           //步距
    BUTTONEXCTRL_DEFAULT_FLOATLEN        //浮点数据的长度 0:为整数据 1:为浮点数小数点后显示的位数为1
};

/***************************************************************************************************
*
*
*/
static void _OnTouch(ButtonRotation_Handle hWin, ButtonRotation_Obj* pObj, WM_MESSAGE* pMsg)
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
            WM_SelectWindow(hWin);
            //WM_BringToTop(hWin);
            //WM_SetFocus(hWin);
            /*if(pObj->triangleSign!=1)
            {
                pObj->triangleSign = 1;
            }
            else
            {
                pObj->triangleSign = 0;
            }*/
            WM_NotifyParent(hWin, WM_NOTIFICATION_CLICKED);
            //WM_BringToTop(hWin);
            //keySign = 1;
            /*if (!WM_HasCaptured(hWin)) {
                WM_SetCapture(hWin, 1);
                pObj->CapturePoint.x = pState->x;
                pObj->CapturePoint.y = pState->y;
            } */
        }
    }
    else
    {
        if ((pObj->Widget.State & BUTTON_STATE_PRESSED) == 0)
        {
            /*if(keySign==1){
                keySign++;
                //当有按键按下，颜色将改变
                if((pObj->BECInfo.ProColor)==(pObj->ProColor1)){
                    pObj->BECInfo.ProColor = pObj->ProColor2;
                    pObj->BECInfo.LineColor = pObj->LineColor2;
                    //pObj->BECInfo.Text1 = pObj->Text12;
                    //pObj->BECInfo.TextColor = pObj->TextColor2;
                }else{
                    pObj->BECInfo.ProColor = pObj->ProColor1;
                    pObj->BECInfo.LineColor = pObj->LineColor1;
                    //pObj->BECInfo.Text1 = pObj->Text11;
                    //bj->BECInfo.TextColor = pObj->TextColor1;
                }*/
            WM_InvalidateWindow(hWin);
            //发送消息
            Msg.MsgId   = WM_NOTIFY_PARENT;
            Msg.Data.v  = WM_NOTIFICATION_RELEASED;
            WM_SendToParent(hWin, &Msg);
            //}
        }
    }
    //}
}

/***************************************************************************************************
*
*
*/
//旋转指针
static void _DrawPaintLine(GUI_RECT r,int Angle)
{
    float x,y;
    float angle = -Angle*3.1415926/180;
    x=(r.x1-r.x0)*130/185-((r.y1-r.y0)*55/94-2)*cos(angle);
    y=(r.y1-r.y0)*55/86+((r.y1-r.y0)*55/94-2)*sin(angle);
    GUI_DrawLine(x,y,(r.x1-r.x0)*130/185,(r.y1-r.y0)*55/86);
}

/***************************************************************************************************
*
*
*/
//x的n次方
static void square(int x, int n, int *result)
{
    if (n > 0)
    {
        (*result) *= x;
        square(x, --n, &(*result));
    }
}

/***************************************************************************************************
*
*
*/
//将整形数据，转换为实际数据，_Float2String
static char* _Float2String(int nData,int floatLen)
{
    float fData;
    int result = 1;
    static char DataTemp[10] = "--";
    char strfmt[8];
    sprintf(strfmt, "%%.%df", floatLen);
    square(10,floatLen,&result);
    fData = (float)nData/result;
    sprintf(DataTemp, strfmt, fData);
    return DataTemp;
}

extern char* _IEDataStr(int data);
extern char* _PEEPDataStr(int data);

/***************************************************************************************************
*
*
*/
static void _DrawPaintButton(ButtonRotation_Obj* pObj)
{
    GUI_RECT r,oldr;
        GUI_RECT r1;
    char cTemp[8]= {0}; //  20130513
    char* szTemp;
    int Angle = 90;
    GUI_GetClientRect(&r);
    oldr = r;
    if (pObj->Widget.State & WIDGET_STATE_FOCUS)
    {
        pObj->BECInfo.TextColor = 0x4080FF;//pObj->TextColor2;
    }
    else
    {
        pObj->BECInfo.TextColor = GUI_WHITE;//pObj->TextColor1;
    }
    //圆弧
    if(pObj->triangleSign!=1) //没按下
    {
        GUI_SetPenSize(1);
        if (pObj->Widget.Win.Status & WM_SF_DISABLED)
        {
            GUI_SetColor(0xA0A0A0);
        }
        else
        {
            GUI_SetColor(pObj->BECInfo.ProColor);
        }
        GUI_FillCircle(r.x1/2,r.x1/2,r.x1/2-5);
        GUI_SetColor(GUI_WHITE);
        GUI_DrawArc( r.x1/2,r.x1/2,r.x1/2-5,r.x1/2-5,45, 235 );
        GUI_SetColor(GUI_BLACK);//0x222222 //cpp md 20130328
        GUI_DrawArc( r.x1/2,r.x1/2,r.x1/2-5,r.x1/2-5,-135, 45 );
    }
    else
    {
        GUI_SetPenSize(1);
        GUI_SetColor(pObj->ProColor1);
        GUI_FillCircle(r.x1/2,r.x1/2,r.x1/2-5);
        GUI_SetColor(GUI_BLACK);//0x222222 //cpp md 20130328
        GUI_DrawArc( r.x1/2,r.x1/2,r.x1/2-5,r.x1/2-5,45, 235 );
        GUI_SetColor(GUI_WHITE);
        GUI_DrawArc( r.x1/2,r.x1/2,r.x1/2-5,r.x1/2-5,-135, 45 );
        pObj->BECInfo.TextColor = GUI_BLACK;
    }
    //画标签，数据的变化，标签也在变化
    //大于最大值或小于最小值，显示的内容为最大值或最小值
    if(pObj->BDInfo.Max<pObj->Data)
    {
        pObj->Data = pObj->BDInfo.Max;
    }
    if(pObj->BDInfo.Min>pObj->Data)
    {
        pObj->Data = pObj->BDInfo.Min;
    }
    if(pObj->BDInfo.Max<pObj->Data1)
    {
        pObj->Data1 = pObj->BDInfo.Max;
    }
    if(pObj->BDInfo.Min>pObj->Data1)
    {
        pObj->Data1 = pObj->BDInfo.Min;
    }

    //*//  20130510
    if(!strcmp(pObj->Text11, Sys_Lib_GetDispStr(DP_IE)))
    {
        if(pObj->Data>=6)
        {
           /* if(pObj->Data1!=24)*/
			 if(pObj->Data1!=pObj->BDInfo.Max)
            {
                sprintf(cTemp,"1:%2.1f",(float)(IE_RatioTable[pObj->Data1])/10);
            }
            else
            {
                sprintf(cTemp,"1:%2.0f",(float)(IE_RatioTable[pObj->Data1])/10);
            }
        }
        else
        {
            sprintf(cTemp,"%2.1f:1",(float)(IE_RatioTable[pObj->Data])/10);
        }
        szTemp = cTemp;
    } //*///
    else
    {
        szTemp = _Float2String(pObj->Data,pObj->BDInfo.FloatLen);
    }

    switch(pObj->DataInvildSign)
    {
    case 1: //下限关闭
        if(pObj->Data<=pObj->BDInfo.Min)
        {
            szTemp = Sys_Lib_GetDispStr(DP_OFF);
        }
        else
        {
            szTemp = _Float2String(pObj->Data,pObj->BDInfo.FloatLen);
        }
        break;
    case 2: //上限关闭
        if(pObj->Data>=pObj->BDInfo.Max)
        {
            szTemp = Sys_Lib_GetDispStr(DP_OFF);
        }
        else
        {
            szTemp = _Float2String(pObj->Data,pObj->BDInfo.FloatLen);
        }
        break;
    case 3: //  21030409
        if(pObj->Data<=pObj->BDInfo.Min)
        {
            szTemp = Sys_Lib_GetDispStr(DP_ON);
        }        
        else if(pObj->Data>=pObj->BDInfo.Max)
        {
            szTemp = Sys_Lib_GetDispStr(DP_OFF);
        }
        else
        {
            szTemp = _Float2String(pObj->Data,pObj->BDInfo.FloatLen);
        }
        break;
    case 4: //djq add 20170407 这个地方后续需要优化，可以是两态切换的按钮，显示的字样可以传入
        if(pObj->Data >= pObj->BDInfo.Max)
        {
            szTemp = Sys_Lib_GetDispStr(DP_AIR);
        }        
        else
        {
            szTemp = Sys_Lib_GetDispStr(DP_O2);
        }
        break;
    default:
        {
            //szTemp = _Float2String(pObj->Data,pObj->BDInfo.FloatLen);
        }
        break;
    }
    pObj->BECInfo.Text = szTemp;
    GUI_SetTextMode(GUI_TM_TRANS);
    /*if((pObj->BECInfo.ProColor)==(pObj->ProColor1))
    {
        GUI_SetColor(pObj->TextColor1);
    }
    else
    {
        GUI_SetColor(pObj->TextColor2);
    }*/
    GUI_SetFont(pObj->BECInfo.pTextFont);
    oldr = r;
    oldr.y1 = oldr.x1;
    //中间text内容的显示判断部分
    if(pObj->textSign)
    {
        GUI_SetColor(pObj->BECInfo.TextColor);
        GUI_SetFont(&GUI_Font_16);
        GUI_SetTextAlign(GUI_TA_HCENTER|GUI_TA_VCENTER);
        GUI_DispStringAt(pObj->BECInfo.Text,r.x1/2,r.x1/2);
        //GUI_SetTextAlign(GUI_TA_HCENTER|GUI_TA_VCENTER);
        //GUI_DispStringAt(pObj->BECInfo.Text2,(r.x1-r.x0)*130/185,r.y1-GUI_GetFontSizeY()+5);
    }
    //GUI_SetColor(pObj->BECInfo.TextColor);
    //控件左方的TEXT
    /*GUI_SetColor(pObj->TextColor1);
    GUI_SetTextAlign(GUI_TA_HCENTER|GUI_TA_VCENTER);
    GUI_SetFont(pObj->pTextFont);
    GUI_DispStringAt(pObj->BECInfo.Text1,r.x0+30,r.y1-25);  */
    GUI_SetColor(GUI_WHITE);
    GUI_SetTextAlign(GUI_TA_HCENTER|GUI_TA_VCENTER);
    GUI_SetFont(pObj->BECInfo.pUnitFont);
    GUI_DispStringAt(pObj->BECInfo.Text2,r.x1/2,r.x1/2+15);
    GUI_SetColor(GUI_BLACK);
    GUI_SetTextAlign(GUI_TA_HCENTER|GUI_TA_VCENTER);
    GUI_SetFont(pObj->pTextFont);

	r1.x0= r.x0;
	r1.y0 =3 * r.y1/4 -5;
	r1.x1 = r.x1;
	r1.y1 = r.y1;
	GUI_DispStringInRectWrap(pObj->BECInfo.Text1, &r1 , GUI_TA_HCENTER|GUI_TA_VCENTER, GUI_WRAPMODE_WORD);//软件问题-GUI #1229
//     GUI_DispStringAt(pObj->BECInfo.Text1,r.x1/2,r.y1-15);
}

/***************************************************************************************************
*
*
*/
static void _Paint(ButtonRotation_Obj* pObj)
{
    /* Draw Frame */
    GUI_RECT rClient;
    //int xsize = pObj->Widget.Win.Rect.x1 - pObj->Widget.Win.Rect.x0;
    //int ysize = pObj->Widget.Win.Rect.y1 - pObj->Widget.Win.Rect.y0;
    GUI_GetClientRect(&rClient);
    WM_SetUserClipRect(&rClient);
    GUI_SetColor(pObj->BECInfo.BkColor);
    GUI_FillRect  (rClient.x0, rClient.y0, rClient.x1, rClient.y1);
    _DrawPaintButton(pObj);
    WM_SetUserClipRect(NULL);
}

/***************************************************************************************************
*
*
*/
static void _AdjustData(ButtonRotation_Handle hWin,ButtonRotation_Obj* pObj,int Id)
{
    GUI_RECT r;
    char cTemp[8]= {0}; //  20130513    
    char* szTemp;
    WM_SelectWindow(hWin);
    GUI_GetClientRect(&r);
    GUI_SetColor(pObj->BECInfo.BkColor);
    if(!strcmp(pObj->Text11, Sys_Lib_GetDispStr(DP_IE)))
    {
        if(pObj->triangleSign)
        {
            if(pObj->Data1>=6)
            {
                /*if(pObj->Data1!=24)*/
				if(pObj->Data1!=pObj->BDInfo.Max)
                {
                    sprintf(cTemp,"1:%2.1f",(float)(IE_RatioTable[pObj->Data1])/10);
                }
                else
                {
                    sprintf(cTemp,"1:%2.0f",(float)(IE_RatioTable[pObj->Data1])/10);
                }
            }
            else
            {
                sprintf(cTemp,"%2.1f:1",(float)(IE_RatioTable[pObj->Data1])/10);
            }
        }
        else
        {
            if(pObj->Data>=6)
            {
                sprintf(cTemp,"1:%2.1f",(float)(IE_RatioTable[pObj->Data1])/10);
            }
            else
            {
                sprintf(cTemp,"%2.1f:1",(float)(IE_RatioTable[pObj->Data1])/10);
            }
        }
        szTemp = cTemp; 
    }
    else
    {
        if(pObj->triangleSign)
        {
            szTemp = _Float2String(pObj->Data1,pObj->BDInfo.FloatLen);
        } 
        else 
        {

            szTemp = _Float2String(pObj->Data,pObj->BDInfo.FloatLen);
        }
    }
    switch(pObj->DataInvildSign)
    {
    case 1: //下限关闭
        if(pObj->Data1<=pObj->BDInfo.Min)
        {
            szTemp = Sys_Lib_GetDispStr(DP_OFF);
        }
        else
        {
            szTemp = _Float2String(pObj->Data1,pObj->BDInfo.FloatLen);
        }
        break;
    case 2: //上限关闭
        if(pObj->Data1>=pObj->BDInfo.Max)
        {
            szTemp = Sys_Lib_GetDispStr(DP_OFF);
        }
        else
        {
            szTemp = _Float2String(pObj->Data1,pObj->BDInfo.FloatLen);
        }
        break;
    case 3: //  21030409
        if(pObj->Data1<=pObj->BDInfo.Min)
        {
            szTemp = Sys_Lib_GetDispStr(DP_ON);
        }
        else if(pObj->Data1>=pObj->BDInfo.Max)
        {
            szTemp = Sys_Lib_GetDispStr(DP_OFF);
        }
        else
        {
            szTemp = _Float2String(pObj->Data1,pObj->BDInfo.FloatLen);
        }
        break;
    case 4: //djq add 20170407 这个地方后续需要优化，可以是两态切换的按钮，显示的字样可以传入
        if(pObj->Data1>=pObj->BDInfo.Max)
        {
            szTemp = Sys_Lib_GetDispStr(DP_AIR);
        }        
        else
        {
            szTemp = Sys_Lib_GetDispStr(DP_O2);
        }
        break;
    default:
        {
//          szTemp = _Float2String(pObj->Data1,pObj->BDInfo.FloatLen);
        }
        break;
    }
    pObj->BECInfo.Text = szTemp;
    GUI_SetFont(pObj->BECInfo.pTextFont);
    r.y1 = r.x1;
    //WIDGET_EFFECT_3D_DrawDownRect(r.x0+12,r.y0+30,r.x1-12,r.y1-22);
    //中间text内容的显示判断部分

    GUI_SetColor(0x4080FF);//这两行重刷圆形控件内数字显示方形区
	//GUI_SetColor(GUI_RED);
    GUI_FillRect(r.x1/2-20,r.x1/2-8,r.x1/2+20,r.x1/2+8); //cpp md 20130513
	
	GUI_SetTextMode(GUI_TEXTMODE_TRANS);
    GUI_SetColor(pObj->BECInfo.TextColor);
    GUI_SetTextAlign(GUI_TA_HCENTER|GUI_TA_VCENTER);
    GUI_DispStringAt(pObj->BECInfo.Text,r.x1/2,r.x1/2);
}

/***************************************************************************************************
*
*
*/
static void _ButtonRotation_Callback (WM_MESSAGE *pMsg)
{
    WM_MESSAGE Msg;
    ButtonRotation_Handle hWin = (ButtonRotation_Handle)(pMsg->hWin);
    ButtonRotation_Obj* pObj = ButtonRotation_H2P(hWin);
    int i;
    //GUI_RECT* pRect = (GUI_RECT*)(pMsg->Data.p);
    if (WIDGET_HandleActive(hWin, pMsg) == 0)
    {
        return;
    }
    switch (pMsg->MsgId)
    {
    case WM_PAINT:
        _Paint(pObj);
        break;
    case WM_TOUCH:
        _OnTouch(hWin, pObj, pMsg);
        return;
    case WM_KEY:
        break;
    case WM_NOTIFICATION_VALUE_CHANGED:
        _AdjustData(hWin,pObj,pMsg->Data.v);
        break;
    }
    /* Let widget handle the standard messages */
    WM_DefaultProc(pMsg);
}

/***************************************************************************************************
*
*
*/
ButtonRotation_Handle ButtonRotation_CreateAsChild(
    int x0, int y0, int xsize, int ysize, WM_HWIN hParent,int Id,
    WM_CALLBACK* cb, int Flags)
{
    ButtonRotation_Handle hObj;
    /* Create the window */
    WM_LOCK();
    hObj = WM_CreateWindowAsChild(x0, y0, xsize/*+2*HBorder*/, ysize/*+TBorder+BBorder*/, hParent,
                                  Flags, _ButtonRotation_Callback, 
                                  sizeof(ButtonRotation_Obj) - sizeof(WM_Obj));
    if (hObj)
    {
        ButtonRotation_Obj* pObj = ButtonRotation_H2P(hObj);
        /* init widget specific variables */
        WIDGET__Init(&pObj->Widget,Id, WIDGET_STATE_FOCUSSABLE );
        //pObj->Widget.Id     = Id;
        pObj->BECInfo = BECInfo_DEFAULT;
        pObj->BDInfo = BDInfo_DEFAULT;
        pObj->ProColor1 = VIEWCOLOR_PROBUTTON;
        pObj->LineColor1 = VIEWCOLOR_PROBUTTON;
        pObj->ProColor2 = GUI_BLUE;
        pObj->LineColor2 = GUI_YELLOW_EX;
        pObj->Data = 60;
        pObj->Data1 = 60;
        pObj->Text11 = "";
        pObj->Text12 = "";
        pObj->TextColor1 = GUI_BLACK;
        pObj->BECInfo.TextColor = pObj->TextColor1;
        pObj->TextColor2 = GUI_YELLOW;
        pObj->pTextFont = BUTTONEXCTRL_DEFAULT_TEXTFONT;
        pObj->changeSign = 'N';     //A:停止增加    S:停止减少　N:即可增加也可减少
        pObj->triangleSign = 0;
        pObj->textSign = 1;
        pObj->DataInvildSign = 0;
    }
    WM_UNLOCK();
    return hObj;
}

/***************************************************************************************************
*
*
*/
void SetChangeSigner(ButtonRotation_Handle hObj, char sign)
{
    ButtonRotation_Obj * pObj;
    if(!WM_IsWindow(hObj))
    {
        return;
    }
    WM_LOCK();
    pObj = ButtonRotation_H2P(hObj);
    pObj->changeSign = sign;
    WM_UNLOCK();
}

/***************************************************************************************************
*
*
*/
ButtonRotation_Handle ButtonRotation_Create(WM_CALLBACK* cb,
        int Flags,
        int x0, int y0, int xsize, int ysize,int Id)
{
    return ButtonRotation_CreateAsChild(x0, y0, xsize, ysize, WM_HWIN_NULL,Id, cb, Flags);
}

/***************************************************************************************************
*
*
*/
ButtonRotation_Handle ButtonRotation_CreateIndirect(const GUI_WIDGET_CREATE_INFO* pCreateInfo, 
                                                    WM_HWIN hWinParent, int x0, int y0, 
                                                    WM_CALLBACK* pCallback)
{
    ButtonRotation_Handle hObj;
    hObj = ButtonRotation_CreateAsChild(pCreateInfo->x0 + x0, pCreateInfo->y0 + y0, 
                                        pCreateInfo->xSize, pCreateInfo->ySize, 
                                        hWinParent, pCreateInfo->Id,pCallback, 0);
    return hObj;
}
#else
void WIDGET_viewWin(void) {} /* avoid empty object files */
#endif /* GUI_WINSUPPORT */



