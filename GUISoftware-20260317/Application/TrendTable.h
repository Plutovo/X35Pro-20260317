#ifndef _TRENDTABLE_H_
#define _TRENDTABLE_H_
#include "WM.h"
#include "DIALOG_Intern.h"      /* Req. for Create indirect data structure */
#include "WIDGET.h"      /* Req. for Create indirect data structure */
#include "GUIDebug.h" /* Req. for GUI_DEBUG_LEVEL */
#include "Static_Menu_Creat.h"
 #include "queue_manul.h"
#ifndef NULL
#define NULL 0
#endif

/********************************************************
*               程序移植配置区
*********************************************************
*/
#define TREND_GRAPH_ON   1    //趋势图开关
#define TREND_TABLE_ON   0    //趋势表开关
#define TREND_INFO_24H   1    //趋势的时间范围，24小时
#define TREND_INFO_72H   0    //趋势的时间范围，72小时
//#define TREND_PARA_NUM      (17) //趋势图参数的个数

//参数索引值,根据保存参数所用的字节数区分为单字节和双字节，单字节在前，双字节在后，索引值从0开始
#ifdef SPO2_SWITCH//add ylk 20171031 X6定制
//8U 
#define INDEX_START_8U  PPEAK //单字节参数起始索引值
#define PPEAK    0
#define PMEAN    1
#define PEEP     2
#define FTOTAL   3
#define FSPN     4
#define FIO2     5
#define ETCO2    6

#define FICO2    7  //ylk add 20181121
#define ETN2O    8
#define FIN2O    9

#define SPO2     10
#define PULSE    11
//16U 
#define INDEX_START_16U  VTE //双字节参数起始索引值
#define VTE      12
#define VTI      13
#define RST      14
#define CDYN     15
#define MV       16
#define MVSPN    17

#define ETAA1    18   //最高4位为标志位，共有5种麻醉气体 //ylk add 20181121
#define FIAA1    19   //
#define ETAA2    20
#define FIAA2    21
#define MAC_     22
#else 

//8U 
#define INDEX_START_8U  PPEAK //单字节参数起始索引值
#define PPEAK    0
#define PMEAN    1
#define PEEP     2
#define FTOTAL   3
#define FSPN     4
#define FIO2     5
#define ETCO2    6

#define FICO2    7
#define ETN2O    8
#define FIN2O    9

//16U 
#define INDEX_START_16U  VTE //双字节参数起始索引值
#define VTE      10
#define VTI      11
#define RST      12
#define CDYN     13
#define MV       14
#define MVSPN    15

#define ETAA1    16   //最高4位为标志位，共有5种麻醉气体
#define FIAA1    17   //
#define ETAA2    18
#define FIAA2    19
#define MAC_     20

// #if (MACHINE_TYPE == X45 || MACHINE_TYPE == X45A || MACHINE_TYPE == X45C)
// #define SPO2     21
// #define PULSE    22
// #endif

#endif

//BDU发送的检测值为实际值的10倍的参数集合
#define PARA_10M_NUM   5         //Multiple
extern unsigned char ParaNameIndex[TREND_PARA_NUM];
 extern unsigned short ParaMaxLimit[TREND_PARA_NUM];
extern unsigned short ParaMinLimit[TREND_PARA_NUM];


typedef WM_HMEM TrendTable_Handle;
//重绘函数的标志变量
enum TrendTableFlag {Paint_All,Paint_AllData,Paint_Row,Paint_Column,Paint_Time};


typedef struct
{
    WIDGET Widget;//GUI结构体
    //项目总共的行数（页码总数*每页的行数）
    unsigned short ItemNumber;
    //行列信息
    unsigned char RowNumber;    //行数
    unsigned char ColumnNumber;   //列数,包含时间轴
    //每一列参数，显示的小数位数
    unsigned char *ParaDispDecimal;  //在创建函数中动态分配地址，在回调函数Delete中释放
    //显示buffer
    short **TrendTable_Buffer;//二维数组：[列][行]
    //当前选中的位置（行数）
    unsigned char RowPosition;      //当前选中的行号
    unsigned char Per_RowPosition;  //上一次选中的行号
    unsigned char Page_Position;    //当前的页码
    unsigned char Per_Page_Position;//上一次的页码
    //当前要修改的列
    unsigned char ColumnPosition;
	//外形大小
	unsigned char Width; //宽度
    unsigned char Heigth;
    //间距
    unsigned char Interval;
	//颜色
	GUI_COLOR Table_BkColor;//默认背景颜色
	GUI_COLOR Table_Pressed_BkColor;//选中的背景颜色
	GUI_COLOR Font_Color;//未选中的字体颜色
	GUI_COLOR Font_Pressed_Color;//选中的字体颜色
    GUI_COLOR Frame_Color;//边框颜色 
    GUI_COLOR Time_Color;//时间显示颜色

	GUI_FONT Table_Font;//字体

    //趋势表的时间当前时间
    RTC_TIMER TrendTable_Time;
    //趋势表时间量程
    unsigned char TimeRange;//当前量程
    //趋势表两行之间的时间间隔  单位s
    unsigned short TimeInterval;//时间间隔 
    //时间轴位置
    GUI_RECT TimeRect;
    //本次重绘标志
    enum TrendTableFlag PaintFlag;  //默认为全部重绘（边框、时间和全部数据），可选重绘标志有：行重绘，列重绘
} TrendTable_Obj;

#define TrendTable_H2P(h) (TrendTable_Obj*) GUI_ALLOC_h2p(h)
#define TrendTable_Invalidate(hObj) WM_InvalidateWindow(hObj)









TrendTable_Handle TrendTable_CreateIndirect(const GUI_WIDGET_CREATE_INFO* pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK* cb);
TrendTable_Handle TrendTable_CreateAsChild(int x0, int y0, int xsize, int ysize, WM_HWIN hParent,int Id,WM_CALLBACK* cb, int Flags);



/*
*TrendTable_SetInfo
*设置趋势表的配置信息
*/
void TrendTable_SetInfo(TrendTable_Handle hObj,int RowNumber,int ColumnNumber,int RowPosition,int ItemNumber,RTC_TIMER Time,int TimeRange);

/*
*TrendTable_SetPagePosition
*设置当前的页数，用于计算每一行对应的时间点，还有当前选中的行在标尺上的位置。
*/
void TrendTable_SetPagePosition(TrendTable_Handle hObj,int Page_Position,int RowPosition);

/*
*TrendTable_SetSel
*设置当前页面上，被选中的项目所在的行数。
*/
void TrendTable_SetSel(TrendTable_Handle hObj,unsigned char RowPosition);

/*
*TrendTable_GetSel
*设置当前页面上，被选中的项目所在的行数。
*/
unsigned char TrendTable_GetSel(TrendTable_Handle hObj);

/*
*TrendTable_IncSel
*选中下一个项目
*return 0:当前页面移动   1:切换到下一页
*/
int TrendTable_IncSel(TrendTable_Handle hObj);

/*
*TrendTable_IncSel
*选中上一个项目
*return 0:当前页面移动   1:切换到上一页
*/
int TrendTable_DecSel(TrendTable_Handle hObj);

/*
*TrendTable_SetColumn
*设置想要更改参数的一列列号
*/
void TrendTable_SetColumn(TrendTable_Handle hObj,unsigned char Column);



#endif  //_TRENDTABLE_H_
