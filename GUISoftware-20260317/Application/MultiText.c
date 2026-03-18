#include <string.h>
#include <stdarg.h>
#include "stdio.h"
#include <math.h>

#include "MultiText_Private.h"
#include "DIALOG.h"
//#include "GUI.h"
#include <string.h>
#include "GUI.h"
/*********************************************************************
*
*       Private config defaults
*
**********************************************************************
*/
/* Define colors */
#ifndef   MULTITEXT_COLOR_DEFAULT
	#define MULTITEXT_COLOR_DEFAULT	GUI_WHITE
#endif

/* Define default fonts */
#ifndef   MULTITEXT_FONT_DEFAULT
	#define MULTITEXT_FONT_DEFAULT	&GUI_Font_16
#endif

#ifndef    MULTITEXT_RECT_DEFULT
	#define MULTITEXT_RECT_DEFULT	0,0,0,0
#endif

#ifndef   MULTITEXT_ALIGN_DEFAULT
	#define MULTITEXT_ALIGN_DEFAULT     GUI_TA_LEFT|GUI_TA_TOP
#endif

#ifndef  MULTITEXT_WRAPMODE_DEFAULT
	#define MULTITEXT_WRAPMODE_DEFAULT	GUI_WRAPMODE_NONE
#endif

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
MULTITEXT_PROPS MULTITEXT__DefaultProps = {
	MULTITEXT_COLOR_DEFAULT,
	MULTITEXT_FONT_DEFAULT,
	MULTITEXT_RECT_DEFULT,
	MULTITEXT_ALIGN_DEFAULT,
	MULTITEXT_WRAPMODE_DEFAULT
};
static void DrawArcScale(int x0,int y0,int pressure,int flag) 
{
	int i;
	char ac[4];
	int x,y,x1,y1;
	float a;
	float nCos,nSin;
	int rx;
	// 	int ry1;
	// 	int ry2;

	rx = 63;//68;
	// 	ry1 = 53;
	// 	ry2 = 57;

	// 	GUI_SetColor(GUI_WHITE);
	for (i=0; i<= 12; i++) 
	{
		//a = (-45+i*22.5)*3.1415926/180; //角度转换为弧度
		a = (-30+i*20)*3.1415926/180; //角度转换为弧度
		nCos = cos(a);
		nSin = sin(a);
		if(i%2 == 0)
		{
			x = -rx*nCos+x0;
			y = -rx*nSin+y0+0.5;
			// 			x1= -ry1*nCos+x0;
			// 			y1= -ry1*nSin+y0;
			//GUI_SetPenSize( 2 );
			//GUI_AA_DrawLine(x,y,x1,y1);
			//GUI_DrawLine(x,y,x1,y1);
			//
			//if (flag==1)
			{
				// 				x = -42*nCos+x0;
				// 				y = -42*nSin+y0;
				sprintf(ac, "%d", 10*(i-2));
				GUI_SetTextAlign(GUI_TA_VCENTER|GUI_TA_HCENTER);
				//GUI_SetPenSize( 1 );
				//GUI_SetColor(GUI_RED);
				// 				GUI_SetBkColor(GUI_RED);
				GUI_SetColor(GUI_WHITE);
				GUI_DispStringHCenterAt(ac,x,y);
			}
		}
		// 		else
		// 		{
		// 			x = -rx*nCos+x0;
		// 			y = -rx*nSin+y0;
		// 			x1= -ry2*nCos+x0;
		// 			y1= -ry2*nSin+y0;
		//GUI_SetPenSize( 1 );
		//GUI_AA_DrawLine(x,y,x1,y1);
		//GUI_DrawLine(x,y,x1,y1);
		// 		}
	}
}

static void DrawArcScale_2(int x0,int y0,int pressure,int flag) 
{
	int i;
	char ac[4];
	int x,y,x1,y1;
	float a;
	float nCos,nSin;
	int rx;
	// 	int ry1;
	// 	int ry2;

	rx = 63;//68;
	// 	ry1 = 53;
	// 	ry2 = 57;

	// 	GUI_SetColor(GUI_WHITE);
	for (i=0; i<= 12; i++) 
	{
		//a = (-45+i*22.5)*3.1415926/180; //角度转换为弧度
		a = (-30+i*20)*3.1415926/180; //角度转换为弧度
		nCos = cos(a);
		nSin = sin(a);
		if(i%2 == 0)
		{
			x = -rx*nCos+x0;
			y = -rx*nSin+y0+0.5;
			// 			x1= -ry1*nCos+x0;
			// 			y1= -ry1*nSin+y0;
			//GUI_SetPenSize( 2 );
			//GUI_AA_DrawLine(x,y,x1,y1);
			//GUI_DrawLine(x,y,x1,y1);
			//
			//if (flag==1)
			{
				// 				x = -42*nCos+x0;
				// 				y = -42*nSin+y0;
				sprintf(ac, "%d", 10*(i-2));
				GUI_SetTextAlign(GUI_TA_VCENTER|GUI_TA_HCENTER);
				//GUI_SetPenSize( 1 );
				//GUI_SetColor(GUI_RED);
				// 				GUI_SetBkColor(GUI_RED);
				GUI_SetColor(GUI_WHITE);
				GUI_DispStringHCenterAt(ac,x,y);
			}
		}
		// 		else
		// 		{
		// 			x = -rx*nCos+x0;
		// 			y = -rx*nSin+y0;
		// 			x1= -ry2*nCos+x0;
		// 			y1= -ry2*nSin+y0;
		//GUI_SetPenSize( 1 );
		//GUI_AA_DrawLine(x,y,x1,y1);
		//GUI_DrawLine(x,y,x1,y1);
		// 		}
	}
}

static void DrawArcScale_3(int x0,int y0,int pressure,int flag) 
{
	int i;
	char ac[4];
	int x,y,x1,y1;
	float a;
	float nCos,nSin;
	int rx;
	rx = 63;//68;
	for (i=0; i<= 12; i++) 
	{
		a = (-35+i*21)*3.1415926/180; //角度转换为弧度
		nCos = cos(a);
		nSin = sin(a);
		if(i%2 == 0)
		{
			x = -rx*nCos+x0;
			y = -rx*nSin+y0;
			sprintf(ac, "%d", 10*(i-2));
			GUI_SetTextAlign(GUI_TA_VCENTER|GUI_TA_HCENTER);
			GUI_SetColor(GUI_WHITE);
			GUI_DispStringHCenterAt(ac,x,y);
		}
	}
}

