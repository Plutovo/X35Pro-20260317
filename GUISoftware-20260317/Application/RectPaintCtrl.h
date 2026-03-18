#ifndef RectPaintCtrl_H
#define RectPaintCtrl_H
#include "WM.H"
#if GUI_WINSUPPORT
#include "Dialog.h"      /* Req. for Create indirect data structure */
#include "LCDConf.h"


typedef enum{RECT_DISPSTYLE_SINGLE,RECT_DISPSTYLE_DOUBLE} RECT_DISPSTYLE;//ylk add 20181123 单个参数和两个参数
typedef WM_HMEM RectPaintCtrl_Handle;
typedef struct
{
    GUI_COLOR BkColor;                              ///背景色
    GUI_COLOR ProColor;                             ///前景色
    GUI_COLOR TextColorTopBot;                      ///顶和底的字体颜色
    GUI_COLOR TextColorCen;                         ///中间部分字体颜色
    char* TextTopL;                                 ///顶左显示内容
    char* TextTopR;                                 ///顶右显示内容
    char* TextCenterL;                              ///中间左显示内容
    char* TextCenterR;                              ///中间右显示内容
    const GUI_FONT GUI_UNI_PTR * pFontTopL;         ///左上角字体大小
    const GUI_FONT GUI_UNI_PTR * pFontBotR;         ///右下角字体大小
    const GUI_FONT GUI_UNI_PTR * pFontCenL;         ///中间的位置左字体大小
    const GUI_FONT GUI_UNI_PTR * pFontCenR;         ///中间的位置右字体大小
    int SignBot;                                    ///是否显示右下角的内容的标示 0：不用显示   1：显示并在数据前画一个符号
    char* TextBotRT;                                ///显示内容
    char* TextBotRB;                                ///显示内容
	RECT_DISPSTYLE Rect_DispStyle;                  ///主监测框参数显示样式（一个或两个）
} RectPaintCtrlInfo;
//定义控件RectPaintCtrl在父窗口的坐标位置
typedef struct
{
    int x0;                         //左上角x
    int y0;                         //左上角y
    int xsize;                      //宽
    int ysize;                      //高
} RectPaintCtrlRect;
enum E_ValuesInfo
{
    ValuesInfo_begin=0,
    Ppeak = ValuesInfo_begin,
    Pplat,
    PEEP,
    Pmean,
    Pmin,
    VT,
    VTe,
    MV,
    MVspn,
    FiO2,
    etCO2,
    Vds,
    Vds_VTe,
    VCO2,
    ftotal,
    fmand,
    fspn,
    R,
    C,
    Temp,
    SpO2,
    Pulse,
    VTPSupp,
    NIF_2,
    RSB,
    SC_fspn,
    SC_VT,
    SC_etCO2,
    SC_PSupp,
    Duration,
    Phase,
    Diagnosis,
    ValuesInfo_end
};
static const char ValuesInfo[ValuesInfo_end][12] =
{
    "Ppeak",
    "Pplat",
    "PEEP",
    "Pmean",
    "Pmin",
    "VT",
    "VTe",
    "MV",
    "MVspn",
    "FiO2",
    "EtCO2",
    "Vds",
    "Vds/VTe",
    "VCO2",
    "ftotal",
    "fmand",
    "fspn",
    "R",
    "C",
    "Temp.",
    "SpO2",
    "Pulse",
    "VTPSupp",
    "NIF",
    "RSB",
    "SC_fspn",
    "SC_VT",
    "SC_etCO2",
    "SC_PSupp",
    "Duration",
    "Phase",
    "Diagnosis"
};
RectPaintCtrl_Handle RectPaintCtrl_Create       (WM_CALLBACK* cb, int Flags,
        int x0, int y0, int xsize, int ysize,int Id);
RectPaintCtrl_Handle RectPaintCtrl_CreateAsChild(int x0, int y0, int xsize, int ysize, WM_HWIN hParent,int Id,
        WM_CALLBACK* cb, int Flags);
