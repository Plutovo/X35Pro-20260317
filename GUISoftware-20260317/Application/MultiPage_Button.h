#ifndef MultiPage_Button_H
#define MultiPage_Button_H
#include "WM.H"
#if GUI_WINSUPPORT
#include "Dialog.h"      /* Req. for Create indirect data structure */
//#include "includeAll.h"
typedef WM_HMEM MultiPage_Button_Handle;
typedef struct
{
    GUI_COLOR BkColor;                              ///背景色
    GUI_COLOR ProColor;                             ///前景色
    GUI_COLOR ProLineColor;                         ///前景色　四周线
    GUI_COLOR TextColor;                            ///字体颜色
    const GUI_FONT GUI_UNI_PTR *pTextFont;          ///字体大小  默认的字体GUI_Font13_1
    char* Text;                                     ///显示内容
} MultiPage_ButtonInfo;
MultiPage_Button_Handle MultiPage_Button_Create       (WM_CALLBACK* cb, int Flags,
        int x0, int y0, int xsize, int ysize,int Id);
MultiPage_Button_Handle MultiPage_Button_CreateAsChild(int x0, int y0, int xsize, int ysize, WM_HWIN hParent,int Id,
        WM_CALLBACK* cb, int Flags);
MultiPage_Button_Handle MultiPage_Button_CreateIndirect(const GUI_WIDGET_CREATE_INFO* pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK* pCallback);
void MultiPage_Button_SetMode(MultiPage_Button_Handle hObj,int mode);
void MultiPage_Button_SetText(MultiPage_Button_Handle hObj,char* Text);
void MultiPage_Button_SetTextColor(MultiPage_Button_Handle hObj,GUI_COLOR TextColor);
///设置字体大小
void MultiPage_Button_SetFont(MultiPage_Button_Handle hObj,const GUI_FONT GUI_UNI_PTR *pTextFont);
/***************************************************************************************************
*               MultiPage_Button_SetProColor
*
*       功能：设置控件前景色
*       输入：
*           GUI_COLOR ProColor; //前景色
*/
void MultiPage_Button_SetProColor(MultiPage_Button_Handle hObj,GUI_COLOR ProColor);
/***************************************************************************************************
*               MultiPage_Button_SetColor
*
*       功能：设置控件相关参数颜色
*       输入：
*           GUI_COLOR BkColor;      //背景颜色
*           GUI_COLOR ProColor1;    //前景色，无焦点时
*           GUI_COLOR ProColor2;    //前景色，有焦点时
*           ProColor2,GUI_COLOR ProLineColor;       //有焦点时，四周线的颜色
*           GUI_COLOR TextColor;    //字体颜色
*/
void MultiPage_Button_SetColor(MultiPage_Button_Handle hObj,GUI_COLOR BkColor,GUI_COLOR ProColor1,GUI_COLOR ProColor2,GUI_COLOR ProLineColor,GUI_COLOR TextColor);
/***************************************************************************************************
*               MultiPage_Button_KillFocus
*
*       功能：控件失去焦点，即将控件颜色变为 pObj->ProColor1，是create时的颜色
*       输出：
*/
void MultiPage_Button_KillFocus(MultiPage_Button_Handle hObj);
/***************************************************************************************************
*               MultiPage_Button_SetFocus
*
*       功能：控件获得焦点，即将控件颜色变为 pObj->ProColor2，是选中时的颜色
*       输出：
*/
void MultiPage_Button_SetFocus(MultiPage_Button_Handle hObj);
/***************************************************************************************************
*               MultiPage_Button_GetFocus
*
*       功能：Get控件的状态
*       输出：
*           -1      数据无效 error
*           0       控件被点击一次
*           1       控件被点击偶数次 或0次
*           2       回车选中
*/
int MultiPage_Button_GetFocus(MultiPage_Button_Handle hObj);
//响应回车键消息的相关接口
//控件获得焦点,状态与回车键选中时一样，不响应回车键时，将与MultiPage_Button_SetFocus功能一样
void MultiPage_Button_SetEnterFocus(MultiPage_Button_Handle hObj);
//回车键对应控件的状态
void MultiPage_Button_SetKeyEnterState(MultiPage_Button_Handle hObj,GUI_COLOR color);
//确定是否响应回车键消息    0:不响应    1:响应
void MultiPage_Button_SetKeyEnterSign(MultiPage_Button_Handle hObj,int KeySign);
//使控件无效，当显示，显示颜色为灰色，并且不能操作　0:有效（默认）　1:无效
void MultiPage_Button_SetInvalidate(MultiPage_Button_Handle hObj,int nInvalidate);
#endif
#endif
