//
/***************************************************************************************************
标题:  ButtonRotationEx.c
功能:  复杂的控件（圆形、外围为弧线并有以个小的三角形），能通过上下、左右键调整显示内容，
       并有一个小的线条位置的改变
说明:

当前版本： 0.1.0
修改记录：
***************************************************************************************************/
//
#include "ButtonRotation.h"
//#include "alldata.h"
//#include "datatypes.h"
//#include "pf.h"
//#include "pb_apc.h"
//
#ifndef SIZE_OF_ARRAY
#define SIZE_OF_ARRAY(Array) (sizeof(Array) / sizeof(Array[0]))
#endif
//
#if GUI_WINSUPPORT

//设置传入参数在全局的唯一ID号
void ButtonRotation_SetDataInvildSign(ButtonRotation_Handle hObj,int flg)
{
    if (hObj)
    {
        ButtonRotation_Obj* pObj;
        WM_LOCK();
        pObj = ButtonRotation_H2P(hObj);
        pObj->DataInvildSign = flg;
        WM_UNLOCK();
    }
}
//获得传入参数在全局的唯一ID号
/*int ButtonRotation_GetParameterId(ButtonRotation_Handle hObj)
{
  int ret = -1;
  if (hObj) {
    ButtonRotation_Obj* pObj;
    WM_LOCK();
    pObj = ButtonRotation_H2P(hObj);
    ret = pObj->ParameterId;
    WM_UNLOCK();
  }
  return ret;
}
*/

/***************************************************************************************************
*           ButtonRotation_SetColor
*   功能：设置控件的颜色信息
*   输入：
*       GUI_COLOR BkColor;      //背景色
*
*/
void ButtonRotation_SetBkColor(ButtonRotation_Handle hObj,GUI_COLOR BkColor)
{
    if (hObj)
    {
        ButtonRotation_Obj* pObj;
        WM_LOCK();
        pObj = ButtonRotation_H2P(hObj);
        pObj->BECInfo.BkColor = BkColor;
        ButtonRotation_Invalidate(hObj);
        WM_UNLOCK();
    }
}

/***************************************************************************************************
*           ButtonRotation_SetolorEx
*   功能：设置控件的颜色信息 扩展的
*   输入：
*       GUI_COLOR Color;        ///颜色
*       int Index;              ///不同颜色号
*
*/
void ButtonRotation_SetColorEx(ButtonRotation_Handle hObj,GUI_COLOR Color,int Index)
{
    if (hObj)
    {
        ButtonRotation_Obj* pObj;
        WM_LOCK();
        pObj = ButtonRotation_H2P(hObj);
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
        ButtonRotation_Invalidate(hObj);
        WM_UNLOCK();
    }
}

/***************************************************************************************************
*           ButtonRotation_SetFontInfo
*   功能：设置控件中间区域字体的信息
*   输入：
*       GUI_COLOR TextColor1;                           //焦点不在控件上字体的颜色
*       GUI_COLOR TextColor2;                           //焦点在控件上字体的颜色
*       char *Text11;                                   //焦点不在控件上下方显示的内容
*       char *Text12;                                   //焦点在控件上下方显示的内容
*
*/
void ButtonRotation_SetFontInfo(ButtonRotation_Handle hObj,
                                GUI_COLOR TextColor1,GUI_COLOR TextColor2,char *Text11,char *Text12)
{
    if (hObj)
    {
        ButtonRotation_Obj* pObj;
        WM_LOCK();
        pObj = ButtonRotation_H2P(hObj);
        pObj->TextColor1 = TextColor1;
        pObj->TextColor2 = TextColor2;
        pObj->BECInfo.TextColor = TextColor1;
        pObj->Text11 = Text11;
        pObj->Text12 = Text12;
        pObj->BECInfo.Text1 = Text11;
        ButtonRotation_Invalidate(hObj);
        WM_UNLOCK();
    }
}

/***************************************************************************************************
*           ButtonRotation_SetTextColor
*   功能：设置控件中间区域字体颜色
*   输入：
*       GUI_COLOR TextColor;                            //控件上字体的颜色
*       int Index;                                      //[0,1],
*
*/
void ButtonRotation_SetTextColor(ButtonRotation_Handle hObj,GUI_COLOR TextColor,int Index)
{
    if (hObj)
    {
        ButtonRotation_Obj* pObj;
        WM_LOCK();
        pObj = ButtonRotation_H2P(hObj);
        if(0==Index)            //焦点不在控件上字体的颜色
        {
            pObj->TextColor1 = TextColor;
        }
        else                    //焦点在控件上字体的颜色
        {
            pObj->TextColor2 = TextColor;
        }
        ButtonRotation_Invalidate(hObj);
        WM_UNLOCK();
    }
}

