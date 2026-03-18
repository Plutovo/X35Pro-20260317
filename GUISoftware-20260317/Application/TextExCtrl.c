/***************************************************************************************************
标题:  TextExCtrl.c
功能:  将三个TEXT控件包在一块，进行参数的显示，格式为　[name data unit]
说明:
当前版本： 0.1.0
修改记录：
2007.10.18   yanglin, created
***************************************************************************************************/
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include "TextExCtrl.h"
#include "WIDGET.h"
#include "GUI_Protected.h"
#include "RectInfoCtrl.h"
#include "MainTask.h"

//#include "alldata.h"
//#include "modman.h"
#if GUI_WINSUPPORT
#define TEXTEXCTRL_DEFAULT_BKCOLOR              0xA05C40
#define TEXTEXCTRL_DEFAULT_TEXTNUCOLOR          GUI_WHITE
#define TEXTEXCTRL_DEFAULT_TEXTDATACOLOR        GUI_YELLOW
#define TEXTEXCTRL_DEFAULT_TEXTFONT             &GUI_Font_16//&GUI_Font_16
#define TEXTEXCTRL_DEFAULT_VALUEFONT            &GUI_Font_16//监测值的字体
#define TEXTEXCTRL_DEFAULT_TEXTDATA             0//(float)65536 //cpp md 20130615
#define TEXTEXCTRL_DEFAULT_FLOATLEN             0
#define TEXTEXCTRL_DEFAULT_VALUENAME            ""
#define TEXTEXCTRL_DEFAULT_VALUEUNIT            ""
#define TEXTEXCTRL_DEFAULT_MAXVALUE             1000  //  20130328
#define TEXTEXCTRL_DEFAULT_MINVALUE             -1000 //  20130328
#define TEXTEXCTRL_DEFAULT_UNITFONT             &GUI_Font_15
#define TEXTEXCTRL_DEFAULT_INVALIDATION         0xffff
#define TEXTEXCTRL_DEFAULT_CTRLID               -1
#define TEXTEXCTRL_DEFAULT_TEXTDATA1            0 //  20130615

extern VENT_TIME VentTime;
///参数I:E
/*enum _E_DATAIE
{
    VALUE_IE = 8,
    SETTINGS_IE = 60,
    DATAIE_END
};*/
typedef struct
{
    WIDGET Widget;
    GUI_POINT CapturePoint;
    //TEXT_Handle hText[3];                               ///Text 句柄
    //TEXT_Handle hText;                                  ///Text 句柄
    GUI_COLOR BkColor;                                    ///控件背景色
    GUI_COLOR TextNUColor;                                ///控件中名字和单位的颜色（hText[0]、hText[2]的字体颜色）
    GUI_COLOR TextDataColor;                              ///控件中变化的数据的颜色（hText[1]的字体颜色）
    const GUI_FONT GUI_UNI_PTR * pTextFont;               ///Text字体大小
	const GUI_FONT GUI_UNI_PTR * pValueFont;               ///Data字体大小
    const GUI_FONT GUI_UNI_PTR * pUnitFont;               ///unit字体大小
    int TextData;                                         ///中间text显示的data 
    int ParameterId;                                      ///参数ID(全局)
    int FloatLen;                                         ///浮点数据有效位数
    int MaxData;   //  20130328
    int MinData;   //  20130328                                         
    const char* ValueName;                                ///参数的名字
    const char* ValueUnit;                                ///参数的单位
    long TextData1;                               //  20130615    
} TextExCtrl_Obj;
//
#define TextExCtrl_H2P(h) (TextExCtrl_Obj*) GUI_ALLOC_h2p(h)
#define TextExCtrl_Invalidate(hObj) WM_InvalidateWindow(hObj)

// static const int IE_RatioTable[] =  
// {40,35,30,25,20,15,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100}; //  20130615
extern const int IE_RatioTable[];

