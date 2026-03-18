//
#ifndef ButtonRotation_H
#define ButtonRotation_H
#include "WM.H"
//
#if GUI_WINSUPPORT
#include "Dialog.h"      /* Req. for Create indirect data structure */
//#include "datatypes.h"
//
typedef WM_HMEM ButtonRotation_Handle;
#define ButtonRotation_H2P(h) (ButtonRotation_Obj*) GUI_ALLOC_h2p(h)
#define ButtonRotation_Invalidate(hObj) WM_InvalidateWindow(hObj)
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
    char* Text2;                              ///显示内容在下方
} ButtonRotationInfo;

///所有的值为整形，与实际的数据相差10的FloatLen次方倍
typedef struct
{
    int Min;                //最小值
    int Max;                //最大值
    int DefaultSize;        //默认值　小的三角形指向的值
    int Step;               //步距
    int FloatLen;           //浮点数据的长度        0:为整数据　1:为浮点数小数点后显示的位数为1
} ButtonRotationDataInfo;
//
typedef struct
{
    WIDGET Widget;
    GUI_POINT CapturePoint;
    ButtonRotationInfo BECInfo;
    ButtonRotationDataInfo BDInfo;
    int Data;                                  ///实际数据
    int Data1;                                 ///焦点在控件上时，变化中的数据
    GUI_COLOR ProColor1;                       ///焦点不在控件上的颜色
    GUI_COLOR TextColor1;                      ///焦点不在控件上字体的颜色
    GUI_COLOR LineColor1;                      ///焦点不在控件上线的颜色
    GUI_COLOR ProColor2;                       ///焦点在控件上的颜色
    GUI_COLOR TextColor2;                      ///焦点在控件上字体的颜色
    GUI_COLOR LineColor2;                      ///焦点在控件上线的颜色
    int triangleSign; //按钮按下标志 1:按下    ///三角形的显示标志　1:显示（默认） 0:不显示
    int textSign;                              ///中间Text显示 1:显示（默认） 0:不显示 
    const GUI_FONT GUI_UNI_PTR *pTextFont1;    ///焦点不在控件上字体的大小     
    char *Text11;                              ///焦点不在控件上下方显示的内容
    const GUI_FONT GUI_UNI_PTR *pTextFont2;    ///焦点在控件上字体的大小       
    char *Text12;                              ///焦点在控件上下方显示的内容
    const GUI_FONT GUI_UNI_PTR *pTextFont;     ///下方显示的内容的字体大小
    char changeSign;                           ///A:停止增加   S:停止减少　N:即可增加也可减少
    int DataInvildSign;                        ///功能关闭(全局)0:正常,1:下限关闭,2:上限关闭 3:ON/OFF
} ButtonRotation_Obj;
//
ButtonRotation_Handle ButtonRotation_Create       (WM_CALLBACK* cb, int Flags,
        int x0, int y0, int xsize, int ysize,int Id);

ButtonRotation_Handle ButtonRotation_CreateAsChild(int x0, int y0, int xsize, int ysize, 
                                                   WM_HWIN hParent,int Id,
                                                   WM_CALLBACK* cb, int Flags);

ButtonRotation_Handle ButtonRotation_CreateIndirect(const GUI_WIDGET_CREATE_INFO* pCreateInfo, 
                                                    WM_HWIN hWinParent, int x0, int y0, 
                                                    WM_CALLBACK* pCallback);

///设置传入参数在全局的唯一ID号
void ButtonRotation_SetParameterId(ButtonRotation_Handle hObj,int ParameterId);

///获得传入参数在全局的唯一ID号
int ButtonRotation_GetParameterId(ButtonRotation_Handle hObj);

void SetChangeSigner(ButtonRotation_Handle hObj, char sign);        //设置控件显示数值是否可以增加

/***************************************************************************************************
*           ButtonRotation_SetBkColor
*   功能：设置控件的颜色信息
*   输入：
*       GUI_COLOR BkColor;      //背景色*
*/
void ButtonRotation_SetBkColor(ButtonRotation_Handle hObj,GUI_COLOR BkColor);

/***************************************************************************************************
*           ButtonRotation_SetColorEx
*   功能：设置控件的颜色信息　 扩展的
*   输入：
*       GUI_COLOR Color;        ///颜色
*       int Index;              ///不同颜色号
*
*/
void ButtonRotation_SetColorEx(ButtonRotation_Handle hObj,GUI_COLOR Color,int Index);

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
                                GUI_COLOR TextColor1,GUI_COLOR TextColor2,char *Text11,char *Text12);
/***************************************************************************************************
*           ButtonRotation_SetTextColor
*   功能：设置控件中间区域字体颜色
*   输入：
*       GUI_COLOR TextColor;                            //控件上字体的颜色
*       int Index;                                      //[0,1],
*
*/
void ButtonRotation_SetTextColor(ButtonRotation_Handle hObj,GUI_COLOR TextColor,int Index);

/***************************************************************************************************
*           ButtonRotation_SetFontEx
*   功能：设置控件中间区域字体的大小
*   输入：
*       const GUI_FONT GUI_UNI_PTR *pTextFont;          ///控件上字体的大小     默认的字体GUI_Font_15　'1':为另外一种字体
*       int Index;                                      ///不同字体的字体号
*
*/
void ButtonRotation_SetFontEx(ButtonRotation_Handle hObj,
                              const GUI_FONT GUI_UNI_PTR *pTextFont,int Index);