/***************************************************************************************************
*           ButtonRotation_SetFontEx
*   功能：设置控件中间区域字体的大小和下方显示的内容的字体大小
*   输入：
*       const GUI_FONT GUI_UNI_PTR *pTextFont;          ///控件上字体的大小
*       int Index;                                      ///不同字体的字体号
*
*/
void ButtonRotation_SetFontEx(ButtonRotation_Handle hObj,
                              const GUI_FONT GUI_UNI_PTR *pTextFont,int Index)
{
    if (hObj)
    {
        ButtonRotation_Obj* pObj;
        WM_LOCK();
        pObj = ButtonRotation_H2P(hObj);
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
        ButtonRotation_Invalidate(hObj);
        WM_UNLOCK();
    }
}

/***************************************************************************************************
*           ButtonRotation_SetButtonRotationDataInfo
*   功能：设置控件的数据变化的信息
*   输入：
*       int Min;                //最小值
*       int Max;                //最大值
*       int DefaultSize;        //默认值　小的三角形指向的值
*       int Step;               //步距
*       int FloatLen;           //浮点数据的长度        0:为整数据　1:为浮点数小数点后显示的位数为1
*/
void ButtonRotation_SetDataInfo(ButtonRotation_Handle hObj,
                                int Min,int Max,int DefaultSize,int Step,int FloatLen)
{
    if (hObj)
    {
        ButtonRotation_Obj* pObj;
        WM_LOCK();
        pObj = ButtonRotation_H2P(hObj);
        pObj->BDInfo.Min = Min;
        pObj->BDInfo.Max = Max;
        pObj->BDInfo.DefaultSize = DefaultSize;
        pObj->BDInfo.Step = Step;
        pObj->BDInfo.FloatLen = FloatLen;
        ButtonRotation_Invalidate(hObj);
        WM_UNLOCK();
    }
}

/***************************************************************************************************
*           ButtonRotation_SetDefaultSize
*   功能：设置控件的默认数据
*   输入：
*/
void ButtonRotation_SetDefaultSize(ButtonRotation_Handle hObj,int DefaultSize)
{
    if (hObj)
    {
        ButtonRotation_Obj* pObj;
        WM_LOCK();
        pObj = ButtonRotation_H2P(hObj);
        pObj->BDInfo.DefaultSize = DefaultSize;
        ButtonRotation_Invalidate(hObj);
        WM_UNLOCK();
    }
}

/***************************************************************************************************
*           ButtonRotation_SetData
*   功能：设置控件的中间显示的内容
*   输入：
*       int Data;           //实际数据
*
*/
void ButtonRotation_SetData(ButtonRotation_Handle hObj,int Data)
{
    if (hObj)
    {
        ButtonRotation_Obj* pObj;
        WM_LOCK();
        pObj = ButtonRotation_H2P(hObj);
        pObj->Data = Data;
        pObj->Data1 = Data;
        pObj->BDInfo.DefaultSize = Data;
        ButtonRotation_Invalidate(hObj);
        WM_UNLOCK();
    }
}

/***************************************************************************************************
*           ButtonRotation_SetData1
*   功能：设置控件的中间显示的内容
*   输入：
*       int Data;           //实际数据
*
*/
void ButtonRotation_SetData1(ButtonRotation_Handle hObj,int Data)
{
    if (hObj)
    {
        ButtonRotation_Obj* pObj;
        WM_LOCK();
        pObj = ButtonRotation_H2P(hObj);
        pObj->Data = Data;
        pObj->Data1 = Data;
        pObj->BDInfo.DefaultSize = Data;
        ButtonRotation_Invalidate(hObj);
        WM_UNLOCK();
    }
}

/***************************************************************************************************
*           ButtonRotation_SetSign
*   功能：设置控件的数据变化标识
*   输入：
*       char changeSign;            //A:停止增加    S:停止减少　N:即可增加也可减少
*
*/
void ButtonRotation_SetSign(ButtonRotation_Handle hObj,char changeSign)
{
    if (hObj)
    {
        ButtonRotation_Obj* pObj;
        WM_LOCK();
        pObj = ButtonRotation_H2P(hObj);
        pObj->changeSign = changeSign;
        //ButtonRotation_Invalidate(hObj);
        WM_UNLOCK();
    }
}

