//
#ifndef ButtonExCtrl_H
#define ButtonExCtrl_H
//
#include "WM.H"
#include "GUI.H"
#if GUI_WINSUPPORT
#include "Dialog.h"      /* Req. for Create indirect data structure */
typedef WM_HMEM ButtonExCtrl_Handle;
//
typedef struct
{
    GUI_COLOR BkColor;                        ///背景色
    GUI_COLOR ProColor;                       ///前景色
    GUI_COLOR LabelColor;                     ///标签的颜色
    GUI_COLOR LineColor;                      ///颜色画圆弧、圆的外边缘
    GUI_COLOR TextColor;                      ///字体颜色
    const GUI_FONT GUI_UNI_PTR *pTextFont;    ///字体大小  默认的字体GUI_Font_15　为另外一种字体
    const GUI_FONT GUI_UNI_PTR *pUnitFont;
    char* Text;                               ///显示内容在中间
    char* Text1;                              ///显示内容在下方
    char* Text2;
} ButtonExCtrlInfo;
///所有的值为整形，与实际的数据相差10的FloatLen次方倍
typedef struct
{
    int Min;                //最小值
    int Max;                //最大值
    int DefaultSize;        //默认值　小的三角形指向的值
    int Step;               //步距
    int FloatLen;           //浮点数据的长度        0:为整数据　1:为浮点数小数点后显示的位数为1
} ButtonDataInfo;
//
typedef struct
{
    WIDGET Widget;
    GUI_POINT CapturePoint;
    ButtonExCtrlInfo BECInfo;
    ButtonDataInfo BDInfo;
    int Data;                                   ///实际数据
    int Data1;                                  ///焦点在控件上时，变化中的数据
    GUI_COLOR ProColor1;                        ///焦点不在控件上的颜色
    GUI_COLOR TextColor1;                       ///焦点不在控件上字体的颜色
    GUI_COLOR LineColor1;                       ///焦点不在控件上线的颜色
    GUI_COLOR ProColor2;                        ///焦点在控件上的颜色
    GUI_COLOR TextColor2;                       ///焦点在控件上字体的颜色
    GUI_COLOR LineColor2;                       ///焦点在控件上线的颜色
    int triangleSign;                           ///三角形的显示标志　1:显示（默认）　0:不显示
    int textSign;                               ///中间Text显示  1:显示（默认）　0:不显示 
    const GUI_FONT GUI_UNI_PTR *pTextFont1;     ///焦点不在控件上字体的大小     
    char *Text11;                               ///焦点不在控件上下方显示的内容
    const GUI_FONT GUI_UNI_PTR *pTextFont2;     ///焦点在控件上字体的大小
    char *Text12;                               ///焦点在控件上下方显示的内容
    const GUI_FONT GUI_UNI_PTR *pTextFont;      ///下方显示的内容的字体大小
    char changeSign;                            ///A:停止增加   S:停止减少　N:即可增加也可减少
// int TouchSign;                               ///参数ID(全局)
	int Style_Color;							///ylk add 20181009 预设模式控制参数按钮左上边框颜色
	GUI_COLOR ProgressBkColor;                            ///进度条背景色
	GUI_COLOR ProgressProColor;                           ///进度条前景色

	GUI_COLOR Edge_Color;
} ButtonExCtrl_Obj;
//
#define ButtonExCtrl_H2P(h) (ButtonExCtrl_Obj*) GUI_ALLOC_h2p(h)
#define ButtonExCtrl_Invalidate(hObj) WM_InvalidateWindow(hObj)
//
ButtonExCtrl_Handle ButtonExCtrl_Create(WM_CALLBACK* cb, int Flags,
                                        int x0, int y0, int xsize, int ysize,int Id);
//
ButtonExCtrl_Handle ButtonExCtrl_CreateAsChild(int x0, int y0, int xsize, int ysize, 
                                               WM_HWIN hParent,int Id,
                                               WM_CALLBACK* cb, int Flags);
//
ButtonExCtrl_Handle ButtonExCtrl_CreateIndirect(const GUI_WIDGET_CREATE_INFO* pCreateInfo, 
                                                WM_HWIN hWinParent, int x0, int y0, 
                                                WM_CALLBACK* pCallback);
///设置传入参数在全局的唯一ID号
void ButtonExCtrl_SetParameterId(ButtonExCtrl_Handle hObj,int ParameterId);
///获得传入参数在全局的唯一ID号
int ButtonExCtrl_GetParameterId(ButtonExCtrl_Handle hObj);
/***************************************************************************************************
*           ButtonExCtrl_SetButtonExCtrlColor
*   功能：设置控件的颜色信息
*   输入：
*       GUI_COLOR BkColor;      //背景色
*       GUI_COLOR ProColor1;    //焦点不在控件上的颜色
*       GUI_COLOR ProColor2;    //焦点在控件上的颜色
*       GUI_COLOR LabelColor;   //标签的颜色
*       GUI_COLOR LineColor1;   //颜色画圆弧、圆的外边缘
*       GUI_COLOR LineColor2;   //颜色画圆弧、圆的外边缘
*
*/
void ButtonExCtrl_SetButtonExCtrlColor(ButtonExCtrl_Handle hObj,
                                       GUI_COLOR BkColor,GUI_COLOR ProColor1,GUI_COLOR ProColor2,
                                       GUI_COLOR LabelColor,GUI_COLOR LineColor1,GUI_COLOR LineColor2);

