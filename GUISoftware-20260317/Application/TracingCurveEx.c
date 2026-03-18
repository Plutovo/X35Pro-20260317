
/***************************************************************************************************
标题:  TracingCurveEx.c
功能:  波形显示
说明:  能实现曲线、时序等的显示，能对坐标、标尺等属性进行配置

当前版本： 0.1.0
修改记录：
2007.11.27   yanglin, created
***************************************************************************************************/
#include "GUI_Protected.h"
#include "TracingCurve.h"
#include "Static_Menu_Creat.h"
#if GUI_WINSUPPORT
#define TracingCurve_Invalidate(hObj) WM_InvalidateWindow(hObj)
static char bLoopsSave = 0;                 ///保存LOOPS数据标志位  0：不保存 1：保存
extern void _DrawLoopOldBuffer(TracingCurve_Obj* pObj);
extern RTC_TIMER RTCTime;
float MultiArray[4] = {5, 20, 20,1};  // Paw,Flow,Volume,CO2
float MultiAA[2] = {1, 1};
int PawRangeFlag[8] = {1,2,3,4,5,10,15,20};
int Trend_Xsacn = TREND_DEFAULT_POSITION;  //趋势图中，光标的位置
//Masimo波形buffer,在SpO2选配的地方创建。
signed char *Masimo_Buffer;
extern unsigned char SpO2_Choose;//1：国产   2：Masimo
extern ALARM_SET_PARA AlarmSettingPara;
extern CO2_S CO2_Disp;//  [2/15/2022 yaoyao add]
extern int nTracingCurveClearLine;//  [2/17/2022 yaoyao add]数据被刷新区域的线数

/***************************************************************************************************
*
*       TracingCurve_Init
*
*       功能：初始化    未完成
*
*/
void TracingCurve_Init(TracingCurve_Handle hObj)
{
    if (hObj)
    {
        //TracingCurve_Obj* pObj;
        WM_LOCK();
        //  add ...
        WM_UNLOCK();
    }
}
/***************************************************************************************************
*               TracingCurve_SetVigilanceLine
*
*       功能：设置警戒线
*       输入：
*           char viewSign;          //警戒线是否可见    ‘T’：可见   ‘F’：隐藏
*           GUI_COLOR color;        //警戒线的颜色
*           float size;             //警戒线数据大小
*/
void TracingCurve_SetVigilanceLine(TracingCurve_Handle hObj,char viewSign,GUI_COLOR color,float size)
{
    if (hObj)
    {
        TracingCurve_Obj* pObj;
        WM_LOCK();
        pObj = TracingCurve_H2P(hObj);
        pObj->VigilanceSize.ViewSign = viewSign;
        pObj->VigilanceSize.color = color;
        pObj->VigilanceSize.size = size;
        TracingCurve_Invalidate(hObj);
        WM_UNLOCK();
    }
}
/***************************************************************************************************
*               TracingCurve_SetBKColor
*
*       功能：设置控件背景色
*       输入：
*           GUI_COLOR color;   //背景颜色
*/
void TracingCurve_SetBKColor(TracingCurve_Handle hObj,GUI_COLOR color)
{
    if (hObj)
    {
        TracingCurve_Obj* pObj;
        WM_LOCK();
        pObj = TracingCurve_H2P(hObj);
        pObj->DIALOGINFO.BKColor = color;
        TracingCurve_Invalidate(hObj);
        WM_UNLOCK();
    }
}
/***************************************************************************************************
*               TracingCurve_SetYsign
*
*       功能： 设置Y轴显示标志 
*       输入：
*              flg  'T' 显示  'F' 不显示 
*/
void TracingCurve_SetYsign(TracingCurve_Handle hObj,signed char flg)
{
    if (hObj)
    {
        TracingCurve_Obj* pObj;
        WM_LOCK();
        pObj = TracingCurve_H2P(hObj);
        pObj->XYAD_INFO.YSign = flg;
        TracingCurve_Invalidate(hObj);
        WM_UNLOCK();
    }
}

void TracingCurve_SetRangUpdateFlag(TracingCurve_Handle hObj,signed char flg)
{
	if (hObj)
	{
		TracingCurve_Obj* pObj;
		WM_LOCK();
		pObj = TracingCurve_H2P(hObj);
		pObj->CurveRangUpdate = flg;
		// 		TracingCurve_Invalidate(hObj);
		WM_UNLOCK();
	}
}

/***************************************************************************************************
*               TracingCurve_SetXInfo()
*
*       功能：设置X轴的信息
*       输入：
*           signed char XSign;          //X轴显示标志，不显示则为X轴及坐标也不显示
*           signed char xFloatSign;     //X轴浮点标志       只有在0~1之间的数才能用浮点数显示
*           float XMin;                 //X轴最小值
*           float XMax;                 //X轴最大值
*           float XStep;                //X步距
*           char* XText;                //X轴标注
*           GUI_COLOR XColor;           //X轴标颜色
*/
void TracingCurve_SetXInfo(TracingCurve_Handle hObj,signed char XSign,signed char xFloatSign,float XMin,float XMax,float XStep,GUI_COLOR xcolor,char* XText)
{
    if (hObj)
    {
        TracingCurve_Obj* pObj;
        WM_LOCK();
        pObj = TracingCurve_H2P(hObj);
        pObj->XYAD_INFO.XSign = XSign;
        if(XSign=='T')
        {
            pObj->XYAD_INFO.XFloatSign = xFloatSign;
            pObj->XYAD_INFO.XMax = XMax;
            pObj->XYAD_INFO.XMin = XMin;
            pObj->XYAD_INFO.XStep = XStep;
            pObj->XYAD_INFO.XColor = xcolor;
            pObj->XYAD_INFO.XText = XText;
        }
        TracingCurve_Invalidate(hObj);
        WM_UNLOCK();
    }
}
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
void TracingCurve_SetYInfo(TracingCurve_Handle hObj,signed char yFloatSign,float YMin,float YMax,float YStep,GUI_COLOR ycolor,char* YText)
{
    int Index = 0;
    if (hObj)
    {
        TracingCurve_Obj* pObj;
        WM_LOCK();
        pObj = TracingCurve_H2P(hObj);
        pObj->XYAD_INFO.YFloatSign = yFloatSign;
        pObj->XYAD_INFO.YMax = YMax;
        //处理最小值与步距的逻辑关系
        if(0!=YMin)
        {
            if(YMin>0)
            {
                YMin = 0;
            }
            else if(YMin<0)
            {
                Index = (int)YMin/YStep;
                if(0 == Index)
                {
                    YStep = 0-YMin;
                }
                else
                {
                    YStep = YMin/Index;
                }
            }
        }
        pObj->XYAD_INFO.YStep = YStep;
        pObj->XYAD_INFO.YMin = YMin;
        pObj->XYAD_INFO.YColor = ycolor;
        pObj->XYAD_INFO.YText = YText;
        TracingCurve_Invalidate(hObj);
        WM_UNLOCK();
    }
}
/***************************************************************************************************
*               TracingCurve_SetYUpChange()
*
*       功能：改变Y轴正数区和负数据区的最大值、最小值和步距,超出了显示的区域，将坐标变大（Max、Step）,
*             数据太小，负区域坐标大将变小（Max、Step）
*       输入：
*           float fData;    //相对数据
*/
void TracingCurve_SetYUpChange(TracingCurve_Handle hObj,float fData)
{
    float YMin,YMax,YStep;
    if (hObj)
    {
        TracingCurve_Obj* pObj;
        WM_LOCK();
        pObj = TracingCurve_H2P(hObj);
        YMin = pObj->XYAD_INFO.YMin;
        YMax = pObj->XYAD_INFO.YMax;
        YStep = pObj->XYAD_INFO.YStep;
        //实际的数据超出了显示区域的正区域
        if(fData>0)
        {
            if(fData >= (YMax-YStep*2))
            {
                YMax = YMax+YStep*2;
                if((YMax-YMin)/YStep>=16)
                {
                    YStep = YStep*2;
                    YMax = YStep*(int)(YMax/YStep);
                    YMin = YStep*(int)(YMin/YStep);
                }
                pObj->XYAD_INFO.YMin = YMin;
                pObj->XYAD_INFO.YStep = YStep;
                pObj->XYAD_INFO.YMax = YMax;
                TracingCurve_Invalidate(hObj);
            }
        }
        //实际的数据超出了显示区域的负区域
        else
        {
            if(fData<YMin)
            {
                YMin = YMin-YStep*2;
                if((YMax-YMin)/YStep>=16)
                {
                    YStep = YStep*2;
                    YMin = YStep*(int)(YMin/YStep);
                    YMax = YStep*(int)(YMax/YStep);
                }
                pObj->XYAD_INFO.YMin = YMin;
                pObj->XYAD_INFO.YStep = YStep;
                pObj->XYAD_INFO.YMax = YMax;
                TracingCurve_Invalidate(hObj);
            }
        }
        WM_UNLOCK();
    }
}
/***************************************************************************************************
*           TracingCurve_SetYDownChange
*   功能：改变Y轴正数区和负数据区的最大值、最小值和步距,显示在很小的部分中时，将对应的值改变了，
*          使显示的效果更好
*   输入：
*           float fData;    //相对数据
*/
void TracingCurve_SetYDownChange(TracingCurve_Handle hObj,float fData)
{
    float YMin,YMax,YStep;
    if (hObj)
    {
        TracingCurve_Obj* pObj;
        WM_LOCK();
        pObj = TracingCurve_H2P(hObj);
        YMin = pObj->XYAD_INFO.YMin;
        YMax = pObj->XYAD_INFO.YMax;
        YStep = pObj->XYAD_INFO.YStep;
        //实际的数据在显示区域的正区域
        if(fData>0)
        {
            if(fData<(YMax-YStep*3))
            {
                YMax = YMax-YStep*2;
                pObj->XYAD_INFO.YMax = YMax;
                TracingCurve_Invalidate(hObj);
            }
        }
        //实际的数据在显示区域的负区域
        else
        {
            if(fData>YMin+YStep*3)
            {
                YMin = YMin+YStep*2;
                pObj->XYAD_INFO.YMin = YMin;
                TracingCurve_Invalidate(hObj);
            }
        }
        WM_UNLOCK();
    }
}
/***************************************************************************************************
*               TracingCurve_SetXYINFO()
*       功能：设置XY轴的信息
*       输入：
*               signed char XSign; //X轴显示标志，不显示则为X轴及坐标也不显示
*
*               float XMin;       //X轴最小值
*               float XMax;       //X轴最大值
*               float XStep;      //X步距
*               char* XText;      //X轴标注
*               GUI_COLOR XColor; //X轴标颜色
*
*               float YMin;       //Y轴最小值
*               float YMax;       //Y轴最大值
*               float YStep;      //Y步距
*               char* YText;      //Y轴标注
*               GUI_COLOR YColor; //Y轴标颜色
*
*               GUI_COLOR color; //xy轴颜色
*               char* XYText;    //图右上角的TEXT
*/
void TracingCurve_SetXYINFO(TracingCurve_Handle hObj,signed XSign,float XMin,float XMax,float XStep,GUI_COLOR xcolor,char* XText,
							float YMin,float YMax,float YStep,GUI_COLOR ycolor,char* YText,GUI_COLOR color,char* XYText)
{
    if (hObj)
    {
        TracingCurve_Obj* pObj;
        WM_LOCK();
        pObj = TracingCurve_H2P(hObj);
        pObj->XYAD_INFO.XSign = XSign;
        if(XSign=='T')
        {
            pObj->XYAD_INFO.XMax = XMax;
            pObj->XYAD_INFO.XMin = XMin;
            pObj->XYAD_INFO.XStep = XStep;
            pObj->XYAD_INFO.XColor = xcolor;
            pObj->XYAD_INFO.XText = XText;
        }
        pObj->XYAD_INFO.YMax = YMax;
        pObj->XYAD_INFO.YMin = YMin;
        pObj->XYAD_INFO.YStep = YStep;
        pObj->XYAD_INFO.YColor = ycolor;
        pObj->XYAD_INFO.YText = YText;
        pObj->XYAD_INFO.XYColor = color;
        
        pObj->XYAD_INFO.XYText = XYText;
		//  [2/17/2022 yaoyao add ]
		nTracingCurveClearLine = (XMax - XMin)/XStep;
		//
        TracingCurve_Invalidate(hObj);
        WM_UNLOCK();
    }
}

