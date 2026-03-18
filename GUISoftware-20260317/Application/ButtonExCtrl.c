//
/***************************************************************************************************
标题:  ButtonExCtrl.c
功能:  复杂的控件（圆形、外围为弧线并有以个小的三角形），能通过上下、左右键调整显示内容，
       并有一个小的线条位置的改变
说明:  控制部分的数据为10的FloatLen次方倍的数据，与显示的数据不同，要保证传进的数据为整形
当前版本： 0.1.0
修改记录：
***************************************************************************************************/
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include "GUI.H"
#include "GUI_Protected.h"
#include "ButtonExCtrl.h"
#include "WIDGET.h"
#include "MainTask.h"

//
#ifndef SIZE_OF_ARRAY
#define SIZE_OF_ARRAY(Array) (sizeof(Array) / sizeof(Array[0]))
#endif
//
#if GUI_WINSUPPORT
#ifndef GUI_YELLOW_EX
#define GUI_YELLOW_EX                   0x00DDDD
#endif
#ifndef VIEWCOLOR_BLACK
#define VIEWCOLOR_BLACK                 0x626563//0x404040  //
#endif
#ifndef VIEWCOLOR_PROBUTTON
#define VIEWCOLOR_PROBUTTON             0xA05C40//0x888B00  
#endif
//
#define BUTTONEXCTRL_DEFAULT_BKCOLOR        0x181818//0X221713//0xD05356
#define BUTTONEXCTRL_DEFAULT_TEXTCOLOR      0x4080FF
#define BUTTONEXCTRL_DEFAULT_TEXTFONT       &GUI_Font_16//GUI_Font13B_1
#define BUTTONEXCTRL_DEFAULT_TEXT           "36"
#define BUTTONEXCTRL_DEFAULT_TEXT1          "O2"
#define BUTTONEXCTRL_DEFAULT_TEXTFONT1      &GUI_Font_24//GUI_Font_24    ///焦点不在控件上字体的大小
#define BUTTONEXCTRL_DEFAULT_TEXTFONT2      &GUI_Font_24//GUI_Font_24    ///焦点在控件上字体的大小
#define BUTTONEXCTRL_DEFAULT_UNITFONT       &GUI_Font_15//&GUI_Font8_1
#define BUTTONEXCTRL_DEFAULT_MIN            0
#define BUTTONEXCTRL_DEFAULT_MAX            1000 //100 
#define BUTTONEXCTRL_DEFAULT_DEFAULTSIZE    20
#define BUTTONEXCTRL_DEFAULT_STEP           1
#define BUTTONEXCTRL_DEFAULT_FLOATLEN       0
//static const int IE_RatioTable[] = 
//{40,35,30,25,20,15,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100};
// #if (LOGO_FLAG == ENDURE_LOGO)
// const int IE_RatioTable[] = 
// {40,35,30,25,20,15,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,105,110,115,120};
// #else
// const int IE_RatioTable[] = 
// {40,35,30,25,20,15,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100};
// #endif
// const int IE_RatioTableMoniter[] = 
// {90,85,80,75,70,65,60,55,50,45,40,35,30,25,20,15,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,
// 105,110,115,120,125,130,135,140,145,150,155,160,165,170,175,180,185,190,195,};

extern const int IE_RatioTable[];
extern const int IE_RatioTableMoniter[];

static  int keySign = 0;                       //只响应一次左键弹起消息
extern WM_HWIN Focus_hWin;
//extern MESSAGE_QUEUE MsgQueue;

#define BUTTONEXCTRL_DEFAULT_PROGRESSBKCOLOR   0xAA9587
#define BUTTONEXCTRL_DEFAULT_PROGRESSPROCOLOR  GUI_WHITE