/***************************************************************************************************
*           ButtonExCtrl_SetButtonExCtrlColorEx
*   功能：设置控件的颜色信息　 扩展的
*   输入：
*       GUI_COLOR Color;        ///颜色
*       int Index;              ///不同颜色号
*
*/
void ButtonExCtrl_SetButtonExCtrlColorEx(ButtonExCtrl_Handle hObj,GUI_COLOR Color,int Index);

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
                              GUI_COLOR TextColor1,GUI_COLOR TextColor2,
                              char *Text11,char *Text12);

/***************************************************************************************************
*           ButtonExCtrl_SetTextColor
*   功能：设置控件中间区域字体颜色
*   输入：
*       GUI_COLOR TextColor;                            //控件上字体的颜色
*       int Index;                                      //[0,1],
*
*/
void ButtonExCtrl_SetTextColor(ButtonExCtrl_Handle hObj,GUI_COLOR TextColor,int Index);

/***************************************************************************************************
*           ButtonExCtrl_SetFontEx
*   功能：设置控件中间区域字体的大小
*   输入：
*       const GUI_FONT GUI_UNI_PTR *pTextFont;          ///控件上字体的大小     默认的字体GUI_Font_15　'1':为另外一种字体
*       int Index;                                      ///不同字体的字体号
*
*/
void ButtonExCtrl_SetFontEx(ButtonExCtrl_Handle hObj,const GUI_FONT GUI_UNI_PTR *pTextFont,int Index);

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
                                    int Min,int Max,int DefaultSize,int Step,int FloatLen);
/***************************************************************************************************
*           ButtonExCtrl_SetData
*   功能：设置控件的中间显示的内容
*   输入：
*       int Data;           //实际数据
*
*/
void ButtonExCtrl_SetData(ButtonExCtrl_Handle hObj,int Data);

/***************************************************************************************************
*           ButtonExCtrl_SetChangeSign
*   功能：设置控件的数据变化标识
*   输入：
*       char changeSign;            //A:停止增加    S:停止减少　N:即可增加也可减少
*
*/
void ButtonExCtrl_SetChangeSign(ButtonExCtrl_Handle hObj,char changeSign);

/***************************************************************************************************
*           ButtonExCtrl_DataAdd
*   功能：控件的中间显示的内容累加
*   输入：
*
*/
void ButtonExCtrl_DataAdd(ButtonExCtrl_Handle hObj);

/***************************************************************************************************
*           ButtonExCtrl_DataSub
*   功能：控件的中间显示的内容累减
*   输入：
*
*/
void ButtonExCtrl_DataSub(ButtonExCtrl_Handle hObj);

/***************************************************************************************************
*               ButtonExCtrl_SetFocus
*
*       功能：控件获得焦点，即将控件颜色变为 pObj->ProColor2，是选中时的颜色
*       输出：
*/
void ButtonExCtrl_SetFocus(ButtonExCtrl_Handle hObj);

/***************************************************************************************************
*               ButtonExCtrl_KillFocus
*
*       功能：控件失去焦点，即将控件颜色变为 pObj->ProColor1，是create时的颜色
*       输出：
*/
void ButtonExCtrl_KillFocus(ButtonExCtrl_Handle hObj);

/***************************************************************************************************
*               ButtonExCtrl_GetFocus
*
*       功能：Get控件的状态
*       输出：
*           -1      数据无效 error
*           0       控件被点击一次
*           1       控件被点击偶数次 或0次
*/
int ButtonExCtrl_GetFocus(ButtonExCtrl_Handle hObj);

/***************************************************************************************************
*               ButtonExCtrl_GetData
*
*       功能：Get控件的Data
*       输出：
*
*/
int ButtonExCtrl_GetData(ButtonExCtrl_Handle hObj);

/***************************************************************************************************
*               ButtonExCtrl_GetData1
*
*       功能：Get控件的Data1，焦点在控件上时，变化中的数据
*       输出：
*
*/
int ButtonExCtrl_GetData1(ButtonExCtrl_Handle hObj);
void ButtonExCtrl_SetTriangleSign(ButtonExCtrl_Handle hObj,int triangleSign);
void ButtonExCtrl_SetTextSign(ButtonExCtrl_Handle hObj,int textSign);
void ButtonExCtrl_SetStyle(ButtonExCtrl_Handle hObj,int Style);
void ButtonExCtrl_SetEdgeColor(ButtonExCtrl_Handle hObj, GUI_COLOR color);
//
#endif //#if GUI_WINSUPPORT
#endif //#ifndef ButtonExCtrl_H
//