/***************************************************************************************************
*           ButtonRotation_DataAdd
*   功能：控件的中间显示的内容累加
*   输入：
*
*/
unsigned ButtonRotation_DataAdd(ButtonRotation_Handle hObj)
{
    unsigned r = 0;
    if (hObj)
    {
        ButtonRotation_Obj* pObj;
        WM_LOCK();
        pObj = ButtonRotation_H2P(hObj);
        if(pObj->changeSign != 'A') //不停止增加
        {
            pObj->Data1 = (pObj->Data1) + (pObj->BDInfo.Step);
            if(pObj->Data1>pObj->BDInfo.Max)
            {
                pObj->Data1 = pObj->BDInfo.Max;
                r = 1;
            }
            pObj->changeSign = 'N';
            //ButtonRotation_Invalidate(hObj);
        }
        WM_UNLOCK();
    }
}

/***************************************************************************************************
*           ButtonRotation_DataAdd_1
*   功能：控件的中间显示的内容累加
*   输入：
*
*/
unsigned ButtonRotation_DataAdd_1(ButtonRotation_Handle hObj)
{
    unsigned r = 0;
    if (hObj)
    {
        ButtonRotation_Obj* pObj;
        WM_LOCK();
        pObj = ButtonRotation_H2P(hObj);
        if(pObj->changeSign != 'A') //不停止增加
        {
            pObj->Data1 = (pObj->Data1) + (pObj->BDInfo.Step);
            if(pObj->Data1>pObj->BDInfo.Max)
            {
                pObj->Data1 = pObj->BDInfo.Min;
                r = 1;
            }
            pObj->changeSign = 'N';
            //ButtonRotation_Invalidate(hObj);
        }
        WM_UNLOCK();
    }
}

/***************************************************************************************************
*           ButtonRotation_DataSub
*   功能：控件的中间显示的内容累减
*   输入：
*
*/
unsigned ButtonRotation_DataSub(ButtonRotation_Handle hObj)
{
    unsigned r = 0;
    if (hObj)
    {
        ButtonRotation_Obj* pObj;
        WM_LOCK();
        pObj = ButtonRotation_H2P(hObj);
        if(pObj->changeSign != 'S')
        {
            pObj->Data1 = (pObj->Data1) - (pObj->BDInfo.Step);
            if(pObj->Data1<pObj->BDInfo.Min)
            {
                pObj->Data1 = pObj->BDInfo.Min;
                r = 1;
            }
            pObj->changeSign = 'N';
            //ButtonRotation_Invalidate(hObj);
        }
        WM_UNLOCK();
    }
    return r;
}
/***************************************************************************************************
*           ButtonRotation_DataSub_1
*   功能：控件的中间显示的内容累减
*   输入：
*
*/
unsigned ButtonRotation_DataSub_1(ButtonRotation_Handle hObj)
{
    unsigned r = 0;
    if (hObj)
    {
        ButtonRotation_Obj* pObj;
        WM_LOCK();
        pObj = ButtonRotation_H2P(hObj);
        if(pObj->changeSign != 'S')
        {
            pObj->Data1 = (pObj->Data1) - (pObj->BDInfo.Step);
            if(pObj->Data1<pObj->BDInfo.Min)
            {
                pObj->Data1 = pObj->BDInfo.Max;
                r = 1;
            }
            pObj->changeSign = 'N';
            //ButtonRotation_Invalidate(hObj);
        }
        WM_UNLOCK();
    }
    return r;
}

/***************************************************************************************************
*               ButtonRotation_SetFocus
*
*       功能：控件获得焦点，即将控件颜色变为 pObj->ProColor2，是选中时的颜色
*       输出：
*/
void ButtonRotation_SetFocus(ButtonRotation_Handle hObj)
{
    if (hObj)
    {
        ButtonRotation_Obj* pObj;
        WM_LOCK();
        pObj = ButtonRotation_H2P(hObj);
        pObj->BECInfo.ProColor = pObj->ProColor2;
        pObj->BECInfo.LineColor = pObj->LineColor2;
        pObj->BECInfo.Text1 = pObj->Text12;
        WM_SetFocus(hObj);
        ButtonRotation_Invalidate(hObj);
        WM_UNLOCK();
    }
}

/***************************************************************************************************
* @brief  
* @para   
* @retval 
*/
void ButtonRotation_SetTriangleSign(ButtonRotation_Handle hObj,int triangleSign)
{
    if (hObj)
    {
        ButtonRotation_Obj* pObj;
        WM_LOCK();
        pObj = ButtonRotation_H2P(hObj);
        pObj->triangleSign = triangleSign; //1: 按下
        ButtonRotation_Invalidate(hObj);
        WM_UNLOCK();
    }
}

