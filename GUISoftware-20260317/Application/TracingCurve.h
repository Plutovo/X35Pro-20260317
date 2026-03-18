#ifndef TracingCurve_H
#define TracingCurve_H
//
#include "WM.H"
#include <stdlib.h>
#include <string.h>
#include "MainTask.h"
#include "stdtypes.h"
#include "arm_comm.h"

//
#if GUI_WINSUPPORT
#include "Dialog.h"      /* Req. for Create indirect data structure */

#ifndef TRACINGCURVE_DATALEN_DEFAULT
#define  TRACINGCURVE_DATALEN_DEFAULT       456
#endif
#ifndef TRACINGCURVE_CLEARLINE_DEFAULT              ///数据被刷新区域的线数
#define  TRACINGCURVE_CLEARLINE_DEFAULT     8   //  [2/17/2022 yaoyao 将宏定义改为全局变量，便于动态更改大小]
#endif
//int nTracingCurveClearLine = 8;//  [2/17/2022 yaoyao add]数据被刷新区域的线数

#define TREND_DEFAULT_POSITION  (360 - 1)

#define TRACINGCURVE_TREND_DEFAULT_MAX      3000    ///趋势图中数据最大
#define TRACINGCURVE_TREND_DEFAULT_MIN      -2000   ///趋势图中数据最小
#define TracingCurve_H2P(h) (TracingCurve_Obj*) GUI_ALLOC_h2p(h)
#define TracingCurve_InvalidateRect(hWin,pRect) WM_InvalidateRect(hWin,pRect)

typedef WM_HMEM TracingCurve_Handle;

///XY轴数据结构
typedef struct
{
    signed char XSign;                    ///X轴标志
    signed char XFloatSign;               ///X轴浮点标志      只有在0~1之间的数才能用浮点数显示
    int nXFloatLen;                       ///X轴浮点位长度    0为整形，1~4为小数
    float XMin;                           ///X轴最小值
    float XMax;                           ///X轴最大值
    float XStep;                          ///X步距
    GUI_COLOR XColor;                     ///X轴标颜色
    char* XTrendText;                     ///X轴标注，为趋势波形时，显示在原点位置的信息
    char* XText;                          ///X轴标注

    signed char YFloatSign;               ///Y轴浮点标志      只有在0~1之间的数才能用浮点数显示
    int nYFloatLen;                       ///Y轴浮点位长度    0为整形，1~4为小数
    float YMin;                           ///Y轴最小值
    float YMax;                           ///Y轴最大值
    float YStep;                          ///Y步距
    GUI_COLOR YColor;                     ///Y轴标颜色
    char* YText;                          ///Y轴标注

    GUI_COLOR XYColor;                    ///xy轴颜色
    char* XYText;                         ///图右上角的TEXT
    signed char FullScanSign;             ///量程范围:1,2
    
    signed char YSign;                    ///Y轴显示标志  'T'显示  'F'不显示
} TracingCurve_XYAxesData;

///XY轴数据结构
typedef struct
{
    GUI_COLOR BKColor;   ///图形框的背景色
} TracingCurve_DialogInfo;

///坐标内显示的具体信息数据
typedef struct
{
    float XDotStep;     // X轴步距对应的像素点
    int XDot;           // 圆点的X
    int XDotStart;      // X轴最左测的点
    int XDotEnd;        // X轴最右测的点
    float YDotStep;     // Y轴步距对应的像素点
    int YDot;           // 圆点的Y
    int YDotStart;      // Y轴最下的点
    int YDotEnd;        // Y轴最上的点
} TracingCurve_DataInfo;

typedef struct
{
    GUI_COLOR DataColor;
    float Datax;
    float Datay;
} TracingCurve_DataDotInfo;

typedef struct
{
    float Datax;
    float Datay;
	float Datay2;
} TracingCurve_DataDotInfo2;

typedef struct
{
    int Mode;           ///数据表现的模式   0:  曲线
    ///                                     1:  柱形
    char loopSign;      ///环形数据        T：
    ///                                    F：
	char  DoubleFlag;//单波形和双波形的区分
    TracingCurve_DataDotInfo    *DataDotInfo;
} TracingCurve_Data;