/***************************************************************************************************
*
*
*/
static void _UpdateTextData(TextExCtrl_Obj* pObj)//text显示刷新
{
    char *strTemp = NULL;
    float insp2exp = 0;
    int xsize = pObj->Widget.Win.Rect.x1 - pObj->Widget.Win.Rect.x0;
    int ysize = pObj->Widget.Win.Rect.y1 - pObj->Widget.Win.Rect.y0;

    GUI_SetColor  (pObj->BkColor);
//    GUI_SetColor(GUI_GREEN); //  for test 20130615
	GUI_FillRect  (xsize/2-pObj->pTextFont->YDist - 13+5, //xsize/2-pObj->pTextFont->YDist //cpp md 20130615 
                   ysize/2-pObj->pTextFont->YSize/2,
                   xsize/2+pObj->pTextFont->YDist + 10+2, //xsize/2+pObj->pTextFont->YDist //cpp md 20130615
                   ysize/2+pObj->pTextFont->YSize/2);

    GUI_SetTextMode(GUI_TM_TRANS);
    GUI_SetColor(pObj->TextDataColor);
    GUI_SetFont(pObj->pValueFont);
    GUI_SetTextAlign(GUI_TA_RIGHT|GUI_TA_VCENTER);
    if(((pObj->Widget.Id==GUI_ID_TEXT2)&&(!strcmp(pObj->ValueName,Sys_Lib_GetDispStr(DP_MV))))|((pObj->Widget.Id==GUI_ID_TEXT3)&&(!strcmp(pObj->ValueName,Sys_Lib_GetDispStr(DP_MVSPN)))))
    {
        if((pObj->TextData>=10000)|(pObj->TextData==0))
        {
            pObj->FloatLen = 0;
        }
        else if((pObj->TextData<10000)&(pObj->TextData>=1000))
        {
            pObj->FloatLen = 1;
        }
        else if((pObj->TextData<1000)|(pObj->TextData!=0))
        {
            pObj->FloatLen = 2;
        }
        strTemp = _Float2String((float)(pObj->TextData)/100,pObj->FloatLen);
    }
    //*//  20130716
    else if(!strcmp(pObj->ValueName,Sys_Lib_GetDispStr(DP_IE)))
    {
        char cTemp[20] = {0};

        if((VentTime.InspTime>0)&&(VentTime.ExpTime))
        {
            if(VentTime.InspTime >= VentTime.ExpTime)//
            {
                insp2exp = (float)VentTime.InspTime/((float)VentTime.ExpTime);
                if((unsigned int)insp2exp > 9)
                {
                    sprintf(cTemp,"--");
                }
                else
                {
                    sprintf(cTemp,"%.1f:1",insp2exp);
                }
            }
            else 
            {
                insp2exp = (float)VentTime.ExpTime/((float)VentTime.InspTime);
                if((unsigned int)insp2exp > 99)
                {
                    sprintf(cTemp,"--");
                }
                else
                {
                    sprintf(cTemp,"1:%.1f",insp2exp);
                }
            }
        }
        else
        {
            strcpy(cTemp,"--");
        }
        strTemp = cTemp;
    }//*///
    else
    {
        if( (pObj->TextData > pObj->MaxData)||(pObj->TextData < pObj->MinData)/*||(pObj->TextData == 0)*/ )          
        {
            strTemp = "--";			
        }
		else if(pObj->TextData == 0)//ylk
		{
			strTemp = _Float2String((float)pObj->TextData,0);
		}
        else
        {
            if(pObj->FloatLen)
            {
				if(!strcmp(pObj->ValueName,Sys_Lib_GetDispStr(DP_PI)))//zkq 20171110
				{
					strTemp = _Float2String((float)pObj->TextData/100.0,pObj->FloatLen);
				}
				else
				{
					strTemp = _Float2String((float)pObj->TextData/10,1);
				}
            }
            else
            {
                strTemp = _Float2String((float)pObj->TextData,0);
            }
        }

    }
    GUI_DispStringAt(strTemp,xsize/2+15+5+5,ysize/2);//刷新监测菜单里text显示位置
}