/***************************************************************************************************
* @brief  
* @para   
* @retval 
*/
void ButtonRotation_SetTextSign(ButtonRotation_Handle hObj,int textSign)
{
    if (hObj)
    {
        ButtonRotation_Obj* pObj;
        WM_LOCK();
        pObj = ButtonRotation_H2P(hObj);
        pObj->textSign = textSign;
        ButtonRotation_Invalidate(hObj);
        WM_UNLOCK();
    }
}

/***************************************************************************************************
* @brief  
* @para   
* @retval 
*/
//参数名标识
void ButtonRotation_SetText(ButtonRotation_Handle hObj,char* Text1,char* Text2)
{
    if (hObj)
    {
        ButtonRotation_Obj* pObj;
        WM_LOCK();
        pObj = ButtonRotation_H2P(hObj);
        pObj->BECInfo.Text1=Text1;
        pObj->BECInfo.Text2=Text2;
        ButtonRotation_Invalidate(hObj);
        WM_UNLOCK();
    }
}

/***************************************************************************************************
* @brief  
* @para   
* @retval 
*/
void ButtonRotation_SetMaxMin(ButtonRotation_Handle hObj, int max, int min)
{
    if (hObj)
    {
        ButtonRotation_Obj* pObj;
        WM_LOCK();
        pObj = ButtonRotation_H2P(hObj);
        pObj->BDInfo.Max = max;
        pObj->BDInfo.Min = min;
        ButtonRotation_Invalidate(hObj);
        WM_UNLOCK();
    }
}

/***************************************************************************************************
*               ButtonRotation_KillFocus
*
*       功能：控件失去焦点，即将控件颜色变为 pObj->ProColor1，是create时的颜色
*       输出：
*/
void ButtonRotation_KillFocus(ButtonRotation_Handle hObj)
{
    if (hObj)
    {
        ButtonRotation_Obj* pObj;
        WM_LOCK();
        pObj = ButtonRotation_H2P(hObj);
        pObj->BECInfo.ProColor = pObj->ProColor1;
        pObj->BECInfo.LineColor = pObj->LineColor1;
        pObj->Data1 = pObj->Data;               //使控件在选中状态下的值复位
        ButtonRotation_Invalidate(hObj);
        WM_UNLOCK();
    }
}

/***************************************************************************************************
*               ButtonRotation_GetFocus
*
*       功能：Get控件的状态
*       输出：
*           -1      数据无效 error
*           0       控件被点击一次
*           1       控件被点击偶数次 或0次
*/
int ButtonRotation_GetFocus(ButtonRotation_Handle hObj)
{
    int bFocus = -1;
    if (hObj)
    {
        ButtonRotation_Obj* pObj;
        WM_LOCK();
        pObj = ButtonRotation_H2P(hObj);
        if(pObj->BECInfo.ProColor == pObj->ProColor1)
        {
            bFocus = 1;
        }
        else if(pObj->BECInfo.ProColor == pObj->ProColor2)
        {
            bFocus = 0;
        }
        else
        {
            bFocus = -1;
        }
        WM_UNLOCK();
    }
    return bFocus;
}

/***************************************************************************************************
*               ButtonRotation_GetData
*
*       功能：Get控件的Data
*       输出：
*
*/
int ButtonRotation_GetData(ButtonRotation_Handle hObj)
{
    int nTemp = 0;
    if (hObj)
    {
        ButtonRotation_Obj* pObj;
        WM_LOCK();
        pObj = ButtonRotation_H2P(hObj);
        nTemp = pObj->Data;
        WM_UNLOCK();
    }
    return nTemp;
}

/***************************************************************************************************
*               ButtonRotation_GetTriangleSign
*
*       功能：Get控件的Data
*       输出：
*
*/
int ButtonRotation_GetTriangleSign(ButtonRotation_Handle hObj)
{
    int nTemp = 0;
    if (hObj)
    {
        ButtonRotation_Obj* pObj;
        WM_LOCK();
        pObj = ButtonRotation_H2P(hObj);
        nTemp = pObj->triangleSign; //1: 按下
        WM_UNLOCK();
    }
    return nTemp;
}

/***************************************************************************************************
*               ButtonRotation_GetData1
*
*       功能：Get控件的Data1
*       输出：
*
*/
int ButtonRotation_GetData1(ButtonRotation_Handle hObj)
{
    int nTemp = 0;
    if (hObj)
    {
        ButtonRotation_Obj* pObj;
        WM_LOCK();
        pObj = ButtonRotation_H2P(hObj);
        nTemp = pObj->Data1;
        WM_UNLOCK();
    }
    return nTemp;
}
//
#else
void WIDGET_viewWin(void) {} /* avoid empty object files */
#endif /* GUI_WINSUPPORT */