//x的n次方      *result初始值为１
static void square(int x, int n, int *result)
{
    if (n > 0)
    {
        (*result) *= x;
        square(x, --n, &(*result));
    }
}
/***************************************************************************************************
*       TracingCurve_SetPaintTrend
*   功能：画趋势图
*   参数：
*       TracingCurve_Handle hObj;       //控件句柄
*       GUI_COLOR color;                //画出的颜色
*       int* data;                      //显示的内容buffer
*/
void TracingCurve_SetPaintTrend(TracingCurve_Handle hObj,GUI_COLOR color,int* data)
{
    float fData = 0;
    int result = 1;
    unsigned int count = 0;
    unsigned int i;
    if (hObj)
    {
        TracingCurve_Obj* pObj;
        WM_LOCK();
        pObj = TracingCurve_H2P(hObj);
        count = (pObj->DATAINFO.XDotEnd)-(pObj->DATAINFO.XDot);
        if(data==NULL)                      ///add by yanglin at 2008-2-28
        {
            WM_UNLOCK();
            return;
        }
        for(i=0; i<count; i++)
        {
            if((data[i]<TRACINGCURVE_TREND_DEFAULT_MAX)&&(data[i]>TRACINGCURVE_TREND_DEFAULT_MIN))
            {
                fData = data[i]/result;
            }
            else
            {
                fData = 0;
            }
            //超出了显示的区域，将坐标变大
            //if(fData>pObj->XYAD_INFO.YMax)
            //TracingCurve_SetYInfo(hObj,pObj->XYAD_INFO.YFloatSign,pObj->XYAD_INFO.YMin,pObj->XYAD_INFO.YMax+(pObj->XYAD_INFO.YStep)*2,pObj->XYAD_INFO.YStep,pObj->XYAD_INFO.YColor,pObj->XYAD_INFO.YText);
            if((count-i)>=0)
            {
                (pObj->Data.DataDotInfo+count - i)->Datay = fData;
                (pObj->Data.DataDotInfo+count - i)->DataColor = color;
            }
        }
        TracingCurve_Invalidate(hObj);
        WM_UNLOCK();
    }
}
///为趋势时，X轴显示的信息  0：默认 1：显示
void TracingCurve_SetTrendType(TracingCurve_Handle hObj,int nTrendType)
{
    if (hObj)
    {
        TracingCurve_Obj* pObj;
        WM_LOCK();
        pObj = TracingCurve_H2P(hObj);
        pObj->nTrendType = nTrendType;
        TracingCurve_Invalidate(hObj);
        WM_UNLOCK();
    }
}
///趋势情况下，X轴显示的信息    XTrendText为原点信息，XText为X轴标注信息
void TracingCurve_SetTrendText(TracingCurve_Handle hObj,char* XTrendText,char* XText)
{
    if (hObj)
    {
        TracingCurve_Obj* pObj;
        WM_LOCK();
        pObj = TracingCurve_H2P(hObj);
        pObj->XYAD_INFO.XTrendText  = XTrendText;
        pObj->XYAD_INFO.XText       = XText;
        TracingCurve_Invalidate(hObj);
        WM_UNLOCK();
    }
}
void TracingCurve_SetPenSize(TracingCurve_Handle hObj,int PenSize)
{
    if (hObj)
    {
        TracingCurve_Obj* pObj;
        WM_LOCK();
        pObj = TracingCurve_H2P(hObj);
        pObj->PenSize = PenSize;
        TracingCurve_Invalidate(hObj);
        WM_UNLOCK();
    }
}
///图右上角的TEXT颜色
void TracingCurve_SetXYTextColor(TracingCurve_Handle hObj,GUI_COLOR XYTextColor)
{
    if (hObj)
    {
        TracingCurve_Obj* pObj;
        WM_LOCK();
        pObj = TracingCurve_H2P(hObj);
        pObj->XYTextColor = XYTextColor;
        TracingCurve_Invalidate(hObj);
        WM_UNLOCK();
    }
}


///图左上Y轴单位的TEXT内容 //  20130415
void TracingCurve_SetYText(TracingCurve_Handle hObj,char* YText)
{
    if (hObj)
    {
        TracingCurve_Obj* pObj;
        WM_LOCK();
        pObj = TracingCurve_H2P(hObj);
        pObj->XYAD_INFO.YText = YText;
        TracingCurve_Invalidate(hObj);
        WM_UNLOCK();
    }
}