/*********************************************************************
*
*       _Paint
*/
static void _Paint(MultiText_Obj * pObj, MULTITEXT_Handle hObj)
{
	GUI_RECT rClient, rInside;
	int EffectSize;
	int x0 = 0;
	int y0 = 0;
	int i;
	char ac[4];
	int angle = 0;

	GUI_DEBUG_LOG("MultipleText: Paint(..)\n");

	GUI_GetClientRect(&rClient);
	rInside = rClient;
	if (pObj->nStyle == 1)//计时控件：粗圆角边框
	{
// 		GUI_SetColor(pObj->WinBkColor);
// 		GUI_FillRectEx(&rInside);
		LCD_SetBkColor(pObj->WinBkColor);
		WM_SetUserClipRect(&rInside);
		GUI_Clear();

		GUI_SetColor(pObj->BkColor);
		FillBevel(rInside.x0 +8,rInside.y0+8,rInside.x1-8,rInside.y1-8,8);

		LCD_SetColor(pObj->MultiTextFrame.FrameColor);
		Arc(rInside.x0 + 8, rInside.y0 + 8, rInside.x1 - 8, rInside.y1 - 8,7,8,0xE1);
		Arc(rInside.x0 + 8, rInside.y0 + 8, rInside.x1 - 8, rInside.y1 - 8,7,8,0x1E);
		Bevel(rInside.x0 + 8, rInside.y0 + 8, rInside.x1 - 8, rInside.y1 - 8,8);
	}
	else if (pObj->nStyle == 2)//细圆角边框
	{
		
		EffectSize = pObj->MultiTextFrame.MultiFrameSizeLeft;
		rInside.x0 += EffectSize;

		EffectSize = pObj->MultiTextFrame.MultiFrameSizeRight;
		rInside.x1 -= EffectSize;

		EffectSize = pObj->MultiTextFrame.MultiFrameSizeTop;
		rInside.y0 += EffectSize;

		EffectSize = pObj->MultiTextFrame.MultiFrameSizeBot;
		rInside.y1 -= EffectSize;

		//LCD_SetBkColor(pObj->BkColor);
		LCD_SetBkColor(pObj->WinBkColor);
		WM_SetUserClipRect(&rInside);
		GUI_Clear();

// 		GUI_SetColor(pObj->WinBkColor);
// 		GUI_FillRectEx(&rInside);

// 		GUI_SetColor(pObj->BkColor);
// 		FillBevel(rInside.x0 +5,rInside.y0+5,rInside.x1-5,rInside.y1-5,5);
// 
// 		LCD_SetColor(pObj->MultiTextFrame.FrameColor);
// 		Bevel(rInside.x0 + 5, rInside.y0 + 5, rInside.x1 - 5, rInside.y1 - 5,5);

		GUI_SetColor(pObj->BkColor);
		FillBevel(rInside.x0 +8,rInside.y0+8,rInside.x1-8,rInside.y1-8,8);

		LCD_SetColor(pObj->MultiTextFrame.FrameColor);
		Bevel(rInside.x0 + 8, rInside.y0 +8, rInside.x1 - 8, rInside.y1 - 8,8);

	}
	else if (pObj->nStyle == 3)//突出显示数据
	{
		LCD_SetColor(pObj->MultiTextFrame.FrameColor);
		GUI_FillRect(rInside.x0, rInside.y0, rInside.x1, rInside.y1);
		EffectSize = pObj->MultiTextFrame.MultiFrameSizeLeft;
		rInside.x0 += EffectSize;

		EffectSize = pObj->MultiTextFrame.MultiFrameSizeRight;
		rInside.x1 -= EffectSize;

		EffectSize = pObj->MultiTextFrame.MultiFrameSizeTop;
		rInside.y0 += EffectSize;

		EffectSize = pObj->MultiTextFrame.MultiFrameSizeBot;
		rInside.y1 -= EffectSize;

		LCD_SetBkColor(pObj->BkColor);
		WM_SetUserClipRect(&rInside);
		GUI_Clear();
		//
		GUI_SetColor(GUI_DARKRED);
		GUI_FillRectEx(&pObj->ParaData.Rect);

	}
	else
	{
// 		LCD_SetColor(pObj->MultiTextFrame.FrameColor);
// 		GUI_FillRect(rInside.x0, rInside.y0, rInside.x1, rInside.y1);
		EffectSize = pObj->MultiTextFrame.MultiFrameSizeLeft;
		rInside.x0 += EffectSize;

		EffectSize = pObj->MultiTextFrame.MultiFrameSizeRight;
		rInside.x1 -= EffectSize;
		
		EffectSize = pObj->MultiTextFrame.MultiFrameSizeTop;
		rInside.y0 += EffectSize;
		
		EffectSize = pObj->MultiTextFrame.MultiFrameSizeBot;
 		rInside.y1 -= EffectSize;

		LCD_SetBkColor(pObj->BkColor);
		WM_SetUserClipRect(&rInside);
		GUI_Clear();
	}
	if (pObj->nCircle==1)
	{
// 		x0 = (int)rInside.x0+88;
// 		y0 = (int)rInside.y0+80;
		x0 = (rInside.x0+rInside.x1)/2;
		y0 = (rInside.y0+rInside.y1)/2+10;
		//
		GUI_SetPenSize(16);
		GUI_SetColor(pObj->ParaData1.Color);					//蓝条
		GUI_AA_FillCircle(x0-43,y0+43,9);
		GUI_AA_FillCircle(x0+43,y0+43,9);
		GUI_AA_DrawArc( x0,y0,61, 61,-45, 225 );
		//
 		DrawArcScale(x0,y0,pObj->nPressure,1);//画压力监测表
 		//
		GUI_SetColor(pObj->ParaData2.Color);//画绿色内圈
		GUI_SetPenSize( 3 );
		GUI_AA_DrawArc( x0,y0,32, 32,-45, 225 );

		//
		GUI_SetTextMode(GUI_TM_TRANS);
		GUI_SetFont(pObj->ParaData.Font);
// 		GUI_SetColor(GUI_YELLOW);
// 		GUI_DrawRectEx( &pObj->ParaData.Rect);
		GUI_SetColor(pObj->ParaData.Color);
		GUI_DispStringInRectWrap(pObj->hpParaData, &pObj->ParaData.Rect, pObj->ParaData.Align, pObj->ParaData.WrapMode);
		//
		GUI_SetFont(pObj->ParaUnit.Font);
		GUI_SetColor(pObj->ParaUnit.Color);
		GUI_DispStringInRectWrap(pObj->hpParaUnit, &pObj->ParaUnit.Rect, pObj->ParaUnit.Align, pObj->ParaUnit.WrapMode);
	}
//	else if (pObj->nCircle==2)
//	{
//		// 		x0 = (int)rInside.x0+88;
//		// 		y0 = (int)rInside.y0+80;
//		x0 = (rInside.x0+rInside.x1)/2;
//		y0 = (rInside.y0+rInside.y1)/2+10;
//		//绘制仪表盘		
//		DrawGradientArc(x0,y0,76,-45,225,GUI_WHITE,GUI_BLACK,270);
//		DrawGradientArc(x0,y0,51,-45,225,GUI_BLACK,GUI_WHITE,270);
//		GUI_SetColor(GUI_WHITE);
//		GUI_SetPenSize(1);
//		GUI_DrawLine(x0-52,y0+54,x0-35,y0+37);
//		GUI_DrawLine(x0+52,y0+54,x0+35,y0+37);
//		//
//		//
//// 		GUI_SetColor(pObj->ParaData2.Color);//画绿色内圈
//// 		GUI_SetPenSize( 3 );
//// 		GUI_AA_DrawArc( x0,y0,32, 32,-45, 225 );
//
//		//
//		GUI_SetTextMode(GUI_TM_TRANS);
//		//
//		GUI_SetFont(pObj->ParaUnit.Font);
//		GUI_SetColor(pObj->ParaUnit.Color);
//		GUI_DispStringInRectWrap(pObj->hpParaUnit, &pObj->ParaUnit.Rect, pObj->ParaUnit.Align, pObj->ParaUnit.WrapMode);
//		DrawArcScale_2(x0,y0,pObj->nPressure,1);//画压力监测表
//
//		GUI_SetFont(pObj->ParaData.Font);
//		// 		GUI_SetColor(GUI_YELLOW);
//		// 		GUI_DrawRectEx( &pObj->ParaData.Rect);
//		GUI_SetColor(pObj->ParaData.Color);
//		GUI_DispStringInRectWrap(pObj->hpParaData, &pObj->ParaData.Rect, pObj->ParaData.Align, pObj->ParaData.WrapMode);
//	}
	else if (pObj->nCircle==3)
	{
		x0 = (rInside.x0+rInside.x1)/2;
		y0 = (rInside.y0+rInside.y1)/2+10;
		//绘制仪表盘		
		GUI_SetPenSize(22);
		GUI_SetColor(pObj->ParaData1.Color);
		GUI_DrawArc(x0,y0,63,63,-45,225);
		//
		if (pObj->nPressure> 0)	//压力大于0
		{
			angle = ((int)(pObj->nPressure)/10.0*(-2.25) + 180);
			GUI_SetColor(pObj->ParaData2.Color);
			GUI_DrawArc( x0,y0,63, 63,angle, 180 );//右边是0刻度开始
		} 
		else if (pObj->nPressure == 0)
		{
		}
		else								//压力小于0
		{
			angle = ((int)(pObj->nPressure)/10.0*(-2.25) + 180);
			GUI_SetColor(pObj->ParaData2.Color);
			if (angle <= 225)
			{
				GUI_DrawArc( x0,y0,63, 63,180, angle );
			}
		}

		GUI_SetPenSize(1);
		//
		GUI_SetTextMode(GUI_TM_TRANS);
		//Unit
		GUI_SetFont(pObj->ParaUnit.Font);
		GUI_SetColor(pObj->ParaUnit.Color);
		GUI_DispStringInRectWrap(pObj->hpParaUnit, &pObj->ParaUnit.Rect, pObj->ParaUnit.Align, pObj->ParaUnit.WrapMode);
		//表刻度
		DrawArcScale_3(x0,y0,pObj->nPressure,1);//画压力监测表

		//Data
		GUI_SetFont(pObj->ParaData.Font);
		GUI_SetColor(pObj->ParaData.Color);
		GUI_DispStringInRectWrap(pObj->hpParaData, &pObj->ParaData.Rect, pObj->ParaData.Align, pObj->ParaData.WrapMode);	
	}
	else
	{
		GUI_SetTextMode(GUI_TM_TRANS);

		GUI_SetFont(pObj->ParaName.Font);
		GUI_SetColor(pObj->ParaName.Color);
		GUI_DispStringInRectWrap(pObj->hpParaName, &pObj->ParaName.Rect, pObj->ParaName.Align, pObj->ParaName.WrapMode);
// 		GUI_FillRectEx(&pObj->ParaName.Rect);

		GUI_SetFont(pObj->ParaName1.Font);
		GUI_SetColor(pObj->ParaName1.Color);
		GUI_DispStringInRectWrap(pObj->hpParaName1, &pObj->ParaName1.Rect, pObj->ParaName1.Align, pObj->ParaName1.WrapMode);

		GUI_SetFont(pObj->ParaName2.Font);
		GUI_SetColor(pObj->ParaName2.Color);
		GUI_DispStringInRectWrap(pObj->hpParaName2, &pObj->ParaName2.Rect, pObj->ParaName2.Align, pObj->ParaName2.WrapMode);

		GUI_SetFont(pObj->ParaName3.Font);
		GUI_SetColor(pObj->ParaName3.Color);
		GUI_DispStringInRectWrap(pObj->hpParaName3, &pObj->ParaName3.Rect, pObj->ParaName3.Align, pObj->ParaName3.WrapMode);

		GUI_SetFont(pObj->ParaData.Font);
		GUI_SetColor(pObj->ParaData.Color);
		GUI_DispStringInRectWrap(pObj->hpParaData, &pObj->ParaData.Rect, pObj->ParaData.Align, pObj->ParaData.WrapMode);
// 		GUI_SetColor(GUI_RED);
// 		GUI_FillRectEx(&pObj->ParaData.Rect);

		GUI_SetFont(pObj->ParaData1.Font);
		GUI_SetColor(pObj->ParaData1.Color);
		GUI_DispStringInRectWrap(pObj->hpParaData1, &pObj->ParaData1.Rect, pObj->ParaData1.Align, pObj->ParaData1.WrapMode);
// 		GUI_SetColor(GUI_GREEN);
// 		GUI_FillRectEx(&pObj->ParaData1.Rect);

		GUI_SetFont(pObj->ParaData2.Font);
		GUI_SetColor(pObj->ParaData2.Color);
		GUI_DispStringInRectWrap(pObj->hpParaData2, &pObj->ParaData2.Rect, pObj->ParaData2.Align, pObj->ParaData2.WrapMode);

		GUI_SetFont(pObj->ParaData3.Font);
		GUI_SetColor(pObj->ParaData3.Color);
		GUI_DispStringInRectWrap(pObj->hpParaData3, &pObj->ParaData3.Rect, pObj->ParaData3.Align, pObj->ParaData3.WrapMode);

		GUI_SetFont(pObj->ParaUnit.Font);
		GUI_SetColor(pObj->ParaUnit.Color);
		GUI_DispStringInRectWrap(pObj->hpParaUnit, &pObj->ParaUnit.Rect, pObj->ParaUnit.Align, pObj->ParaUnit.WrapMode);
// 		GUI_SetColor(GUI_YELLOW);
// 		GUI_FillRectEx(&pObj->ParaUnit.Rect);

		GUI_SetFont(pObj->ParaUnit1.Font);
		GUI_SetColor(pObj->ParaUnit1.Color);
		GUI_DispStringInRectWrap(pObj->hpParaUnit1, &pObj->ParaUnit1.Rect, pObj->ParaUnit1.Align, pObj->ParaUnit1.WrapMode);

		GUI_SetFont(pObj->ParaMaxValue.Font);
		GUI_SetColor(pObj->ParaMaxValue.Color);
		GUI_DispStringInRectWrap(pObj->hpParaMaxValue, &pObj->ParaMaxValue.Rect, pObj->ParaMaxValue.Align, pObj->ParaMaxValue.WrapMode);

		GUI_SetFont(pObj->ParaMaxValue1.Font);
		GUI_SetColor(pObj->ParaMaxValue1.Color);
		GUI_DispStringInRectWrap(pObj->hpParaMaxValue1, &pObj->ParaMaxValue1.Rect, pObj->ParaMaxValue1.Align, pObj->ParaMaxValue1.WrapMode);

		GUI_SetFont(pObj->ParaMinValue.Font);
		GUI_SetColor(pObj->ParaMinValue.Color);
		GUI_DispStringInRectWrap(pObj->hpParaMinValue, &pObj->ParaMinValue.Rect, pObj->ParaMinValue.Align, pObj->ParaMinValue.WrapMode);

		GUI_SetFont(pObj->ParaMinValue1.Font);
		GUI_SetColor(pObj->ParaMinValue1.Color);
		GUI_DispStringInRectWrap(pObj->hpParaMinValue1, &pObj->ParaMinValue1.Rect, pObj->ParaMinValue1.Align, pObj->ParaMinValue1.WrapMode);

		GUI_DRAW__Draw(pObj->ahDrawObj, 0, 0);//Bitmap
	}

	WM_SetUserClipRect(NULL);

}
/*********************************************************************
*
*       _Delete
*
* Delete attached objects (if any)
*/
static void _Delete(MultiText_Obj* pObj)
{
	GUI_ALLOC_FreePtr(&pObj->ahDrawObj);
}
static void _OnTouch(MULTITEXT_Handle hWin, MultiText_Obj* pObj, WM_MESSAGE* pMsg)
{
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
            {
                Msg.MsgId   = WM_NOTIFY_PARENT;
                Msg.Data.v  = WM_NOTIFICATION_CLICKED;
                WM_SendToParent(hWin, &Msg);
            }
        }
    }
}