RectPaintCtrl_Handle RectPaintCtrl_CreateIndirect(const GUI_WIDGET_CREATE_INFO* pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK* pCallback);
/***************************************************************************************************
*               RectPaintCtrl_SetBkColor
*
*       功能：设置控件背景颜色
*       输入：
*           GUI_COLOR BkColor;      //背景颜色
*/
void RectPaintCtrl_SetBkColor(RectPaintCtrl_Handle hObj,GUI_COLOR BkColor);
/***************************************************************************************************
*               RectPaintCtrl_SetColor
*
*       功能：设置控件显示内容的颜色
*       输入：
*           GUI_COLOR TextColorTopBot;      //顶和底内容的颜色
*           GUI_COLOR TextColorCen;         //中间内容颜色
*/
void RectPaintCtrl_SetColor(RectPaintCtrl_Handle hObj,GUI_COLOR TextColorTopBot,GUI_COLOR TextColorCen);
/***************************************************************************************************
*               RectPaintCtrl_SetMaxMinText
*
*       功能：设置控件右下角显示内容
*       输入：
*           int Sign;       //是否显示右下角的内容的标示 0：不用显示    1：显示并在数据前画一个符号
*           char* MinText;
*           char* MaxText
*/
void RectPaintCtrl_SetMaxMinText(RectPaintCtrl_Handle hObj,int Sign,char* MinText,char* MaxText);
/***************************************************************************************************
*               RectPaintCtrl_SetCenterText
*
*       功能：设置控件中间区域显示的内容
*       输入：
*           char* TextCenterL;      //中间左显示内容
*           char* TextCenterR;      //中间右显示内容 如果值为 NULL，则只显示TextCenterL内容，并位置字体大小等是不同的
*/
void RectPaintCtrl_SetCenterText(RectPaintCtrl_Handle hObj,char* TextCenterL,char* TextCenterR);
/***************************************************************************************************
*               RectPaintCtrl_SetTopText
*
*       功能：设置控件顶区域显示的内容
*       输入：
*           char* TextTopL;     //顶左显示内容
*           char* TextTopR;     //顶右显示内容
*/
void RectPaintCtrl_SetTopText(RectPaintCtrl_Handle hObj,char* TextTopL,char* TextTopR);
void RectPaintCtrl_SetTopTextL(RectPaintCtrl_Handle hObj,char* TextTopL);
//在ValuesInfoMansge中的一种 左
int RectPaintCtrl_GetIndexL(RectPaintCtrl_Handle hObj);
/***************************************************************************************************
*               RectPaintCtrl_SetCenterTextL
*
*       功能：设置控件中间区域显示的内容L
*       输入：
*           char* TextCenterL;      //中间左显示内容
*/
void RectPaintCtrl_SetCenterTextL(RectPaintCtrl_Handle hObj,char* TextCenterL);
//在ValuesInfoMansge中的一种 右
int RectPaintCtrl_GetIndexR(RectPaintCtrl_Handle hObj);
/***************************************************************************************************
*               RectPaintCtrl_SetCenterTextR
*
*       功能：设置控件中间区域显示的内容R
*       输入：
*           char* TextCenterR;      //中间右显示内容 如果值为 NULL，则只显示TextCenterL内容，并位置字体大小等是不同的
*/
void RectPaintCtrl_SetCenterTextR(RectPaintCtrl_Handle hObj,char* TextCenterR);
///SetFont
void RectPaintCtrl_SetFont(RectPaintCtrl_Handle hObj,const GUI_FONT GUI_UNI_PTR * pFont,int Index);
void RectPaintCtrl_SetMinText(RectPaintCtrl_Handle hObj,char* MinText);
void RectPaintCtrl_SetMinData(RectPaintCtrl_Handle hObj,int data);
void RectPaintCtrl_SetMaxData(RectPaintCtrl_Handle hObj,int data);
void RectPaintCtrl_SetMaxText(RectPaintCtrl_Handle hObj,char* MaxText);
void RectPaintCtrl_SetDispStyle(RectPaintCtrl_Handle hObj,RECT_DISPSTYLE DispStyle);
void RectPaintCtrl_SetSignBottom(RectPaintCtrl_Handle hObj,int sign);
#endif
#endif