typedef struct
{
    char ViewSign;      ///警戒线是否可见    ‘T’：可见   ‘F’：隐藏
    GUI_COLOR color;    ///警戒线的颜色
    float size;         ///警戒线数据大小

} TracingCurve_VigilanceLine;


typedef struct   //存储同一个趋势图中，每个参数不同的数据    20140505
{
    const char *ParaText;           //初始化NULL
    GUI_COLOR ParaColor;            //初始化0
    short *ParaDateBuf_Short;       //初始化NULL
    char  *ParaDateBuf_Char;
}TracingTrend_S;//最大可以容纳3个参数

#ifndef RTC_TIMER_S
#define RTC_TIMER_S

//#ifndef Int8U
//#define Int8U unsigned char
//#define Int16U unsigned short int
//#endif


#endif //RTC_TIMER
typedef struct
{
    WIDGET Widget;
    GUI_POINT CapturePoint;
    TracingCurve_XYAxesData XYAD_INFO;            ///xy轴信息
    TracingCurve_DialogInfo DIALOGINFO;           ///窗口信息
    TracingCurve_DataInfo DATAINFO;               ///xy轴信息对应像素点的值
    TracingCurve_Data Data;                       ///数据buffer
    TracingCurve_VigilanceLine VigilanceSize;     ///警戒值的信息
    GUI_COLOR XYTextColor;                        ///图右上角的TEXT颜色
    int xscale;                                   ///现在点的x值
    int PenSize;                                  ///画线时线的宽度
    GUI_HMEM          Data_hMem;                  ///创建Buffer的句柄
    //int yoldscale;                              ///前一个点的y值
    int nStopRemove;                              ///停止刷新坐标上的数据显示区   0:正常刷新数据（默认值）　1:到数刷新到结尾，将停止刷新数据
    int nLoopsRef;                                ///环的当前一个周期的数据标志位 0：不保存、显示（默认值） 2：LOOPS保存显示一个周期
    int nSPO2Bar;                                 ///波形显示的另外情况，为SPO2时，波形显示区域的右侧显示棒图  0：不显示（默认值） 1：显示
    int nSPO2Count;                               ///SPO2棒图，显示数据大小  [0，16）
    int ParameterId;                              ///参数ID(全局)

    float Data_Max_High; //最高点值 //cpp ac 20130715
    float Data_Max_Low;  //最低点值 
    float RangeX;
    float Data_Max_HighX;
    float Data_Max_LowX;

    int ExceedRangeFlg;
	int yrange;
    int Range;
	char CurveRangUpdate;
	char CurveRefreshCycleIndex;

    //Trend
    const GUI_FONT *TracingFont;
    int nTrendType;                               ///1:趋势    0:正常
    int TrendPixelNum;                            ///趋势图模式时，buffer的大小以及x轴的宽度
    int TrendNumber;                              ///已创建buffer的个数
    int TrendBufferSize;                          ///创建buffer的类型，大小
    unsigned char TrendPaintFlag;                 ///趋势图刷新标志
    TracingTrend_S Trend_Info[3];                 ///趋势图中，每一个参数独有的信息
    unsigned char AA_Flag;    //麻醉气体标志  初始化0   1:主要麻醉气体   2：辅助麻醉气体
    RTC_TIMER TrendStartTime;                     ///本次趋势图开始的时间
    int lastxscan;
    
    int BufferSize;
} TracingCurve_Obj;

///static void 
void _DrawPaintCoordinate(TracingCurve_Obj* pObj);
void _DrawLoopBuffer(TracingCurve_Obj* pObj);
void _DrawVigilanceLine(TracingCurve_Obj* pObj);
TracingCurve_Handle TracingCurve_Create(WM_CALLBACK* cb, int Flags,
        int x0, int y0, int xsize, int ysize,int Id);
TracingCurve_Handle TracingCurve_CreateAsChild(int x0, int y0, int xsize, int ysize, WM_HWIN hParent,int Id,
        WM_CALLBACK* cb, int Flags);
