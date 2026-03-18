//
/***************************************************************************************************
标题:  ButtonExCtrlA.c
功能:  复杂的控件（圆形、外围为弧线并有以个小的三角形），能通过上下、左右键调整显示内容，
       并有一个小的线条位置的改变
说明:

当前版本： 0.1.0
修改记录：
2007.11.27   yanglin, created
***************************************************************************************************/

#include "ButtonExCtrl.h"
#include "GUI_Protected.h"
//#include "alldata.h"
//#include "datatypes.h"
//#include "pf.h"
#ifndef SIZE_OF_ARRAY
#define SIZE_OF_ARRAY(Array) (sizeof(Array) / sizeof(Array[0]))
#endif
#if GUI_WINSUPPORT
//设置传入参数在全局的唯一ID号
/*void ButtonExCtrl_SetParameterId(ButtonExCtrl_Handle hObj,int ParameterId)
{
  if (hObj) {
    ButtonExCtrl_Obj* pObj;
    WM_LOCK();
    pObj = ButtonExCtrl_H2P(hObj);
    pObj->ParameterId = ParameterId;
    WM_UNLOCK();
  }
}
//获得传入参数在全局的唯一ID号
int ButtonExCtrl_GetParameterId(ButtonExCtrl_Handle hObj)
{
  int ret = -1;
  if (hObj) {
    ButtonExCtrl_Obj* pObj;
    WM_LOCK();
    pObj = ButtonExCtrl_H2P(hObj);
    ret = pObj->ParameterId;
    WM_UNLOCK();
  }
  return ret;
}
*/
/***************************************************************************************************
*           ButtonExCtrl_SetButtonExCtrlColor
*   功能：设置控件的颜色信息
*   输入：
*       GUI_COLOR BkColor;      //背景色
*       GUI_COLOR   ProColor1;  //焦点不在控件上的颜色
*       GUI_COLOR   ProColor2;  //焦点在控件上的颜色
*       GUI_COLOR LabelColor;   //标签的颜色
*       GUI_COLOR LineColor1;   //颜色画圆弧、圆的外边缘
*       GUI_COLOR LineColor2;   //颜色画圆弧、圆的外边缘
*
*/
void ButtonExCtrl_SetButtonExCtrlColor(ButtonExCtrl_Handle hObj,
                                       GUI_COLOR BkColor,GUI_COLOR ProColor1,GUI_COLOR ProColor2,
                                       GUI_COLOR LabelColor,GUI_COLOR LineColor1,GUI_COLOR LineColor2)
{
    if (hObj)
    {
        ButtonExCtrl_Obj* pObj;
        WM_LOCK();
        pObj = ButtonExCtrl_H2P(hObj);
        pObj->BECInfo.BkColor = BkColor;
        pObj->BECInfo.ProColor = ProColor1;
        pObj->ProColor1 = ProColor1;
        pObj->ProColor2 = ProColor2;
        pObj->BECInfo.LineColor = LineColor1;
        pObj->BECInfo.LabelColor = LabelColor;
        pObj->LineColor1 = LineColor1;
        pObj->LineColor2 = LineColor2;
        //pObj->BECInfo.TextColor = TextColor;
        ButtonExCtrl_Invalidate(hObj);
        WM_UNLOCK();
    }
}
/***************************************************************************************************
*           ButtonExCtrl_SetButtonExCtrlColorEx
*   功能：设置控件的颜色信息 扩展的
*   输入：
*       GUI_COLOR Color;        ///颜色
*       int Index;              ///不同颜色号
*
*/
void ButtonExCtrl_SetButtonExCtrlColorEx(ButtonExCtrl_Handle hObj,GUI_COLOR Color,int Index)
{
    if (hObj)
    {
        ButtonExCtrl_Obj* pObj;
        WM_LOCK();
        pObj = ButtonExCtrl_H2P(hObj);
        switch(Index)
        {
        case 0:                             ///背景色
            pObj->BECInfo.BkColor = Color;
            break;
        case 1:                             ///焦点不在控件上的颜色
            pObj->BECInfo.ProColor = Color;
            pObj->ProColor1 = Color;
            break;
        case 2:                             ///焦点在控件上的颜色
            pObj->ProColor2 = Color;
            break;
        case 3:                             ///标签的颜色
            pObj->BECInfo.LabelColor = Color;
            break;
        case 4:                             ///颜色画圆弧、圆的外边缘
            pObj->BECInfo.LineColor = Color;
            pObj->LineColor1 = Color;
            break;
        case 5:                             ///颜色画圆弧、圆的外边缘
            pObj->LineColor2 = Color;
            break;
        }
        ButtonExCtrl_Invalidate(hObj);
        WM_UNLOCK();
    }
}
/********************************************************************************
*           ButtonExCtrl_GetButtonExCtrlColorEx
*   功能：获得控件的颜色信息 扩展的
*   输入：
*       GUI_COLOR Color;        ///颜色
*       int Index;              ///不同颜色号
*
*/
GUI_COLOR ButtonExCtrl_GetButtonExCtrlColorEx(ButtonExCtrl_Handle hObj,int Index)
{
	GUI_COLOR Color;
	if (hObj)
	{
		ButtonExCtrl_Obj* pObj;
		WM_LOCK();
		pObj = ButtonExCtrl_H2P(hObj);
		switch(Index)
		{
		case 0:                             ///背景色
			Color = pObj->BECInfo.BkColor;
			break;
		case 1:                             ///焦点不在控件上的颜色
			Color = pObj->BECInfo.ProColor;
			break;
		case 2:                             ///焦点在控件上的颜色
			Color = pObj->ProColor2;
			break;
		case 3:                             ///标签的颜色
			Color = pObj->BECInfo.LabelColor;
			break;
		case 4:                             ///颜色画圆弧、圆的外边缘
			Color = pObj->BECInfo.LineColor;
			break;
		case 5:                             ///颜色画圆弧、圆的外边缘
			Color = pObj->LineColor2;
			break;
// 		case 6:                             ///进度条背景色
// 			Color = pObj->ProgressBkColor;
// 			break;
// 		case 7:                             ///进度条前景色
// 			Color = pObj->ProgressProColor;
// 			break;
		}
		WM_UNLOCK();
	}
	return Color;
}
/***************************************************************************************************
*           ButtonExCtrl_SetFontInfo
*   功能：设置控件中间区域字体的信息
*   输入：
*       GUI_COLOR TextColor1;                           //焦点不在控件上字体的颜色
*       GUI_COLOR TextColor2;                           //焦点在控件上字体的颜色
*       char *Text11;                                   //焦点不在控件上下方显示的内容
*       char *Text12;                                   //焦点在控件上下方显示的内容
*
*/
void ButtonExCtrl_SetFontInfo(ButtonExCtrl_Handle hObj,
                              GUI_COLOR TextColor1,GUI_COLOR TextColor2,char *Text11,char *Text12)
{
    if (hObj)
    {
        ButtonExCtrl_Obj* pObj;
        WM_LOCK();
        pObj = ButtonExCtrl_H2P(hObj);
        pObj->TextColor1 = TextColor1;
        pObj->TextColor2 = TextColor2;
        pObj->BECInfo.TextColor = TextColor1;
        pObj->Text11 = Text11;
        pObj->Text12 = Text12;
        pObj->BECInfo.Text1 = Text11;
        ButtonExCtrl_Invalidate(hObj);
        WM_UNLOCK();
    }
}
/***************************************************************************************************
*           ButtonExCtrl_SetTextColor
*   功能：设置控件中间区域字体颜色
*   输入：
*       GUI_COLOR TextColor;                            //控件上字体的颜色
*       int Index;                                      //[0,1],
*
*/
void ButtonExCtrl_SetTextColor(ButtonExCtrl_Handle hObj,GUI_COLOR TextColor,int Index)
{
    if (hObj)
    {
        ButtonExCtrl_Obj* pObj;
        WM_LOCK();
        pObj = ButtonExCtrl_H2P(hObj);
        if(0==Index)            //焦点不在控件上字体的颜色
        {
            pObj->TextColor1 = TextColor;
        }
        else                    //焦点在控件上字体的颜色
        {
            pObj->TextColor2 = TextColor;
        }
        ButtonExCtrl_Invalidate(hObj);
        WM_UNLOCK();
    }
}
/***************************************************************************************************
*           ButtonExCtrl_SetFontEx
*   功能：设置控件中间区域字体的大小和下方显示的内容的字体大小
*   输入：
*       const GUI_FONT GUI_UNI_PTR *pTextFont;          ///控件上字体的大小
*       int Index;                                      ///不同字体的字体号
*
*/
void ButtonExCtrl_SetFontEx(ButtonExCtrl_Handle hObj,const GUI_FONT GUI_UNI_PTR *pTextFont,int Index)
{
    if (hObj)
    {
        ButtonExCtrl_Obj* pObj;
        WM_LOCK();
        pObj = ButtonExCtrl_H2P(hObj);
        switch(Index)
        {
        case 0:
            pObj->pTextFont1 = pTextFont;
            pObj->BECInfo.pTextFont = pTextFont;
            break;
        case 1:
            pObj->pTextFont2 = pTextFont;
            break;
        case 2:
            pObj->pTextFont = pTextFont;
            break;
        }
        ButtonExCtrl_Invalidate(hObj);
        WM_UNLOCK();
    }
}
/***************************************************************************************************
*           ButtonExCtrl_SetButtonDataInfo
*   功能：设置控件的数据变化的信息
*   输入：
*       int Min;                //最小值
*       int Max;                //最大值
*       int DefaultSize;        //默认值　小的三角形指向的值
*       int Step;               //步距
*       int FloatLen;           //浮点数据的长度        0:为整数据　1:为浮点数小数点后显示的位数为1
*/
void ButtonExCtrl_SetButtonDataInfo(ButtonExCtrl_Handle hObj,
                                    int Min,int Max,int DefaultSize,int Step,int FloatLen)
{
    if (hObj)
    {
        ButtonExCtrl_Obj* pObj;
        WM_LOCK();
        pObj = ButtonExCtrl_H2P(hObj);
        pObj->BDInfo.Min = Min;
        pObj->BDInfo.Max = Max;
        pObj->BDInfo.DefaultSize = DefaultSize;
        pObj->BDInfo.Step = Step;
        pObj->BDInfo.FloatLen = FloatLen;
        ButtonExCtrl_Invalidate(hObj);
        WM_UNLOCK();
    }
}
/***************************************************************************************************
*           ButtonExCtrl_SetData
*   功能：设置控件的中间显示的内容
*   输入：
*       int Data;           //实际数据
*
*/
void ButtonExCtrl_SetData(ButtonExCtrl_Handle hObj,int Data)
{
    if (hObj)
    {
        ButtonExCtrl_Obj* pObj;
        WM_LOCK();
        pObj = ButtonExCtrl_H2P(hObj);
        pObj->Data = Data;
        pObj->Data1 = Data;
        ButtonExCtrl_Invalidate(hObj);
        WM_UNLOCK();
    }
}
/***************************************************************************************************
*           ButtonExCtrl_SetChangeSign
*   功能：设置控件的数据变化标识
*   输入：
*       char changeSign;            //A:停止增加    S:停止减少　N:即可增加也可减少
*
*/
void ButtonExCtrl_SetChangeSign(ButtonExCtrl_Handle hObj,char changeSign)
{
    if (hObj)
    {
        ButtonExCtrl_Obj* pObj;
        WM_LOCK();
        pObj = ButtonExCtrl_H2P(hObj);
        pObj->changeSign = changeSign;
        //ButtonExCtrl_Invalidate(hObj);
        WM_UNLOCK();
    }
}
/***************************************************************************************************
*           ButtonExCtrl_DataAdd
*   功能：控件的中间显示的内容累加
*   输入：
*
*/
void ButtonExCtrl_DataAdd(ButtonExCtrl_Handle hObj)
{
    if (hObj)
    {
        ButtonExCtrl_Obj* pObj;
        WM_LOCK();
        pObj = ButtonExCtrl_H2P(hObj);
        if(pObj->changeSign != 'A')
        {
            pObj->Data1 = (pObj->Data1) + (pObj->BDInfo.Step);
            if(pObj->Data1>pObj->BDInfo.Max)
            {
                pObj->Data1 = pObj->BDInfo.Max;
            }
            pObj->changeSign = 'N';
            ButtonExCtrl_Invalidate(hObj);
        }
        WM_UNLOCK();
    }
}
/***************************************************************************************************
*           ButtonExCtrl_DataSub
*   功能：控件的中间显示的内容累减
*   输入：
*
*/
void ButtonExCtrl_DataSub(ButtonExCtrl_Handle hObj)
{
    if (hObj)
    {
        ButtonExCtrl_Obj* pObj;
        WM_LOCK();
        pObj = ButtonExCtrl_H2P(hObj);
        if(pObj->changeSign != 'S')
        {
            pObj->Data1 = (pObj->Data1) - (pObj->BDInfo.Step);
            if(pObj->Data1<pObj->BDInfo.Min)
            {
                pObj->Data1 = pObj->BDInfo.Min;
            }
            pObj->changeSign = 'N';
            ButtonExCtrl_Invalidate(hObj);
        }
        WM_UNLOCK();
    }
}
/***************************************************************************************************
*               ButtonExCtrl_SetFocus
*
*       功能：控件获得焦点，即将控件颜色变为 pObj->ProColor2，是选中时的颜色
*       输出：
*/
void ButtonExCtrl_SetFocus(ButtonExCtrl_Handle hObj)
{
    if (hObj)
    {
        ButtonExCtrl_Obj* pObj;
        WM_LOCK();
        pObj = ButtonExCtrl_H2P(hObj);
        pObj->BECInfo.ProColor = pObj->ProColor2;
        pObj->BECInfo.LineColor = pObj->LineColor2;
        pObj->BECInfo.Text1 = pObj->Text12;
        pObj->BECInfo.TextColor = GUI_BLACK;//pObj->TextColor2;
        pObj->BECInfo.pTextFont = pObj->pTextFont2;
        WM_SetFocus(hObj);
        ButtonExCtrl_Invalidate(hObj);
        WM_UNLOCK();
    }
}
/***************************************************************************************************
*               
*
*       功能：
*       输出：
*
*/
void ButtonExCtrl_SetTriangleSign(ButtonExCtrl_Handle hObj,int triangleSign)
{
    if (hObj)
    {
        ButtonExCtrl_Obj* pObj;
        WM_LOCK();
        pObj = ButtonExCtrl_H2P(hObj);
        pObj->triangleSign = triangleSign;
        ButtonExCtrl_Invalidate(hObj);
        WM_UNLOCK();
    }
}
/***************************************************************************************************
*               
*
*       功能：
*       输出：
*
*/
//add by yanglin for 2007-9-19
void ButtonExCtrl_SetTextSign(ButtonExCtrl_Handle hObj,int textSign)
{
    if (hObj)
    {
        ButtonExCtrl_Obj* pObj;
        WM_LOCK();
        pObj = ButtonExCtrl_H2P(hObj);
        pObj->textSign = textSign;
        ButtonExCtrl_Invalidate(hObj);
        WM_UNLOCK();
    }
}
/***************************************************************************************************
*               ButtonExCtrl_KillFocus
*
*       功能：控件失去焦点，即将控件颜色变为 pObj->ProColor1，是create时的颜色
*       输出：
*/
void ButtonExCtrl_KillFocus(ButtonExCtrl_Handle hObj)
{
    if (hObj)
    {
        ButtonExCtrl_Obj* pObj;
        WM_LOCK();
        pObj = ButtonExCtrl_H2P(hObj);
        pObj->BECInfo.ProColor = pObj->ProColor1;
        pObj->BECInfo.LineColor = pObj->LineColor1;
        pObj->BECInfo.Text1 = pObj->Text11;
        pObj->BECInfo.TextColor = GUI_WHITE;//pObj->TextColor1;
        pObj->BECInfo.pTextFont = pObj->pTextFont1;
        pObj->Data1 = pObj->Data;               //使控件在选中状态下的值复位
        ButtonExCtrl_Invalidate(hObj);
        WM_UNLOCK();
    }
}
/***************************************************************************************************
*               ButtonExCtrl_GetFocus
*
*       功能：Get控件的状态
*       输出：
*           -1      数据无效 error
*           0       控件被点击一次
*           1       控件被点击偶数次 或0次
*/
int ButtonExCtrl_GetFocus(ButtonExCtrl_Handle hObj)
{
    int bFocus = -1;
    if (hObj)
    {
        ButtonExCtrl_Obj* pObj;
        WM_LOCK();
        pObj = ButtonExCtrl_H2P(hObj);
        if(pObj->BECInfo.ProColor == pObj->ProColor1)
        {
            bFocus = 1;
        }
        else if(pObj->BECInfo.ProColor == pObj->ProColor2)
        {
            bFocus = 0;
        }
        WM_UNLOCK();
    }
    return bFocus;
}
/***************************************************************************************************
*               
*
*       功能：
*       输出：
*
*/
int ButtonExCtrl_GetTriangleSign(ButtonExCtrl_Handle hObj)
{
    int _Sign = 0;
    if (hObj)
    {
        ButtonExCtrl_Obj* pObj;
        WM_LOCK();
        pObj = ButtonExCtrl_H2P(hObj);
        if(pObj->triangleSign == 1)
        {
            _Sign = 1;
        }
        WM_UNLOCK();
    }
    return _Sign;
}
/***************************************************************************************************
*               ButtonExCtrl_GetData
*
*       功能：Get控件的Data
*       输出：
*
*/
int ButtonExCtrl_GetData(ButtonExCtrl_Handle hObj)
{
    int nTemp = 0;
    if (hObj)
    {
        ButtonExCtrl_Obj* pObj;
        WM_LOCK();
        pObj = ButtonExCtrl_H2P(hObj);
        nTemp = pObj->Data;
        WM_UNLOCK();
    }
    return nTemp;
}
/***************************************************************************************************
*               ButtonExCtrl_GetData1
*
*       功能：Get控件的Data1
*       输出：
*
*/
int ButtonExCtrl_GetData1(ButtonExCtrl_Handle hObj)
{
    int nTemp = 0;
    if (hObj)
    {
        ButtonExCtrl_Obj* pObj;
        WM_LOCK();
        pObj = ButtonExCtrl_H2P(hObj);
        nTemp = pObj->Data1;
        WM_UNLOCK();
    }
    return nTemp;
}

void ButtonExCtrl_SetStyle(ButtonExCtrl_Handle hObj,int Style)//ylk add 20181009
{
	if (hObj)
	{
		ButtonExCtrl_Obj* pObj;
		WM_LOCK();
		pObj = ButtonExCtrl_H2P(hObj);
		pObj->Style_Color = Style;
		ButtonExCtrl_Invalidate(hObj);
		WM_UNLOCK();
	}
}
void ButtonExCtrl_SetEdgeColor(ButtonExCtrl_Handle hObj, GUI_COLOR color)
{
	if (hObj)
	{
		ButtonExCtrl_Obj* pObj;
		WM_LOCK();
		pObj = ButtonExCtrl_H2P(hObj);
		pObj->Edge_Color = color;
		ButtonExCtrl_Invalidate(hObj);
		WM_UNLOCK();
	}
}
#else
void WIDGET_viewWin(void) {} /* avoid empty object files */
#endif /* GUI_WINSUPPORT */