void TracingCurve_SetStopRemove(TracingCurve_Handle hObj,int nStopRemove)
{
    if (hObj)
    {
        TracingCurve_Obj* pObj;
        WM_LOCK();
        pObj = TracingCurve_H2P(hObj);
        pObj->nStopRemove = nStopRemove;
        //TracingCurve_Invalidate(hObj);
        WM_UNLOCK();
    }
}
int TracingCurve_GetStopRemove(TracingCurve_Handle hObj)
{
    int nStopRemove;
    if (hObj)
    {
        TracingCurve_Obj* pObj;
        WM_LOCK();
        pObj = TracingCurve_H2P(hObj);
        nStopRemove = pObj->nStopRemove;
        WM_UNLOCK();
    }
    return nStopRemove;
}
///设置轴浮点位长度 0为整形，1~4为小数
///Index 为0：设置的为X轴浮点位长度  1：设置的为Y轴浮点位长度
void TracingCurve_SetFloatLen(TracingCurve_Handle hObj,int nFloatLen,int Index)
{
    if (hObj)
    {
        TracingCurve_Obj* pObj;
        WM_LOCK();
        pObj = TracingCurve_H2P(hObj);
        switch(Index)
        {
        case 0:
            pObj->XYAD_INFO.nXFloatLen = nFloatLen;
            break;
        case 1:
            pObj->XYAD_INFO.nYFloatLen = nFloatLen;
            break;
        default:
            break;
        }
        //TracingCurve_Invalidate(hObj);
        WM_UNLOCK();
    }
}
void TracingCurve_CleanYData(TracingCurve_Handle hObj)
{
    if (hObj)
    {
        TracingCurve_Obj* pObj;
        WM_LOCK();
        pObj = TracingCurve_H2P(hObj);
        memset(pObj->Data.DataDotInfo,0xff,sizeof(pObj->Data.DataDotInfo));     //????
        if(pObj->Data.loopSign=='T')
        {
            pObj->xscale = 0;                           //
            (pObj->Data.DataDotInfo+pObj->xscale)->Datax = 0;
        }
        if(pObj->xscale>0)
        {
            (pObj->Data.DataDotInfo+pObj->xscale-1)->Datay = 0;
            (pObj->Data.DataDotInfo+pObj->xscale-1)->DataColor = pObj->DIALOGINFO.BKColor;
        }
        (pObj->Data.DataDotInfo+pObj->xscale)->Datay = 0;
        (pObj->Data.DataDotInfo+pObj->xscale)->DataColor = pObj->DIALOGINFO.BKColor;
        TracingCurve_Invalidate(hObj);
        WM_UNLOCK();
    }
}
/**************************************************************************************************/
void TracingCurve_CleanCurveData(TracingCurve_Handle hObj)
{
    int i;
    if (hObj)
    {
        TracingCurve_Obj* pObj;
        WM_LOCK();
        pObj = TracingCurve_H2P(hObj);
        for(i=0; i<(pObj->DATAINFO.XDotEnd - pObj->DATAINFO.XDotStart); i++)
        {
            (pObj->Data.DataDotInfo+i)->Datax = 0;
            (pObj->Data.DataDotInfo+i)->Datay = 0;
            (pObj->Data.DataDotInfo+i)->DataColor = pObj->DIALOGINFO.BKColor;
        }
        TracingCurve_Invalidate(hObj);
        WM_UNLOCK();
    }
}
//设置传入参数在全局的唯一ID号
void TracingCurve_SetParameterId(TracingCurve_Handle hObj,int ParameterId)
{
    if (hObj)
    {
        TracingCurve_Obj* pObj;
        WM_LOCK();
        pObj = TracingCurve_H2P(hObj);
        pObj->ParameterId = ParameterId;
        WM_UNLOCK();
    }
}
//获得传入参数在全局的唯一ID号
int TracingCurve_GetParameterId(TracingCurve_Handle hObj)
{
    int ret = -1;
    if (hObj)
    {
        TracingCurve_Obj* pObj;
        WM_LOCK();
        pObj = TracingCurve_H2P(hObj);
        ret = pObj->ParameterId;
        WM_UNLOCK();
    }
    return ret;
}
//设置现在点的x值
void TracingCurve_SetXScale(TracingCurve_Handle hObj,int xscale)
{
    if (hObj)
    {
        TracingCurve_Obj* pObj;
        WM_LOCK();
        pObj = TracingCurve_H2P(hObj);
        pObj->xscale = xscale;
        TracingCurve_Invalidate(hObj);
        WM_UNLOCK();
    }
}
//获得现在点的x值
int TracingCurve_GetXScale(TracingCurve_Handle hObj)
{
    int ret = 0;
    if (hObj)
    {
        TracingCurve_Obj* pObj;
        WM_LOCK();
        pObj = TracingCurve_H2P(hObj);
        ret = pObj->xscale;
        WM_UNLOCK();
    }
    return ret;
}
//设置保存LOOPS历史一个周期的数据
void TracingCurve_SetLoopsRef(TracingCurve_Handle hObj,char bLoopsRef)
{
    if (hObj)
    {
        TracingCurve_Obj* pObj;
        WM_LOCK();
        pObj = TracingCurve_H2P(hObj);
        pObj->nLoopsRef = bLoopsRef;
        WM_UNLOCK();
    }
}
//波形显示的另外情况，为SPO2时，波形显示区域的右侧显示棒图  0：不显示（默认值） 1：显示
void TracingCurve_SetSPO2Bar(TracingCurve_Handle hObj,int nSPO2Bar)
{
    if (hObj)
    {
        TracingCurve_Obj* pObj;
        WM_LOCK();
        pObj = TracingCurve_H2P(hObj);
        pObj->nSPO2Bar = nSPO2Bar;
        TracingCurve_Invalidate(hObj);
        WM_UNLOCK();
    }
}
///SPO2棒图，显示数据大小  [0，16）
void TracingCurve_SetSPO2Count(TracingCurve_Handle hObj,int nSPO2Count)
{
    GUI_RECT Rect;
    int xSize,ySize;
    int yWidth;
    if (hObj)
    {
        TracingCurve_Obj* pObj;
        WM_LOCK();
        pObj = TracingCurve_H2P(hObj);
        pObj->nSPO2Count = nSPO2Count;
        //TracingCurve_Invalidate(hObj);
        ///局部刷新
        xSize = pObj->Widget.Win.Rect.x1 - pObj->Widget.Win.Rect.x0;
        ySize = pObj->Widget.Win.Rect.y1 - pObj->Widget.Win.Rect.y0;
        yWidth = (ySize-15)/29;
        Rect.x0 = pObj->DATAINFO.XDotEnd+nTracingCurveClearLine/*TRACINGCURVE_CLEARLINE_DEFAULT*/;
        Rect.y0 = ySize-10-yWidth*29;
        Rect.x1 = xSize;
        Rect.y1 = ySize-10;
        WM_InvalidateRect(hObj,&Rect);
        WM_UNLOCK();
    }
}
/***************************************************************************************************
*               TracingCurve_SetADD2Data()
*   功能：将数据添加到显示区域和数据buffer区
*   输入：
*       hObj
*       color                   数据在显示区域显示的颜色
*       data1                   数据值   范围是(pObj->XYAD_INFO.YMin,pObj->XYAD_INFO.YMax)
*       data2                   数据值   范围是(pObj->XYAD_INFO.YMin,pObj->XYAD_INFO.YMax)
*
*
*/
void TracingCurve_SetADD2Data(TracingCurve_Handle hObj,GUI_COLOR color,float datax,float datay,int flag,int WaveForm)
{
    int x,y,oldx,oldy;
    int i=0;
    static int stop = 0;
    if (hObj)
    {
        TracingCurve_Obj* pObj;
        WM_LOCK();
        pObj = TracingCurve_H2P(hObj);
        WM_SelectWindow(hObj);
        pObj->Data.loopSign = 'T';


        if(datay > pObj->Data_Max_High)
        {
            pObj->Data_Max_High = datay;
        }
        if(datay < pObj->Data_Max_Low)
        {
            pObj->Data_Max_Low = datay;
        }
        if(datax > pObj->Data_Max_HighX)
        {
            pObj->Data_Max_HighX = datax;
        }
        if(datax < pObj->Data_Max_LowX)
        {
            pObj->Data_Max_LowX = datax;
        }
        if(flag)
        {
            TraceingCurve_NeedChangeRange(hObj,flag,WaveForm);
        }




        if(flag == 1)
        {
            if(!(pObj->nStopRemove))
            {
                GUI_SetColor(pObj->DIALOGINFO.BKColor);
                GUI_FillRect(pObj->DATAINFO.XDotStart,pObj->DATAINFO.YDotEnd,pObj->DATAINFO.XDotEnd,pObj->DATAINFO.YDotStart);
                pObj->xscale = 1;
                if(stop)
                {
                    stop = 0;
                }
            }
            else
            {
                stop = 1;
            }
        }
        //超出了显示的区域，将坐标变大
        //pObj->Data.loopSign = 'T';
        if(!stop)
        {
            if(((pObj->xscale)+(pObj->DATAINFO.XDot))>pObj->DATAINFO.XDotEnd)
            {
                pObj->xscale = 1;
            }
            x = (pObj->DATAINFO.XDot)+(pObj->DATAINFO.XDotStep)*datax/((pObj->XYAD_INFO.XStep)*(pObj->RangeX));
            y = (pObj->DATAINFO.YDot)-(pObj->DATAINFO.YDotStep)*datay/((pObj->XYAD_INFO.YStep)*(pObj->Range));
            if(y<pObj->DATAINFO.YDotEnd)
            {
                y = pObj->DATAINFO.YDotEnd;
                datay = pObj->XYAD_INFO.YMax*(pObj->Range);
                pObj->ExceedRangeFlg = 1;
            }
            else if(y>pObj->DATAINFO.YDotStart)
            {
                y = pObj->DATAINFO.YDotStart;
                datay = pObj->XYAD_INFO.YMin*(pObj->Range);
                pObj->ExceedRangeFlg = 1;
            }
            if(x>pObj->DATAINFO.XDotEnd)
            {
                x = pObj->DATAINFO.XDotEnd;
                datax = pObj->XYAD_INFO.XMax*(pObj->RangeX);
                pObj->ExceedRangeFlg = 1;
            }
            else if(x<pObj->DATAINFO.XDotStart)
            {
                x = pObj->DATAINFO.XDotStart;
                datax = pObj->XYAD_INFO.XMin*(pObj->RangeX);
                pObj->ExceedRangeFlg = 1;
            }
            //上一个点的信息
            if(pObj->xscale == 1)
            {
                oldy = y;
                oldx = x;
            }
            else
            {
                oldx = (pObj->DATAINFO.XDot)+(pObj->DATAINFO.XDotStep)*((pObj->Data.DataDotInfo+pObj->xscale-1)->Datax)/((pObj->XYAD_INFO.XStep)*(pObj->RangeX));
                oldy = (pObj->DATAINFO.YDot)-(pObj->DATAINFO.YDotStep)*((pObj->Data.DataDotInfo+pObj->xscale-1)->Datay)/((pObj->XYAD_INFO.YStep)*(pObj->Range));
            }
            GUI_SetColor(color);
            //画出表现数据的方式
            GUI_SetPenSize(1);
            GUI_DrawLine(oldx,oldy,x,y);
            (pObj->Data.DataDotInfo+pObj->xscale)->Datax = datax;
            (pObj->Data.DataDotInfo+pObj->xscale)->Datay = datay;
            (pObj->Data.DataDotInfo+pObj->xscale)->DataColor = color;
            pObj->xscale++;
            //重新画X轴Y轴
            GUI_SetColor(pObj->XYAD_INFO.XYColor);
            if(pObj->XYAD_INFO.XSign=='T')
            {
                GUI_DrawHLine(pObj->DATAINFO.YDotStart,pObj->DATAINFO.XDotStart,pObj->DATAINFO.XDotEnd);  //水平方向
                GUI_DrawVLine(pObj->DATAINFO.XDotStart,pObj->DATAINFO.YDotEnd,pObj->DATAINFO.YDotStart);  //垂直方向
            }
            GUI_DrawHLine(pObj->DATAINFO.YDot,pObj->DATAINFO.XDotStart,pObj->DATAINFO.XDotEnd);  //水平方向 0
            GUI_DrawVLine(pObj->DATAINFO.XDot,pObj->DATAINFO.YDotEnd,pObj->DATAINFO.YDotStart);  //垂直方向 0
        }
        WM_UNLOCK();
    }
}
/***************************************************************************************************
*               TracingCurve_SetXYInfo
*
*       功能：设置XY轴的信息部分信息
*       输入：
*           GUI_COLOR XYColor;   //xy轴颜色
*           char* XYText;      //图右上角的TEXT
*/
void TracingCurve_SetXYInfo(TracingCurve_Handle hObj,GUI_COLOR color,char* XYText)
{
    if (hObj)
    {
        TracingCurve_Obj* pObj;
        WM_LOCK();
        pObj = TracingCurve_H2P(hObj);
        pObj->XYAD_INFO.XYColor = color;
        pObj->XYAD_INFO.XYText = XYText;
        TracingCurve_Invalidate(hObj);
        WM_UNLOCK();
    }
}
/***************************************************************************************************
*               TracingCurve_SetViewMode
*
*       功能：设置图形表现形式
*       输入：
*           int mode;       //画出表现数据的方式  0:曲线方式  1:柱形方式
*/
void TracingCurve_SetViewMode(TracingCurve_Handle hObj,int mode,char loopSign)
{
    if (hObj)
    {
        TracingCurve_Obj* pObj;
        WM_LOCK();
        pObj = TracingCurve_H2P(hObj);
        pObj->Data.Mode = mode;
        pObj->Data.loopSign = loopSign;
        TracingCurve_Invalidate(hObj);
        WM_UNLOCK();
    }
}
/**************************************************************************************************/
int TracingCurve_GetRange(TracingCurve_Handle hObj)
{
    int times = 0;
    if (hObj)
    {
        TracingCurve_Obj* pObj;
        WM_LOCK();
        pObj = TracingCurve_H2P(hObj);
        times = pObj->Range;
        WM_UNLOCK();
    }
    return times;
}
/**************************************************************************************************/
void TracingCurve_SetExceedRangeFlg(TracingCurve_Handle hObj,int flag)
{
    if (hObj)
    {
        TracingCurve_Obj* pObj;
        WM_LOCK();
        pObj = TracingCurve_H2P(hObj);
        pObj->ExceedRangeFlg = flag;
        TracingCurve_Invalidate(hObj);
        WM_UNLOCK();
    }
}
/**********************************************************************************************/
void TracingCurve_SetRangeX(TracingCurve_Handle hObj,float times)
{
    if (hObj)
    {
        TracingCurve_Obj* pObj;
        WM_LOCK();
        pObj = TracingCurve_H2P(hObj);
        pObj->RangeX = times;
        TracingCurve_Invalidate(hObj);
        WM_UNLOCK();
    }
}
/**************************************************************************************************/
void TracingCurve_SetRange(TracingCurve_Handle hObj,int times)
{
    if (hObj)
    {
        TracingCurve_Obj* pObj;
        WM_LOCK();
        pObj = TracingCurve_H2P(hObj);
        pObj->Range = times;
        TracingCurve_Invalidate(hObj);
        WM_UNLOCK();
    }
}
#ifdef WAVE_PAW_ADD_ALARMHIGH
int GetPawAlarmRange(void)
{
    float PawRange;
    int multi = 0,PawFlag;

    PawRange = AlarmSettingPara.PressureHighValue/5.0;
    if((int)(PawRange*100)%100 != 0)
    {
        PawFlag = (int)PawRange + 1;
    }
    else
    {
        PawFlag = (int)PawRange;
    }
    if(PawFlag > 0)
    {
        if(PawFlag <= 1)
        {
            multi = 0;
        }
        else if (PawFlag <=2)
        {
            multi = 1;
        }
        else if(PawFlag <= 3)
        {
            multi = 2;
        }
        else if(PawFlag <= 4)
        {
            multi = 3;
        }
        else if(PawFlag <= 5)
        {
            multi = 4;
        }
        else if(PawFlag <= 10)
        {
            multi = 5;
        }
        else if(PawFlag <= 15)
        {
            multi = 6;
        }
        else
        {
            multi = 7;
        }
    }
    PawFlag = PawRangeFlag[multi];
    return PawFlag;
}
#endif

/**********************************************************************************************/
/*函数功能：2016.6.3 zy 增加y坐标量程滤波函数 //add 20170116
输入参数：int WaveForm,波形类型
			int high_or_low,最大值最小值标志
			float data,波形数据
			float *Data_Filter
返回值：
*/
int Data_Max_Filter_High_count[20],Data_Max_Filter_Low_count[20];
float Data_Max_High_Filter[20][WAVEFORM_FILTER],Data_Max_Low_Filter[20][WAVEFORM_FILTER];  
float Data_Max_Filter(int WaveForm,int high_or_low,float data,float *Data_Filter)
{
	int i;
	float return_value = -5000;   
	if(high_or_low)
	{	
		*(Data_Filter + Data_Max_Filter_High_count[WaveForm] ) = data;
		if( ++Data_Max_Filter_High_count[WaveForm] >= WAVEFORM_FILTER)
		{
			Data_Max_Filter_High_count[WaveForm] = 0;
		}
		for(i=0;i<WAVEFORM_FILTER;i++)
		{
			if(return_value < *(Data_Filter + i ))
			{
				return_value = *(Data_Filter + i );
			}
		}
	} 
	else
	{
		*(Data_Filter + Data_Max_Filter_Low_count[WaveForm] ) = data;
		if( ++Data_Max_Filter_Low_count[WaveForm] >= WAVEFORM_FILTER)
		{
			Data_Max_Filter_Low_count[WaveForm] = 0;
		}
		for(i=0;i<WAVEFORM_FILTER;i++)
		{
			if(return_value < *(Data_Filter + i ))
			{
				return_value = *(Data_Filter + i );
			}
		}
	}
	return  return_value;
}