static const ButtonExCtrlInfo BECInfo_DEFAULT =
{
    BUTTONEXCTRL_DEFAULT_BKCOLOR,               //背景色
    VIEWCOLOR_PROBUTTON,                        //前景色
    GUI_WHITE,//VIEWCOLOR_BLACK,                //标签的颜色
    VIEWCOLOR_PROBUTTON,                        //颜色画圆弧、圆的外边缘
    BUTTONEXCTRL_DEFAULT_TEXTCOLOR,             //字体颜色
    BUTTONEXCTRL_DEFAULT_TEXTFONT,              //字体大小
    BUTTONEXCTRL_DEFAULT_UNITFONT,
    BUTTONEXCTRL_DEFAULT_TEXT,                  //显示内容在中间
    BUTTONEXCTRL_DEFAULT_TEXT1                  //显示内容在下方
};
static const ButtonDataInfo BDInfo_DEFAULT =
{
    BUTTONEXCTRL_DEFAULT_MIN,            //最小值
    BUTTONEXCTRL_DEFAULT_MAX,            //最大值
    BUTTONEXCTRL_DEFAULT_DEFAULTSIZE,    //默认值　小的三角形指向的值
    BUTTONEXCTRL_DEFAULT_STEP,           //步距
    BUTTONEXCTRL_DEFAULT_FLOATLEN        //浮点数据的长度  0:为整数据　1:为浮点数小数点后显示的位数为1
};
static const GUI_POINT aPoints[] =
{
    {0,  0},
    {-5, 5},
    {-5,-5}
};
GUI_POINT aPointsDest[3];
/***************************************************************************************************
* @brief  
* @para   
* @retval 
*/
static void _OnTouch(ButtonExCtrl_Handle hWin, ButtonExCtrl_Obj* pObj, WM_MESSAGE* pMsg)
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
            WM_NotifyParent(hWin, WM_NOTIFICATION_CLICKED);
        }
    }
    else
    {
        //_ButtonReleased(hObj, pObj, WM_NOTIFICATION_MOVED_OUT);
        //pObj->TouchSign = 0;
        //pObj->triangleSign = 0;
        //pObj->Widget.State &= ~WIDGET_STATE_FOCUS;
        //Msg.MsgId   = WM_PAINT;
        //WM_SendMessage(hWin,&Msg);
        //Msg.hWin = hWin;
        //if(!QueueFull(&MsgQueue))
        //{
        //  EnterQueue(&MsgQueue,Msg);
        //}
        //Msg.Data.v  = WM_NOTIFICATION_RELEASED;
        //WM_SendToParent(hWin, &Msg);
        //WM_InvalidateWindow(hWin);
    }
}
/***************************************************************************************************
* @brief  
* @para   
* @retval 
*/
static void _DrawPaintLine(GUI_RECT r,int Angle)
{
    float x,y,x1,y1;
    float angle = -Angle*3.1415926/180;
    x = (r.x1-r.x0)/2-((r.x1-r.x0)*5/12)*cos(angle);
    y = (r.x1-r.x0)/2+((r.x1-r.x0)*5/12)*sin(angle);
    x1 = (r.x1-r.x0)/2-((r.x1-r.x0)*4/12)*cos(angle);
    y1 = (r.x1-r.x0)/2+((r.x1-r.x0)*4/12)*sin(angle);
    //GUI_DrawLine(x,y,(r.x1-r.x0)/2,(r.x1-r.x0)/2);
    GUI_DrawLine(x,y,x1,y1);
}
/***************************************************************************************************
* @brief  
* @para   
* @retval 
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
* @brief  
* @para   
* @retval 
*/
//将整形数据，转换为实际数据，_Float2String
static char* _Float2String(int nData,int floatLen)
{
    float fData;
    int result = 1;
    static char DataTemp[10] = "--";
    char strfmt[8];
    sprintf(strfmt, "%%.%df", (int)floatLen);
    square(10,floatLen,&result);
    fData = (float)nData/result;
    sprintf(DataTemp, strfmt, fData);
    return DataTemp;
}
//将整形数据，转换为实际数据，_Float2String I:E
/*static char* _Float2StringIE(int nData,int floatLen)
{
    float fData;
    float fData1;
    int result = 1;
    static char DataTemp[10] = "---";
    square(10,floatLen,&result);
    fData = (float)nData/result;
    if(fData>1){
        sprintf(DataTemp,"%.1f:1",fData);
    }
    else{
        fData1 = 1/fData;
        sprintf(DataTemp,"1:%.1f",fData1);
    }
    return DataTemp;
}*/
extern char* _IEDataStr(int data, int floatLen);
/***************************************************************************************************
* @brief  
* @para   
* @retval 
*/
static void _DrawPaintButton(ButtonExCtrl_Obj* pObj)
{
    GUI_RECT r,oldr;
    int i=0,j=0;
	int xLength=0,xRealLenth=0,xRealPos=0;
    char *szTemp;
	char cTemp[8]= {0};
    //int Angle = 90;
    //float angle = -Angle*3.1415926/180;
    GUI_GetClientRect(&r);
    oldr = r;

    if (pObj->Widget.State & WIDGET_STATE_FOCUS)
    {
        pObj->BECInfo.ProColor = pObj->ProColor2;
        pObj->BECInfo.LineColor = pObj->LineColor2;
        //pObj->BECInfo.Text1 = pObj->Text12;
        pObj->BECInfo.TextColor = 0x4080FF;//pObj->TextColor2;
        //pObj->BECInfo.pTextFont = pObj->pTextFont2;
    }
    else
    {
        pObj->BECInfo.ProColor = pObj->ProColor1;
        pObj->BECInfo.LineColor = pObj->LineColor1;
        //pObj->BECInfo.Text1 = pObj->Text11;
        pObj->BECInfo.TextColor = GUI_WHITE;//pObj->TextColor1;
        //pObj->BECInfo.pTextFont = pObj->pTextFont1;
        pObj->Data1 = pObj->Data;               //使控件在选中状态下的值复位
    }

    //GUI_YELLOW_EX：用黄色浅一点的颜色画圆弧、圆的外边缘
    //圆弧
    //GUI_SetPenSize(1);
    //GUI_SetColor(pObj->BECInfo.LineColor);
    //GUI_DrawCircle((r.x1-r.x0)/2,(r.x1-r.x0)/2,(r.x1-r.x0)*5/12);
    //GUI_SetColor(pObj->BECInfo.BkColor);
    //GUI_DrawArc((r.x1-r.x0)/2,(r.x1-r.x0)/2,(r.x1-r.x0)*5/12,(r.x1-r.x0)*5/12,-45,225);
    //圆弧两端的标尺
    //GUI_SetPenSize(2);
    //GUI_SetColor(pObj->BECInfo.LineColor);
    //_DrawPaintLine(r,225);
    //_DrawPaintLine(r,-45);
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
    
    GUI_SetTextMode(GUI_TM_TRANS);
	if(!strcmp(pObj->Text11, Sys_Lib_GetDispStr(DP_IE)))
    {
        
        if(pObj->Data>=6)
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
            sprintf(cTemp,"%2.1f:1",(float)(IE_RatioTable[pObj->Data])/10);
        }
        szTemp = cTemp;
    }
    else if(!strcmp(pObj->Text11, Sys_Lib_GetDispStr(DP_PEEP)))
    {
        char cTemp[8]= {0};
        if(pObj->Data<=2)
        {
			szTemp = Sys_Lib_GetDispStr(DP_OFF);
        }
        else
        {
            szTemp = _Float2String(pObj->Data,pObj->BDInfo.FloatLen);
        }
    }
    else if(!strcmp(pObj->Text11, Sys_Lib_GetDispStr(DP_TSLOPE)))
    {
        szTemp = _Float2String(pObj->Data/10,pObj->BDInfo.FloatLen-1);
    }