/*********************************************************************
*
*       MULTITEXT_h2p
*/
#if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
MultiText_Obj * MULTITEXT_h2p(MULTITEXT_Handle h) 
{
	MultiText_Obj * p = (MultiText_Obj *)GUI_ALLOC_h2p(h);
	if (p)
	{
		if (p->DebugId != MULTITEXTID) 
		{
			GUI_DEBUG_ERROROUT("MultipleText.c: Wrong handle type or Object not init'ed");
			return 0;
		}
	}
	return p;
}
#endif

/*********************************************************************
*
*       _UpdateMultiTextDataLeft
*/
static void _UpdateMultiTextDataLeft(MULTITEXT_Handle hWin, MultiText_Obj* pObj)
{
	GUI_RECT rClient;
	GUI_RECT r0;
	WM_SelectWindow(hWin);

	GUI_GetClientRect(&rClient);

	//rInside = rClient;

	r0.x0 =  pObj->ParaData.Rect.x0;
	r0.y0 =  pObj->ParaData.Rect.y0;
	r0.x1 =  pObj->ParaData.Rect.x1;
	r0.y1 =  pObj->ParaData.Rect.y1;

	if (pObj->nStyle == 3)
	{
		GUI_SetColor(GUI_DARKRED);
	}
	else
	{
		GUI_SetColor(pObj->BkColor);
	}
	GUI_FillRect(r0.x0, r0.y0, r0.x1, r0.y1);
	GUI_SetTextMode( GUI_TM_TRANS);
	GUI_SetFont(pObj->ParaData.Font);
// 	GUI_SetColor(GUI_YELLOW);
// 	GUI_DrawRectEx( &pObj->ParaData.Rect);
	GUI_SetColor(pObj->ParaData.Color);
	GUI_DispStringInRectWrap(pObj->hpParaData, &pObj->ParaData.Rect, pObj->ParaData.Align, pObj->ParaData.WrapMode);

	WM_SetUserClipRect(NULL);
}
//
static void _UpdateMultiTextDataLeft1(MULTITEXT_Handle hWin, MultiText_Obj* pObj)
{
	GUI_RECT rClient;
	GUI_RECT r0;
	WM_SelectWindow(hWin);

	GUI_GetClientRect(&rClient);

	//rInside = rClient;

	r0.x0 =  pObj->ParaData2.Rect.x0;
	r0.y0 =  pObj->ParaData2.Rect.y0;
	r0.x1 =  pObj->ParaData2.Rect.x1;
	r0.y1 =  pObj->ParaData2.Rect.y1;

	GUI_SetColor(pObj->BkColor);
	GUI_FillRect(r0.x0, r0.y0, r0.x1, r0.y1);
	GUI_SetTextMode(GUI_TM_TRANS);
	GUI_SetFont(pObj->ParaData2.Font);
	GUI_SetColor(pObj->ParaData2.Color);
	GUI_DispStringInRectWrap(pObj->hpParaData2, &pObj->ParaData2.Rect, pObj->ParaData2.Align, pObj->ParaData2.WrapMode);

	WM_SetUserClipRect(NULL);
}
//
static void _UpdateMultiTextDataRight(MULTITEXT_Handle hWin, MultiText_Obj* pObj)
{
	GUI_RECT rClient;
	GUI_RECT r;

	WM_SelectWindow(hWin);
	GUI_GetClientRect(&rClient);
	//rInside = rClient;

	r.x0 =  pObj->ParaData1.Rect.x0;
	r.y0 =  pObj->ParaData1.Rect.y0;
	r.x1 =  pObj->ParaData1.Rect.x1;
	r.y1 =  pObj->ParaData1.Rect.y1;

	GUI_SetColor(pObj->BkColor);
	GUI_FillRect(r.x0, r.y0, r.x1, r.y1);
	GUI_SetTextMode( GUI_TM_TRANS);
	GUI_SetFont(pObj->ParaData1.Font);
	GUI_SetColor(pObj->ParaData1.Color);
	GUI_DispStringInRectWrap(pObj->hpParaData1, &pObj->ParaData1.Rect, pObj->ParaData1.Align, pObj->ParaData1.WrapMode);

	WM_SetUserClipRect(NULL);
}
//
static void _UpdateMultiTextDataRight1(MULTITEXT_Handle hWin, MultiText_Obj* pObj)
{
	GUI_RECT rClient;
	GUI_RECT r;

	WM_SelectWindow(hWin);
	GUI_GetClientRect(&rClient);
	//rInside = rClient;

	r.x0 =  pObj->ParaData3.Rect.x0;
	r.y0 =  pObj->ParaData3.Rect.y0;
	r.x1 =  pObj->ParaData3.Rect.x1;
	r.y1 =  pObj->ParaData3.Rect.y1;

	GUI_SetColor(pObj->BkColor);
	GUI_FillRect(r.x0, r.y0, r.x1, r.y1);
	GUI_SetTextMode( GUI_TM_TRANS);
	GUI_SetFont(pObj->ParaData3.Font);
	GUI_SetColor(pObj->ParaData3.Color);
	GUI_DispStringInRectWrap(pObj->hpParaData3, &pObj->ParaData3.Rect, pObj->ParaData3.Align, pObj->ParaData3.WrapMode);

	WM_SetUserClipRect(NULL);
}
//
static void _UpdateMultiTextUnit(MULTITEXT_Handle hWin, MultiText_Obj* pObj)
{
	GUI_RECT rClient;
	GUI_RECT r0;

	WM_SelectWindow(hWin);
	GUI_GetClientRect(&rClient);
	//rInside = rClient;

	r0.x0 =  pObj->ParaUnit.Rect.x0;
	r0.y0 =  pObj->ParaUnit.Rect.y0;
	r0.x1 =  pObj->ParaUnit.Rect.x1;
	r0.y1 =  pObj->ParaUnit.Rect.y1;

	GUI_SetColor(pObj->BkColor);
	GUI_FillRect(r0.x0, r0.y0, r0.x1, r0.y1);
	GUI_SetTextMode( GUI_TM_TRANS);
	GUI_SetFont(pObj->ParaUnit.Font);
	GUI_SetColor(pObj->ParaUnit.Color);
	GUI_DispStringInRectWrap(pObj->hpParaUnit, &pObj->ParaUnit.Rect, pObj->ParaUnit.Align, pObj->ParaUnit.WrapMode);

	WM_SetUserClipRect(NULL);
}
static void _UpdateMultiTextUnit1(MULTITEXT_Handle hWin, MultiText_Obj* pObj)
{
	GUI_RECT rClient;
	GUI_RECT r0;

	WM_SelectWindow(hWin);
	GUI_GetClientRect(&rClient);
	//rInside = rClient;

	r0.x0 =  pObj->ParaUnit1.Rect.x0;
	r0.y0 =  pObj->ParaUnit1.Rect.y0;
	r0.x1 =  pObj->ParaUnit1.Rect.x1;
	r0.y1 =  pObj->ParaUnit1.Rect.y1;

	GUI_SetColor(pObj->BkColor);
	GUI_FillRect(r0.x0, r0.y0, r0.x1, r0.y1);
	GUI_SetTextMode( GUI_TM_TRANS);
	GUI_SetFont(pObj->ParaUnit1.Font);
	GUI_SetColor(pObj->ParaUnit1.Color);
	GUI_DispStringInRectWrap(pObj->hpParaUnit1, &pObj->ParaUnit1.Rect, pObj->ParaUnit1.Align, pObj->ParaUnit1.WrapMode);

	WM_SetUserClipRect(NULL);
}
//
static void _UpdateHighLimitValue(MULTITEXT_Handle hWin, MultiText_Obj* pObj)
{
	GUI_RECT rClient;
	GUI_RECT r0;

	WM_SelectWindow(hWin);
	GUI_GetClientRect(&rClient);
	//rInside = rClient;

	r0.x0 =  pObj->ParaMaxValue.Rect.x0;
	r0.y0 =  pObj->ParaMaxValue.Rect.y0;
	r0.x1 =  pObj->ParaMaxValue.Rect.x1;
	r0.y1 =  pObj->ParaMaxValue.Rect.y1;
	GUI_SetColor(pObj->BkColor);
	GUI_FillRect(r0.x0, r0.y0, r0.x1, r0.y1);
	GUI_SetTextMode(GUI_TM_TRANS);
	GUI_SetFont(pObj->ParaMaxValue.Font);
	GUI_SetColor(pObj->ParaMaxValue.Color);
	GUI_DispStringInRectWrap(pObj->hpParaMaxValue, &pObj->ParaMaxValue.Rect, pObj->ParaMaxValue.Align, pObj->ParaMaxValue.WrapMode);

	WM_SetUserClipRect(NULL);
}
static void _UpdateHighLimitValue1(MULTITEXT_Handle hWin, MultiText_Obj* pObj)
{
	GUI_RECT rClient;
	GUI_RECT r0;

	WM_SelectWindow(hWin);
	GUI_GetClientRect(&rClient);
	//rInside = rClient;

	r0.x0 =  pObj->ParaMaxValue1.Rect.x0;
	r0.y0 =  pObj->ParaMaxValue1.Rect.y0;
	r0.x1 =  pObj->ParaMaxValue1.Rect.x1;
	r0.y1 =  pObj->ParaMaxValue1.Rect.y1;
	GUI_SetColor(pObj->BkColor);
	GUI_FillRect(r0.x0, r0.y0, r0.x1, r0.y1);
	GUI_SetTextMode(GUI_TM_TRANS);
	GUI_SetFont(pObj->ParaMaxValue1.Font);
	GUI_SetColor(pObj->ParaMaxValue1.Color);
	GUI_DispStringInRectWrap(pObj->hpParaMaxValue1, &pObj->ParaMaxValue1.Rect, pObj->ParaMaxValue1.Align, pObj->ParaMaxValue1.WrapMode);

	WM_SetUserClipRect(NULL);
}
static void _UpdateLowLimitValue(MULTITEXT_Handle hWin, MultiText_Obj* pObj)
{
	GUI_RECT rClient;
	GUI_RECT r1;

	WM_SelectWindow(hWin);
	GUI_GetClientRect(&rClient);
	//rInside = rClient;

	r1.x0 =  pObj->ParaMinValue.Rect.x0;
	r1.y0 =  pObj->ParaMinValue.Rect.y0;
	r1.x1 =  pObj->ParaMinValue.Rect.x1;
	r1.y1 =  pObj->ParaMinValue.Rect.y1;
	GUI_SetColor(pObj->BkColor);
	GUI_FillRect(r1.x0, r1.y0, r1.x1, r1.y1);
	GUI_SetTextMode(GUI_TM_TRANS);
	GUI_SetFont(pObj->ParaMinValue.Font);
	GUI_SetColor(pObj->ParaMinValue.Color);
	GUI_DispStringInRectWrap(pObj->hpParaMinValue, &pObj->ParaMinValue.Rect, pObj->ParaMinValue.Align, pObj->ParaMinValue.WrapMode);

	WM_SetUserClipRect(NULL);
}
static void _UpdateLowLimitValue1(MULTITEXT_Handle hWin, MultiText_Obj* pObj)
{
	GUI_RECT rClient;
	GUI_RECT r1;

	WM_SelectWindow(hWin);
	GUI_GetClientRect(&rClient);
	//rInside = rClient;

	r1.x0 =  pObj->ParaMinValue1.Rect.x0;
	r1.y0 =  pObj->ParaMinValue1.Rect.y0;
	r1.x1 =  pObj->ParaMinValue1.Rect.x1;
	r1.y1 =  pObj->ParaMinValue1.Rect.y1;
	GUI_SetColor(pObj->BkColor);
	GUI_FillRect(r1.x0, r1.y0, r1.x1, r1.y1);
	GUI_SetTextMode(GUI_TM_TRANS);
	GUI_SetFont(pObj->ParaMinValue1.Font);
	GUI_SetColor(pObj->ParaMinValue1.Color);
	GUI_DispStringInRectWrap(pObj->hpParaMinValue1, &pObj->ParaMinValue1.Rect, pObj->ParaMinValue1.Align, pObj->ParaMinValue1.WrapMode);

	WM_SetUserClipRect(NULL);
}
static void _UpdateParaName(MULTITEXT_Handle hWin, MultiText_Obj* pObj)
{
	GUI_RECT rClient;
	GUI_RECT r1;

	WM_SelectWindow(hWin);
	GUI_GetClientRect(&rClient);
	//rInside = rClient;

	r1.x0 =  pObj->ParaName.Rect.x0+1;//不刷新边框
	r1.y0 =  pObj->ParaName.Rect.y0+1;
	r1.x1 =  pObj->ParaName.Rect.x1-1;
	r1.y1 =  pObj->ParaName.Rect.y1-1;
	GUI_SetColor(pObj->BkColor);
	GUI_FillRect(r1.x0, r1.y0, r1.x1, r1.y1);
	GUI_SetTextMode(GUI_TM_TRANS);
	GUI_SetFont(pObj->ParaName.Font);
	GUI_SetColor(pObj->ParaName.Color);
	GUI_DispStringInRectWrap(pObj->hpParaName, &pObj->ParaName.Rect, pObj->ParaName.Align, pObj->ParaName.WrapMode);

	WM_SetUserClipRect(NULL);
}
//
static void _UpdateParaName1(MULTITEXT_Handle hWin, MultiText_Obj* pObj)
{
	GUI_RECT rClient;
	GUI_RECT r1;

	WM_SelectWindow(hWin);
	GUI_GetClientRect(&rClient);
	//rInside = rClient;

	r1.x0 =  pObj->ParaName2.Rect.x0;
	r1.y0 =  pObj->ParaName2.Rect.y0;
	r1.x1 =  pObj->ParaName2.Rect.x1;
	r1.y1 =  pObj->ParaName2.Rect.y1;
	GUI_SetColor(pObj->BkColor);
	GUI_FillRect(r1.x0, r1.y0, r1.x1, r1.y1);
	GUI_SetTextMode(GUI_TM_TRANS);
	GUI_SetFont(pObj->ParaName2.Font);
	GUI_SetColor(pObj->ParaName2.Color);
	GUI_DispStringInRectWrap(pObj->hpParaName2, &pObj->ParaName2.Rect, pObj->ParaName2.Align, pObj->ParaName2.WrapMode);

	WM_SetUserClipRect(NULL);
}
//
static void _UpdateParaOtherName(MULTITEXT_Handle hWin, MultiText_Obj* pObj)
{
	GUI_RECT rClient;
	GUI_RECT r1;

	WM_SelectWindow(hWin);
	GUI_GetClientRect(&rClient);
	//rInside = rClient;

	r1.x0 =  pObj->ParaName1.Rect.x0;
	r1.y0 =  pObj->ParaName1.Rect.y0;
	r1.x1 =  pObj->ParaName1.Rect.x1;
	r1.y1 =  pObj->ParaName1.Rect.y1;
	GUI_SetColor(pObj->BkColor);
	GUI_FillRect(r1.x0, r1.y0, r1.x1, r1.y1);
	GUI_SetTextMode(GUI_TM_TRANS);
	GUI_SetFont(pObj->ParaName1.Font);
	GUI_SetColor(pObj->ParaName1.Color);
	GUI_DispStringInRectWrap(pObj->hpParaName1, &pObj->ParaName1.Rect, pObj->ParaName1.Align, pObj->ParaName1.WrapMode);

	WM_SetUserClipRect(NULL);
}
//
static void _UpdateParaOtherName1(MULTITEXT_Handle hWin, MultiText_Obj* pObj)
{
	GUI_RECT rClient;
	GUI_RECT r1;

	WM_SelectWindow(hWin);
	GUI_GetClientRect(&rClient);
	//rInside = rClient;

	r1.x0 =  pObj->ParaName3.Rect.x0;
	r1.y0 =  pObj->ParaName3.Rect.y0;
	r1.x1 =  pObj->ParaName3.Rect.x1;
	r1.y1 =  pObj->ParaName3.Rect.y1;
	GUI_SetColor(pObj->BkColor);
	GUI_FillRect(r1.x0, r1.y0, r1.x1, r1.y1);
	GUI_SetTextMode(GUI_TM_TRANS);
	GUI_SetFont(pObj->ParaName3.Font);
	GUI_SetColor(pObj->ParaName3.Color);
	GUI_DispStringInRectWrap(pObj->hpParaName3, &pObj->ParaName3.Rect, pObj->ParaName3.Align, pObj->ParaName3.WrapMode);

	WM_SetUserClipRect(NULL);
}
static void _UpdateMultiPressureData(MULTITEXT_Handle hWin, MultiText_Obj* pObj)
{
	GUI_RECT rClient,rInside;
	GUI_RECT r0;
	int x0,y0,i = 0;
	int angle = 0;
	WM_SelectWindow(hWin);
	GUI_GetClientRect(&rClient);
	rInside = rClient;

	x0 = (rInside.x0+rInside.x1)/2;
	y0 = (rInside.y0+rInside.y1)/2+10;

//	if (pObj->nCircle == 2)
//	{
//		if ((int)(pObj->nPressure) > 0)	//压力大于0
//		{
//			angle = ((int)(pObj->nPressure)*(-2.25) + 180);
//			GUI_SetFont(&GUI_Font_20);
//			GUI_SetTextMode(GUI_TM_TRANS);
//			GUI_SetPenSize( 22 );
//			GUI_SetColor(pObj->ParaData2.Color);
//			GUI_AA_DrawArc( x0,y0,63, 63,angle/*压力值*/, 180 );//右边是0刻度开始
//
//			GUI_SetColor(pObj->ParaData1.Color);
//			//GUI_SetColor(pObj->BkColor);
//			GUI_SetPenSize( 22 );
//			GUI_AA_DrawArc( x0,y0,63, 63,-45/*压力值*/, angle );	//右半边
//			GUI_AA_DrawArc( x0,y0,63, 63,225/*压力值*/, 0 );
//			//
//			GUI_SetFont(pObj->ParaUnit.Font);
//			GUI_SetColor(pObj->ParaUnit.Color);
//			DrawArcScale_2(x0,y0,pObj->nPressure,0);
//		} 
//		else								//压力小于0
//		{
//			angle = ((int)(pObj->nPressure)*(-2.25) + 180);
//			GUI_SetFont(&GUI_Font_20);
//			GUI_SetTextMode(GUI_TM_TRANS);
//			GUI_SetPenSize( 22 );
//			GUI_SetColor(pObj->ParaData2.Color);
//			if (angle <= 225)
//			{
//				GUI_AA_DrawArc( x0,y0,63, 63,180/*压力值*/, angle );
//			}
//
//
//			GUI_SetColor(pObj->ParaData1.Color);
//			// 		GUI_SetColor(pObj->BkColor);
//			GUI_SetPenSize( 22 );
//			GUI_AA_DrawArc( x0,y0,63, 63,-45/*压力值*/, 180 );  //左半边
//			GUI_AA_DrawArc( x0,y0,63, 63,angle/*压力值*/, 225 );  //左半边
//			//
//			GUI_SetFont(pObj->ParaUnit.Font);
//			GUI_SetColor(pObj->ParaUnit.Color);
//			DrawArcScale_2(x0,y0,pObj->nPressure,0);
//		}
//	}
//	else 
		if (pObj->nCircle == 3)
	{
		GUI_SetFont(&GUI_Font_20);
		GUI_SetTextMode(GUI_TM_TRANS);
		GUI_SetPenSize( 22 );
		if (pObj->nPressure> 0)	//压力大于0
		{
			angle = ((int)(pObj->nPressure)/10.0*(-2.25) + 180);
			GUI_SetColor(pObj->ParaData2.Color);
 			GUI_DrawArc( x0,y0,63, 63,angle, 180 );//右边是0刻度开始

			GUI_SetColor(pObj->ParaData1.Color);
			GUI_DrawArc( x0,y0,63, 63,-45, angle );	//右半边
			GUI_DrawArc( x0,y0,63, 63,180, 225);
			//
			GUI_SetFont(pObj->ParaUnit.Font);
			GUI_SetColor(pObj->ParaUnit.Color);
			DrawArcScale_3(x0,y0,pObj->nPressure,0);
		} 
		else if (pObj->nPressure == 0)
		{
			GUI_SetColor(pObj->ParaData1.Color);
			GUI_DrawArc(x0,y0,63,63,-45,225);
			//
			GUI_SetFont(pObj->ParaUnit.Font);
			GUI_SetColor(pObj->ParaUnit.Color);
			DrawArcScale_3(x0,y0,pObj->nPressure,0);
		}
		else								//压力小于0
		{
			angle = ((int)(pObj->nPressure)/10.0*(-2.25) + 180);
			GUI_SetColor(pObj->ParaData2.Color);
			if (angle <= 225)
			{
				GUI_DrawArc( x0,y0,63, 63,180, angle );
			}

			GUI_SetColor(pObj->ParaData1.Color);
			GUI_DrawArc( x0,y0,63, 63,-45, 180 );  //左半边
			GUI_DrawArc( x0,y0,63, 63,angle, 225 );  //左半边
			//
			GUI_SetFont(pObj->ParaUnit.Font);
			GUI_SetColor(pObj->ParaUnit.Color);
			DrawArcScale_3(x0,y0,pObj->nPressure,0);
		}
		GUI_SetPenSize( 1 );
	}
	WM_SetUserClipRect(NULL);
}
/*********************************************************************
*
*       MULTITEXT_Callback
*/
void MULTITEXT_Callback(WM_MESSAGE *pMsg)
{
	int Id,NCode;
	//static int index;
	MULTITEXT_Handle hObj = pMsg->hWin;
	//MultiText_Obj *pObj = MULTITEXT_H2P(hObj);
	MultiText_Obj * pObj = (MultiText_Obj *)GUI_ALLOC_h2p(hObj);
	if (WIDGET_HandleActive(hObj, pMsg) == 0)
	{
		return;
	}
	switch(pMsg->MsgId)
	{
	case WM_PAINT:	  
		_Paint(pObj, hObj);
		return;
	case WM_TOUCH:
		if (pObj->nStyle == 2)
		{
			_OnTouch(hObj, pObj, pMsg);
		}
		return;
	case WM_NOTIFICATION_VALUE_CHANGED:
		switch(pMsg->Data.v)
		{
		case WM_USER_VALUE_CHANGED1:
			_UpdateMultiTextDataLeft(hObj, pObj);
			break;
		case WM_USER_VALUE_CHANGED2:
			_UpdateMultiTextDataRight(hObj, pObj);
			break;
		case WM_USER_VALUE_CHANGED3:
			_UpdateHighLimitValue(hObj, pObj);
			break;
		case WM_USER_VALUE_CHANGED4:
			_UpdateLowLimitValue(hObj, pObj);
			break;
		case WM_USER_VALUE_CHANGED5:
			_UpdateMultiTextUnit(hObj, pObj);
			break;
		case WM_USER_VALUE_CHANGED6:
			_UpdateParaName(hObj, pObj);
			break;
		case WM_USER_VALUE_CHANGED7:
			_UpdateParaOtherName(hObj, pObj);
			break;
		case WM_USER_VALUE_CHANGED8:
			_UpdateMultiTextDataLeft1(hObj, pObj);
			break;
		case WM_USER_VALUE_CHANGED9:
			_UpdateMultiTextDataRight1(hObj, pObj);
			break;
		case WM_USER_VALUE_CHANGED10:
			_UpdateParaName1(hObj, pObj);
			break;
		case WM_USER_VALUE_CHANGED11:
			_UpdateParaOtherName1(hObj, pObj);
			break;
		case WM_USER_VALUE_CHANGED12:
			_UpdateHighLimitValue1(hObj, pObj);
			break;
		case WM_USER_VALUE_CHANGED13:
			_UpdateLowLimitValue1(hObj, pObj);
			break;
		case WM_USER_VALUE_CHANGED14:
			_UpdateMultiTextUnit1(hObj, pObj);
			break;
		case WM_USER_VALUE_CHANGED15:
			_UpdateMultiPressureData(hObj,pObj);
			break;
		}		
		break;
	case WM_DELETE:
		_Delete(pObj);
		break;
	}
	WM_DefaultProc(pMsg);
}
/*********************************************************************
*
*       MULTITEXT_CreateEx
*/
MULTITEXT_Handle MULTITEXT_CreateEx(int x0, int y0, int xsize, int ysize, WM_HWIN hParent, int WinFlags, int ExFlags, int Id) 
{
	MULTITEXT_Handle hObj;
	//GUI_USE_PARA(ExFlags);
	/* Create the window */
	WM_LOCK();

	if(ExFlags & MULTITEXT_CF_MultiDropDownStyle)//DropDown与下方的文本框分离
	{
		hObj = WM_CreateWindowAsChild(x0, y0 + (int)(ysize*0.3), xsize, ysize -(int)(ysize*0.3), hParent, WinFlags, MULTITEXT_Callback, sizeof(MultiText_Obj) - sizeof(WM_Obj));
	}
	else
	{
		hObj = WM_CreateWindowAsChild(x0, y0, xsize, ysize, hParent, WinFlags, MULTITEXT_Callback, sizeof(MultiText_Obj) - sizeof(WM_Obj));
	}
	if(hObj)
	{
		MultiText_Obj* pObj = (MultiText_Obj *)GUI_ALLOC_h2p(hObj);//MULTITEXT_H2P(hObj);
		/* init widget specific variables */
		WIDGET__Init(&pObj->Widget, Id, MULTITEXT_CF_FOCUSSABLE);
		/* init member variables */
		MULTITEXT_INIT_ID(pObj);

		pObj->SeleParaFlag = ExFlags;
		pObj->BkColor = 0x191918;
		pObj->WinBkColor = 0x191918;
		pObj->nStyle = 2;

		pObj->ParaData = MULTITEXT__DefaultProps;
		pObj->ParaData1 = MULTITEXT__DefaultProps;
		pObj->ParaData2 = MULTITEXT__DefaultProps;
		pObj->ParaData3 = MULTITEXT__DefaultProps;
		pObj->ParaName = MULTITEXT__DefaultProps;
		pObj->ParaName1 = MULTITEXT__DefaultProps;
		pObj->ParaName2 = MULTITEXT__DefaultProps;
		pObj->ParaName3 = MULTITEXT__DefaultProps;
		pObj->ParaMaxValue = MULTITEXT__DefaultProps;
		pObj->ParaMinValue = MULTITEXT__DefaultProps;
		pObj->ParaMaxValue1 = MULTITEXT__DefaultProps;
		pObj->ParaMinValue1 = MULTITEXT__DefaultProps;
		pObj->ParaUnit = MULTITEXT__DefaultProps;
		pObj->ParaUnit1 = MULTITEXT__DefaultProps;
// 		pObj->hTimerProps.flag = 1;
		memset(pObj->hpParaName,0,35);
		memset(pObj->hpParaName1,0,10);
		memset(pObj->hpParaName2,0,10);
		memset(pObj->hpParaName3,0,10);
		memset(pObj->hpParaUnit,0,10);
		memset(pObj->hpParaUnit1,0,10);
		memset(pObj->hpParaData,0,10);
		memset(pObj->hpParaData1,0,10);
		memset(pObj->hpParaData2,0,10);
		memset(pObj->hpParaData3,0,10);
		memset(pObj->hpParaMaxValue,0,10);
		memset(pObj->hpParaMinValue,0,10);
		memset(pObj->hpParaMaxValue1,0,10);
		memset(pObj->hpParaMinValue1,0,10);
	} 
	else 
	{
		GUI_DEBUG_ERROROUT_IF(hObj==0, "MULTITEXT_Create failed");
	}
	WM_UNLOCK();
	return hObj;
}