/***************************************************************************************************
*函数功能：
*输入参数：TracingCurve_Handle hObj,
			char flag,LOOP的标志
			int WaveForm波形类型
*
*/
int TraceingCurve_NeedChangeRange(TracingCurve_Handle hObj,char flag, int WaveForm)
{
    int ChangeRange = 0;
	float Low_Multi = 0.0;
    float High_Multi = 0.0;
	int multi = 0,PawFlag = 0;
    int multi_last = 0;
	int ChangFlg = 0;
    if (hObj)
    {
        TracingCurve_Obj* pObj;
        WM_LOCK();
        pObj = TracingCurve_H2P(hObj);

        switch (WaveForm)
		{
		case WAVEFORM_PRESSURE://Paw
            if(((pObj->xscale)+(pObj->DATAINFO.XDot)) > (pObj->DATAINFO.XDotEnd))
            {
                if((pObj->XYAD_INFO.YMin!=0.0)&(pObj->XYAD_INFO.YMax!=0.0))
                {
				    /*
					    zy 2016.6.3修改获取量程范围代码
				    */
					Low_Multi =  Data_Max_Filter( WAVEFORM_PRESSURE, 0, pObj->Data_Max_Low  / (-1.0), &Data_Max_Low_Filter[WAVEFORM_PRESSURE][0]);
					High_Multi = Data_Max_Filter( WAVEFORM_PRESSURE, 1, pObj->Data_Max_High / 5.0,    &Data_Max_High_Filter[WAVEFORM_PRESSURE][0]);
                    if(High_Multi > Low_Multi)
                    {
                        High_Multi = pObj->Data_Max_High / 5.0;
                    }
                    else
                    {
                        High_Multi = pObj->Data_Max_Low / (-1.0);
                    }
                    if(High_Multi > 0)
                    {
                        if(High_Multi <= 1)
                        {
                            multi = 1;
                        }
                        else if (High_Multi <=2)
                        {
                            multi = 2;
                        }
                        else if(High_Multi <= 3)
                        {
                            multi = 3;
                        }
                        else if(High_Multi <= 4)
                        {
                            multi = 4;
                        }
                        else if(High_Multi <= 5)
                        {
                            multi = 5;
                        }
                        else if(High_Multi <= 10)
                        {
                            multi = 10;
                        }
                        else if(High_Multi <= 15)
                        {
                            multi = 15;
                        }
                        else //if(High_Multi <= 20)
                        {
                            multi = 20;
                        }
#ifdef WAVE_PAW_ADD_ALARMHIGH
                        PawFlag = GetPawAlarmRange();
                        if(PawFlag > multi)
                        {
                            multi = PawFlag;
                        }
                        if(((int)(pObj->Range) != multi) && 
                            (PawFlag <= multi)
                            )
#else
						multi_last = pObj->Range;
						if (multi > multi_last)
						{
							ChangFlg = 1;
							pObj->CurveRefreshCycleIndex = 0;
							pObj->CurveRangUpdate = 0;
						}
						else if (multi < multi_last)
						{
							pObj->CurveRefreshCycleIndex++;
							if (pObj->CurveRefreshCycleIndex>1 && pObj->CurveRangUpdate)
							{
								ChangFlg = 1;
								pObj->CurveRefreshCycleIndex = 0;
								pObj->CurveRangUpdate = 0;
							}
						}
						else
						{
							pObj->CurveRefreshCycleIndex = 0;
							pObj->CurveRangUpdate = 0;
						}
						if (ChangFlg)
							if(multi_last != multi)
#endif
							{
								//pObj->yrange = 1;
								TracingCurve_SetRange(hObj, (float)(multi));
								MultiArray[0] = (float)multi;
							}
                    }
                    pObj->Data_Max_High = -0xFFFF;
                    pObj->Data_Max_Low = 0xFFFF;
                }
            }
            break;
        case WAVEFORM_FLOW://Flow
            if(((pObj->xscale)+(pObj->DATAINFO.XDot))>(pObj->DATAINFO.XDotEnd))
            {
                //  if((pObj->XYAD_INFO.YMin!=0.0)&(pObj->XYAD_INFO.YMax!=0.0))
                {
				    Low_Multi =  Data_Max_Filter( WAVEFORM_FLOW, 0, pObj->Data_Max_Low  / (-3.0), &Data_Max_Low_Filter[WAVEFORM_FLOW][0]);
				    High_Multi = Data_Max_Filter( WAVEFORM_FLOW, 1, pObj->Data_Max_High / 3.0,    &Data_Max_High_Filter[WAVEFORM_FLOW][0]);
                    if(High_Multi > Low_Multi)
                    {
                        High_Multi = pObj->Data_Max_High / 3.0;
                    }
                    else
                    {
                        High_Multi = pObj->Data_Max_Low / (-3.0);
                    }
                    if(High_Multi > 0)
                    {
                        if(High_Multi <= 1)
                        {
                            multi = 1;
                        }
                        else if (High_Multi <=2)
                        {
                            multi = 2;
                        }
                        else if(High_Multi <= 4)
                        {
                            multi = 4;
                        }
                        else if(High_Multi <= 5)
                        {
                            multi = 5;
                        }
                        else if (High_Multi <= 10)
                        {
                            multi = 10;
                        }
                        else if(High_Multi <= 20)
                        {
                            multi = 20;
                        }
                        else if (High_Multi <= 40)
                        {
                            multi = 40;
                        }
                        else
                        {
                            multi = 60;
                        }
						multi_last = pObj->Range;
						if (multi > multi_last)
						{
							ChangFlg = 1;
							pObj->CurveRefreshCycleIndex = 0;
							pObj->CurveRangUpdate = 0;
						}
						else if (multi < multi_last)
						{
							pObj->CurveRefreshCycleIndex++;
							if (pObj->CurveRefreshCycleIndex>1 && pObj->CurveRangUpdate)
							{
								ChangFlg = 1;
								pObj->CurveRefreshCycleIndex = 0;
								pObj->CurveRangUpdate = 0;
							}
						}
						else
						{
							pObj->CurveRefreshCycleIndex = 0;
							pObj->CurveRangUpdate = 0;
						}
						if (ChangFlg)
							if(multi_last != multi)
							{
								//pObj->yrange = 1;
								TracingCurve_SetRange(hObj, (float)(multi));
								MultiArray[1] = (float)multi;
							}
                    }
                    pObj->Data_Max_High = -0xFFFF;
                    pObj->Data_Max_Low = 0xFFFF;
                }
            }
            break;
#ifdef AA_WAVE_SWITCH
        case WAVEFORM_SAA:
            if(((pObj->xscale)+(pObj->DATAINFO.XDot)) > (pObj->DATAINFO.XDotEnd))
            {
                if((pObj->XYAD_INFO.YMax!=0.0))
                {
				    /*
					    zy 2016.6.3修改获取量程范围代码
				    */
					Low_Multi =  Data_Max_Filter( WAVEFORM_SAA, 0, pObj->Data_Max_Low  / (-0.5), &Data_Max_Low_Filter[WAVEFORM_SAA][0]);
					High_Multi = Data_Max_Filter( WAVEFORM_SAA, 1, pObj->Data_Max_High / 2.5,    &Data_Max_High_Filter[WAVEFORM_SAA][0]);
					
                    if(High_Multi > Low_Multi)
                    {
                        High_Multi = pObj->Data_Max_High / 2.5;
                    }
                    else
                    {
                        High_Multi = pObj->Data_Max_Low / (-0.5);
                    }
                    if(High_Multi > 0) //ylk md 20190411 修改波形量程
                    {
// 						if(High_Multi <= 1)
// 						{
// 							multi = 1;
// 						}
// 						else if(High_Multi <= 2)
// 						{
// 							multi = 2;
// 						}
// 						else if(High_Multi <= 3)
// 						{
// 							multi = 3;
// 						}
// 						else if(High_Multi <= 4)
// 						{
// 							multi = 4;
// 						}
// 						else
// 						{
// 							multi = 5;
// 						}
                        
                        if(High_Multi <= 1.8)
						{
							multi = 1;
						}
						else if(High_Multi <= 3.8)
						{
							multi = 2;
						}
						else if(High_Multi <= 5.8)
						{
							multi = 3;
						}
						else if(High_Multi <= 7.8)
						{
							multi = 4;
						}
                        else
                        {
                            multi = 5;
                        }
                        
                        if((int)(pObj->Range) != multi)
                        {
                            pObj->yrange = 1;
                            TracingCurve_SetRange(hObj, (float)(multi));
                            MultiAA[1] = (float)multi;
                        }
                    }
                    pObj->Data_Max_High = -0xFFFF;
                    pObj->Data_Max_Low = 0xFFFF;
                }
            }
            break;
        case WAVEFORM_PAA:
            if(((pObj->xscale)+(pObj->DATAINFO.XDot)) > (pObj->DATAINFO.XDotEnd))
            {
                if((pObj->XYAD_INFO.YMax!=0.0))
//                if((pObj->XYAD_INFO.YMin!=0.0)&(pObj->XYAD_INFO.YMax!=0.0))
                {
				    Low_Multi =  Data_Max_Filter( WAVEFORM_PAA, 0, pObj->Data_Max_Low  / -1, &Data_Max_Low_Filter[WAVEFORM_PAA][0]);
				    High_Multi = Data_Max_Filter( WAVEFORM_PAA, 1, pObj->Data_Max_High / 3.0,    &Data_Max_High_Filter[WAVEFORM_PAA][0]);
                    if(High_Multi > Low_Multi)
                    {
                        High_Multi = pObj->Data_Max_High / 2.5;
                    }
                    else
                    {
                        High_Multi = pObj->Data_Max_Low / (-0.5);
                    }
                    if(High_Multi > 0) //ylk md 20190411 修改波形量程
                    {
// 						if(High_Multi <= 1)
// 						{
// 							multi = 1;
// 						}
// 						else if(High_Multi <= 2)
// 						{
// 							multi = 2;
// 						}
// 						else if(High_Multi <= 3)
// 						{
// 							multi = 3;
// 						}
// 						else if(High_Multi <= 4)
// 						{
// 							multi = 4;
// 						}
// 						else
// 						{
// 							multi = 5;
// 						}

						if(High_Multi <= 1.8)
						{
							multi = 1;
						}
						else if(High_Multi <= 3.8)
						{
							multi = 2;
						}
						else if(High_Multi <= 5.8)
						{
							multi = 3;
						}
						else if(High_Multi <= 7.8)
						{
							multi = 4;
						}
                        else
                        {
                            multi = 5;
                        }

                        if((int)(pObj->Range) != multi)
                        {
                            pObj->yrange = 1;
                            TracingCurve_SetRange(hObj, (float)(multi));
                            MultiAA[0] = (float)multi;
                        }
                    }
                    pObj->Data_Max_High = -0xFFFF;
                    pObj->Data_Max_Low = 0xFFFF;
                }
            }
            break;
			//  [2/14/2022 yaoyao add]CO2加上量程变换
		case WAVEFORM_CO2:
			if (((pObj->xscale)+(pObj->DATAINFO.XDot)) > (pObj->DATAINFO.XDotEnd))
			{
				if (pObj->Data_Max_High > CO2_Disp.co2max*2)
				{
					multi = 3;
				}
				else if (pObj->Data_Max_High > CO2_Disp.co2max)
				{
					multi = 2;
				}
				else
				{
					multi = 1;
				}
				multi_last = (int)(pObj->Range);
				if(multi_last != multi)
				{
					TracingCurve_SetRange(hObj, (float)(multi));
					MultiArray[3] = (float)multi;
				}
				pObj->Data_Max_High = -0xFFFF;
				pObj->Data_Max_Low = 0xFFFF;
			}	
			break;
#endif
        case WAVEFORM_VOLUME://V
            if(((pObj->xscale)+(pObj->DATAINFO.XDot))>(pObj->DATAINFO.XDotEnd))
            {
                if(pObj->XYAD_INFO.YMax!=0.0)
                {
                     High_Multi = Data_Max_Filter( WAVEFORM_VOLUME, 1, pObj->Data_Max_High / 30.0,    &Data_Max_High_Filter[WAVEFORM_VOLUME][0]);
                    if(High_Multi > 0)
                    {
                        if(High_Multi <= 1)
                        {
                            multi = 1;
                        }
                        else if (High_Multi <= 2)
                        {
                            multi = 2;
                        }
                        else if(High_Multi <= 3)
                        {
                            multi = 3;
                        }
                        else if (High_Multi <= 4)
                        {
                            multi = 4;
                        }
                        else if(High_Multi <= 5)
                        {
                            multi = 5;
                        }
                        else if (High_Multi <= 10)
                        {
                            multi = 10;
                        }
                        else if(High_Multi <= 20)
                        {
                            multi = 20;
                        }
                        else if(High_Multi <= 30)
                        {
                            multi = 30;
                        }
                        else if(High_Multi <= 60)
                        {
                            multi = 60;
                        }
                        else if(High_Multi <= 100)
                        {
                            multi = 100;
                        }
                        else
                        {
                            multi = 150;
                        }
						multi_last = pObj->Range;
						if (multi > multi_last)
						{
							ChangFlg = 1;
							pObj->CurveRefreshCycleIndex = 0;
							pObj->CurveRangUpdate = 0;
						}
						else if (multi < multi_last)
						{
							pObj->CurveRefreshCycleIndex++;
							if (pObj->CurveRefreshCycleIndex>1 && pObj->CurveRangUpdate)
							{
								ChangFlg = 1;
								pObj->CurveRefreshCycleIndex = 0;
								pObj->CurveRangUpdate = 0;
							}
						}
						else
						{
							pObj->CurveRefreshCycleIndex = 0;
							pObj->CurveRangUpdate = 0;
						}
						if (ChangFlg)
							if(multi_last != multi)
							{
								//pObj->yrange = 1;
								TracingCurve_SetRange(hObj, (float)multi);
								MultiArray[2] = multi;
							}
                    }
                    pObj->Data_Max_High = -0xFFFF;
                    pObj->Data_Max_Low = 0xFFFF;
                }
            }
            break;
        case LOOP_VF://FlowV
            if(flag)
            {
					Low_Multi =  Data_Max_Filter( LOOP_VF, 0, pObj->Data_Max_Low  / (-3.0), &Data_Max_Low_Filter[LOOP_VF][0]);
					High_Multi = Data_Max_Filter( LOOP_VF, 1, pObj->Data_Max_High / 3.0,    &Data_Max_High_Filter[LOOP_VF][0]);
                    if(High_Multi >= Low_Multi)
                    {
                        High_Multi = pObj->Data_Max_High  / 3;
                    }
                    else
                    {
                        High_Multi = pObj->Data_Max_Low / (-3.0);
                    }
                    if(High_Multi > 0)
                    {
                        if(High_Multi <= 1)
                        {
                            multi = 1;
                        }
                        else if (High_Multi <=2)
                        {
                            multi = 2;
                        }
                        else if(High_Multi <= 4)
                        {
                            multi = 4;
                        }
                        else if(High_Multi <= 5)
                        {
                            multi = 5;
                        }
                        else if (High_Multi <= 10)
                        {
                            multi = 10;
                        }
                        else if(High_Multi <= 20)
                        {
                            multi = 20;
                        }
                        else if (High_Multi <= 40)
                        {
                            multi = 40;
                        }
                        else
                        {
                            multi = 60;
                        }
                        multi_last = (int)(pObj->Range);
                        if(multi_last != multi)
                        {
                            //pObj->yrange = 1;
                            TracingCurve_SetRange(hObj, (float)(multi));
                            MultiArray[1] = (float)(multi);
                        }
                    }
                    pObj->Data_Max_High = (float)-0xFFFF;
                    pObj->Data_Max_Low = (float)0xFFFF;
                    High_Multi = pObj->Data_Max_HighX / 30.0;
                    if(High_Multi > 0)
                    {
                        //6月7号
                        if(High_Multi <= 1)
                        {
                            multi = 1;
                        }
                        else if (High_Multi <= 2)
                        {
                            multi = 2;
                        }
                        else if(High_Multi <= 3)
                        {
                            multi = 3;
                        }
                        else if (High_Multi <= 4)
                        {
                            multi = 4;
                        }
                        else if(High_Multi <= 5)
                        {
                            multi = 5;
                        }
                        else if (High_Multi <= 10)
                        {
                            multi = 10;
                        }
                        else if(High_Multi <= 20)
                        {
                            multi = 20;
                        }
                        else if(High_Multi <= 30)
                        {
                            multi = 30;
                        }
                        else if(High_Multi <= 60)
                        {
                            multi = 60;
                        }
                        else if(High_Multi <= 100)
                        {
                            multi = 100;
                        }
                        else
                        {
                            multi = 150;
                        }
                        multi_last = (int)(pObj->RangeX);
                        if(multi_last != multi)
                        {
                            //pObj->yrange = 1;
                            TracingCurve_SetRangeX(hObj,(float)multi);
                            MultiArray[2] = multi;
                        }
                    }
                    pObj->Data_Max_HighX = (float)-0xFFFF;
                    pObj->Data_Max_LowX = (float)0xFFFF;
            }
            break;
        case LOOP_PV://PV
            if(flag)
            {
                    if(pObj->XYAD_INFO.YMax!=0.0)
                    {
                        High_Multi = Data_Max_Filter( LOOP_PV, 1, pObj->Data_Max_High / 30.0,    &Data_Max_High_Filter[LOOP_PV][0]);
                        if(High_Multi > 0)
                        {
                            if(High_Multi <= 1)
                            {
                                multi = 1;
                            }
                            else if (High_Multi <= 2)
                            {
                                multi = 2;
                            }
                            else if(High_Multi <= 3)
                            {
                                multi = 3;
                            }
                            else if (High_Multi <= 4)
                            {
                                multi = 4;
                            }
                            else if(High_Multi <= 5)
                            {
                                multi = 5;
                            }
                            else if (High_Multi <= 10)
                            {
                                multi = 10;
                            }
                            else if(High_Multi <= 20)
                            {
                                multi = 20;
                            }
                            else if(High_Multi <= 30)
                            {
                                multi = 30;
                            }
                            else if(High_Multi <= 60)
                            {
                                multi = 60;
                            }
                            else if(High_Multi <= 100)
                            {
                                multi = 100;
                            }
                            else
                            {
                                multi = 150;
                            }
                            multi_last = (int)(pObj->Range);
                            if(multi_last != multi)
                            {
                                //pObj->yrange = 1;
                                TracingCurve_SetRange(hObj, (float)multi);
                                MultiArray[2] = multi;
                            }
                        }
                    }
                    pObj->Data_Max_High = -0xFFFF;
                    pObj->Data_Max_Low = 0xFFFF;
                    Low_Multi = pObj->Data_Max_LowX / (-1.0);
                    High_Multi = pObj->Data_Max_HighX /5.0;
                    if(High_Multi > Low_Multi)
                    {
                        High_Multi = pObj->Data_Max_HighX / 5.0;
                    }
                    else
                    {
                        High_Multi = pObj->Data_Max_LowX / (-1.0);
                    }
                    if(High_Multi > 0)
                    {
                        if(High_Multi <= 1)
                        {
                            multi = 1;
                        }
                        else if (High_Multi <=2)
                        {
                            multi = 2;
                        }
                        else if(High_Multi <= 3)
                        {
                            multi = 3;
                        }
                        else if(High_Multi <= 4)
                        {
                            multi = 4;
                        }
                        else if(High_Multi <= 5)
                        {
                            multi = 5;
                        }
                        else if(High_Multi <= 10)
                        {
                            multi = 10;
                        }
                        else if(High_Multi <= 15)
                        {
                            multi = 15;
                        }
                        else //if(High_Multi <= 20)
                        {
                            multi = 20;
                        }
                        multi_last = (int)(pObj->RangeX);
                        if(multi_last != multi)
                        {
                            //pObj->yrange = 1;
                            TracingCurve_SetRangeX(hObj,(float)multi);
                            MultiArray[0] = (float)multi;
                        }
                    }
                    pObj->Data_Max_HighX = -0xFFFF;
                    pObj->Data_Max_LowX = 0xFFFF;
            }
            break;

		case LOOP_PF:
			if(flag)
			{
				//if(Paint_Limit_Loop_B)
				{
					Low_Multi = pObj->Data_Max_Low / (-3.0);
					High_Multi = pObj->Data_Max_High / 3;
					if(High_Multi >= Low_Multi)
					{
						High_Multi = pObj->Data_Max_High  / 3;
					}
					else
					{
						High_Multi = pObj->Data_Max_Low / (-3.0);
					}
					if(High_Multi > 0)
					{
						if(High_Multi <= 1)
						{
							multi = 1;
						}
						else if (High_Multi <=2)
						{
							multi = 2;
						}
						else if(High_Multi <= 4)
						{
							multi = 4;
						}
						else if(High_Multi <= 5)
						{
							multi = 5;
						}
						else if (High_Multi <= 10)
						{
							multi = 10;
						}
						else if(High_Multi <= 20)
						{
							multi = 20;
						}
						else if (High_Multi <= 40)
						{
							multi = 40;
						}
						else
						{
							multi = 60;
						}
						multi_last = (int)(pObj->Range);
						if(multi_last != multi)
						{
							pObj->yrange = 1;
							TracingCurve_SetRange(hObj, (float)(multi));
							MultiArray[1] = (float)(multi);
						}
					}
					pObj->Data_Max_High = (float)-0xFFFF;
					pObj->Data_Max_Low = (float)0xFFFF;
					Low_Multi = pObj->Data_Max_LowX / (-1.0);
					High_Multi = pObj->Data_Max_HighX /5.0;
					if(High_Multi > Low_Multi)
					{
						High_Multi = pObj->Data_Max_HighX / 5.0;
					}
					else
					{
						High_Multi = pObj->Data_Max_LowX / (-1.0);
					}
					if(High_Multi > 0)
					{
						if(High_Multi <= 1)
						{
							multi = 1;
						}
						else if (High_Multi <=2)
						{
							multi = 2;
						}
						else if(High_Multi <= 3)
						{
							multi = 3;
						}
						else if(High_Multi <= 4)
						{
							multi = 4;
						}
						else if(High_Multi <= 5)
						{
							multi = 5;
						}
						else if(High_Multi <= 10)
						{
							multi = 10;
						}
						else if(High_Multi <= 15)
						{
							multi = 15;
						}
						else //if(High_Multi <= 20)
						{
							multi = 20;
						}
						multi_last = (int)(pObj->RangeX);
						if(multi_last != multi)
						{
							pObj->yrange = 1;
							TracingCurve_SetRangeX(hObj,(float)multi);
							MultiArray[0] = (float)multi;
						}
					}
					pObj->Data_Max_HighX = -0xFFFF;
					pObj->Data_Max_LowX = 0xFFFF;
				}
				//                else
				//                {
				//                    Paint_Limit_Loop_B = 1;
				//                }
			}
			break;

		case LOOP_V_CO2:
			if(flag)
			{
				//
//				if (((pObj->xscale)+(pObj->DATAINFO.XDot)) > (pObj->DATAINFO.XDotEnd))
				{
					if (pObj->Data_Max_High > CO2_Disp.co2max*2)
					{
						multi = 3;
					}
					else if (pObj->Data_Max_High > CO2_Disp.co2max)
					{
						multi = 2;
					}
					else
					{
						multi = 1;
					}
					multi_last = (int)(pObj->Range);
					if(multi_last != multi)
					{
						TracingCurve_SetRange(hObj, (float)(multi));
						MultiArray[3] = (float)multi;
					}
					pObj->Data_Max_High = -0xFFFF;
					pObj->Data_Max_Low = 0xFFFF;
				}	
				//
				High_Multi = pObj->Data_Max_HighX / 30.0;
				if(High_Multi > 0)
				{
					if(High_Multi <= 1)
					{
						multi = 1;
					}
					else if (High_Multi <= 2)
					{
						multi = 2;
					}
					else if(High_Multi <= 5)
					{
						multi = 5;
					}
					else if (High_Multi <= 10)
					{
						multi = 10;
					}
					else if(High_Multi <= 20)
					{
						multi = 20;
					}
					else if(High_Multi <= 30)
					{
						multi = 30;
					}
					else if(High_Multi <= 60)
					{
						multi = 60;
					}
					else if(High_Multi <= 100)
					{
						multi = 100;
					}
					else
					{
						multi = 150;
					}
					multi_last = (int)(pObj->RangeX);
					if(multi_last != multi)
					{
//						pObj->yrange = 1;
						TracingCurve_SetRangeX(hObj, (float)multi);
						MultiArray[2] = multi;
					}
				}
				pObj->Data_Max_HighX = -0xFFFF;
				pObj->Data_Max_LowX = 0xFFFF;
			}
			break;
        default :
            break;
		}
        WM_UNLOCK();
    }
    return ChangeRange;
}


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
int TracingCurve_SetADDData(TracingCurve_Handle hObj,char flag,GUI_COLOR color,float data,float data2 ,int WaveForm, char doubleflag)
{
    int y,oldy,y1,oldy1;
    int ret=0,phy = 0;
    unsigned int i;
    if (hObj)
    {
        TracingCurve_Obj* pObj;
        WM_LOCK();
        pObj = TracingCurve_H2P(hObj);
        WM_SelectWindow(hObj);
        pObj->Data.loopSign = 'F';

		//  [2/15/2022 yaoyao 测试]
// 		data = 110;
		//
        if(data > pObj->Data_Max_High)//sun add 20140603
        {
            pObj->Data_Max_High = data;
        }
        if(data < pObj->Data_Max_Low)//sun add 20140603
        {
            pObj->Data_Max_Low = data;
        }
        TraceingCurve_NeedChangeRange(hObj, flag,WaveForm);//sun add 20140603


        //一行的结束，将回到首位置在开始刷图
        if(((pObj->xscale)+(pObj->DATAINFO.XDot)) > pObj->DATAINFO.XDotEnd)
        {
            pObj->xscale = 1;
        }
        if((pObj->nStopRemove==1)&&(pObj->xscale==1))
        {
        }
        else
        {   
			if(pObj->Data.DoubleFlag != doubleflag)//djq modify 20170523 修改同一个波形上显示两条曲线的设置
			{
				pObj->Data.DoubleFlag = doubleflag;
			}
			//保存当前数据	//zkq add 20171109
			if(doubleflag == 1)
			{
				//       原点的Y     -          ( 布距对应像素点 * 当前值 / (Y布距 * 范围) ) //cpp ac 20130605
				y1 = (pObj->DATAINFO.YDot)-(int)((pObj->DATAINFO.YDotStep)*data2/((pObj->XYAD_INFO.YStep)*(pObj->Range)));
				if(y1 <pObj->DATAINFO.YDotEnd)  //y是相对于液晶屏左上角原点的值 
				{
					y1 = pObj->DATAINFO.YDotEnd;
					data2 = pObj->XYAD_INFO.YMax*(pObj->Range);
					pObj->ExceedRangeFlg = 1;                                                                                                 
				}
				if(y1 > pObj->DATAINFO.YDotStart)
				{
					y1 = pObj->DATAINFO.YDotStart;
					data2 = pObj->XYAD_INFO.YMin*(pObj->Range);
					pObj->ExceedRangeFlg = 1;
				}
				//上一个点的信息
				if(pObj->xscale == 1)
				{
					oldy1 = y1;
				}
				else
				{    //             原点的Y     -              ( 布距对应像素点 * 前一个点的值 / (Y布距 * 范围) ) //cpp ac 20130605
					oldy1 = (pObj->DATAINFO.YDot)-(int)((pObj->DATAINFO.YDotStep)*((pObj->Data.DataDotInfo+pObj->xscale-1)->Datax)/((pObj->XYAD_INFO.YStep)*(pObj->Range)));
				}
			}

			//       原点的Y     -          ( 布距对应像素点 * 当前值 / (Y布距 * 范围) ) //cpp ac 20130605
            y = (pObj->DATAINFO.YDot)-(int)((pObj->DATAINFO.YDotStep)*data/((pObj->XYAD_INFO.YStep)*(pObj->Range)));
            if(y <pObj->DATAINFO.YDotEnd)  //y是相对于液晶屏左上角原点的值 
            {
                y = pObj->DATAINFO.YDotEnd;
                data = pObj->XYAD_INFO.YMax*(pObj->Range);
                pObj->ExceedRangeFlg = 1;                                                                                                 
            }
            if(y>pObj->DATAINFO.YDotStart)
            {
                y = pObj->DATAINFO.YDotStart;
                data = pObj->XYAD_INFO.YMin*(pObj->Range);
                pObj->ExceedRangeFlg = 1;
            }
            //上一个点的信息
            if(pObj->xscale == 1)
            {
                oldy = y;
            }
            else
            {    //             原点的Y     -              ( 布距对应像素点 * 前一个点的值 / (Y布距 * 范围) ) //cpp ac 20130605
                oldy = (pObj->DATAINFO.YDot)-(int)((pObj->DATAINFO.YDotStep)*((pObj->Data.DataDotInfo+pObj->xscale-1)->Datay)/((pObj->XYAD_INFO.YStep)*(pObj->Range)));
            }
            //WM_SelectWindow(hObj);
            GUI_SetColor(pObj->DIALOGINFO.BKColor);
            //////////////////////////////////////////////////////////////////////////
            //当要求停止刷新数据区时，将pObj->xscale设置为指向末尾的地方
            //Clear Line 8
            for(i=0; i<nTracingCurveClearLine/*TRACINGCURVE_CLEARLINE_DEFAULT*/; i++) //以背景色擦掉8列 //cpp ac 20130605
            {
                GUI_SetColor(pObj->DIALOGINFO.BKColor);
                if((pObj->xscale+pObj->DATAINFO.XDot+i)<=pObj->DATAINFO.XDotEnd)
                {
                    //                  X0                                           Y0                     Y1     画条竖线                     
                    //                                         X轴偏移            Y轴最上的点                      Y轴最下的点
                    GUI_DrawVLine(((pObj->xscale)+(pObj->DATAINFO.XDot))+i,pObj->DATAINFO.YDotEnd-1,pObj->DATAINFO.YDotStart-1);  //djq curve flash
#ifdef WAVE_PAW_ADD_ALARMHIGH
                    //画压力报警上限直线
                    if(!strcmp(pObj->XYAD_INFO.XYText, Sys_Lib_GetDispStr(DP_PAW)))
                    {
                        GUI_SetColor(GUI_RED);
                        phy = (pObj->DATAINFO.YDot)-(int)((pObj->DATAINFO.YDotStep)*(AlarmSettingPara.PressureHighValue)/((pObj->XYAD_INFO.YStep)*(pObj->Range)));
                        //GUI_DrawHLine(phy-1,pObj->DATAINFO.XDotStart+1,pObj->DATAINFO.XDotEnd-1);
                        if(phy >= pObj->DATAINFO.YDotEnd)
                        {
                            GUI_DrawVLine(((pObj->xscale)+(pObj->DATAINFO.XDot))+i,phy-1,phy-1);
                        }
                    }
#endif
                }
                else
                {
                    GUI_DrawVLine((pObj->xscale+pObj->DATAINFO.XDot+pObj->DATAINFO.XDot)+i-pObj->DATAINFO.XDotEnd+1,pObj->DATAINFO.YDotEnd-1,pObj->DATAINFO.YDotStart-1);  //djq curve flash
#ifdef WAVE_PAW_ADD_ALARMHIGH
                    //画压力报警上限直线
                    if(!strcmp(pObj->XYAD_INFO.XYText, Sys_Lib_GetDispStr(DP_PAW)))
                    {
                        GUI_SetColor(GUI_RED);
                        phy = (pObj->DATAINFO.YDot)-(int)((pObj->DATAINFO.YDotStep)*(AlarmSettingPara.PressureHighValue)/((pObj->XYAD_INFO.YStep)*(pObj->Range)));
                        //GUI_DrawHLine(phy-1,pObj->DATAINFO.XDotStart+1,pObj->DATAINFO.XDotEnd-1);
                        if(phy >= pObj->DATAINFO.YDotEnd)
                        {
                            GUI_DrawVLine((pObj->xscale+pObj->DATAINFO.XDot+pObj->DATAINFO.XDot)+i-pObj->DATAINFO.XDotEnd+1,phy-1,phy-1);
                        }
                    }
#endif
                }
            }

            GUI_SetColor(pObj->XYAD_INFO.XYColor);//djq curve flash
            GUI_DrawPoint(((pObj->xscale)+(pObj->DATAINFO.XDot)),pObj->DATAINFO.YDot);//djq curve flash

            GUI_SetColor(color);
            //画出表现数据的方式    0 曲线 1 柱形
            if(pObj->Data.Mode==0) 
            {
                //画曲线的信息
                if((pObj->xscale)==1)
                {
                    GUI_SetPenSize(1);
                    GUI_DrawPoint(((pObj->xscale)+(pObj->DATAINFO.XDot)),y);
					if(doubleflag == 1)//zkq add 20171109
					{
						GUI_SetColor(GUI_GREEN);
						GUI_DrawPoint(((pObj->xscale)+(pObj->DATAINFO.XDot)),y1);
					}
                }
                else
                {
                    GUI_SetPenSize(1);
                    GUI_DrawLine(((pObj->xscale)+(pObj->DATAINFO.XDot))-1,oldy,((pObj->xscale)+(pObj->DATAINFO.XDot)),y);
					if(doubleflag == 1)//zkq add 20171109
					{
						GUI_SetColor(GUI_GREEN);
						GUI_DrawLine(((pObj->xscale)+(pObj->DATAINFO.XDot))-1,oldy1,((pObj->xscale)+(pObj->DATAINFO.XDot)),y1);
					}
                }
            }
            else
            {
                //画柱形的信息
                if(data>=0)
                {
                    GUI_DrawVLine(((pObj->xscale)+(pObj->DATAINFO.XDot)),y,pObj->DATAINFO.YDot);
                }
                else
                {
                    GUI_DrawVLine(((pObj->xscale)+(pObj->DATAINFO.XDot)),pObj->DATAINFO.YDot,y);
                }
            }
            (pObj->Data.DataDotInfo+pObj->xscale)->Datay = data;
			(pObj->Data.DataDotInfo+pObj->xscale)->Datax = data2;
            (pObj->Data.DataDotInfo+pObj->xscale)->DataColor = color;
            pObj->xscale++;
        }
        ret = pObj->xscale;
        //重新画X轴Y轴
        GUI_SetColor(pObj->XYAD_INFO.XYColor);
        if(pObj->XYAD_INFO.XSign=='T')
        {
            GUI_DrawHLine(pObj->DATAINFO.YDotStart,pObj->DATAINFO.XDotStart,pObj->DATAINFO.XDotEnd-1);  //水平方向 
        }
        WM_UNLOCK();
    }
    return ret;
}
#else
void WIDGET_viewWin(void) {} /* avoid empty object files */
#endif /* GUI_WINSUPPORT */