//  else if(!strcmp(pObj->Text11, "TI"))
//  {
//      szTemp = _Float2String(pObj->Data/10,pObj->BDInfo.FloatLen-1);
//  }
    else
    {
        szTemp = _Float2String(pObj->Data,pObj->BDInfo.FloatLen);
    }
    pObj->BECInfo.Text = szTemp;
    GUI_SetColor(pObj->BECInfo.TextColor);
    GUI_SetFont(pObj->pTextFont1);
    oldr = r;
    oldr.y1 = oldr.x1;
    GUI_SetTextMode(GUI_TM_TRANS);

	xLength=r.x1-r.x0-20;
	xRealLenth=pObj->BDInfo.Max-pObj->BDInfo.Min;
	if(((int)pObj->BECInfo.Text==0) || (xRealLenth == 0))
	{
		xRealPos=1;
	}
	else
	{
		xRealPos=xLength*(((int)pObj->BECInfo.Text-pObj->BDInfo.Min)/xRealLenth);
	}

    //中间text内容的显示判断部分
	if(pObj->textSign)
	{
		if(pObj->triangleSign == 1) 
		{
			GUI_SetColor(0xDE7A48); 
			FillBevel(r.x0 + 8, r.y0 + 8, r.x1 - 8, r.y1 - 8, 6);
			GUI_SetColor(pObj->TextColor2);
			GUI_SetTextMode(GUI_TM_TRANS); 
		}
		else 
		{
			GUI_SetColor(pObj->BECInfo.TextColor);
		}

		GUI_SetTextAlign(GUI_TA_HCENTER|GUI_TA_VCENTER);
		GUI_DispStringAt(pObj->BECInfo.Text, (r.x0+r.x1)/2, (r.y0+r.y1)/2+1);
	}
   
	//Unit
 //   GUI_SetColor(GUI_WHITE);//0xAAAAAA
 //   GUI_SetTextAlign(GUI_TA_HCENTER|GUI_TA_VCENTER);//GUI_TA_RIGHT
 //   GUI_SetFont(pObj->BECInfo.pUnitFont);
 //   //GUI_DispStringAt(pObj->Text12,r.x1-18,r.y1-10-59+30-21-5);
	//if(!strcmp(pObj->Text11,Sys_Lib_GetDispStr(DP_PEEP)))
	//{
	//	GUI_DispStringAt(pObj->Text12,r.x1-2-30+10,r.y1-65+10+42);
	//}
	//else if(!strcmp(pObj->Text11,Sys_Lib_GetDispStr(DP_PINSP)) || !strcmp(pObj->Text11,Sys_Lib_GetDispStr(DP_PSUPP)) || 
	//	!strcmp(pObj->Text11,Sys_Lib_GetDispStr(DP_PSENS))) 
	//{
	//	GUI_DispStringAt(pObj->Text12,r.x1-2-32+5,r.y1-65+10+42);
	//}
	//else if(!strcmp(pObj->Text11,Sys_Lib_GetDispStr(DP_HOLDPRESSURE)) || !strcmp(pObj->Text11,Sys_Lib_GetDispStr(DP_HOLDTIME)))
	//{
	//	GUI_DispStringAt(pObj->Text12,r.x1-2-32+8,r.y1-65+10+42);
	//}
	//else
	//{
	//	GUI_DispStringAt(pObj->Text12,r.x1-2-34,r.y1-65+10+42);
	//}
	{
		int unit_Y_Pos = r.y1 - 21; 

		//Unit
		GUI_SetColor(GUI_WHITE);
		GUI_SetTextAlign(GUI_TA_HCENTER | GUI_TA_VCENTER);
		GUI_SetFont(pObj->BECInfo.pUnitFont);
		GUI_DispStringAt(pObj->Text12, (r.x0 + r.x1) / 2, unit_Y_Pos);
	}

	//Name	
    GUI_SetColor(GUI_WHITE);//GUI_BLACK
    //GUI_SetTextAlign(GUI_TA_LEFT|GUI_TA_VCENTER);
	GUI_SetTextAlign(GUI_TA_HCENTER | GUI_TA_VCENTER);
	GUI_SetFont(pObj->pTextFont);
	GUI_DispStringAt(pObj->Text11, (r.x0+r.x1)/2, r.y0+12);

	if(!strcmp(pObj->Text11,Sys_Lib_GetDispStr(DP_IE)))
	{
		//进度条最小值
		sprintf(cTemp,"%d:1",(IE_RatioTable[pObj->BDInfo.Min]/10));
		GUI_DispStringAt(cTemp,r.x0+5,r.y1-20);   //待优化

		//进度条最大值
		//数值范围 进度条两边  有待优化
		sprintf(cTemp,"1:%2d",(IE_RatioTable[pObj->BDInfo.Max]/10));
		GUI_DispStringAt(cTemp,r.x1-5-GUI_GetStringDistX(cTemp),r.y1-20);
	}
	else
	{
		//进度条最小值
		if((!strcmp(pObj->Text11,Sys_Lib_GetDispStr(DP_PEEP))) || (!strcmp(pObj->Text11,Sys_Lib_GetDispStr(DP_ADDSIGH))))
		{
			GUI_DispStringAt(Sys_Lib_GetDispStr(DP_OFF),r.x0+5-4,r.y1-20);   //待优化
		}
		else
		{
			if(pObj->BDInfo.Min == 0)
			{
				GUI_DispStringAt(_Float2String(pObj->BDInfo.Min,0),r.x0+5-3,r.y1-20);   //待优化
			}
			else
			{
				GUI_DispStringAt(_Float2String(pObj->BDInfo.Min,pObj->BDInfo.FloatLen),r.x0+5-3,r.y1-20);   //待优化
			}
		}
		//进度条最大值
		//数值范围 进度条两边  有待优化
		strcpy(cTemp,_Float2String(pObj->BDInfo.Max,pObj->BDInfo.FloatLen));
		GUI_DispStringAt(cTemp,r.x1-5-GUI_GetStringDistX(cTemp)+3,r.y1-20);
	}
	
	//GUI_SetColor(pObj->ProgressProColor);
	//GUI_SetPenSize(2);
	//xLength=r.x1-r.x0-12;
	//xRealLenth=pObj->BDInfo.Max-pObj->BDInfo.Min;
	//if(xRealLenth != 0)
	//{
	//	xRealPos=xLength*((pObj->Data1-pObj->BDInfo.Min)*100/xRealLenth)/100;
	//}
	//GUI_DrawHLine(r.y1-5, r.x0+6, r.x0+6+xRealPos);
	//GUI_DrawHLine(r.y1-6, r.x0+6, r.x0+6+xRealPos);
	//
	//GUI_SetColor(pObj->ProgressBkColor);
	//GUI_DrawHLine(r.y1-5, r.x0+6+xRealPos, r.x1-6);
	//GUI_DrawHLine(r.y1-6, r.x0+6+xRealPos, r.x1-6);
	{
		int bar_MarginLeftRight = 5; // 左右边距
		int bar_MarginBottom = 6;     // 距离底部的像素
		int bar_Thickness = 6;        // 进度条的粗细
		int r_corner = bar_Thickness / 2; // 圆角半径（设为粗细的一半，正好形成完美的半圆端点）

		int startX = r.x0 + bar_MarginLeftRight;
		int endX   = r.x1 - bar_MarginLeftRight;
		int topY   = r.y1 - bar_MarginBottom - bar_Thickness;
		int bottomY= r.y1 - bar_MarginBottom;
		int centerY = (topY + bottomY) / 2;

		xLength = endX - startX;
		xRealLenth = pObj->BDInfo.Max - pObj->BDInfo.Min;
		xRealPos = 0;

		if(xRealLenth != 0)
		{
			xRealPos = xLength * ((pObj->Data1 - pObj->BDInfo.Min) * 100 / xRealLenth) / 100;
		}

		if (xRealPos > 0 && xRealPos < (r_corner * 2)) 
		{
			xRealPos = r_corner * 2; 
		}

		GUI_SetColor(pObj->ProgressBkColor);
		GUI_FillCircle(startX + r_corner, centerY, r_corner);
		GUI_FillCircle(endX - r_corner, centerY, r_corner);
		GUI_FillRect(startX + r_corner, topY, endX - r_corner, bottomY);

		if (xRealPos > 0)
		{
			GUI_SetColor(pObj->ProgressProColor);
			GUI_FillCircle(startX + r_corner, centerY, r_corner);
			GUI_FillCircle(startX + xRealPos - r_corner, centerY, r_corner);
			GUI_FillRect(startX + r_corner, topY, startX + xRealPos - r_corner, bottomY);
		}
	}
}
/***************************************************************************************************
* @brief  
* @para   
* @retval 
*/
static void _Paint(ButtonExCtrl_Obj* pObj)
{
    /* Draw Frame */
    GUI_RECT rClient;
    int xsize = pObj->Widget.Win.Rect.x1 - pObj->Widget.Win.Rect.x0;//add 20150917
    int ysize = pObj->Widget.Win.Rect.y1 - pObj->Widget.Win.Rect.y0;//add 20150917
    GUI_GetClientRect(&rClient);
    //GUI_SetColor  (0x420606);//add 20150917
	GUI_SetColor(pObj->Edge_Color);

    GUI_FillRect  (0,0, xsize, ysize);//add 20150917
    //WM_SetUserClipRect(&rClient);
    GUI_SetPenSize(1);
	if(pObj->Style_Color == 1)//ylk add 20181009
	{
		LCD_SetColor(0x4080FF);
	}
	else
	{
		LCD_SetColor(0xeab9b1);//GUI_WHITE
	}
    Arc(rClient.x0 + 8, rClient.y0 + 8, rClient.x1 - 8, rClient.y1 - 8,7,8,0xE1);
    LCD_SetColor(0x222222);
    Arc(rClient.x0 + 8, rClient.y0 + 8, rClient.x1 - 8, rClient.y1 - 8,7,8,0x1E);
    Bevel(rClient.x0 + 8, rClient.y0 + 8, rClient.x1 - 8, rClient.y1 - 8,8);
    //LCD_SetColor (pObj->Props.aBkColor[ColorIndex]);
    GUI_SetColor(pObj->BECInfo.BkColor);
    FillBevel(rClient.x0 +8,rClient.y0+8,rClient.x1-8,rClient.y1-8,6);
    _DrawPaintButton(pObj);
    //WM_SetUserClipRect(NULL);
    //WIDGET_EFFECT_3D_DrawUp();
}
/***************************************************************************************************
* @brief  
* @para   
* @retval 
*/
static void _AdjustData(ButtonExCtrl_Handle hWin,ButtonExCtrl_Obj* pObj,int Id)
{
    GUI_RECT r;
    char* szTemp;
	char cTemp[8]= {0};
	int xRealLenth=0,xRealPos=0,xLength=0;

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
    else if(!strcmp(pObj->Text11, Sys_Lib_GetDispStr(DP_PEEP)))
    {
        char cTemp[8]= {0};
        if(pObj->Data1<=2)
        {
            szTemp = Sys_Lib_GetDispStr(DP_OFF);
        }
        else
        {
            szTemp = _Float2String(pObj->Data1,pObj->BDInfo.FloatLen);
        }
    }
//    else if(!strcmp(pObj->Text11, "TI"))
//    {
//        szTemp = _Float2String(pObj->Data1/10,pObj->BDInfo.FloatLen-1);
//    }
    else if(!strcmp(pObj->Text11, Sys_Lib_GetDispStr(DP_TSLOPE)))
    {
        szTemp = _Float2String(pObj->Data1/10,pObj->BDInfo.FloatLen-1);
    }
    else
    {
        if(pObj->triangleSign)
        {
			if(!strcmp(pObj->Text11, Sys_Lib_GetDispStr(DP_TSLOPE)))
			{
				szTemp = _Float2String(pObj->Data1/10,pObj->BDInfo.FloatLen-1);
			}
			else
			{
				szTemp = _Float2String(pObj->Data1,pObj->BDInfo.FloatLen);
			}            
        }
        else
        {
            szTemp = _Float2String(pObj->Data,pObj->BDInfo.FloatLen);
        }
    }
    pObj->BECInfo.Text = szTemp;
    GUI_SetFont(pObj->pTextFont1);
	GUI_SetColor(0xDE7A48);
	GUI_FillRect(r.x0 + 20, (r.y0+r.y1)/2 - 15, r.x1 - 20, (r.y0+r.y1)/2 + 15);
	GUI_SetColor(pObj->TextColor2);
	GUI_SetTextMode(GUI_TM_TRANS); // 强制透明模式，防止文字自带底色破坏整体感
	GUI_SetTextAlign(GUI_TA_HCENTER|GUI_TA_VCENTER);
	GUI_DispStringAt(pObj->BECInfo.Text, (r.x0+r.x1)/2, (r.y0+r.y1)/2+1);

#if 0
	//
	GUI_SetColor(pObj->ProgressProColor);	 
	GUI_SetPenSize(2);
	xLength=r.x1-r.x0-12;
	xRealLenth=pObj->BDInfo.Max-pObj->BDInfo.Min;
	if(xRealLenth != 0)
	{
		xRealPos=xLength*((pObj->Data1-pObj->BDInfo.Min)*100/xRealLenth)/100;
	}
	//GUI_FillRect(r.x0+10,r.y0+65,r.x0+10+xRealPos,r.y1-4);
	GUI_DrawHLine(r.y1-5, r.x0+6, r.x0+6+xRealPos);
	GUI_DrawHLine(r.y1-6, r.x0+6, r.x0+6+xRealPos);
	//GUI_DrawHLine(r.y0+67,r.x0+11,r.x0+10+xRealPos-1); 
	GUI_SetColor(pObj->ProgressBkColor);
	GUI_DrawHLine(r.y1-5, r.x0+6+xRealPos, r.x1-6);
	GUI_DrawHLine(r.y1-6, r.x0+6+xRealPos, r.x1-6);
#else
	{
		{
			int bar_MarginLeftRight = 5; // 左右边距
			int bar_MarginBottom = 6;     // 距离底部的像素
			int bar_Thickness = 6;        // 进度条的粗细
			int r_corner = bar_Thickness / 2; // 圆角半径
			// ------------------------------------

			int startX = r.x0 + bar_MarginLeftRight;
			int endX   = r.x1 - bar_MarginLeftRight;
			int topY   = r.y1 - bar_MarginBottom - bar_Thickness;
			int bottomY= r.y1 - bar_MarginBottom;
			int centerY = (topY + bottomY) / 2;

			xLength = endX - startX;
			xRealLenth = pObj->BDInfo.Max - pObj->BDInfo.Min;
			xRealPos = 0;

			if(xRealLenth != 0)
			{
				// 计算当前数据对应的像素长度
				xRealPos = xLength * ((pObj->Data1 - pObj->BDInfo.Min) * 100 / xRealLenth) / 100;
			}

			if (xRealPos > 0 && xRealPos < (r_corner * 2)) 
			{
				xRealPos = r_corner * 2; 
			}

			GUI_SetColor(pObj->ProgressBkColor);
			GUI_FillCircle(startX + r_corner, centerY, r_corner);
			GUI_FillCircle(endX - r_corner, centerY, r_corner);
			GUI_FillRect(startX + r_corner, topY, endX - r_corner, bottomY);

			if (xRealPos > 0)
			{
				GUI_SetColor(pObj->ProgressProColor);
				GUI_FillCircle(startX + r_corner, centerY, r_corner);
				GUI_FillCircle(startX + xRealPos - r_corner, centerY, r_corner);
				GUI_FillRect(startX + r_corner, topY, startX + xRealPos - r_corner, bottomY);
			}
		}
	}
#endif
}
//
/***************************************************************************************************
* @brief  
* @para   
* @retval 
*/
void _ButtonExCtrl_Callback (WM_MESSAGE *pMsg)
{
    WM_MESSAGE Msg;
    WM_HWIN tWin;
    ButtonExCtrl_Handle hWin = (ButtonExCtrl_Handle)(pMsg->hWin);
    ButtonExCtrl_Obj* pObj;
    //WM_LOCK();
    pObj = ButtonExCtrl_H2P(hWin);
    if (WIDGET_HandleActive(hWin, pMsg) == 0)
    {
        return;
    }
    //WM_UNLOCK();
    //GUI_RECT* pRect = (GUI_RECT*)(pMsg->Data.p);
    switch (pMsg->MsgId)
    {
    case WM_PAINT:
        //WM_SelectWindow(hWin);
        _Paint(pObj);
        break;
    case WM_TOUCH:
        _OnTouch(hWin, pObj, pMsg);
        //  return;
        //case WM_SIZE:
        break;
    case WM_KEY:
        break;
    case WM_NOTIFICATION_VALUE_CHANGED:
        _AdjustData(hWin,pObj,pMsg->Data.v);
        break;
    default:
        WM_DefaultProc(pMsg);
    }
    /* Let widget handle the standard messages */
    //if (WIDGET_HandleActive(hWin, pMsg) == 0) {
    //return;
    //}
    //WM_UNLOCK();
}
/***************************************************************************************************
* @brief  
* @para   
* @retval 
*/
ButtonExCtrl_Handle ButtonExCtrl_CreateAsChild(int x0, int y0, int xsize, int ysize, 
                                               WM_HWIN hParent,int Id,
                                               WM_CALLBACK* cb, int Flags)
{
    ButtonExCtrl_Handle hObj;
    /* Create the window */
    WM_LOCK();
    hObj = WM_CreateWindowAsChild(x0, y0, xsize/*+2*HBorder*/, ysize/*+TBorder+BBorder*/, 
                                  hParent, Flags, _ButtonExCtrl_Callback, 
                                  sizeof(ButtonExCtrl_Obj) - sizeof(WM_Obj));
    if (hObj)
    {
        ButtonExCtrl_Obj* pObj = ButtonExCtrl_H2P(hObj);
        /* init widget specific variables */
        WIDGET__Init(&pObj->Widget,Id, WIDGET_STATE_FOCUSSABLE );
        //pObj->Widget.Id     = Id;
        pObj->BECInfo = BECInfo_DEFAULT;
        pObj->BDInfo = BDInfo_DEFAULT;
        pObj->ProColor1 = VIEWCOLOR_PROBUTTON;
        pObj->LineColor1 = VIEWCOLOR_PROBUTTON;
        pObj->ProColor2 = GUI_YELLOW;
        pObj->LineColor2 = GUI_YELLOW_EX;
        pObj->Data = 2;//20->2
        pObj->Data1 = 20;
        pObj->Text11 = "O2";
        pObj->Text12 = "%";
        pObj->TextColor1 = GUI_WHITE;
        pObj->BECInfo.TextColor = pObj->TextColor1;
        pObj->TextColor2 = GUI_BLACK;
        pObj->pTextFont1 = BUTTONEXCTRL_DEFAULT_TEXTFONT1;
        pObj->pTextFont2 = BUTTONEXCTRL_DEFAULT_TEXTFONT2;
        pObj->pTextFont = BUTTONEXCTRL_DEFAULT_TEXTFONT;
        pObj->changeSign = 'N';     //A:停止增加    S:停止减少　N:即可增加也可减少
        pObj->triangleSign = 0;
        pObj->textSign = 1;
		pObj->ProgressBkColor=BUTTONEXCTRL_DEFAULT_PROGRESSBKCOLOR;
		pObj->ProgressProColor=BUTTONEXCTRL_DEFAULT_PROGRESSPROCOLOR;
        //pObj->TouchSign = 0;
		pObj->Edge_Color = 0x404040;//0x420606;
    }
    WM_UNLOCK();
    return hObj;
}
/***************************************************************************************************
* @brief  
* @para   
* @retval 
*/
ButtonExCtrl_Handle ButtonExCtrl_Create(WM_CALLBACK* cb,
                                        int Flags,
                                        int x0, int y0, int xsize, int ysize,int Id)
{
    return ButtonExCtrl_CreateAsChild(x0, y0, xsize, ysize, WM_HWIN_NULL,Id, cb, Flags);
}
/***************************************************************************************************
* @brief  
* @para   
* @retval 
*/
ButtonExCtrl_Handle ButtonExCtrl_CreateIndirect(const GUI_WIDGET_CREATE_INFO* pCreateInfo, 
                                                WM_HWIN hWinParent, int x0, int y0, 
                                                WM_CALLBACK* pCallback)
{
    ButtonExCtrl_Handle hObj;
    hObj = ButtonExCtrl_CreateAsChild(
               pCreateInfo->x0 + x0, pCreateInfo->y0 + y0, pCreateInfo->xSize, pCreateInfo->ySize, 
               hWinParent, pCreateInfo->Id,
               pCallback, 0);
    return hObj;
}

#else
void WIDGET_viewWin(void) {} /* avoid empty object files */

#endif /* GUI_WINSUPPORT */
//