/*********************************************************************
*
*       MULTITEXT_Create
*/
MULTITEXT_Handle MULTITEXT_Create(int x0, int y0, int xsize, int ysize, int Id, int Flags) 
{
	return MULTITEXT_CreateEx(x0, y0, xsize, ysize, WM_HMEM_NULL, Flags, 0, Id);
}
/*********************************************************************
*
*       MultiText_CreateIndirect
*/
MULTITEXT_Handle MultiText_CreateIndirect(const GUI_WIDGET_CREATE_INFO* pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK* cb)
{
	MULTITEXT_Handle  hThis;
	GUI_USE_PARA(cb);
	hThis = MULTITEXT_CreateEx(pCreateInfo->x0 + x0, pCreateInfo->y0 + y0,
		pCreateInfo->xSize, pCreateInfo->ySize,
		hWinParent, 0, pCreateInfo->Flags, pCreateInfo->Id);
	return hThis;
}

/*********************************************************************
*
*       MultiText_SetFrameColor
*/
void MultiText_SetFrameColor(MULTITEXT_Handle hObj, GUI_COLOR Color)
{
	if(hObj)
	{
		MultiText_Obj* pObj;
		WM_LOCK();	
		pObj = MULTITEXT_H2P(hObj);		
		pObj->MultiTextFrame.FrameColor =Color;
		MULTITEXT_Invalidate(hObj);
		WM_UNLOCK();
	}
}
/*********************************************************************
*
*       MultiText_SetLeftFrameSize
*/
void MultiText_SetLeftFrameSize(MULTITEXT_Handle hObj, int size)
{
	if(hObj)
	{
		MultiText_Obj* pObj;
		WM_LOCK();	
		pObj = MULTITEXT_H2P(hObj);		
		pObj->MultiTextFrame.MultiFrameSizeLeft = size;
		MULTITEXT_Invalidate(hObj);
		WM_UNLOCK();
	}
}