/***********************************************************************
*函数名：TracingTrend_SetTime
*功能：设置本次进入趋势图的时间,用于x轴时间的显示
*参数：pObj:波形图的结构体指针
       RTCTime:实时时间参数
*备注：如果进入趋势图的时间不是时间1个小时内时间间隔的整数倍，则取前一个
    时间间隔点。（最小时间为1小时、360像素，最小时间间隔为1*60*60/360 = 10s）
*
************************************************************************
*/
void TracingTrend_SetTime(TracingCurve_Obj *pObj,RTC_TIMER *RTCTime)
{
    //月，日，时，分，秒
    int interval = ((int)(pObj->DATAINFO.XDotStep)/(int)(pObj->XYAD_INFO.XMax));//1个小时内，每个点的时间间隔
    pObj->TrendStartTime.RTC_Year = RTCTime->RTC_Year;
    pObj->TrendStartTime.RTC_Mon = RTCTime->RTC_Mon;
    pObj->TrendStartTime.RTC_Mday = RTCTime->RTC_Mday;
    pObj->TrendStartTime.RTC_Hour = RTCTime->RTC_Hour;
    pObj->TrendStartTime.RTC_Min = RTCTime->RTC_Min;
    if((RTCTime->RTC_Sec % interval) != 0)
    {
        RTCTime->RTC_Sec -= (RTCTime->RTC_Sec % interval);
    }
    pObj->TrendStartTime.RTC_Sec = RTCTime->RTC_Sec;
}