/***************************************************************************************************
*           ButtonRotation_SetDataInfo
*   功能：设置控件的数据变化的信息
*   输入：
*       int Min;                //最小值
*       int Max;                //最大值
*       int DefaultSize;        //默认值　小的三角形指向的值
*       int Step;               //步距
*       int FloatLen;           //浮点数据的长度        0:为整数据　1:为浮点数小数点后显示的位数为1
*/
void ButtonRotation_SetDataInfo(ButtonRotation_Handle hObj,
                                int Min,int Max,int DefaultSize,int Step,int FloatLen);

/***************************************************************************************************
*           ButtonRotation_SetData
*   功能：设置控件的中间显示的内容
*   输入：
*       int Data;           //实际数据
*
*/
void ButtonRotation_SetData(ButtonRotation_Handle hObj,int Data);

/***************************************************************************************************
*           ButtonRotation_SetChangeSign
*   功能：设置控件的数据变化标识
*   输入：
*       char changeSign;            //A:停止增加    S:停止减少　N:即可增加也可减少
*
*/
void ButtonRotation_SetChangeSign(ButtonRotation_Handle hObj,char changeSign);

/***************************************************************************************************
*           ButtonRotation_DataAdd
*   功能：控件的中间显示的内容累加
*   输入：
*
*/
unsigned ButtonRotation_DataAdd(ButtonRotation_Handle hObj);

/***************************************************************************************************
*           ButtonRotation_DataAdd_1
*   功能：控件的中间显示的内容累加
*   输入：
*
*/
unsigned ButtonRotation_DataAdd_1(ButtonRotation_Handle hObj);

/***************************************************************************************************
*           ButtonRotation_DataSub
*   功能：控件的中间显示的内容累减
*   输入：
*
*/
unsigned ButtonRotation_DataSub(ButtonRotation_Handle hObj);

/***************************************************************************************************
*           ButtonRotation_DataSub_1
*   功能：控件的中间显示的内容累减
*   输入：
*
*/
unsigned ButtonRotation_DataSub_1(ButtonRotation_Handle hObj);

/***************************************************************************************************
*               ButtonRotation_SetFocus
*
*       功能：控件获得焦点，即将控件颜色变为 pObj->ProColor2，是选中时的颜色
*       输出：
*/
void ButtonRotation_SetFocus(ButtonRotation_Handle hObj);

/***************************************************************************************************
*               ButtonRotation_KillFocus
*
*       功能：控件失去焦点，即将控件颜色变为 pObj->ProColor1，是create时的颜色
*       输出：
*/
void ButtonRotation_KillFocus(ButtonRotation_Handle hObj);

/***************************************************************************************************
*               ButtonRotation_GetFocus
*
*       功能：Get控件的状态
*       输出：
*           -1      数据无效 error
*           0       控件被点击一次
*           1       控件被点击偶数次 或0次
*/
int ButtonRotation_GetFocus(ButtonRotation_Handle hObj);

/***************************************************************************************************
*               ButtonRotation_GetData
*
*       功能：Get控件的Data
*       输出：
*
*/
int ButtonRotation_GetData(ButtonRotation_Handle hObj);

/***************************************************************************************************
*               ButtonRotation_GetData1
*
*       功能：Get控件的Data1，焦点在控件上时，变化中的数据
*       输出：
*
*/
int ButtonRotation_GetData1(ButtonRotation_Handle hObj);

//BOOL  SettingsRelating(int DataId,int data);

/***************************************************************************************************
* @brief  
* @para   
* @retval 
*/
void ButtonRotation_SetMaxMin(ButtonRotation_Handle hObj, int max, int min);

/***************************************************************************************************
* @brief  
* @para   
* @retval 
*/
void ButtonRotation_SetTriangleSign(ButtonRotation_Handle hObj,int triangleSign);

/***************************************************************************************************
* @brief  
* @para   
* @retval 
*/
void ButtonRotation_SetTextSign(ButtonRotation_Handle hObj,int textSign);

/***************************************************************************************************
* @brief  
* @para   
* @retval 
*/
void ButtonRotation_SetText(ButtonRotation_Handle hObj,char* Text1,char* Text2);

/***************************************************************************************************
* @brief  
* @para   
* @retval 
*/
void ButtonRotation_SetDefaultSize(ButtonRotation_Handle hObj,int DefaultSize);

/***************************************************************************************************
* @brief  
* @para   
* @retval 
*/
int ButtonRotation_GetTriangleSign(ButtonRotation_Handle hObj);

/***************************************************************************************************
* @brief  
* @para   
* @retval 
*/
void ButtonRotation_SetData1(ButtonRotation_Handle hObj,int Data);

/***************************************************************************************************
* @brief  
* @para   
* @retval 
*/
void ButtonRotation_SetDataInvildSign(ButtonRotation_Handle hObj,int flg);

#endif  //#if GUI_WINSUPPORT
#endif  //#ifndef ButtonRotation_H