void MultiText_SetRightFrameSize(MULTITEXT_Handle hObj, int size)
{
	if(hObj)
	{
		MultiText_Obj* pObj;
		WM_LOCK();	
		pObj = MULTITEXT_H2P(hObj);		
		pObj->MultiTextFrame.MultiFrameSizeRight = size;
		MULTITEXT_Invalidate(hObj);
		WM_UNLOCK();
	}
}
void MultiText_SetTopFrameSize(MULTITEXT_Handle hObj, int size)
{
	if(hObj)
	{
		MultiText_Obj* pObj;
		WM_LOCK();	
		pObj = MULTITEXT_H2P(hObj);		
		pObj->MultiTextFrame.MultiFrameSizeTop = size;
		MULTITEXT_Invalidate(hObj);
		WM_UNLOCK();
	}
}
void MultiText_SetBotFrameSize(MULTITEXT_Handle hObj, int size)
{
	if(hObj)
	{
		MultiText_Obj* pObj;
		WM_LOCK();	
		pObj = MULTITEXT_H2P(hObj);		
		pObj->MultiTextFrame.MultiFrameSizeBot = size;
		MULTITEXT_Invalidate(hObj);
		WM_UNLOCK();
	}
}

/*********************************************************************
*
*       MultiText_SetBKColor
*/
void MultiText_SetBKColor(MULTITEXT_Handle hObj, GUI_COLOR color)
{
	if(hObj)
	{
		MultiText_Obj* pObj;
		WM_LOCK();	
		pObj = MULTITEXT_H2P(hObj);
		pObj->BkColor = color;
		MULTITEXT_Invalidate(hObj);
		WM_UNLOCK();
	}
}
/*********************************************************************
*
*       MultiText_SetBKColor
*/
void MultiText_SetWinBkColor(MULTITEXT_Handle hObj, GUI_COLOR color)
{
	if(hObj)
	{
		MultiText_Obj* pObj;
		WM_LOCK();	
		pObj = MULTITEXT_H2P(hObj);
		pObj->WinBkColor = color;
		MULTITEXT_Invalidate(hObj);
		WM_UNLOCK();
	}
}
/*********************************************************************
*
*       MultiText_SetParaName
*/
void MultiText_SetParaName(MULTITEXT_Handle hObj, char* str)
{
	if(hObj)
	{
		MultiText_Obj* pObj;
		int len = 0;
		WM_LOCK();	
		pObj = MULTITEXT_H2P(hObj);
		len = strlen(str);
		if(len < sizeof(pObj->hpParaName))
		{
			sprintf(pObj->hpParaName, "%s", str);
		}		
			
		WM_UNLOCK();
	}
}
void MultiText_SetParaName1(MULTITEXT_Handle hObj, char* str)
{
	if(hObj)
	{
		MultiText_Obj* pObj;
		WM_LOCK();	
		pObj = MULTITEXT_H2P(hObj);

		if(sizeof(str) < sizeof(pObj->hpParaName2))
		{
			sprintf(pObj->hpParaName2, "%s", str);
		}	

		WM_UNLOCK();
	}
}
void MultiText_SetOtherParaName(MULTITEXT_Handle hObj, char* str)
{
	if(hObj)
	{
		MultiText_Obj* pObj;
		WM_LOCK();	
		pObj = MULTITEXT_H2P(hObj);

		if(sizeof(str) < sizeof(pObj->hpParaName1))
		{
			sprintf(pObj->hpParaName1, "%s", str);
		}	

		WM_UNLOCK();
	}
}
void MultiText_SetOtherParaName1(MULTITEXT_Handle hObj, char* str)
{
	if(hObj)
	{
		MultiText_Obj* pObj;
		WM_LOCK();	
		pObj = MULTITEXT_H2P(hObj);

		if(sizeof(str) < sizeof(pObj->hpParaName3))
		{
			sprintf(pObj->hpParaName3, "%s", str);
		}	

		WM_UNLOCK();
	}
}
/*********************************************************************
*
*       MultiText_SetParaData
*/
void MultiText_SetParaData(MULTITEXT_Handle hObj, char * str)
{
	if(hObj)
	{
		MultiText_Obj* pObj;
		WM_LOCK();	
		pObj = MULTITEXT_H2P(hObj);

		if(sizeof(str) < sizeof(pObj->hpParaData))
		{
			sprintf(pObj->hpParaData, "%s", str);
		}		

		WM_UNLOCK();
	}
}
//
void MultiText_SetParaData1(MULTITEXT_Handle hObj, char * str)
{
	if(hObj)
	{
		MultiText_Obj* pObj;
		WM_LOCK();	
		pObj = MULTITEXT_H2P(hObj);

		if(sizeof(str) < sizeof(pObj->hpParaData2))
		{
			sprintf(pObj->hpParaData2, "%s", str);
		}		

		WM_UNLOCK();
	}
}
//
void MultiText_SetOtherParaData(MULTITEXT_Handle hObj, char * str)
{
	if(hObj)
	{
		MultiText_Obj* pObj;
		WM_LOCK();	
		pObj = MULTITEXT_H2P(hObj);
		if(sizeof(str) < sizeof(pObj->hpParaData1))
		{
			sprintf(pObj->hpParaData1, "%s", str);
		}	
		WM_UNLOCK();
	}
}
//
void MultiText_SetOtherParaData1(MULTITEXT_Handle hObj, char * str)
{
	if(hObj)
	{
		MultiText_Obj* pObj;
		WM_LOCK();	
		pObj = MULTITEXT_H2P(hObj);
		if(sizeof(str) < sizeof(pObj->hpParaData3))
		{
			sprintf(pObj->hpParaData3, "%s", str);
		}	
		WM_UNLOCK();
	}
}
void MultiText_SetPressureData(MULTITEXT_Handle hObj, int pressure)
{
	if(hObj)
	{
		MultiText_Obj* pObj;
		int oldpressure = 0;
		WM_LOCK();	
		pObj = MULTITEXT_H2P(hObj);

		oldpressure = pObj->nPressure;		
		if (pressure != oldpressure)
		{
			pObj->nPressure = pressure;
		}

		WM_UNLOCK();
	}
}
/*********************************************************************
*
*       MultiText_SetParaUnit
*/
void MultiText_SetParaUnit(MULTITEXT_Handle hObj, char* str)
{
	if(hObj)
	{
		MultiText_Obj* pObj;
		WM_LOCK();	
		pObj = MULTITEXT_H2P(hObj);

		if(sizeof(str) < sizeof(pObj->hpParaUnit))
		{
			sprintf(pObj->hpParaUnit, "%s", str);
		}	

		WM_UNLOCK();
	}
}
void MultiText_SetOtherParaUnit(MULTITEXT_Handle hObj, char* str)
{
	if(hObj)
	{
		MultiText_Obj* pObj;
		WM_LOCK();	
		pObj = MULTITEXT_H2P(hObj);

		if(sizeof(str) < sizeof(pObj->hpParaUnit1))
		{
			sprintf(pObj->hpParaUnit1, "%s", str);
		}	

		WM_UNLOCK();
	}
}
/*********************************************************************
*
*       MultiText_SetParaMaxValue
*/
void MultiText_SetParaMaxValue(MULTITEXT_Handle hObj, char * str)
{
	if(hObj)
	{
		MultiText_Obj* pObj;
		WM_LOCK();	
		pObj = MULTITEXT_H2P(hObj);
		if(sizeof(str) < sizeof(pObj->hpParaMaxValue))
		{
			sprintf(pObj->hpParaMaxValue, "%s", str);
		}	
		WM_UNLOCK();
	}
}
void MultiText_SetOtherParaMaxValue(MULTITEXT_Handle hObj, char * str)
{
	if(hObj)
	{
		MultiText_Obj* pObj;
		WM_LOCK();	
		pObj = MULTITEXT_H2P(hObj);
		if(sizeof(str) < sizeof(pObj->hpParaMaxValue1))
		{
			sprintf(pObj->hpParaMaxValue1, "%s", str);
		}	
		WM_UNLOCK();
	}
}
/*********************************************************************
*
*       MultiText_SetParaMinValue
*/
void MultiText_SetParaMinValue(MULTITEXT_Handle hObj, char * str)
{
	if(hObj)
	{
		MultiText_Obj* pObj;
		WM_LOCK();	
		pObj = MULTITEXT_H2P(hObj);
		if(sizeof(str) < sizeof(pObj->hpParaMinValue))
		{
			sprintf(pObj->hpParaMinValue, "%s", str);
		}	
		WM_UNLOCK();
	}
}
void MultiText_SetOtherParaMinValue(MULTITEXT_Handle hObj, char * str)
{
	if(hObj)
	{
		MultiText_Obj* pObj;
		WM_LOCK();	
		pObj = MULTITEXT_H2P(hObj);
		if(sizeof(str) < sizeof(pObj->hpParaMinValue1))
		{
			sprintf(pObj->hpParaMinValue1, "%s", str);
		}	
		WM_UNLOCK();
	}
}
/*********************************************************************
*
*       MultiText_SetTextFont
*/
void MultiText_SetTextFont(MULTITEXT_Handle hObj, const GUI_FONT GUI_UNI_PTR * pFont, MULTITEXTTYPE_t index)
{
	if(hObj)
	{
		MultiText_Obj* pObj;
		WM_LOCK();	
		pObj = MULTITEXT_H2P(hObj);
		switch(index)
		{
		case MultiText_Name:
			pObj->ParaName.Font = pFont;
			break;
		case MultiText_Name1:
			pObj->ParaName1.Font = pFont;
			break;
		case MultiText_Name2://
			pObj->ParaName2.Font = pFont;
			break;
		case MultiText_Name3://
			pObj->ParaName3.Font = pFont;
			break;
		case MultiText_Unit:
			pObj->ParaUnit.Font = pFont;
			break;
		case MultiText_Unit1:
			pObj->ParaUnit1.Font = pFont;
			break;
		case MultiText_Data:
			pObj->ParaData.Font = pFont;
			break;
		case MultiText_Data1:
			pObj->ParaData1.Font = pFont;
			break;
		case MultiText_Data2://
			pObj->ParaData2.Font = pFont;
			break;
		case MultiText_Data3://
			pObj->ParaData3.Font = pFont;
			break;
		case MultiText_MaxValue:
			pObj->ParaMaxValue.Font = pFont;
			break;
		case MultiText_MinValue:
			pObj->ParaMinValue.Font = pFont;
			break;
		case MultiText_MaxValue1:
			pObj->ParaMaxValue1.Font = pFont;
			break;
		case MultiText_MinValue1:
			pObj->ParaMinValue1.Font = pFont;
			break;
		}
		MULTITEXT_Invalidate(hObj);
		WM_UNLOCK();
	}
}
/*********************************************************************
*
*       MultiText_SetTextColor
*/
void MultiText_SetTextColor(MULTITEXT_Handle hObj, GUI_COLOR color, MULTITEXTTYPE_t index)
{
	if(hObj)
	{
		MultiText_Obj* pObj;
		WM_LOCK();	
		pObj = MULTITEXT_H2P(hObj);
		switch(index)
		{
		case MultiText_Name:
			pObj->ParaName.Color = color;
			break;
		case MultiText_Name1:
			pObj->ParaName1.Color = color;
			break;
		case MultiText_Name2://
			pObj->ParaName2.Color = color;
			break;
		case MultiText_Name3://
			pObj->ParaName3.Color = color;
			break;
		case MultiText_Unit:
			pObj->ParaUnit.Color = color;
			break;
		case MultiText_Unit1:
			pObj->ParaUnit1.Color = color;
			break;
		case MultiText_Data:
			pObj->ParaData.Color = color;
			break;
		case MultiText_Data1:
			pObj->ParaData1.Color = color;
			break;
		case MultiText_Data2://
			pObj->ParaData2.Color = color;
			break;
		case MultiText_Data3://
			pObj->ParaData3.Color = color;
			break;
		case MultiText_MaxValue:
			pObj->ParaMaxValue.Color = color;
			break;
		case MultiText_MinValue:
			pObj->ParaMinValue.Color = color;
			break;
		case MultiText_MaxValue1:
			pObj->ParaMaxValue1.Color = color;
			break;
		case MultiText_MinValue1:
			pObj->ParaMinValue1.Color = color;
			break;
		}
		MULTITEXT_Invalidate(hObj);
		WM_UNLOCK();
	}
}
/*********************************************************************
*
*       MultiText_SetTextRect
*/
void MultiText_SetTextRect(MULTITEXT_Handle hObj, int x0, int xsize, int y0, int ysize, MULTITEXTTYPE_t index)
{
	if(hObj)
	{
		MultiText_Obj* pObj;
		WM_LOCK();	
		pObj = MULTITEXT_H2P(hObj);
		switch(index)
		{
		case MultiText_Name:
			pObj->ParaName.Rect.x0 = x0;
			pObj->ParaName.Rect.x1 = x0+xsize;
			pObj->ParaName.Rect.y0 = y0;
			pObj->ParaName.Rect.y1 = y0+ysize;
			break;
		case MultiText_Name1:
			pObj->ParaName1.Rect.x0 = x0;
			pObj->ParaName1.Rect.x1 = x0+xsize;
			pObj->ParaName1.Rect.y0 = y0;
			pObj->ParaName1.Rect.y1 = y0+ysize;
			break;
		case MultiText_Name2:
			pObj->ParaName2.Rect.x0 = x0;
			pObj->ParaName2.Rect.x1 = x0+xsize;
			pObj->ParaName2.Rect.y0 = y0;
			pObj->ParaName2.Rect.y1 = y0+ysize;
			break;
		case MultiText_Name3:
			pObj->ParaName3.Rect.x0 = x0;
			pObj->ParaName3.Rect.x1 = x0+xsize;
			pObj->ParaName3.Rect.y0 = y0;
			pObj->ParaName3.Rect.y1 = y0+ysize;
			break;
		case MultiText_Unit:
			pObj->ParaUnit.Rect.x0 = x0;
			pObj->ParaUnit.Rect.x1 = x0+xsize;
			pObj->ParaUnit.Rect.y0 = y0;
			pObj->ParaUnit.Rect.y1 = y0+ysize;
			break;
		case MultiText_Unit1:
			pObj->ParaUnit1.Rect.x0 = x0;
			pObj->ParaUnit1.Rect.x1 = x0+xsize;
			pObj->ParaUnit1.Rect.y0 = y0;
			pObj->ParaUnit1.Rect.y1 = y0+ysize;
			break;
		case MultiText_Data:
			pObj->ParaData.Rect.x0 = x0;
			pObj->ParaData.Rect.x1 = x0+xsize;
			pObj->ParaData.Rect.y0 = y0;
			pObj->ParaData.Rect.y1 = y0+ysize;
			break;
		case MultiText_Data1:
			pObj->ParaData1.Rect.x0 = x0;
			pObj->ParaData1.Rect.x1 = x0+xsize;
			pObj->ParaData1.Rect.y0 = y0;
			pObj->ParaData1.Rect.y1 = y0+ysize;
			break;
		case MultiText_Data2:
			pObj->ParaData2.Rect.x0 = x0;
			pObj->ParaData2.Rect.x1 = x0+xsize;
			pObj->ParaData2.Rect.y0 = y0;
			pObj->ParaData2.Rect.y1 = y0+ysize;
			break;
		case MultiText_Data3:
			pObj->ParaData3.Rect.x0 = x0;
			pObj->ParaData3.Rect.x1 = x0+xsize;
			pObj->ParaData3.Rect.y0 = y0;
			pObj->ParaData3.Rect.y1 = y0+ysize;
			break;
		case MultiText_MaxValue:
			pObj->ParaMaxValue.Rect.x0 = x0;
			pObj->ParaMaxValue.Rect.x1 = x0+xsize;
			pObj->ParaMaxValue.Rect.y0 = y0;
			pObj->ParaMaxValue.Rect.y1 = y0+ysize;
			break;
		case MultiText_MinValue:
			pObj->ParaMinValue.Rect.x0 = x0;
			pObj->ParaMinValue.Rect.x1 = x0+xsize;
			pObj->ParaMinValue.Rect.y0 = y0;
			pObj->ParaMinValue.Rect.y1 = y0+ysize;
			break;
		case MultiText_MaxValue1:
			pObj->ParaMaxValue1.Rect.x0 = x0;
			pObj->ParaMaxValue1.Rect.x1 = x0+xsize;
			pObj->ParaMaxValue1.Rect.y0 = y0;
			pObj->ParaMaxValue1.Rect.y1 = y0+ysize;
			break;
		case MultiText_MinValue1:
			pObj->ParaMinValue1.Rect.x0 = x0;
			pObj->ParaMinValue1.Rect.x1 = x0+xsize;
			pObj->ParaMinValue1.Rect.y0 = y0;
			pObj->ParaMinValue1.Rect.y1 = y0+ysize;
			break;
		}
		MULTITEXT_Invalidate(hObj);
		WM_UNLOCK();
	}
}
/*********************************************************************
*
*       MultiText_SetTextWrapMode
*/
void MultiText_SetTextWrapMode(MULTITEXT_Handle hObj, GUI_WRAPMODE WrapMode, MULTITEXTTYPE_t index)
{
	if(hObj)
	{
		MultiText_Obj* pObj;
		WM_LOCK();	
		pObj = MULTITEXT_H2P(hObj);
		switch(index)
		{
		case MultiText_Name:
			pObj->ParaName.WrapMode = WrapMode;
			break;
		case MultiText_Name1:
			pObj->ParaName1.WrapMode = WrapMode;
			break;
		case MultiText_Name2:
			pObj->ParaName2.WrapMode = WrapMode;
			break;
		case MultiText_Name3:
			pObj->ParaName3.WrapMode = WrapMode;
			break;
		case MultiText_Unit:
			pObj->ParaUnit.WrapMode = WrapMode;
			break;
		case MultiText_Unit1:
			pObj->ParaUnit1.WrapMode = WrapMode;
			break;
		case MultiText_Data:
			pObj->ParaData.WrapMode = WrapMode;
			break;
		case MultiText_Data1:
			pObj->ParaData1.WrapMode = WrapMode;
			break;
		case MultiText_Data2:
			pObj->ParaData2.WrapMode = WrapMode;
			break;
		case MultiText_Data3:
			pObj->ParaData3.WrapMode = WrapMode;
			break;
		case MultiText_MaxValue:
			pObj->ParaMaxValue.WrapMode = WrapMode;
			break;
		case MultiText_MinValue:
			pObj->ParaMinValue.WrapMode = WrapMode;
			break;
		case MultiText_MaxValue1:
			pObj->ParaMaxValue1.WrapMode = WrapMode;
			break;
		case MultiText_MinValue1:
			pObj->ParaMinValue1.WrapMode = WrapMode;
			break;
		}
		MULTITEXT_Invalidate(hObj);
		WM_UNLOCK();
	}
}
/*********************************************************************
*
*       MultiText_SetTextAlign
*/
void MultiText_SetTextAlign(MULTITEXT_Handle hObj, int Align, MULTITEXTTYPE_t index)
{
	if(hObj)
	{
		MultiText_Obj* pObj;
		WM_LOCK();	
		pObj = MULTITEXT_H2P(hObj);
		switch(index)
		{
		case MultiText_Name:
			pObj->ParaName.Align = Align;
			break;
		case MultiText_Name1:
			pObj->ParaName1.Align = Align;
			break;
		case MultiText_Name2:
			pObj->ParaName2.Align = Align;
			break;
		case MultiText_Name3:
			pObj->ParaName3.Align = Align;
			break;
		case MultiText_Unit:
			pObj->ParaUnit.Align = Align;
			break;
		case MultiText_Unit1:
			pObj->ParaUnit1.Align = Align;
			break;
		case MultiText_Data:
			pObj->ParaData.Align = Align;
			break;
		case MultiText_Data1:
			pObj->ParaData1.Align = Align;
			break;
		case MultiText_Data2:
			pObj->ParaData2.Align = Align;
			break;
		case MultiText_Data3:
			pObj->ParaData3.Align = Align;
			break;
		case MultiText_MaxValue:
			pObj->ParaMaxValue.Align = Align;
			break;
		case MultiText_MinValue:
			pObj->ParaMinValue.Align = Align;
			break;
		case MultiText_MaxValue1:
			pObj->ParaMaxValue1.Align = Align;
			break;
		case MultiText_MinValue1:
			pObj->ParaMinValue1.Align = Align;
			break;
		}
		MULTITEXT_Invalidate(hObj);
		WM_UNLOCK();
	}
}
/*********************************************************************
*
*       MultiText_SetSeleParaFlag
*/
void MultiText_SetSeleParaFlag(MULTITEXT_Handle hObj, int flag)
{
	if(hObj)
	{
		MultiText_Obj* pObj;
		WM_LOCK();	
		pObj = MULTITEXT_H2P(hObj);
		pObj->SeleParaFlag = flag;
		//MULTITEXT_Invalidate(hObj);
		WM_UNLOCK();
	}
}