TracingCurve_Handle TracingCurve_CreateIndirect(const GUI_WIDGET_CREATE_INFO* pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK* pCallback);
/***************************************************************************************************
*               TracingCurve_SetXYINFO()
*       功能：设置XY轴的信息
*       输入：
*               float XMin;         //X轴最小值
*               float XMax;         //X轴最大值
*               float XStep;        //X步距
*               char* XText;        //X轴标注
*               GUI_COLOR XColor;   //X轴标颜色
*               float YMin;         //Y轴最小值
*               float YMax;         //Y轴最大值
*               float YStep;        //Y步距
*               char* YText;        //Y轴标注
*               GUI_COLOR YColor;   //Y轴标颜色
*               GUI_COLOR color;    //xy轴颜色
*               char* XYText;       //图右上角的TEXT
*
*/
void TracingCurve_SetXYINFO(TracingCurve_Handle hObj,signed XSign,float XMin,float XMax,float XStep,GUI_COLOR xcolor,char* XText,float YMin,float YMax,float YStep,GUI_COLOR ycolor,char* YText,GUI_COLOR color,char* XYText);
/***************************************************************************************************
*               TracingCurve_SetXYInfo
*
*       功能：设置XY轴的信息部分信息
*       输入：
*           GUI_COLOR XYColor;      //xy轴颜色
*           char* XYText;           //图右上角的TEXT
*/
void TracingCurve_SetXYInfo(TracingCurve_Handle hObj,GUI_COLOR color,char* XYText);
/***************************************************************************************************
*               TracingCurve_SetXInfo()
*
*       功能：设置X轴的信息
*       输入：
*           float XMin;         //X轴最小值
*           float XMax;         //X轴最大值
*           float XStep;        //X步距
*           char* XText;        //X轴标注
*           GUI_COLOR XColor;   //X轴标颜色
*/
void TracingCurve_SetXInfo(TracingCurve_Handle hObj,signed char XSign,signed char xFloatSign,float XMin,float XMax,float XStep,GUI_COLOR xcolor,char* XText);
/***************************************************************************************************
*               TracingCurve_SetYInfo()
*
*       功能：设置Y轴的信息
*       输入：
*           signed char yFloatSign;     //Y轴浮点标志       只有在0~1之间的数才能用浮点数显示
*           float YMin;                 //Y轴最小值
*           float YMax;                 //Y轴最大值
*           float YStep;                //Y步距
*           char* YText;                //Y轴标注
*           GUI_COLOR YColor;           //Y轴标颜色
*/
void TracingCurve_SetYInfo(TracingCurve_Handle hObj,signed char yFloatSign,float YMin,float YMax,float YStep,GUI_COLOR ycolor,char* YText);
/***************************************************************************************************
*               TracingCurve_SetYUpChange()
*
*       功能：改变Y轴正数区和负数据区的最大值、最小值和步距,超出了显示的区域，将坐标变大（Max、Step）,数据太小，负区域坐标大将变小（Max、Step）
*       输入：
*/
void TracingCurve_SetYUpChange(TracingCurve_Handle hObj,float fData);
/***************************************************************************************************
*           TracingCurve_SetYDownChange
*   功能：改变Y轴正数区和负数据区的最大值、最小值和步距,显示在很小的部分中时，将对应的值改变了，使显示的效果更好
*   输入：
*           float fData;    //相对数据
*/
void TracingCurve_SetYDownChange(TracingCurve_Handle hObj,float fData);
/***************************************************************************************************
*               TracingCurve_SetADD2DATA()
*   功能：将数据添加到显示区域和数据buffer区
*   输入：
*       hObj
*       color                   数据在显示区域显示的颜色
*       data1                   数据值   范围是(pObj->XYAD_INFO.YMin,pObj->XYAD_INFO.YMax)
*       data2                   数据值   范围是(pObj->XYAD_INFO.YMin,pObj->XYAD_INFO.YMax)
*
*
*/
void TracingCurve_SetADD2Data(TracingCurve_Handle hObj,GUI_COLOR color,float datax,float datay,int flag, int WaveForm);
/***************************************************************************************************
*               TracingCurve_SetADDDATA()
*   功能：将数据添加到显示区域和数据buffer区
*   输入：
*       hObj
*       color                   数据在显示区域显示的颜色
*       data                    数据值   范围是(pObj->XYAD_INFO.YMin,pObj->XYAD_INFO.YMax)
*   输出：
*       int ret                 当前的pObj->xscale
*/
//int TracingCurve_SetADDData(TracingCurve_Handle hObj,char flag,GUI_COLOR color,float data, int WaveForm);
int TracingCurve_SetADDData(TracingCurve_Handle hObj,char flag,GUI_COLOR color,float data,float data2 ,int WaveForm, char doubleflag);
/***************************************************************************************************
*
*       TracingCurve_Init
*
*       功能：初始化    未完成
*
*/
void TracingCurve_Init(TracingCurve_Handle hObj);
/***************************************************************************************************
*               TracingCurve_SetBKColor
*
*       功能：设置控件背景色
*       输入：
*           GUI_COLOR color;   //背景颜色
*/
void TracingCurve_SetBKColor(TracingCurve_Handle hObj,GUI_COLOR color);
/***************************************************************************************************
*               TracingCurve_SetViewMode
*
*       功能：设置图形表现形式
*       输入：
*           int mode;       //画出表现数据的方式  0:曲线方式  1:柱形方式
*/
void TracingCurve_SetViewMode(TracingCurve_Handle hObj,int mode,char loopSign);
/***************************************************************************************************
*               TracingCurve_SetVigilanceLine
*
*       功能：设置警戒线
*       输入：
*           char viewSign;          //警戒线是否可见    ‘T’：可见   ‘F’：隐藏
*           GUI_COLOR color;        //警戒线的颜色
*           float size;             //警戒线数据大小
*/
void TracingCurve_SetVigilanceLine(TracingCurve_Handle hObj,char viewSign,GUI_COLOR color,float size);
void TracingCurve_SetPenSize(TracingCurve_Handle hObj,int PenSize);
void TracingCurve_SetStopRemove(TracingCurve_Handle hObj,int nStopRemove);
int TracingCurve_GetStopRemove(TracingCurve_Handle hObj);
void TracingCurve_CleanYData(TracingCurve_Handle hObj);
/***************************************************************************************************
*       TracingCurve_SetPaintTrend
*   功能：画趋势图
*   参数：
*       TracingCurve_Handle hObj;       //控件句柄
*       GUI_COLOR color;                //画出的颜色
*       int* data;                      //显示的内容buffer
*/
void TracingCurve_SetPaintTrend(TracingCurve_Handle hObj,GUI_COLOR color,int* data);
///为趋势时，X轴显示的信息  0：默认 1：显示
void TracingCurve_SetTrendType(TracingCurve_Handle hObj,int nTrendType);
///趋势情况下，X轴显示的信息    XTrendText为原点信息，XText为X轴标注信息
void TracingCurve_SetTrendText(TracingCurve_Handle hObj,char* XTrendText,char* XText);
///设置传入参数在全局的唯一ID号
void TracingCurve_SetParameterId(TracingCurve_Handle hObj,int ParameterId);
///获得传入参数在全局的唯一ID号
int TracingCurve_GetParameterId(TracingCurve_Handle hObj);
///设置现在点的x值
void TracingCurve_SetXScale(TracingCurve_Handle hObj,int xscale);
///获得现在点的x值
int TracingCurve_GetXScale(TracingCurve_Handle hObj);
///图右上角的TEXT颜色
void TracingCurve_SetXYTextColor(TracingCurve_Handle hObj,GUI_COLOR XYTextColor);
///设置轴浮点位长度 0为整形，1~4为小数
///Index 为0：设置的为X轴浮点位长度  1：设置的为Y轴浮点位长度
void TracingCurve_SetFloatLen(TracingCurve_Handle hObj,int nFloatLen,int Index);
//设置保存LOOPS历史一个周期的数据
void TracingCurve_SetLoopsRef(TracingCurve_Handle hObj,char bLoopsRef);
//波形显示的另外情况，为SPO2时，波形显示区域的右侧显示棒图  0：不显示（默认值） 1：显示
void TracingCurve_SetSPO2Bar(TracingCurve_Handle hObj,int nSPO2Bar);
///SPO2棒图，显示数据大小  [0，16）
void TracingCurve_SetSPO2Count(TracingCurve_Handle hObj,int nSPO2Count);
void TracingCurve_SetExceedRangeFlg(TracingCurve_Handle hObj,int flag);
void TracingCurve_SetRange(TracingCurve_Handle hObj,int times);
void TracingCurve_SetRangeX(TracingCurve_Handle hObj,float times);
int TracingCurve_GetRange(TracingCurve_Handle hObj);
int TraceingCurve_NeedChangeRange(TracingCurve_Handle hObj,char flag, int WaveForm);
#ifdef WAVE_PAW_ADD_ALARMHIGH
int GetPawAlarmRange(void);
#endif

