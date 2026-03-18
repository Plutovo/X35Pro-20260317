#ifndef RectInfoCtrl_H
#define RectInfoCtrl_H
#include "WM.H"
#if GUI_WINSUPPORT
#include "Dialog.h"      /* Req. for Create indirect data structure */
#include "LCDConf.h"
//#include "IncludeCommon.h"
typedef WM_HMEM RectInfoCtrl_Handle;
typedef struct
{
    GUI_COLOR BkColor;          //背景色
    GUI_COLOR TextColor;        //字体颜色
    GUI_COLOR DataColor;        //数据颜色
    const char *strText[3];     //左边的标识
    int strEnum[3];             //左边的标识的id号
    const char *strTextR[3];    //右边的标识，（单位）
    float dataText[3];          //右边的数据
    int floatLen[3];            //数据浮点位数
    int SignDefault;            //0:显示默认的内容,默认标志
    //1:显示以上三个数组中的strText[1]的内容　              0001
    //2:显示以上三个数组中的strText[2]的内容                0010
    //3:显示以上三个数组中的strText[1],strText[2]的内容     0011
    //4:显示以上三个数组中的strText[3]的内容                0100
    //5:显示以上三个数组中的strText[1],strText[3]的内容     0101
    //6:显示以上三个数组中的strText[2],strText[3]的内容     0110
    //7:显示以上三个数组中的strText[1/2/3]的内容            0111
    //8:Psupp.按键对应的显示框
    //9:FlowAssist按键对应的显示框
    //10:VolAssist按键对应的显示框
    //-1:自定义内容
} RectInfoCtrlInfo;
RectInfoCtrl_Handle RectInfoCtrl_Create       (WM_CALLBACK* cb, int Flags,
        int x0, int y0, int xsize, int ysize,int Id);
RectInfoCtrl_Handle RectInfoCtrl_CreateAsChild(int x0, int y0, int xsize, int ysize, WM_HWIN hParent,int Id,
        WM_CALLBACK* cb, int Flags);
RectInfoCtrl_Handle RectInfoCtrl_CreateIndirect(const GUI_WIDGET_CREATE_INFO* pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK* pCallback);
void RectInfoCtrl_SetBkColor(RectInfoCtrl_Handle hObj,GUI_COLOR BkColor);
void RectInfoCtrl_SetSignDefault(RectInfoCtrl_Handle hObj,int SignDefault);
void RectInfoCtrl_SetMode(RectInfoCtrl_Handle hObj,char Mode);
void RectInfoCtrl_SetDataEx(RectInfoCtrl_Handle hObj,int Index,float Data);
void RectInfoCtrl_SetFontEx(RectInfoCtrl_Handle hObj,int Index, const GUI_FONT GUI_UNI_PTR * pFont);
void RectInfoCtrl_SetTextEx(RectInfoCtrl_Handle hObj,int Index,const char* strText);
//添加了ID号的参数名<以值的ID来管理>
void RectInfoCtrl_SetTextIdEx(RectInfoCtrl_Handle hObj,int Index,const char* strText,int strEnum);
//获取TEXTID号
int RectInfoCtrl_GetTextId(RectInfoCtrl_Handle hObj,int Index);
void RectInfoCtrl_SetFloatDataEx(RectInfoCtrl_Handle hObj,int Index,float Data,int FloatLen);
void RectInfoCtrl_SetFont(RectInfoCtrl_Handle hObj, const GUI_FONT GUI_UNI_PTR * pFont);
void RectInfoCtrl_SetTextColor(RectInfoCtrl_Handle hObj, GUI_COLOR Color);
void RectInfoCtrl_SetDataColor(RectInfoCtrl_Handle hObj, GUI_COLOR Color);
void RectInfoCtrl_SetDataSizeOffset(RectInfoCtrl_Handle hObj, int Offset) ;
void RectInfoCtrl_SetViewUnit(RectInfoCtrl_Handle hObj, char sUnit);
void RectInfoCtrl_SetUnit(RectInfoCtrl_Handle hObj,int Index,const char* strTextR);
char* _Float2String(float fData,int floatLen);
#endif
#endif