/*********************************************************************
*
*       MultiText_DropDownAddString
*/
WM_HWIN MultiText_GetHandle(MULTITEXT_Handle hObj)
{
	WM_HWIN hWin;
	MultiText_Obj* pObj;
	if (hObj)
	{
		WM_LOCK();
		pObj = MULTITEXT_H2P(hObj);
		hWin = pObj->hSelectWin;
		WM_UNLOCK();
	}
	return hWin;
}
/*********************************************************************
*
*       MultiText_EX__SetDrawObj
*/
void MultiText_EX__SetDrawObj(MULTITEXT_Handle hObj, GUI_DRAW_HANDLE hDrawObj) 
{
	if(hObj)
	{
		MultiText_Obj* pObj;
		WM_LOCK();
		pObj = MULTITEXT_H2P(hObj);

		GUI_ALLOC_FreePtr(&pObj->ahDrawObj);

		pObj->ahDrawObj = hDrawObj;
		MULTITEXT_Invalidate(hObj);		
		WM_UNLOCK();
	}
}
void MultiText_EX_SetBitmapEx(MULTITEXT_Handle hObj, const GUI_BITMAP* pBitmap, int x, int y)
{
	MultiText_EX__SetDrawObj(hObj, GUI_DRAW_BITMAP_Create(pBitmap, x, y));
}
void MultiText_EX_SetBitmap(MULTITEXT_Handle hObj, const GUI_BITMAP* pBitmap)
{
	MultiText_EX_SetBitmapEx(hObj, pBitmap, 0, 0);
}
void MultiText_SetsCircle(MULTITEXT_Handle hObj, int Circle)
{
	if (hObj)
	{
		MultiText_Obj* pObj;
		GUI_LOCK();
		pObj = MULTITEXT_H2P(hObj);
		pObj->nCircle = Circle;
		MULTITEXT_Invalidate(hObj);
		GUI_UNLOCK();
	}
}
void MultiText_SetsStyte(MULTITEXT_Handle hObj, int Style)
{
	if (hObj)
	{
		MultiText_Obj* pObj;
		GUI_LOCK();
		pObj = MULTITEXT_H2P(hObj);
		pObj->nStyle = Style;
		MULTITEXT_Invalidate(hObj);
		GUI_UNLOCK();
	}
}