void TracingCurve_SetYsign(TracingCurve_Handle hObj,signed char flg); //  20130516

/***********************************************************************
*函数名：TracingTrend_SetInfo
*功能：将创建好的波形图，设置为Trend类型
*参数：hObj:波形图的句柄
       TimeSign:时间轴是否显示标志（T：显示，F:不显示）
       TimeRange:时间轴范围，单位为小时（当前趋势图）
       YMin:Y轴最小值
       YMax:Y轴最大值
       nYFloatLen:Y轴显示数据浮点位长度，0为整数，1~4为小数的位数
       PixelNum:X轴（时间轴）对应的像素点数（也是开辟数据buff的大小）
       ParaText0:第一个参数的显示字符
       ParaColor0：第一个参数字符和波形的颜色
*
************************************************************************
*/
void TracingTrend_SetInfo(TracingCurve_Handle hObj,signed TimeSign,float TimeRange,float YMin,float YMax,int nYFloatLen,int BufferSize ,int PixelNum,const char* ParaText0,GUI_COLOR ParaColor0);
RTC_TIMER Trend_GetTime(TracingCurve_Obj* pObj);

/***********************************************************************
*函数名：TracingTrend_SetRange
*功能：改变某个趋势的量程范围,并且刷新显示
*参数：hObj:波形图的句柄
       TimeRange:时间轴范围，单位为小时（当前趋势图）
*备注：如果为小数数据，则数据的最大值和Buffer数据输入值应该为扩大10倍的数
*
************************************************************************
*/
void TracingTrend_SetRange(TracingCurve_Handle hObj,float TimeRange);
/***********************************************************************
*函数名：TracingTrend_GetRange
*功能：获取某个趋势图的时间范围
*参数：hObj:波形图的句柄
*备注：
*
************************************************************************
*/
int TracingTrend_GetRange(TracingCurve_Handle hObj);