/***********************************************************************
*函数名：TracingTrend_GetNumber
*功能：查询一个趋势图中已经创建的buffer的个数
*参数：hObj:波形图的句柄
*返回值:0~3   已经创建buffer的个数
*
************************************************************************
*/
int TracingTrend_GetNumber(TracingCurve_Handle hObj)
{
    int ret;
    if (hObj)
    {
        TracingCurve_Obj* pObj;
        WM_LOCK();
        pObj = TracingCurve_H2P(hObj); 
        ret = pObj->TrendNumber;
        WM_UNLOCK();
    }
    return ret;
}
/***********************************************************************
*函数名：TracingTrend_AddPara
*功能：向一个趋势图中添加参数，确定其显示的颜色，并且创建该参数的数据buffer
*参数：hObj:波形图的句柄
       index:buffer的索引值,除了TracingTrend_SetInfo()函数调用可以用0外，
             其他地方调用值为1或者2（建议使用TracingTrend_GetNumber()函数查询当前数量）
       ParaText0:第一个参数的显示字符
       ParaColor0：第一个参数字符和波形的颜色
*返回值:0x55：表示添加失败，动态创建buffer失败
        0xaa：表示添加失败，波形图中已经加了3个参数，已满
        0、1、2：表示添加成功，数据表示添加buffer的索引号
        -1  :表示索引号有误

*备注：因为要动态创建的空间超过内存的容量，所以需要引用外部内存地址（SDRAM）
       或者把程序移植到SDRAM中执行。
*     
************************************************************************
*/
int TracingTrend_AddPara(TracingCurve_Handle hObj,int index,const char* ParaText0,GUI_COLOR ParaColor0)
{
    int ret = 0,cnt;
    int buf_size = 0;
    if (hObj)
    {
        TracingCurve_Obj* pObj;
        WM_LOCK();
        pObj = TracingCurve_H2P(hObj); 
        if(index == 0) //如果波形中第0个buffer为空，则创建buffer，并且将buffer的信息加入到该buffer的结构体Trend_Info中，返回索引号0
        {
            if(pObj->nTrendType == 1)//趋势模式
            {
                if(pObj->TrendBufferSize == sizeof(char))//char
                {
                    if(pObj->Trend_Info[0].ParaDateBuf_Char == NULL)//如果为空则创建
                    {
                        pObj->Trend_Info[0].ParaDateBuf_Char = (char *)malloc(pObj->TrendPixelNum*sizeof(char));      
                        if(pObj->Trend_Info[0].ParaDateBuf_Char == NULL)//创建buffer失败，返回0x55
                        {
                            ret = 0x55; 
                        }
                        else//创建成功 
                        {
                            for(cnt = 0;cnt < pObj->TrendPixelNum;cnt++)//初始化
                            {
                                *(pObj->Trend_Info[0].ParaDateBuf_Char + cnt) = 0;
                            }
                            ret = 0;
                        }
                    }
                    else //buffer不为空，返回-1
                    {
                        ret = -1;
                    }
                }
                else //float
                {
                    if(pObj->Trend_Info[0].ParaDateBuf_Short == NULL)//如果为空则创建
                    {
                        pObj->Trend_Info[0].ParaDateBuf_Short = (short *)malloc(pObj->TrendPixelNum*sizeof(short));                       
                        if(pObj->Trend_Info[0].ParaDateBuf_Short == NULL)//创建buffer失败，返回0x55
                        {
                            ret = 0x55; 
                        }
                        else//创建成功 
                        {
                            for(cnt = 0;cnt < pObj->TrendPixelNum;cnt++)//初始化
                            {
                                *(pObj->Trend_Info[0].ParaDateBuf_Short + cnt) = 0;
                            }
                            ret = 0;
                        }
                    }
                    else //buffer不为空，返回-1
                    {
                        ret = -1;
                    }
                }
            }
                pObj->Trend_Info[0].ParaText = ParaText0;    //第一个参数名显示 ParaText0
                pObj->Trend_Info[0].ParaColor = ParaColor0;
                if(ret == 0)
                {
                    pObj->TrendNumber = 1;
                }
        }
        else  if((index == 1) || (index == 2))//如果不是第一个buffer，则需要WM_LOCK
        {
            if(pObj->nTrendType == 1)//趋势模式
            {
                if(pObj->TrendBufferSize == sizeof(char))
                {
                    if(pObj->Trend_Info[index].ParaDateBuf_Char == NULL)
                    {
                        pObj->Trend_Info[index].ParaDateBuf_Char = (char *)malloc(pObj->TrendPixelNum*sizeof(char));     
                        if(pObj->Trend_Info[index].ParaDateBuf_Char == NULL)//创建buffer失败，返回0x55
                        {
                            ret = 0x55; 
                        }
                        else//创建成功 
                        {
                            for(cnt = 0;cnt < pObj->TrendPixelNum;cnt++)//初始化
                            {
                                *(pObj->Trend_Info[index].ParaDateBuf_Char + cnt) = 0;
                            }
                            ret = index;
                        }
                    }
                    else 
                    {
                        ret = -1;
                    }
                }
                else 
                {
                    if(pObj->Trend_Info[index].ParaDateBuf_Short == NULL)
                    {
                        pObj->Trend_Info[index].ParaDateBuf_Short = (short *)malloc(pObj->TrendPixelNum*sizeof(short));                       
//                        pObj->Trend_Info[index].ParaDateBuf_Short = (float *)(0xa0000000+index*3000);                       
                        if(pObj->Trend_Info[index].ParaDateBuf_Short == NULL)//创建buffer失败，返回0x55
                        {
                            ret = 0x55; 
                        }
                        else//创建成功 
                        {
                            for(cnt = 0;cnt < pObj->TrendPixelNum;cnt++)//初始化
                            {
                                *(pObj->Trend_Info[index].ParaDateBuf_Short + cnt) = 0;
                            }
                            ret = index;
                        }
                    }
                    else 
                    {
                        ret = -1;
                    }
                }
                if((ret == 1) || (ret == 2))
                {
                    pObj->Trend_Info[index].ParaText = ParaText0;    //第一个参数名显示 ParaText0
                    pObj->Trend_Info[index].ParaColor = ParaColor0;
                    pObj->TrendNumber++;
                }
            }
        }
        else
        {
            ret = -1;
        }
        WM_UNLOCK();
    }
    //如果成功创建了buffer，则应该将相应的趋势图参数结构体Trend_Info补全
    return ret;
}


