#ifndef TextExCtrl_H
#define TextExCtrl_H
#include "WM.H"
#include "Dialog.h"      /* Req. for Create indirect data structure */
typedef WM_HMEM TextExCtrl_Handle;
TextExCtrl_Handle TextExCtrl_Create       (WM_CALLBACK* cb, int Flags,
        int x0, int y0, int xsize, int ysize,int Id);
TextExCtrl_Handle TextExCtrl_CreateAsChild(int x0, int y0, int xsize, int ysize, WM_HWIN hParent,int Id,
        WM_CALLBACK* cb, int Flags);
TextExCtrl_Handle TextExCtrl_CreateIndirect(const GUI_WIDGET_CREATE_INFO* pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK* pCallback);
void TextExCtrl_SetTextInfo(TextExCtrl_Handle hObj,int ParameterId,const char* ValueName, const char* ValueUnit);
void TextExCtrl_SetFloatLen(TextExCtrl_Handle hObj,int FloatLen);
//获得传入参数在全局的唯一ID号
int TextExCtrl_GetParameterId(TextExCtrl_Handle hObj);
void TextExCtrl_SetTextData(TextExCtrl_Handle hObj,int TextData);
void TextExCtrl_SetTextData1(TextExCtrl_Handle hObj,long TextData1); //cpp md 20130616
void TextExCtrl_SetMaxMinData(TextExCtrl_Handle hObj,int max,int min);
void TextExCtrl_SetVideoMode(TextExCtrl_Handle hObj, int VideoModeFlag);
void TextExCtrl_SetTextUnit(TextExCtrl_Handle hObj,char* TextUnit);
/************************************************************************************************************
*   TextExCtrl_SetCtrlColor
*   功能：设置控件的颜色信息
*   参数：
*   int Index： [0,2]
*  GUI_COLOR    BkColor;                                    //控件背景色
*  GUI_COLOR    TextNUColor;                                //控件中名字和单位的颜色（hText[0]、hText[2]的字体颜色）
*  GUI_COLOR    TextDataColor;                              //控件中变化的数据的颜色（hText[1]的字体颜色）
*/
void TextExCtrl_SetCtrlColor(TextExCtrl_Handle hObj,int Index,GUI_COLOR color);
/************************************************************************************************************
*   TextExCtrl_SetCtrlFont
*   功能：设置控件的字体大小信息
*/
void TextExCtrl_SetCtrlFont(TextExCtrl_Handle hObj,const GUI_FONT GUI_UNI_PTR * pFont);
#endif