int TracingTrend_FullBuffer(TracingCurve_Handle hObj,int index,void * buffer);
void TracingTrend_SetTime(TracingCurve_Obj *pObj,RTC_TIMER *RTCTime);
int TracingTrend_GetNumber(TracingCurve_Handle hObj);
int TracingTrend_AddPara(TracingCurve_Handle hObj,int index,const char* ParaText0,GUI_COLOR ParaColor0);
void TracingTrend_SetInfo(TracingCurve_Handle hObj,signed TimeSign,float TimeRange,float YMin,float YMax,int nYFloatLen,int BufferSize, int PixelNum,const char* ParaText0,GUI_COLOR ParaColor0);
void TracingTrend_SetRange(TracingCurve_Handle hObj,float TimeRange);
int TracingTrend_GetRange(TracingCurve_Handle hObj);
void TracingTrend_PaintFlag(TracingCurve_Handle hObj,unsigned char Flag);
void TracingTrend_CursorMove(TracingCurve_Handle hObj,int flag,int position);
int TracingTrend_FreeBffer(TracingCurve_Handle hObj);

int TracingCurve_AllocDispBuffer(TracingCurve_Handle hObj,int BufferSize);
void TracingTrend_SetAAFlag(TracingCurve_Handle hObj,int flag);
#endif //GUI_WINSUPPORT
#endif //TracingCurve_H