/***********************************************************************
*函数名：TracingTrend_SetInfo
*功能：将创建好的波形图，设置为Trend类型
*参数：hObj:波形图的句柄
       TimeSign:时间轴是否显示标志（T：显示，F:不显示）
       TimeRange:时间轴范围，单位为小时（当前趋势图）
       YMin:Y轴最小值
       YMax:Y轴最大值
       nYFloatLen:Y轴显示数据浮点位长度，0为整数，1为1位小数
       PixelNum:X轴（时间轴）对应的像素点数（也是开辟数据buff的大小）
       BufferSize:图形数据buufer类型，暂时只支持char和short类型
       ParaText0:第一个参数的显示字符
       ParaColor0：第一个参数字符和波形的颜色
*备注：如果为小数数据，则数据的最大值和Buffer数据输入值应该为扩大10倍的数
*
************************************************************************
*/
void TracingTrend_SetInfo(TracingCurve_Handle hObj,signed TimeSign,float TimeRange,float YMin,float YMax,int nYFloatLen,int BufferSize, int PixelNum,const char* ParaText0,GUI_COLOR ParaColor0)
{
    if (hObj)
    {
        TracingCurve_Obj* pObj;
        WM_LOCK();
        pObj = TracingCurve_H2P(hObj);
        //第一个参数独有的变量     
        pObj->nTrendType = 1;            //设置为趋势图类型
        //当前趋势图中通用的变量
        pObj->XYAD_INFO.XSign = TimeSign;
        pObj->TrendPixelNum = PixelNum;  //当前趋势图，buffer的大小
        pObj->lastxscan = pObj->TrendPixelNum - 1;   //lastxscan和xscale两个变量不一致的时候表示光标被移动了。
        pObj->xscale = pObj->TrendPixelNum - 1;//0 ~ PixelNum-1
//        if(TimeSign == 'T')
        {
            pObj->XYAD_INFO.XMax = TimeRange; //时间范围 TimeRange
            pObj->DATAINFO.XDotStep = (TimeRange*60*60)/PixelNum;//x轴每点间，时间的步进 = 时间/点数
            TracingTrend_SetTime(pObj,&RTCTime);//本次进入趋势图时间 TracingTrend_SetTime()
        }
        pObj->XYAD_INFO.YMax = YMax;     //当前趋势图Y轴最大值
        pObj->XYAD_INFO.YMin = YMin;     //当前趋势图Y轴最小值
        pObj->XYAD_INFO.nYFloatLen = nYFloatLen;  //当前趋势图，参数显示小数的位数
        pObj->TrendBufferSize = BufferSize;

        //if(pObj->XYAD_INFO.nYFloatLen == 0)//一个字节buffer大小
        //{
        //    if(pObj->XYAD_INFO.YMax <= 0xFF)
        //    {
        //        pObj->TrendBufferSize = sizeof(char);
        //    }
        //    else 
        //    {
        //        pObj->TrendBufferSize = sizeof(short);
        //    }
        //}
        //else if(pObj->XYAD_INFO.nYFloatLen)
        //{
        //    if(pObj->XYAD_INFO.YMax <= (float)(0xFF/10.0))
        //    {
        //        pObj->TrendBufferSize = sizeof(char);
        //    }
        //    else 
        //    {
        //        pObj->TrendBufferSize = sizeof(short);
        //    }
        //}
        TracingTrend_AddPara(hObj,0,ParaText0,ParaColor0);//初始化趋势图中第一个参数的名称、颜色和buffer
        TracingCurve_Invalidate(hObj);
        WM_UNLOCK();
    }
}


/***********************************************************************
*函数名：TracingTrend_SetRange
*功能：改变某个趋势的量程范围,并且刷新显示
*参数：hObj:波形图的句柄
       TimeRange:时间轴范围，单位为小时（当前趋势图）
*备注:
*
************************************************************************
*/
void TracingTrend_SetRange(TracingCurve_Handle hObj,float TimeRange)
{
    if (hObj)
    {
        TracingCurve_Obj* pObj;
        WM_LOCK();
        pObj = TracingCurve_H2P(hObj);
        pObj->XYAD_INFO.XMax = TimeRange; //时间范围 TimeRange
        pObj->DATAINFO.XDotStep = (TimeRange*60*60)/pObj->TrendPixelNum;//x轴每点间，时间的步进 = 时间/点数
        pObj->TrendPaintFlag = 1;
        TracingTrend_SetTime(pObj,&RTCTime);//本次进入趋势图时间 TracingTrend_SetTime()
        TracingCurve_Invalidate(hObj);
        WM_UNLOCK();
    }
}
/***********************************************************************
*函数名：TracingTrend_GetRange
*功能：获取某个趋势图的时间范围
*参数：hObj:波形图的句柄
*备注：
*
************************************************************************
*/
int TracingTrend_GetRange(TracingCurve_Handle hObj)
{
    int TimeRange;
    if (hObj)
    {
        TracingCurve_Obj* pObj;
        WM_LOCK();
        pObj = TracingCurve_H2P(hObj);
        TimeRange = (int)(pObj->XYAD_INFO.XMax); //时间范围 TimeRange
        WM_UNLOCK();
    }
    return TimeRange;
}

/***********************************************************************
*函数名：TracingTrend_PaintCurve
*功能：设置趋势图刷新标志
*参数：hObj:波形图的句柄
*备注:
*
************************************************************************
*/
void TracingTrend_PaintFlag(TracingCurve_Handle hObj,unsigned char Flag)
{
    if (hObj)
    {
        TracingCurve_Obj* pObj;
        WM_LOCK();
        pObj = TracingCurve_H2P(hObj);
        pObj->TrendPaintFlag = Flag;
        WM_UNLOCK();
    }
}