/*********************************************************************
*
*       Timer
*/
/*
static void _Timer_Callback (GUI_TIMER_MESSAGE* pTM)
{
	char buf[20] = {0};
	MultiText_Obj* pObj;
	MULTITEXT_Handle hObj = (MULTITEXT_Handle)(pTM->Context);
	if(hObj)
	{	
		WM_LOCK();
		pObj = MULTITEXT_H2P(hObj);
		if(pObj->hTimerProps.TimerFlag)
		{
			pObj->hTimerProps.sec++;
			if(pObj->hTimerProps.sec == 60)
			{
				pObj->hTimerProps.sec = 0;
				pObj->hTimerProps.min++;
			}
			else if(pObj->hTimerProps.min == 60)
			{
				pObj->hTimerProps.min = 0;
				pObj->hTimerProps.hour++;
			}				
			sprintf(buf,"%02d.%02d.%02d",pObj->hTimerProps.hour,pObj->hTimerProps.min,pObj->hTimerProps.sec);
			GUI_TIMER_Restart(pObj->hTimer);
		}

		if(pObj->hTimerProps.flag)
		{
			MultiText_SetParaData(hObj,buf);
			_UpdateMultiTextDataLeft(hObj,pObj);
		}	
		WM_UNLOCK();
	}
}

void MultiText_SetTimeStart(MULTITEXT_Handle hObj)
{
	if(hObj)
	{
		MultiText_Obj* pObj;
		WM_LOCK();
		pObj = MULTITEXT_H2P(hObj);
		if(pObj)
		{
// 			if(pObj->hTimerProps.TimerFlag)
// 			{
// 				pObj->hTimerProps.sec = 0;
// 			}
// 			else
// 			{
// 				pObj->hTimerProps.sec = pObj->hTimerProps.Times;
// 			}

			pObj->hTimer = GUI_TIMER_Create(_Timer_Callback,0,hObj,1);
			GUI_TIMER_SetPeriod(pObj->hTimer,pObj->hTimerProps.TimerPeriod);
			GUI_TIMER_Restart(pObj->hTimer);
		}
	}
}
void MultiText_SetTimerStop(MULTITEXT_Handle hObj)
{
	if(hObj)
	{
		MultiText_Obj * pObj;
		WM_LOCK();
		pObj = MULTITEXT_H2P(hObj);	
		if(pObj) 
		{
			if(pObj->hTimer)
			{
				GUI_TIMER_Delete(pObj->hTimer);
				pObj->hTimer = 0;
			}

// 			sprintf(pObj->hpParaData,"%s", pObj->hTimerProps.hour,pObj->hTimerProps.min,pObj->hTimerProps.sec);
// 
// 			_UpdateMultiTextDataLeft(hObj,pObj);

// 			pObj->hTimerProps.sec = 0;
// 			pObj->hTimerProps.min = 0;
// 			pObj->hTimerProps.hour = 0;

			//MULTITEXT_Invalidate(hObj);	
		}
		WM_UNLOCK();
	}
}
void MultiText_SetTime(MULTITEXT_Handle hObj,int times,int Period,int Flag)
{
	if(hObj)
	{
		MultiText_Obj * pObj;
		WM_LOCK();
		pObj = MULTITEXT_H2P(hObj);	
		if(pObj) 
		{
			pObj->hTimerProps.Times = times;
			pObj->hTimerProps.TimerPeriod = Period;
			pObj->hTimerProps.TimerFlag = Flag;
		}
		WM_UNLOCK();
	}
}
int MultiText_GetSec(MULTITEXT_Handle hObj)
{
	int sec = 0;
	if(hObj)
	{
		MultiText_Obj * pObj;
		WM_LOCK();
		pObj = MULTITEXT_H2P(hObj);	
		if(pObj) 
		{
			sec = pObj->hTimerProps.sec;
		}
		WM_UNLOCK();
	}
	return sec;
}
int MultiText_GetMin(MULTITEXT_Handle hObj)
{
	int min = 0;
	if(hObj)
	{
		MultiText_Obj * pObj;
		WM_LOCK();
		pObj = MULTITEXT_H2P(hObj);	
		if(hObj) 
		{
			min = pObj->hTimerProps.min;
		}
		WM_UNLOCK();
	}
	return min;
}
int MultiText_GetHour(MULTITEXT_Handle hObj)
{
	int hour = 0;
	if(hObj)
	{
		MultiText_Obj * pObj;
		WM_LOCK();
		pObj = MULTITEXT_H2P(hObj);	
		if(pObj) 
		{
			hour = pObj->hTimerProps.hour;
		}
		WM_UNLOCK();
	}
	return hour;
}

void MultiText_SetSec(MULTITEXT_Handle hObj, int sec)
{
	if (hObj)
	{
		MultiText_Obj * pObj;
		WM_LOCK();
		pObj = MULTITEXT_H2P(hObj);	
		pObj->hTimerProps.sec = sec;
		WM_UNLOCK();
	}
}
void MultiText_SetMin(MULTITEXT_Handle hObj, int min)
{
	if (hObj)
	{
		MultiText_Obj * pObj;
		WM_LOCK();
		pObj = MULTITEXT_H2P(hObj);	
		pObj->hTimerProps.min = min;
		WM_UNLOCK();
	}
}
void MultiText_SetHour(MULTITEXT_Handle hObj, int hour)
{
	if (hObj)
	{
		MultiText_Obj * pObj;
		WM_LOCK();
		pObj = MULTITEXT_H2P(hObj);	
		pObj->hTimerProps.hour = hour;
		WM_UNLOCK();
	}
}

void MultiText_SetTimerFlag(MULTITEXT_Handle hObj, int flag)
{
	if (hObj)
	{
		MultiText_Obj * pObj;
		WM_LOCK();
		pObj = MULTITEXT_H2P(hObj);	
		pObj->hTimerProps.flag = flag;
		WM_UNLOCK();
	}
}
*/