/***************************************************************************************************
*
*
*/
static void _Paint(TextExCtrl_Obj* pObj)
{
    /* Draw Frame */
    char *strTemp = NULL;
    float insp2exp = 0;
    int xsize = pObj->Widget.Win.Rect.x1 - pObj->Widget.Win.Rect.x0;
    int ysize = pObj->Widget.Win.Rect.y1 - pObj->Widget.Win.Rect.y0;
    GUI_SetColor  (pObj->BkColor);
    GUI_FillRect  (0, 0, xsize, ysize);
    GUI_SetColor  (GUI_WHITE);
    GUI_DrawRect  (0, 0, xsize, ysize);
    //对控件内的Text控件进行初始化
    //_InitCtrl(pObj);
    GUI_SetTextMode(GUI_TM_TRANS);
    GUI_SetColor(pObj->TextDataColor);
    GUI_SetFont(pObj->pValueFont);
    GUI_SetTextAlign(GUI_TA_RIGHT|GUI_TA_VCENTER);
    //strTemp = _Float2String(pObj->TextData,pObj->FloatLen);
    //if((pObj->Widget.Id==GUI_ID_TEXT2)|(pObj->Widget.Id==GUI_ID_TEXT3))
	if(((pObj->Widget.Id==GUI_ID_TEXT2)&&(!strcmp(pObj->ValueName,Sys_Lib_GetDispStr(DP_MV))))|
		((pObj->Widget.Id==GUI_ID_TEXT3)&&(!strcmp(pObj->ValueName,Sys_Lib_GetDispStr(DP_MVSPN)))))
    {
        if((pObj->TextData>=10000)|(pObj->TextData==0))
        {
            pObj->FloatLen = 0;
        }
        else if((pObj->TextData<10000)&(pObj->TextData>=1000))
        {
            pObj->FloatLen = 1;
        }
        else if((pObj->TextData<1000)|(pObj->TextData!=0))
        {
            pObj->FloatLen = 2;
        }
        strTemp = _Float2String((float)(pObj->TextData)/100,pObj->FloatLen);
    }
    //*//  20130716
    else if(!strcmp(pObj->ValueName, (const char *)Sys_Lib_GetDispStr(DP_IE)))  
    {
        char cTemp[20] = {0};
        if((VentTime.InspTime>0)&&(VentTime.ExpTime))
        {
            if(VentTime.InspTime >= VentTime.ExpTime)//
            {
                insp2exp = (float)VentTime.InspTime/((float)VentTime.ExpTime);
                if((unsigned int)insp2exp > 9)
                {
                    sprintf(cTemp,"--");
                }
                else
                {
                    sprintf(cTemp,"%.1f:1",insp2exp);
                }
                
            }
            else 
            {
                insp2exp = (float)VentTime.ExpTime/((float)VentTime.InspTime);
                if((unsigned int)insp2exp > 99)
                {
                    sprintf(cTemp,"--");
                }
                else
                {
                    sprintf(cTemp,"1:%.1f",insp2exp);
                }
            }
        }
        else
        {
            strcpy(cTemp,"--");
        }
        strTemp = cTemp;
    }//*///
    else
    {
        //strTemp = _Float2String(pObj->TextData,pObj->FloatLen); //cpp del 20130328
		if( (pObj->TextData > pObj->MaxData)||(pObj->TextData < pObj->MinData)/*||(pObj->TextData == 0)*/ )
        {
            strTemp = "--";
        }
		else if(pObj->TextData == 0)
		{
			strTemp = _Float2String(pObj->TextData,0);	
		}
        else
        {
            if(pObj->FloatLen)
            {
                strTemp = _Float2String((float)pObj->TextData/10,1);
            }
            else
            {
                strTemp = _Float2String(pObj->TextData,0);
            }
        }
    }
    GUI_DispStringAt(strTemp,xsize/2+25,ysize/2);//ylk modify 调节监测菜单里text显示位置
    GUI_SetColor(pObj->TextNUColor);
	GUI_SetFont(pObj->pTextFont);
    GUI_DispStringAt(pObj->ValueName,2,ysize/2);
    GUI_SetFont(pObj->pUnitFont);
    GUI_SetTextAlign(GUI_TA_RIGHT|GUI_TA_VCENTER);
    GUI_DispStringAt(pObj->ValueUnit,xsize-1,ysize/2);
}
/***************************************************************************************************
*
*
*/
static void _TextExCtrl_Callback (WM_MESSAGE *pMsg)
{
    TextExCtrl_Handle hWin = (TextExCtrl_Handle)(pMsg->hWin);
    TextExCtrl_Obj* pObj = TextExCtrl_H2P(hWin);
    //GUI_RECT* pRect = (GUI_RECT*)(pMsg->Data.p);
    switch (pMsg->MsgId)
    {
    case WM_PAINT:
        _Paint(pObj);
        break;
    case WM_NOTIFICATION_VALUE_CHANGED:
        _UpdateTextData(pObj);
        break;
    }
    /* Let widget handle the standard messages */
    if (WIDGET_HandleActive(hWin, pMsg) == 0)
    {
        return;
    }
    WM_DefaultProc(pMsg);
}
/***************************************************************************************************
*
*
*/
TextExCtrl_Handle TextExCtrl_CreateAsChild( int x0, int y0, int xsize, int ysize, WM_HWIN hParent,int Id,
        WM_CALLBACK* cb, int Flags)
{
    TextExCtrl_Handle hObj;
    //const int nTextXSize = xsize/4; //cpp del 20130328
    /* Create the window */
    WM_LOCK();
    hObj = WM_CreateWindowAsChild(x0, y0, xsize/*+2*HBorder*/, ysize/*+TBorder+BBorder*/, hParent,
                                  Flags, _TextExCtrl_Callback, sizeof(TextExCtrl_Obj) - sizeof(WM_Obj));
    if (hObj)
    {
        TextExCtrl_Obj* pObj = TextExCtrl_H2P(hObj);
        /* init widget specific variables */
        WIDGET__Init(&pObj->Widget,Id, WIDGET_STATE_FOCUSSABLE );
        pObj->BkColor = TEXTEXCTRL_DEFAULT_BKCOLOR;
        pObj->TextData = TEXTEXCTRL_DEFAULT_TEXTDATA;
        pObj->FloatLen = TEXTEXCTRL_DEFAULT_FLOATLEN;
        pObj->TextNUColor = TEXTEXCTRL_DEFAULT_TEXTNUCOLOR;
        pObj->TextDataColor = TEXTEXCTRL_DEFAULT_TEXTDATACOLOR;
        pObj->pTextFont = TEXTEXCTRL_DEFAULT_TEXTFONT;
		pObj->pValueFont = TEXTEXCTRL_DEFAULT_VALUEFONT;
        pObj->pUnitFont = TEXTEXCTRL_DEFAULT_UNITFONT;
        pObj->ValueName = TEXTEXCTRL_DEFAULT_VALUENAME;
        pObj->ValueUnit = TEXTEXCTRL_DEFAULT_VALUEUNIT;
        pObj->ParameterId = Id;
        pObj->MaxData = TEXTEXCTRL_DEFAULT_MAXVALUE;//  20130328
        pObj->MinData = TEXTEXCTRL_DEFAULT_MINVALUE;//  20130328
        pObj->TextData1 = TEXTEXCTRL_DEFAULT_TEXTDATA1;
    }
    WM_UNLOCK();
    return hObj;
}
/***************************************************************************************************
*
*
*/
TextExCtrl_Handle TextExCtrl_Create(WM_CALLBACK* cb,
                                    int Flags,
                                    int x0, int y0, int xsize, int ysize,int Id)
{
    return TextExCtrl_CreateAsChild(x0, y0, xsize, ysize, WM_HWIN_NULL,Id, cb, Flags);
}
/***************************************************************************************************
*
*
*/
TextExCtrl_Handle TextExCtrl_CreateIndirect(const GUI_WIDGET_CREATE_INFO* pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK* pCallback)
{
    TextExCtrl_Handle hObj;
    hObj = TextExCtrl_CreateAsChild(
               pCreateInfo->x0 + x0, pCreateInfo->y0 + y0, pCreateInfo->xSize, pCreateInfo->ySize, hWinParent, pCreateInfo->Id,
               pCallback, 0);
    return hObj;
}
/***************************************************************************************************
*
*
*/
void TextExCtrl_SetTextInfo(TextExCtrl_Handle hObj,int ParameterId,const char* ValueName,const char* ValueUnit)
{
    if (hObj)
    {
        TextExCtrl_Obj* pObj;
        WM_LOCK();
        pObj = TextExCtrl_H2P(hObj);
        pObj->ParameterId = ParameterId;
        pObj->ValueName = ValueName;
        pObj->ValueUnit = ValueUnit;
        TextExCtrl_Invalidate(hObj);
        WM_UNLOCK();
    }
}
/***************************************************************************************************
*
*
*/
void TextExCtrl_SetFloatLen(TextExCtrl_Handle hObj,int FloatLen)
{
    if (hObj)
    {
        TextExCtrl_Obj* pObj;
        WM_LOCK();
        pObj = TextExCtrl_H2P(hObj);
        pObj->FloatLen = FloatLen;
        TextExCtrl_Invalidate(hObj);
        WM_UNLOCK();
    }
}
//获得传入参数在全局的唯一ID号
int TextExCtrl_GetParameterId(TextExCtrl_Handle hObj)
{
    int ret = -1;
    if (hObj)
    {
        TextExCtrl_Obj* pObj;
        WM_LOCK();
        pObj = TextExCtrl_H2P(hObj);
        ret = pObj->ParameterId;
        WM_UNLOCK();
    }
    return ret;
}
/***************************************************************************************************
*
*
*/
void TextExCtrl_SetTextUnit(TextExCtrl_Handle hObj,char* TextUnit)
{
    if(hObj)
    {
        TextExCtrl_Obj* pObj;
        WM_LOCK();
        pObj = TextExCtrl_H2P(hObj);
        pObj->ValueUnit = TextUnit;
        TextExCtrl_Invalidate(hObj);
        WM_UNLOCK();
    }
}
/***************************************************************************************************
*	设置检测值的字体
*
*/
void TextExCtrl_SetValueFont(TextExCtrl_Handle hObj,const GUI_FONT GUI_UNI_PTR * pFont)
{
    if(hObj)
    {
        TextExCtrl_Obj* pObj;
        WM_LOCK();
        pObj = TextExCtrl_H2P(hObj);
        pObj->pValueFont = pFont;
        TextExCtrl_Invalidate(hObj);
        WM_UNLOCK();
    }
}
/***************************************************************************************************
*   TextExCtrl_SetCtrlColor
*   功能：设置控件的颜色信息
*   参数：
*   int Index： [0,2]
*  GUI_COLOR    BkColor;                                    //控件背景色
*  GUI_COLOR    TextNUColor;                                //控件中名字和单位的颜色（hText[0]、hText[2]的字体颜色）
*  GUI_COLOR    TextDataColor;                              //控件中变化的数据的颜色（hText[1]的字体颜色）
*/
void TextExCtrl_SetCtrlColor(TextExCtrl_Handle hObj,int Index,GUI_COLOR color)
{
    if (hObj)
    {
        TextExCtrl_Obj* pObj;
        WM_LOCK();
        pObj = TextExCtrl_H2P(hObj);
        if(Index == 0)
        {
            pObj->BkColor = color;
        }
        else if(Index == 1)
        {
            pObj->TextNUColor = color;
        }
        else if(Index == 2)
        {
            pObj->TextDataColor = color;
        }
        TextExCtrl_Invalidate(hObj);
        WM_UNLOCK();
    }
}
/***************************************************************************************************
*
*
*/
void TextExCtrl_SetTextData(TextExCtrl_Handle hObj,int TextData)
{
    if (hObj)
    {
        TextExCtrl_Obj* pObj;
        WM_LOCK();
        pObj = TextExCtrl_H2P(hObj);
        pObj->TextData = TextData;
        //TextExCtrl_Invalidate(hObj);
        WM_UNLOCK();
    }
}
/***************************************************************************************************
*
*    //  20130615
*/
void TextExCtrl_SetTextData1(TextExCtrl_Handle hObj,long TextData1) //  20130615
{
    if (hObj)
    {
        TextExCtrl_Obj* pObj;
        WM_LOCK();
        pObj = TextExCtrl_H2P(hObj);
        pObj->TextData1 = TextData1;
        //TextExCtrl_Invalidate(hObj);
        WM_UNLOCK();
    }
}
/***************************************************************************************************
*
*
*/
void TextExCtrl_SetMaxMinData(TextExCtrl_Handle hObj,int max,int min)
{
    if (hObj)
    {
        TextExCtrl_Obj* pObj;
        WM_LOCK();
        pObj = TextExCtrl_H2P(hObj);
        pObj->MaxData = max;
        pObj->MinData = min;
        TextExCtrl_Invalidate(hObj);
        WM_UNLOCK();
    }
}
/***************************************************************************************************
*   TextExCtrl_SetCtrlFont
*   功能：设置控件的字体大小信息
*/
void TextExCtrl_SetCtrlFont(TextExCtrl_Handle hObj,const GUI_FONT GUI_UNI_PTR * pFont)
{
    if (hObj)
    {
        TextExCtrl_Obj* pObj;
        WM_LOCK();
        pObj = TextExCtrl_H2P(hObj);
        pObj->pTextFont = pFont;
        TextExCtrl_Invalidate(hObj);
        WM_UNLOCK();
    }
}
#else
void WIDGET_viewWin(void) {} /* avoid empty object files */
#endif /* GUI_WINSUPPORT */