/***********************************************************************
*函数名：TracingTrend_FullBuffer
*功能：更新一个buffer中的数据
*参数：hObj:波形图的句柄
       index:将要更新的buffer的索引号
       buffer:要显示的数据buffer
*返回值：0：成功
         1：失败，没有该索引值对应的buffer
*
************************************************************************
*/
int TracingTrend_FullBuffer(TracingCurve_Handle hObj,int index,void * buffer)
{
    int ret = 0;
    int cnt = 0;
    if (hObj)
    {
        TracingCurve_Obj* pObj;
        WM_LOCK();
        pObj = TracingCurve_H2P(hObj);
        if(pObj->nTrendType == 1)//如果该句柄为趋势图
        {
            if(index < pObj->TrendNumber)//索引号码应该小于已经创建的buffer数
            {
                //buffer指针类型转换，建议使用使用memcpy()函数填充buffer
                if(pObj->TrendBufferSize == sizeof(char))
                {
                    if(pObj->Trend_Info[index].ParaDateBuf_Char != NULL)  //查看当前的索引号是否有对应的buffer，没有则返回1，有则填满buffer
                    {
                        for(cnt = 0;cnt < pObj->TrendPixelNum; cnt++)
                        {
                            pObj->Trend_Info[index].ParaDateBuf_Char[cnt] = *((char *)(buffer) + cnt);
                        }
                    }
                    else 
                    {
                        ret = 1;
                    }
                }
                else 
                {
                    if(pObj->Trend_Info[index].ParaDateBuf_Short != NULL)  //查看当前的索引号是否有对应的buffer，没有则返回1，有则填满buffer
                    {
                        for(cnt = 0;cnt < pObj->TrendPixelNum; cnt++)
                        {
                            pObj->Trend_Info[index].ParaDateBuf_Short[cnt] = *((short *)(buffer) + cnt);
                        }
                    }
                    else 
                    {
                        ret = 1;
                    }
                }
            }
            else 
            {
                ret = 1;
            }
            TracingCurve_Invalidate(hObj);  //会死机，暂时没有找到解决办法
        }
        WM_UNLOCK();
    }
    return ret;
}
/***********************************************************************
*函数名：TracingTrend_CursorMove
*功能：光标移动一位，或者移动到一个固定的位置
*参数：hObj:波形图的句柄
       flag:0:将光标向右(position=1)，或者向左(position=0)移动一个位
            1:将光标移动到指定位置(position)
       position:光标移动的方向，或者是将要移动到的位置

*备注：如果快速移动，则需要快速的刷新显示，那么显示会晃动。当移动速度过快
       时，可以做一个定时器，每隔一个时间才刷新一次
*
************************************************************************
*/
void TracingTrend_CursorMove(TracingCurve_Handle hObj,int flag,int position)
{
    if (hObj)
    {
        TracingCurve_Obj* pObj;
        WM_LOCK();
        pObj = TracingCurve_H2P(hObj);
        if(pObj->nTrendType == 1)//为趋势图
        {
            if(flag == 0)  //左移、右移一位
            {
                if(position == 0)//左移
                {
                    if(pObj->xscale > 0)
                    {
                        pObj->xscale--;
                    }
                    else 
                    {
                        pObj->xscale = pObj->TrendPixelNum - 1;
                    }
                }
                else if(position == 1)//右移
                {
                    if(pObj->xscale < pObj->TrendPixelNum - 1)
                    {
                        pObj->xscale++;
                    }
                    else 
                    {
                        pObj->xscale = 0;
                    }
                }
            }
            else //移动到指定位置
            {
                pObj->xscale = Trend_Xsacn;
            }
        }
        TracingCurve_Invalidate(hObj);
        WM_UNLOCK();
    }
}

/***********************************************************************
*函数名：TracingTrend_FreeBffer
*功能：删除趋势图之前，先将趋势图中创建的buffer动态释放s
*参数：hObj:波形图的句柄
*返回值：0：成功
         1：失败
*
************************************************************************
*/
int TracingTrend_FreeBffer(TracingCurve_Handle hObj)
{
    int ret = 0;
    if (hObj)
    {
        TracingCurve_Obj* pObj;
        WM_LOCK();
        pObj = TracingCurve_H2P(hObj);
        if(pObj->nTrendType == 1)//为趋势图
        {
            if(pObj->TrendBufferSize == sizeof(char))
            {
                if(pObj->Trend_Info[0].ParaDateBuf_Char != NULL)  //buffer0不为空，则释放buffer0
                {
                    free(pObj->Trend_Info[0].ParaDateBuf_Char);
                    pObj->Trend_Info[0].ParaDateBuf_Char = NULL;
                }
                if(pObj->Trend_Info[1].ParaDateBuf_Char != NULL)  //buffer1不为空，释放buffer1
                {
                    free(pObj->Trend_Info[1].ParaDateBuf_Char);
                    pObj->Trend_Info[1].ParaDateBuf_Char = NULL;
                }
                if(pObj->Trend_Info[2].ParaDateBuf_Char != NULL)  //buffer2不为空，释放buffer
                {
                    free(pObj->Trend_Info[2].ParaDateBuf_Char);
                    pObj->Trend_Info[1].ParaDateBuf_Char = NULL;
                }
                if((pObj->Trend_Info[0].ParaDateBuf_Char != NULL) 
                    || (pObj->Trend_Info[1].ParaDateBuf_Char != NULL) 
                    || (pObj->Trend_Info[2].ParaDateBuf_Char != NULL))   //全部为空，则返回0,不全为空，返回1
                {
                    return 1;
                }                   
            }
            else 
            {
                if(pObj->Trend_Info[0].ParaDateBuf_Short != NULL)  //buffer0不为空，则释放buffer0
                {
                    free(pObj->Trend_Info[0].ParaDateBuf_Short);
                    pObj->Trend_Info[0].ParaDateBuf_Short = NULL;
                }
                if(pObj->Trend_Info[1].ParaDateBuf_Short != NULL)  //buffer1不为空，释放buffer1
                {
                    free(pObj->Trend_Info[1].ParaDateBuf_Short);
                    pObj->Trend_Info[1].ParaDateBuf_Short = NULL;
                }
                if(pObj->Trend_Info[2].ParaDateBuf_Short != NULL)  //buffer2不为空，释放buffer
                {
                    free(pObj->Trend_Info[2].ParaDateBuf_Short);
                    pObj->Trend_Info[2].ParaDateBuf_Short = NULL;
                }
                if((pObj->Trend_Info[0].ParaDateBuf_Short != NULL) 
                    || (pObj->Trend_Info[1].ParaDateBuf_Short != NULL) 
                    || (pObj->Trend_Info[2].ParaDateBuf_Short != NULL))   //全部为空，则返回0,不全为空，返回1
                {
                    return 1;
                }          
            }

            //TracingCurve_Invalidate(hObj);
        }
        WM_UNLOCK();
    }
    return ret;
}


/***********************************************************************
*函数名：TracingTrend_SetAAFlag
*功能：设置当前趋势图为麻醉气体
*参数：hObj:波形图的句柄
*
************************************************************************
*/
void TracingTrend_SetAAFlag(TracingCurve_Handle hObj,int flag)
{
    if (hObj)
    {
        TracingCurve_Obj* pObj;
        WM_LOCK();
        pObj = TracingCurve_H2P(hObj);
        pObj->AA_Flag = flag;
        TracingCurve_Invalidate(hObj);
        WM_UNLOCK();
    }
}
/***********************************************************************
*函数名：Trend_GetTime
*功能：
*参数：hObj:波形图的句柄
*返回值：
         
*
************************************************************************
*/
//返回29：闰年   返回28：平年
static int LeapYear(int year)
{
    int ret;
    if(((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0))
    {
        ret = 29;
    }
    else 
    {
        ret = 28;
    }
    return ret;
}

RTC_TIMER Trend_GetTime(TracingCurve_Obj* pObj)
{
    RTC_TIMER ret_time;
    int tmp= (pObj->TrendPixelNum - (pObj->xscale + 1))*pObj->DATAINFO.XDotStep ;//当前光标时间点 和 当前时间点的时间间隔
    if((tmp%60) <= pObj->TrendStartTime.RTC_Sec)
    {
        ret_time.RTC_Sec  = pObj->TrendStartTime.RTC_Sec - tmp%60;
        tmp = tmp/60;//Min
    }
    else  
    {
        ret_time.RTC_Sec  = 60 + pObj->TrendStartTime.RTC_Sec - tmp%60;
        tmp = tmp/60 + 1;
    }

    if((tmp > 0) && ((tmp % 60) <= pObj->TrendStartTime.RTC_Min))
    {
        ret_time.RTC_Min  = pObj->TrendStartTime.RTC_Min - tmp%60;
        tmp = tmp/60;//Hour
    }
    else if(tmp > 0)
    {
        ret_time.RTC_Min  = 60 + pObj->TrendStartTime.RTC_Min - tmp%60;
        tmp = tmp/60 + 1;//Hour
    }
    else if(tmp == 0)
    {
        ret_time.RTC_Min  = pObj->TrendStartTime.RTC_Min;
        ret_time.RTC_Hour = pObj->TrendStartTime.RTC_Hour;
        ret_time.RTC_Mday = pObj->TrendStartTime.RTC_Mday;
        ret_time.RTC_Mon  = pObj->TrendStartTime.RTC_Mon;
        ret_time.RTC_Year = pObj->TrendStartTime.RTC_Year;
    }

    if((tmp > 0) && ((tmp % 24) <= pObj->TrendStartTime.RTC_Hour))
    {
        ret_time.RTC_Hour  = pObj->TrendStartTime.RTC_Hour - tmp%24;
        tmp = tmp/24;//day
    }
    else if(tmp > 0)
    {
        ret_time.RTC_Hour  = 24 + pObj->TrendStartTime.RTC_Hour - tmp%24;
        tmp = tmp/24 + 1;//day
    }
    else if(tmp == 0)
    {
        ret_time.RTC_Hour = pObj->TrendStartTime.RTC_Hour;
        ret_time.RTC_Mday = pObj->TrendStartTime.RTC_Mday;
        ret_time.RTC_Mon  = pObj->TrendStartTime.RTC_Mon;
        ret_time.RTC_Year = pObj->TrendStartTime.RTC_Year;
    }
    if((tmp > 0) && ((tmp + 1) <= pObj->TrendStartTime.RTC_Mday))
    {
        ret_time.RTC_Mday  = pObj->TrendStartTime.RTC_Mday - tmp;
        ret_time.RTC_Mon   = pObj->TrendStartTime.RTC_Mon;
        ret_time.RTC_Year  = pObj->TrendStartTime.RTC_Year;
    }
    else if(tmp > 0)
    {
        //1、3、5、7、8、10、12为闰月，31天
        if((pObj->TrendStartTime.RTC_Mon == 2) || (pObj->TrendStartTime.RTC_Mon == 4) 
         ||(pObj->TrendStartTime.RTC_Mon == 8) || (pObj->TrendStartTime.RTC_Mon == 9)
         ||(pObj->TrendStartTime.RTC_Mon == 11)|| (pObj->TrendStartTime.RTC_Mon == 1))
        {
            ret_time.RTC_Mday  = 31 + pObj->TrendStartTime.RTC_Mday - tmp;
            if(pObj->TrendStartTime.RTC_Mon == 1)
            {
                ret_time.RTC_Mon = 12;
                ret_time.RTC_Year  = pObj->TrendStartTime.RTC_Year - 1;
            }
            else
            {
                ret_time.RTC_Mon = pObj->TrendStartTime.RTC_Mon - 1;
                ret_time.RTC_Mday  = pObj->TrendStartTime.RTC_Year;
            }
        }
        //4、6、9、11为30天
        else if((pObj->TrendStartTime.RTC_Mon == 5) || (pObj->TrendStartTime.RTC_Mon == 7) 
              ||(pObj->TrendStartTime.RTC_Mon == 10) || (pObj->TrendStartTime.RTC_Mon == 12))
        {
            ret_time.RTC_Mday  = 30 + pObj->TrendStartTime.RTC_Mday - tmp;
            ret_time.RTC_Mon   = pObj->TrendStartTime.RTC_Mon - 1;
            ret_time.RTC_Mday  = pObj->TrendStartTime.RTC_Year;

        }
        else if(pObj->TrendStartTime.RTC_Mon == 2)//2月份，分平年和闰年 
        {
            ret_time.RTC_Mday  = LeapYear(pObj->TrendStartTime.RTC_Year) + pObj->TrendStartTime.RTC_Mday - tmp;//判断平闰年
            ret_time.RTC_Mon   = pObj->TrendStartTime.RTC_Mon - 1;
            ret_time.RTC_Mday  = pObj->TrendStartTime.RTC_Year;
        }
    }
    else if(tmp == 0)
    {
        ret_time.RTC_Mday = pObj->TrendStartTime.RTC_Mday;
        ret_time.RTC_Mon  = pObj->TrendStartTime.RTC_Mon;
        ret_time.RTC_Year = pObj->TrendStartTime.RTC_Year;
    }
    return (ret_time);
}

