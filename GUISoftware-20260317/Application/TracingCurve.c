/*
****************************************************************************************************
标题:  TracingCurve.c
功能:  波形显示
说明:  能实现曲线、时序等的显示，能对坐标、标尺等属性进行配置
当前版本： 0.1.0
修改记录：
****************************************************************************************************
*/
#include "GUI_Protected.h"
#include "TracingCurve.h"
#include "Alarm_handle.h"
#include "Static_Menu_Creat.h"
#include "MasimoSPO2.h"

int nTracingCurveClearLine;//  [2/17/2022 yaoyao add]数据被刷新区域的线数

extern int Trend_Xsacn;  //趋势图中，光标的位置
#define TREND_COORDINATEXSIGN_COLOR    (0xFB9E85) //趋势图显示时间轴的时候，X轴坐标的颜色
#define TREND_COORDINATE_COLOR         (GUI_WHITE)//趋势图坐标轴颜色


#if GUI_WINSUPPORT
#ifndef TRACINGCURVE_BKCOLOR_DEFAULT
#define TRACINGCURVE_BKCOLOR_DEFAULT        GUI_BLACK //0x393c39 //0xFFEDD9
#endif
#ifndef TRACINGCURVE_DEFAULT_FONT
#define TRACINGCURVE_DEFAULT_FONT           &GUI_Font_16//GUI_Font_15
#endif
#define TRACINGCURVE_DOTX                   45//35      //左边界  //cpp md 20130423 
#define TRACINGCURVE_DEFAULT_XSIGN          'T'         //X轴标志 T&F 真 假
#define TRACINGCURVE_DEFAULT_YSIGN          'T'         //X轴标志 T&F 真 假
#define TRACINGCURVE_DEFAULT_XFLOATSIGN     'F'         //X轴浮点标志  T&F 
#define TRACINGCURVE_DEFAULT_XFLOATLEN      0           //X轴浮点为长度
#define TRACINGCURVE_DEFAULT_XMIN           '0'         //X轴最小值
#define TRACINGCURVE_DEFAULT_XMAX           14          //X轴最大值
#define TRACINGCURVE_DEFAULT_XSTEP          1           //X步距
#define TRACINGCURVE_DEFAULT_XCOLOR         GUI_BLACK   //X轴值的颜色
#define TRACINGCURVE_DEFAULT_XTRENDTEXT     ""          //X轴标注，为趋势波形时，显示在原点位置的信息
#define TRACINGCURVE_DEFAULT_XTEXT          "sec."      //X轴标注
#define TRACINGCURVE_DEFAULT_YFLOATSIGN     'F'         //Y轴浮点标志
#define TRACINGCURVE_DEFAULT_YFLOATLEN      0           //Y轴浮点为长度
#define TRACINGCURVE_DEFAULT_YMIN           -10         //Y轴最小值
#define TRACINGCURVE_DEFAULT_YMAX           60          //Y轴最大值
#define TRACINGCURVE_DEFAULT_YSTEP          10          //Y步距
#define TRACINGCURVE_DEFAULT_YCOLOR         GUI_BLACK   //Y轴值的颜色
#define TRACINGCURVE_DEFAULT_YTEXT          "cmH2O"     //Y轴标注
#define TRACINGCURVE_DEFAULT_XYCOLOR        GUI_WHITE   //xy轴颜色 djq curve flash
#define TRACINGCURVE_DEFAULT_XYTEXTCOLOR    GUI_WHITE   //图右上角的TEXT颜色
#define TRACINGCURVE_DEFAULT_XYTEXT         "PAW"       //图右上角的TEXT
//棒图
#define TRACINGCURVE_DEFAULT_SPO2BAR        0           //不显示棒图
#define TRACINGCURVE_DEFAULT_FRAMECOLOR     GUI_BLACK   //边框颜色
#define TRACINGCURVE_DEFAULT_SPO2Y          5           //边框顶边界
#define TRACINGCURVE_DEFAULT_SPO2PROCOLR    GUI_BLUE    //棒的颜色
#define TRACINGCURVE_DEFAULT_SPO2COUNT      0           //棒的大小
#define TRACINGCURVE_DEFAULT_SPO2MAX        29          //棒图区域等分
#define TRACINGCURVE_DEFAULT_FONT_INDEX 0
#define TRACINGCURVE_SMALL_FONT_INDEX   1
#define TRACINGCURVE_DEFAULT_PENSIZE    1       //画线时线的宽度
#define TRACINGCURVE_DEFAULT_MODE       0       //数据表现的模式   0:  曲线 1:  柱形    
#define TRACINGCURVE_DEFAULT_FULLSCAN   1
//
static  int keySign = 0;              //只响应一次左键弹起消息

extern ALARM_SET_PARA AlarmSettingPara;
extern unsigned char SpO2_Choose;//1：国产   2：Masimo

static const TracingCurve_XYAxesData xyAxes_DEFAULT_Data =
{
    TRACINGCURVE_DEFAULT_XSIGN,
    TRACINGCURVE_DEFAULT_XFLOATSIGN,
    TRACINGCURVE_DEFAULT_XFLOATLEN,
    TRACINGCURVE_DEFAULT_XMIN,
    TRACINGCURVE_DEFAULT_XMAX,
    TRACINGCURVE_DEFAULT_XSTEP,
    TRACINGCURVE_DEFAULT_XCOLOR,
    TRACINGCURVE_DEFAULT_XTRENDTEXT,
    TRACINGCURVE_DEFAULT_XTEXT,
    
    TRACINGCURVE_DEFAULT_YFLOATSIGN,
    TRACINGCURVE_DEFAULT_YFLOATLEN,
    TRACINGCURVE_DEFAULT_YMIN,
    TRACINGCURVE_DEFAULT_YMAX,
    TRACINGCURVE_DEFAULT_YSTEP,
    TRACINGCURVE_DEFAULT_YCOLOR,
    TRACINGCURVE_DEFAULT_YTEXT,
    
    TRACINGCURVE_DEFAULT_XYCOLOR,
    TRACINGCURVE_DEFAULT_XYTEXT,
    TRACINGCURVE_DEFAULT_FULLSCAN,

    TRACINGCURVE_DEFAULT_YSIGN
};

//
static float _Fraction(int x,int y,float z)
{
    return ((x-y+1)*z);
}

//x的n次方      *result初始值为1
static void square(int x, int n, int *result)
{
    if (n > 0)
    {
        (*result) *= x;
        square(x, --n, &(*result));
    }
}

/***********************************************************************************************
*       _DrawPaintTrendCoordinate
*
*       功能：通过趋势图的坐标，X轴的像素点，Y轴的高度等信息，绘制出坐标轴，并且将XY轴对应的结构体回填
        扩展功能：只有在第一次进入或者是切换量程时才重新绘制坐标轴
                  移动光标时，擦除原来的光标，并且补全被擦掉的坐标和网格(lastxscan和xscale两个变量不一致的时候表示光标被移动了)
*/
#define Y_Top       15       ///Y轴上侧留下10个像素
#define Y_Bottom    12       ///Y轴下侧留下10个像素
#define Date_X      10       ///日期距离X轴的边距距离
#define Para_X      80       ///参数距离X轴的边距 50
#define Coordinate_Color        GUI_WHITE
#define Coordinate_XSign_Color  0xFB9E85
#define Gridding_Color          0x333333
void _DrawPaintTrendCoordinate(TracingCurve_Obj* pObj)
{
    //圆点位置
    unsigned char hour,min;
    float dotX,dotY;
    float NewXStep,NewYStep;                                                        //计算单位长度
    float NewYTemp,NewXTemp;
    int result = 1;
    int cnt;
    RTC_TIMER Disp_Time = {1,2,3,4,5,6};
    char time[20];
    GUI_RECT r;
    WM_GetWindowRect(&r);  //整个趋势图窗口的可显示区域
    dotX = TRACINGCURVE_DOTX-18; //X轴原点坐标                                                   
    dotY = r.y1 - r.y0 - Y_Bottom; //Y轴原点坐标,    
    pObj->DATAINFO.XDotStart = dotX;
    pObj->DATAINFO.YDotStart = dotY;
    pObj->DATAINFO.YDotStep  = (dotY - Y_Top)/pObj->XYAD_INFO.YMax;
    //GUI_SetFont(TRACINGCURVE_DEFAULT_FONT);
	if(LanguageType == Disp_Lg_Chinese)//趋势图右侧字体
	{
		GUI_SetFont(&GUI_Font_15);
	}
	else
	{
		GUI_SetFont(&GUI_Font_16);
	}
    //未移动光标，可能是创建，也可能是切换量程，也可能是非法重绘
	if((pObj->lastxscan == pObj->xscale)||(pObj->TrendPaintFlag == 1)) ///初始化绘制的图形
	{
		//绘制网格******************************
		GUI_SetColor(Gridding_Color); 
		//水平网格
		GUI_DrawHLine(Y_Top,dotX,dotX + pObj->TrendPixelNum);                  
		GUI_DrawHLine((dotY-Y_Top)/4+Y_Top,dotX,dotX + pObj->TrendPixelNum);                  
		GUI_DrawHLine((dotY-Y_Top)/4*2+Y_Top,dotX,dotX + pObj->TrendPixelNum);                   
		GUI_DrawHLine((dotY-Y_Top)/4*3+Y_Top,dotX,dotX + pObj->TrendPixelNum);                   
		//垂直网格
		GUI_DrawVLine(dotX+pObj->TrendPixelNum/2,Y_Top,dotY);                               
		GUI_DrawVLine(dotX+pObj->TrendPixelNum,Y_Top,dotY);                               

		//绘制坐标******************************
		GUI_SetColor(Coordinate_Color);
		//Y 轴坐标轴
		GUI_DrawVLine(dotX-1,Y_Top,dotY);         
		//Y轴坐标分割点
		GUI_DrawHLine(Y_Top,dotX-3,dotX-1);                  
		GUI_DrawHLine((dotY-Y_Top)/4+Y_Top,dotX-3,dotX-1);                   
		GUI_DrawHLine((dotY-Y_Top)/4*2+Y_Top,dotX-3,dotX-1);                 
		GUI_DrawHLine((dotY-Y_Top)/4*3+Y_Top,dotX-3,dotX-1);         
        //Y轴坐标最大值,坐标轴最大值，暂时支持整数
        GUI_SetColor(Coordinate_Color);
        if(pObj->XYAD_INFO.nYFloatLen == 0)//整数
        {
            GUI_GotoXY(0,Y_Top-GUI_GetFontDistY());  
            GUI_DispDecSpace(pObj->XYAD_INFO.YMax,4);
        }
        else //小数
        {
            GUI_GotoXY(0,Y_Top-GUI_GetFontDistY());
            GUI_DispFloatMin((pObj->XYAD_INFO.YMax)/(pObj->XYAD_INFO.nYFloatLen*10.0),1);
        }

		//X轴坐标轴
		if(pObj->XYAD_INFO.XSign == 'T')//显示X轴（时间轴）坐标和光标所在位置的时间
		{
			//显示时间轴上的时间点，3个      
			GUI_SetColor(Coordinate_Color);
			if(pObj->TrendStartTime.RTC_Hour < (int)(pObj->XYAD_INFO.XMax)%24)
			{
				hour = 24 + pObj->TrendStartTime.RTC_Hour - (int)(pObj->XYAD_INFO.XMax)%24;
			}
			else 
			{
				hour = pObj->TrendStartTime.RTC_Hour - (int)(pObj->XYAD_INFO.XMax)%24;
			}
			GUI_DispDecAt(hour,dotX-5,dotY,2);//时间轴起始
			GUI_DispChar(':');
			GUI_DispDec(pObj->TrendStartTime.RTC_Min,2);

			//中点
			cnt = (int)(pObj->XYAD_INFO.XMax);
			if(cnt%2 == 0)
			{   
				cnt = (cnt/2)%24;
				if(pObj->TrendStartTime.RTC_Hour < cnt)
				{
					hour = 24 + pObj->TrendStartTime.RTC_Hour - cnt;
				}
				else 
				{
					hour = pObj->TrendStartTime.RTC_Hour - cnt;
				}
				GUI_DispDecAt(hour,dotX-14+pObj->TrendPixelNum/2,dotY,2);//时间轴中间
				GUI_DispChar(':');
				GUI_DispDec(pObj->TrendStartTime.RTC_Min,2);
			}
			else 
			{
				cnt = (cnt/2)%24;
				if(pObj->TrendStartTime.RTC_Min >= 30)
				{
					if(pObj->TrendStartTime.RTC_Hour < cnt)
					{
						hour = 24 + pObj->TrendStartTime.RTC_Hour - cnt;
					}
					else 
					{
						hour = pObj->TrendStartTime.RTC_Hour - cnt;
					}
					GUI_DispDecAt(hour,dotX-17+pObj->TrendPixelNum/2,dotY,2);//时间轴中间
					GUI_DispChar(':');
					GUI_DispDec(pObj->TrendStartTime.RTC_Min-30,2);
				}
				else 
				{
					if(pObj->TrendStartTime.RTC_Hour < cnt+1)
					{
						hour = 24 + pObj->TrendStartTime.RTC_Hour - cnt - 1;
					}
					else 
					{
						hour = pObj->TrendStartTime.RTC_Hour - cnt - 1;
					}
					GUI_DispDecAt(hour,dotX-17+pObj->TrendPixelNum/2,dotY,2);//时间轴中间
					GUI_DispChar(':');
					GUI_DispDec(60 + pObj->TrendStartTime.RTC_Min-30,2);
				}
			}

			GUI_DispDecAt(pObj->TrendStartTime.RTC_Hour,dotX-32+pObj->TrendPixelNum,dotY,2);//时间轴末端
			GUI_DispChar(':');
			GUI_DispDec(pObj->TrendStartTime.RTC_Min,2);

			GUI_SetColor(Coordinate_XSign_Color);
		}
		else //不显示X轴（时间轴）
		{
			GUI_SetColor(Coordinate_Color);
		}
		GUI_DrawHLine(dotY,dotX-3,dotX + pObj->TrendPixelNum);        
	   
		//显示趋势图的参数名
        
		for(cnt = 0;cnt < pObj->TrendNumber;cnt++) //重绘参数名
		{
			GUI_SetColor(pObj->Trend_Info[cnt].ParaColor);
			GUI_DispStringAt(pObj->Trend_Info[cnt].ParaText,dotX+pObj->TrendPixelNum+10/*6-6*/,cnt*(dotY-Y_Top)/4+Y_Top);
			GUI_DispStringInRectWrap(pObj->Trend_Info[cnt].ParaText, &r, GUI_TA_LEFT,GUI_WRAPMODE_WORD);
		}
        
	}
	else //移动光标绘制的图形
	{
		//擦除每次重绘的字体（当前点的时间和参数值）
	    GUI_SetColor(pObj->DIALOGINFO.BKColor); 
		GUI_FillRect(dotX+pObj->TrendPixelNum+Para_X, Y_Top,dotX+r.x1-5, (dotY-Y_Top)/4*3+Y_Top);//参数
        if(pObj->XYAD_INFO.XSign == 'T')
        {
		    GUI_FillRect(dotX+pObj->TrendPixelNum+55 - 20,dotY,r.x1, r.y1);//日期
        }

    	//擦除光标,并且在绘制BUFF中重绘
        GUI_SetColor(pObj->DIALOGINFO.BKColor);
        GUI_DrawVLine(pObj->lastxscan+pObj->DATAINFO.XDotStart,0,pObj->DATAINFO.YDotStart);     


		//绘制网格******************************
		GUI_SetColor(Gridding_Color);
		//水平网格
        GUI_DrawPoint(pObj->lastxscan+pObj->DATAINFO.XDotStart,Y_Top);
        GUI_DrawPoint(pObj->lastxscan+pObj->DATAINFO.XDotStart,(dotY-Y_Top)/4+Y_Top);
        GUI_DrawPoint(pObj->lastxscan+pObj->DATAINFO.XDotStart,(dotY-Y_Top)/4*2+Y_Top);
        GUI_DrawPoint(pObj->lastxscan+pObj->DATAINFO.XDotStart,(dotY-Y_Top)/4*3+Y_Top);
		//垂直网格
		if(pObj->lastxscan == pObj->TrendPixelNum/2)
		{
			GUI_DrawVLine(dotX+pObj->TrendPixelNum/2,Y_Top,dotY);                               
		}
		else if(pObj->lastxscan == pObj->TrendPixelNum)
		{
			GUI_DrawVLine(dotX+pObj->TrendPixelNum,Y_Top,dotY);                               
		}

	}
	//显示趋势图的参数值
    for(cnt = 0;cnt < pObj->TrendNumber;cnt++) //重绘参数值
    {
        GUI_SetColor(pObj->Trend_Info[cnt].ParaColor);
        if(pObj->XYAD_INFO.nYFloatLen == 0)
        {
            if(pObj->TrendBufferSize == sizeof(char))//char 
            {
                GUI_GotoXY(dotX+pObj->TrendPixelNum+Para_X,cnt*(dotY-Y_Top)/4+Y_Top);
                GUI_DispDecSpace(*(pObj->Trend_Info[cnt].ParaDateBuf_Char + pObj->xscale),4);
            }
            else //short
            {
                GUI_GotoXY(dotX+pObj->TrendPixelNum+Para_X,cnt*(dotY-Y_Top)/4+Y_Top);
                GUI_DispDecSpace(*(pObj->Trend_Info[cnt].ParaDateBuf_Short + pObj->xscale),4);
            }
        }
        else 
        {
            if(pObj->TrendBufferSize == sizeof(char))//char 
            {
                GUI_GotoXY(dotX+pObj->TrendPixelNum+Para_X+5,cnt*(dotY-Y_Top)/4+Y_Top);
                GUI_DispFloatMin((*(pObj->Trend_Info[cnt].ParaDateBuf_Char + pObj->xscale))/(pObj->XYAD_INFO.nYFloatLen*10.0),1);
            }
            else //short
            {
                GUI_GotoXY(dotX+pObj->TrendPixelNum+Para_X+5,cnt*(dotY-Y_Top)/4+Y_Top);
                GUI_DispFloatMin((*(pObj->Trend_Info[cnt].ParaDateBuf_Short + pObj->xscale))/(pObj->XYAD_INFO.nYFloatLen*10.0),1);
            }
        }
    }

    if(pObj->XYAD_INFO.XSign == 'T')//显示X轴（时间轴）坐标和光标所在位置的时间
    {
        //if() GUI_FLAG时间格式不同显示也不同。YYYY-MM-DD    MM-DD-YYYY  .....
    	GUI_SetColor(Coordinate_XSign_Color);
        Disp_Time = Trend_GetTime(pObj);
        sprintf(time,"%02d:%02d:%02d",Disp_Time.RTC_Hour,Disp_Time.RTC_Min,Disp_Time.RTC_Sec);
        GUI_DispStringAt(time,dotX+pObj->TrendPixelNum+Date_X,3*(dotY-Y_Top)/4+Y_Top);
        sprintf(time,"%02d-%02d-%02d",Disp_Time.RTC_Year,Disp_Time.RTC_Mon,Disp_Time.RTC_Mday);
        GUI_DispStringAt(time,dotX+pObj->TrendPixelNum+Date_X,4*(dotY-Y_Top)/4+Y_Top);
    }
}





/***************************************************************************************************
*                   _DrawPaintCoordinate
*
*       功能：画出与创建窗口匹配的XY轴及相应的描述
*
*
*/
void _DrawPaintCoordinate(TracingCurve_Obj* pObj)
{
    //圆点位置
    float dotX,dotY;
    float NewXStep,NewYStep;                                                        //计算单位长度
    float NewYTemp,NewXTemp;
    int result = 1;
	char String[50]={0};
    int num,phy = 0;
    int PawFlag;
    float x,y,y0=0,x0=0;
    GUI_RECT r;
    y = (pObj->XYAD_INFO.YMax - pObj->XYAD_INFO.YMin)/pObj->XYAD_INFO.YStep + 1;    //Y轴上的标记个数
    //GUI_GetClientRect(&r);
    WM_GetWindowRect(&r);
    //(r.y1 - r.y0)/6;  顶边界
    dotX = TRACINGCURVE_DOTX;                                                       //(r.x1 - r.x0)/10;
    //dotY = (r.y1-r.y0)*5/6;
    dotY = _Fraction(r.y1,r.y0,(float)9.6/12);                                      //最低点的位置
    pObj->DATAINFO.XDotStart = dotX;
    pObj->DATAINFO.YDotStart = dotY;
    //计算单位长度
    //NewYStep = ((r.y1 - r.y0)*2/3)/(y-1);
    NewYStep = (_Fraction(r.y1,r.y0,(float)2/3))/(y-1);
    pObj->DATAINFO.YDotStep = NewYStep;
#ifdef WAVE_PAW_ADD_ALARMHIGH
    if(!strcmp(pObj->XYAD_INFO.XYText, Sys_Lib_GetDispStr(DP_PAW)))
    {
        PawFlag = GetPawAlarmRange();
		if(PawFlag > pObj->Range)
		{
			pObj->Range = PawFlag;
		}
    }
#endif

    if(pObj->XYAD_INFO.YSign == 'T')
    {
        //Y轴
        GUI_SetTextMode(GUI_TM_TRANS);
        for(num=0; num<y; num++)
        {
            GUI_SetColor(pObj->XYAD_INFO.XYColor);
            if(num%2)
            {
                GUI_DrawHLine(dotY-NewYStep*num,dotX-3,dotX-1);                         //change
                NewYTemp = (pObj->XYAD_INFO.YMin)*(pObj->Range)+num*(pObj->XYAD_INFO.YStep)*(pObj->Range);          //显示在Y轴侧的数据
                if((NewYTemp>=(float)-0.00001)&&(NewYTemp<=(float)0.00001))
                {
                    y0=num;                                                             //坐标原点 dotY-NewYStep*y0
                    GUI_SetColor(pObj->XYAD_INFO.YColor);
                    GUI_DispDecAt(NewYTemp,dotX-20,dotY-NewYStep*num-5,1);
                }


                if((num==(y-1)) || (num==0))//sun add 20140520
				{
                    if(NewYTemp>=1000)
                    {
                        GUI_DispDecAt (NewYTemp,dotX-35,dotY-NewYStep*num-5,4);
                    }
                    else if((NewYTemp>=100)&&(NewYTemp<1000))
                    {
						int nTemp = (NewYTemp +1)/1;//  [2/15/2022 yaoyao add]将数据向上取整
                        GUI_DispDecAt (nTemp/*NewYTemp*/,dotX-27,dotY-NewYStep*num-5,3);
                    }
                    else if((NewYTemp>=10)&&(NewYTemp<100))
                    {
						GUI_GotoXY(dotX-32,dotY-NewYStep*num-5);//add ylk 20171102 小数显示
						GUI_DispFloatMin(NewYTemp,1);
                    }
                    else if((NewYTemp<10)&&(NewYTemp>=0))                                   //添加了显示float的模式
                    {
                        if((pObj->XYAD_INFO.YFloatSign!='T')||(NewYTemp==0))
                        {
                            if((pObj->XYAD_INFO.nYFloatLen!=0)&&(NewYTemp!=0))              ///如果要求数据为整形，坐标为小数则将转换
                            {
                                GUI_GotoXY(dotX-22,dotY-NewYStep*num-5);
                                GUI_DispFloatMin(NewYTemp,1);
                            }
                            else
                            {
                                GUI_DispDecAt (NewYTemp,dotX-20,dotY-NewYStep*num-5,1);
                            }
                        }
                        else                                                                //
                        {
                            GUI_GotoXY(dotX-22,dotY-NewYStep*num-5);
                            GUI_DispFloatMin(NewYTemp,1);
                        }
                    }
                    else if((NewYTemp<0)&&(NewYTemp>-10))
                    {
                        if(pObj->XYAD_INFO.YFloatSign!='T')
                        {
                            if(pObj->XYAD_INFO.nYFloatLen!=0)                               ///如果要求数据为整形，坐标为小数则将转换
                            {
                                GUI_GotoXY(dotX-27,dotY-NewYStep*num-5);
                                GUI_DispFloatMin(NewYTemp,1);
                            }
                            else
                            {
                                GUI_DispDecAt (NewYTemp,dotX-27,dotY-NewYStep*num-5,2);
                            }
                        }
                        else                        //
                        {
                            GUI_GotoXY(dotX-30,dotY-NewYStep*num-5);
                            GUI_DispFloatMin(NewYTemp,1);
                        }
                    }
                    else if((-10>=NewYTemp)&&(NewYTemp>=-99))
                    {
                        GUI_DispDecAt (NewYTemp,dotX-27,dotY-NewYStep*num-5,3);
                    }
                    else if((-100>=NewYTemp)&&(NewYTemp>=-999))
                    {
                        GUI_DispDecAt (NewYTemp,dotX-35,dotY-NewYStep*num-5,4);
                    }
                    else if(-1000>=NewYTemp)
                    {
                        GUI_DispDecAt (NewYTemp,dotX-35,dotY-NewYStep*num-5,5);
                    }
				}
            }
            else
            {
                GUI_DrawHLine(dotY-NewYStep*num,dotX-6,dotX-1);                         //   change   dotX-1
                GUI_SetFont(TRACINGCURVE_DEFAULT_FONT);
                GUI_SetColor(pObj->XYAD_INFO.YColor);
                NewYTemp = (pObj->XYAD_INFO.YMin)*(pObj->Range)+num*(pObj->XYAD_INFO.YStep)*(pObj->Range);          //显示在Y轴侧的数据
                ///如果要求数据为整形，坐标为小数则将转换       Add by yanglin for 2007-12-21
                if((pObj->XYAD_INFO.YFloatSign=='F')&&(pObj->XYAD_INFO.nYFloatLen!=0))
                {
                    result = 1;
                    square(10,pObj->XYAD_INFO.nYFloatLen,&result);
                    NewYTemp = NewYTemp/result;
                }

                if((NewYTemp>=(float)-0.00001)&&(NewYTemp<=(float)0.00001))
                {
                    y0=num;    //坐标原点 dotY-NewYStep*y0
					GUI_DispDecAt(NewYTemp,dotX-20,dotY-NewYStep*num-5,1);//sun add 20140520
                }

				if(strcmp(pObj->XYAD_INFO.XYText, Sys_Lib_GetDispStr(DP_PLETH)))//去掉pleth y坐标轴显示
				{
					if((num==(y-1)) || (num==0))//20140520
					{
						if(NewYTemp>=1000)
						{
							GUI_DispDecAt (NewYTemp,dotX-35,dotY-NewYStep*num-5,4);
						}
						else if((NewYTemp>=100)&&(NewYTemp<1000))
						{
							GUI_DispDecAt (NewYTemp,dotX-27,dotY-NewYStep*num-5,3);
						}
						else if((NewYTemp>=10)&&(NewYTemp<100))
						{
							GUI_DispDecAt (NewYTemp,dotX-22,dotY-NewYStep*num-5,2);
						}
						else if((NewYTemp<10)&&(NewYTemp>=0))                                   //添加了显示float的模式
						{
							if((pObj->XYAD_INFO.YFloatSign!='T')||(NewYTemp==0))
							{
								if((pObj->XYAD_INFO.nYFloatLen!=0)&&(NewYTemp!=0))              ///如果要求数据为整形，坐标为小数则将转换
								{
									GUI_GotoXY(dotX-22,dotY-NewYStep*num-5);
									GUI_DispFloatMin(NewYTemp,1);
								}
								else
								{
									GUI_DispDecAt (NewYTemp,dotX-20,dotY-NewYStep*num-5,1);
								}
							}
							else                                                                //
							{
								GUI_GotoXY(dotX-22,dotY-NewYStep*num-5);
								GUI_DispFloatMin(NewYTemp,1);
							}
						}
						else if((NewYTemp<0)&&(NewYTemp>-10))
						{
							if(pObj->XYAD_INFO.YFloatSign!='T')
							{
								if(pObj->XYAD_INFO.nYFloatLen!=0)                               ///如果要求数据为整形，坐标为小数则将转换
								{
									GUI_GotoXY(dotX-27,dotY-NewYStep*num-5);
									GUI_DispFloatMin(NewYTemp,1);
								}
								else
								{
									GUI_DispDecAt (NewYTemp,dotX-27,dotY-NewYStep*num-5,2);
								}
							}
							else                        //
							{
								GUI_GotoXY(dotX-30,dotY-NewYStep*num-5);
								GUI_DispFloatMin(NewYTemp,1);
							}
						}
						else if((-10>=NewYTemp)&&(NewYTemp>=-99))
						{
							GUI_DispDecAt (NewYTemp,dotX-27,dotY-NewYStep*num-5,3);
						}
						else if((-100>=NewYTemp)&&(NewYTemp>=-999))
						{
							GUI_DispDecAt (NewYTemp,dotX-35,dotY-NewYStep*num-5,4);
						}
						else if(-1000>=NewYTemp)
						{
							GUI_DispDecAt (NewYTemp,dotX-35,dotY-NewYStep*num-5,5);
						}
					}
				}
            }
        }
    }
    pObj->DATAINFO.YDot = dotY-NewYStep*y0;

    //X轴
    x = (pObj->XYAD_INFO.XMax - pObj->XYAD_INFO.XMin)/pObj->XYAD_INFO.XStep + 1;    //X轴上的标记个数
	//NewXStep = (r.x1 - r.x0-65)/(x-1); //49->85 389-4-65=320 = 8s=40*25ms*8 //X轴改为8s cpp md 20130424 //  change by YL 2007-7-18 old (r.x1 - r.x0-dotX-45)/(x-1)
	NewXStep = (r.x1 - r.x0-(8*x+1))/(x-1); //  [2/17/2022 yaoyao modify]将根据是否选配麻气修改波形x轴量程
    pObj->DATAINFO.XDotStep = NewXStep;
    if(pObj->XYAD_INFO.XSign=='T')
    {
        for(num=0; num<x; num++)
        {
            GUI_SetColor(pObj->XYAD_INFO.XYColor);
            if(num%2)
            {
                GUI_DrawVLine(dotX+NewXStep*num,dotY+1,dotY+3);                     //change
                if(pObj->Data.loopSign=='T')
                {
                    NewXTemp = (pObj->XYAD_INFO.XMin)*(pObj->RangeX)+num*(pObj->XYAD_INFO.XStep)*(pObj->RangeX);      //显示在Y轴侧的数据
                }
                else
                {
                    NewXTemp = (pObj->XYAD_INFO.XMin)+num*(pObj->XYAD_INFO.XStep);
                }
                if(NewXTemp==0)
                {
                    x0=num;                                                         //坐标原点 dotX+NewXStep*x0
                    GUI_SetColor(pObj->XYAD_INFO.XColor);
                    GUI_DispDecAt(NewXTemp,dotX+NewXStep*num-4,dotY+8,1);
                }
            }
            else
            {
                GUI_DrawVLine(dotX+NewXStep*num,dotY+1,dotY+6);                     //change
                GUI_SetFont(TRACINGCURVE_DEFAULT_FONT);
                GUI_SetColor(pObj->XYAD_INFO.XColor);
                if(pObj->Data.loopSign=='T')
                {
                    NewXTemp = (pObj->XYAD_INFO.XMin)*(pObj->RangeX)+num*(pObj->XYAD_INFO.XStep)*(pObj->RangeX);      //显示在Y轴侧的数据
                }
                else
                {
                    NewXTemp = (pObj->XYAD_INFO.XMin)+num*(pObj->XYAD_INFO.XStep);
                }
                //NewXTemp = (pObj->XYAD_INFO.XMin)+num*(pObj->XYAD_INFO.XStep);        //显示在Y轴侧的数据
                if((pObj->XYAD_INFO.XFloatSign=='F')&&(pObj->XYAD_INFO.nXFloatLen!=0))
                {
                    result = 1;
                    square(10,pObj->XYAD_INFO.nXFloatLen,&result);
                    NewXTemp = NewXTemp/result;
                }
                if(NewXTemp==0)
                {
                    x0=num;    //坐标原点 dotX+NewXStep*x0
                }
                ///趋势坐标判断
                if(pObj->nTrendType == 0)
                {
                    //X轴最后的标注不要写
                    if(num!=(x-1))
                    {
                        if(NewXTemp>=1000)
                        {
                            GUI_DispDecAt(NewXTemp,dotX+NewXStep*num-12,dotY+8,4);
                        }
                        else if((NewXTemp>=100)&&(NewXTemp<1000))
                        {
                            GUI_DispDecAt(NewXTemp,dotX+NewXStep*num-8,dotY+8,3);
                        }
                        else if((NewXTemp>=10)&&(NewXTemp<100))
                        {
                            GUI_DispDecAt(NewXTemp,dotX+NewXStep*num-4,dotY+8,2);
                        }
                        else if((NewXTemp<10)&&(NewXTemp>=0))
                        {
                            if((pObj->XYAD_INFO.XFloatSign!='T')||(NewXTemp==0))
                            {
                                if((pObj->XYAD_INFO.nXFloatLen!=0)&&(NewXTemp!=0))          ///如果要求数据为整形，坐标为小数则将转换
                                {
                                    GUI_GotoXY(dotX+NewXStep*num-2,dotY+8);
                                    GUI_DispFloatMin(NewXTemp,1);
                                }
                                else
                                {
                                    GUI_DispDecAt(NewXTemp,dotX+NewXStep*num-2,dotY+8,1);
                                }
                            }
                            else
                            {
                                GUI_GotoXY(dotX+NewXStep*num-2,dotY+8);
                                GUI_DispFloatMin(NewXTemp,1);
                            }
                        }
                        else if((NewXTemp<0)&&(NewXTemp>-10))
                            if(pObj->XYAD_INFO.XFloatSign!='T')
                            {
                                if(pObj->XYAD_INFO.nXFloatLen!=0)                               ///如果要求数据为整形，坐标为小数则将转换
                                {
                                    GUI_GotoXY(dotX+NewXStep*num-9,dotY+8);
                                    GUI_DispFloatMin(NewXTemp,1);
                                }
                                else
                                {
                                    GUI_DispDecAt(NewXTemp,dotX+NewXStep*num-4,dotY+8,2);
                                }
                            }
                            else
                            {
                                GUI_GotoXY(dotX+NewXStep*num-9,dotY+8);
                                GUI_DispFloatMin(NewXTemp,1);
                            }
                        else if(-10>=NewXTemp)
                        {
                          //  GUI_DispDecAt(NewXTemp,dotX+NewXStep*num-4,dotY+8,3);
                        }
                    }
                }
                else                                                                ///趋势X轴原点显示的信息
                {
                    GUI_SetColor(pObj->XYAD_INFO.XColor);
                    GUI_SetTextAlign(GUI_TA_CENTER);
                    GUI_DispStringAt(pObj->XYAD_INFO.XTrendText,dotX,dotY+10);      ///X_TRENDTEXT
                }
            }
        }
        GUI_SetColor(pObj->XYAD_INFO.XYColor);
        //将有标尺的XY轴向下、左移动一个像素点
        GUI_DrawHLine(dotY,dotX,dotX+NewXStep*(x-1));                           //水平方向 0
        GUI_DrawVLine(dotX+NewXStep*x0,dotY-NewYStep*(y-1),dotY);               //垂直方向 0
        GUI_SetColor(pObj->XYAD_INFO.XColor);
        GUI_SetTextAlign(GUI_TA_CENTER);
	 if(!strcmp(pObj->XYAD_INFO.XText, Sys_Lib_GetDispStr(DP_CMH2O)))				//gyh add 20230728 cmH2o显示不全
	 {
        GUI_DispStringAt(pObj->XYAD_INFO.XText,dotX+NewXStep*(x-1)-14,dotY+8);       //X_TEXT
	 }
	 else
	 {
		GUI_DispStringAt(pObj->XYAD_INFO.XText,dotX+NewXStep*(x-1)-4,dotY+8);       //X_TEXT
	 }
    }

	if((!strcmp(pObj->XYAD_INFO.XYText, Sys_Lib_GetDispStr(DP_PLETH)))&&(SpO2_Choose == 2))//"Pleth"
	{
		GUI_SetColor(pObj->XYTextColor);
		if(Masimo_SmartToneSwitch)
		{
			sprintf(String,"%s","Smart tone:ON");
		}
		else
		{
			sprintf(String,"%s","Smart tone:OFF");
		}
		num = 40;
		GUI_DispStringAt(String,num,0);
		num = 145;
		if((Masimo_FastSATSwitch)||(Masimo_AverageTime==0)||(Masimo_AverageTime==1))
		{
			sprintf(String,"%s","Fast SAT:ON");
		}
		else
		{
			sprintf(String,"%s","Fast SAT:OFF");
		}
		GUI_DispStringAt(String,num,0);
		num = 245;
		switch(Masimo_Sensitivity)
		{
		case 0:
			sprintf(String,"%s","Sensitivity:Max");
			break;
		case 2:
			sprintf(String,"%s","Sensitivity:APOD");
			break;
		case 1:
		default:
			sprintf(String,"%s","Sensitivity:Normal");
			break;
		}
		GUI_DispStringAt(String,num,0);
	}

    pObj->DATAINFO.XDot = dotX+NewXStep*x0;
    pObj->DATAINFO.XDotEnd = dotX+NewXStep*(x-1);                                   //X轴最右测的点
    pObj->DATAINFO.YDotEnd = dotY-NewYStep*(y-1);                                   //Y轴最上的点
    //画X、Y
    GUI_SetColor(pObj->XYAD_INFO.XYColor);
    //将有标尺的XY轴向下、左移动一个像素点
    //if(pObj->DATAINFO.YDotStart==(dotY-NewYStep*y0))
    //  GUI_DrawHLine(dotY-NewYStep*y0+1,dotX,dotX+NewXStep*(x-1));                 //水平方向    r.x1-45
    //else
    GUI_DrawHLine(dotY-NewYStep*y0,dotX,(int)(dotX+NewXStep*(x-1)));                //水平方向    r.x1-45
    GUI_DrawVLine(dotX,(int)(dotY-NewYStep*(y-1)),dotY);                            //垂直方向
    GUI_SetColor(pObj->XYAD_INFO.YColor);
    GUI_DispStringAt(pObj->XYAD_INFO.YText,dotX+3,(int)(dotY-NewYStep*(y-1)-17));   //Y轴TEXT
    GUI_SetColor(pObj->XYTextColor);
    GUI_SetTextAlign(GUI_TA_RIGHT|GUI_TA_TOP);

//    GUI_DispStringAt(pObj->XYAD_INFO.XYText,r.x1-12,(int)(dotY-NewYStep*(y-1)-16)); //XY_TEXT  //cpp del 20130510
    GUI_DispStringAt(pObj->XYAD_INFO.XYText,pObj->DATAINFO.XDotEnd ,(int)(dotY-NewYStep*(y-1)-16)); //XY_TEXT   //  20130510

#ifdef WAVE_PAW_ADD_ALARMHIGH
    //画压力报警上限直线
    if(!strcmp(pObj->XYAD_INFO.XYText, Sys_Lib_GetDispStr(DP_PAW)))
    {
        GUI_SetColor(GUI_RED);
        phy = (pObj->DATAINFO.YDot)-(int)((pObj->DATAINFO.YDotStep)*(AlarmSettingPara.PressureHighValue)/((pObj->XYAD_INFO.YStep)*(pObj->Range)));
        if(phy >= pObj->DATAINFO.YDotEnd)
        {
            GUI_DrawHLine(phy-1,dotX+1,(int)(dotX+NewXStep*(x-1)));//AlarmSettingPara.PressureHighValue
        }
    }
#endif
}

/***********************************************************************************************
*       _DrawTrendBuffer
*
*       功能：通过buffer内的数据重绘趋势图的波形
        扩展功能：只有第一次进入或者切换量程的时候才完全绘制，否则之绘制移动之前的那个点
*/
static void _DrawTrendBuffer(TracingCurve_Obj* pObj)
{
    int cnt ,index;
    float YStep = pObj->DATAINFO.YDotStep;                                                        //计算单位长度
    float per_date_float = 0,date_float = 0;
    char  per_date_char = 0,date_char = 0;
    int dotX = pObj->DATAINFO.XDotStart;
    int datY = pObj->DATAINFO.YDotStart;
    if(pObj->TrendNumber)
    {
    	if((pObj->lastxscan == pObj->xscale)||(pObj->TrendPaintFlag == 1)) ///初始化绘制的图形
	    {
            //按照比例缩小绘制
			if(pObj->TrendBufferSize == sizeof(char))
			{
				for(index = 0;index < pObj->TrendNumber;index++)
				{
					GUI_SetColor(pObj->Trend_Info[index].ParaColor);
					for(cnt = 0;cnt < pObj->TrendPixelNum;cnt++)
					{
						GUI_SetPenSize(1);
					    date_char = *(((char *)pObj->Trend_Info[index].ParaDateBuf_Char) + cnt);
						if(date_char >= pObj->XYAD_INFO.YMax)//最大值既是Y轴的最顶端，坐标为Y_Top
						{
							date_char = Y_Top;
						}
						else 
						{
							date_char = datY -  (date_char*YStep);
                            if(date_char < pObj->XYAD_INFO.YMin)
                            {
                                date_char = pObj->XYAD_INFO.YMin;
                            }
						}
					    //GUI_DrawPoint(dotX + cnt,date_char);

						if(cnt==0)
						{
							GUI_SetPenSize(1);
							date_char = *(((char *)pObj->Trend_Info[index].ParaDateBuf_Char) + cnt);
							if(date_char >= pObj->XYAD_INFO.YMax)//最大值既是Y轴的最顶端，坐标为Y_Top
							{
								date_char = Y_Top;
							}
							else 
							{
								date_char = datY -  (date_char*YStep);
							}
							GUI_DrawPoint(dotX,date_char);
							per_date_char = date_char;
						}
						else
						{
							GUI_SetPenSize(1);
							date_char = *(((char *)pObj->Trend_Info[index].ParaDateBuf_Char) + cnt);
							if(date_char >= pObj->XYAD_INFO.YMax)//最大值既是Y轴的最顶端，坐标为Y_Top
							{
								date_char = Y_Top;
							}
							else 
							{
								date_char = datY -  (date_char*YStep);
							}
							GUI_DrawLine((cnt - 1 + dotX), per_date_char,(cnt + dotX), date_char);
							per_date_char = date_char;
						}
					}
				}
			}
			else if(pObj->TrendBufferSize == sizeof(short))
			{
				for(index = 0;index < pObj->TrendNumber;index++)
				{
					GUI_SetColor(pObj->Trend_Info[index].ParaColor);
					for(cnt = 0;cnt < pObj->TrendPixelNum;cnt++)
					{
						GUI_SetPenSize(1);
						date_float = (*(((short *)pObj->Trend_Info[index].ParaDateBuf_Short) + cnt));
					    if(date_float >= pObj->XYAD_INFO.YMax)
					    {
						    date_float = Y_Top;
					    }
					    else 
					    {
						    date_float = datY -  (date_float*YStep);
                            if(date_float < pObj->XYAD_INFO.YMin)
                            {
                                date_float = pObj->XYAD_INFO.YMin;
                            }
					    }
					    //GUI_DrawPoint(dotX + cnt,date_float);
						if(cnt==0)
						{
							GUI_SetPenSize(1);
							date_float = (*(((short *)pObj->Trend_Info[index].ParaDateBuf_Short) + cnt));
							if(date_float >= pObj->XYAD_INFO.YMax)//最大值既是Y轴的最顶端，坐标为Y_Top
							{
								date_float = Y_Top;
							}
							else 
							{
								date_float = datY -  (date_float*YStep);
							}
							GUI_DrawPoint(dotX,date_float);
							per_date_float = date_float;
						}
						else
						{
							GUI_SetPenSize(1);
							date_float = (*(((short *)pObj->Trend_Info[index].ParaDateBuf_Short) + cnt));
							if(date_float >= pObj->XYAD_INFO.YMax)
							{
								date_float = Y_Top;
							}
							else 
							{
								date_float = datY -  (date_float*YStep);
							}
							GUI_DrawLine(cnt - 1 + dotX,per_date_float,(cnt + dotX),date_float);
							per_date_float = date_float;
						}
					}
				}
			}

			pObj->lastxscan = pObj->xscale;
		}
		else//光标移动 
		{
            //按照比例缩小绘制
			if(pObj->TrendBufferSize == sizeof(char))
			{
				for(index = 0;index < pObj->TrendNumber;index++)
				{
	                cnt = pObj->lastxscan;

					GUI_SetColor(pObj->Trend_Info[index].ParaColor);
					GUI_SetPenSize(1);
					date_char = (*(((char *)pObj->Trend_Info[index].ParaDateBuf_Char) + cnt));
					if(date_char >= pObj->XYAD_INFO.YMax)
					{
						date_char = Y_Top;
					}
					else 
					{
						date_char = datY -  (date_char*YStep);
                        if(date_char < pObj->XYAD_INFO.YMin)
                        {
                            date_char = pObj->XYAD_INFO.YMin;
                        }
					}
					if(cnt >= 1)//连接前一个点
					{
						per_date_char = (*(((char *)pObj->Trend_Info[index].ParaDateBuf_Char) + cnt - 1));
						if(per_date_char >= pObj->XYAD_INFO.YMax)
						{
							per_date_char = Y_Top;
						}
						else 
						{
							per_date_char = datY -  (per_date_char*YStep);
						}
						GUI_DrawLine(cnt - 1 + dotX,per_date_char,(cnt + dotX),date_char);
					}
					if(cnt + 1 < pObj->TrendPixelNum)//连接后一个点
					{
						per_date_char = (*(((char *)pObj->Trend_Info[index].ParaDateBuf_Char) + cnt + 1));
						if(per_date_char >= pObj->XYAD_INFO.YMax)
						{
							per_date_char = Y_Top;
						}
						else 
						{
							per_date_char = datY -  (per_date_char*YStep);
						}
						GUI_DrawLine(cnt + dotX,date_char,(cnt + 1 + dotX),per_date_char);
					}
//                    GUI_DrawPoint(cnt + dotX,date_char);
				}
			}
			else if(pObj->TrendBufferSize == sizeof(short))
			{
				for(index = 0;index < pObj->TrendNumber;index++)
				{
	                cnt = pObj->lastxscan;

					GUI_SetColor(pObj->Trend_Info[index].ParaColor);
					GUI_SetPenSize(1);
					date_float = (*(((short *)pObj->Trend_Info[index].ParaDateBuf_Short) + cnt));
					if(date_float >= pObj->XYAD_INFO.YMax)
					{
						date_float = Y_Top;
					}
					else 
					{
						date_float = datY -  (date_float*YStep);
                        if(date_float < pObj->XYAD_INFO.YMin)
                        {
                            date_float = pObj->XYAD_INFO.YMin;
                        }
					}
					if(cnt >= 1)//连接前一个点
					{
						per_date_float = (*(((short *)pObj->Trend_Info[index].ParaDateBuf_Short) + cnt - 1));
						if(per_date_float >= pObj->XYAD_INFO.YMax)
						{
							per_date_float = Y_Top;
						}
						else 
						{
							per_date_float = datY -  (per_date_float*YStep);
						}
						GUI_DrawLine(cnt - 1 + dotX,per_date_float,(cnt + dotX),date_float);
					}
					if(cnt + 1 < pObj->TrendPixelNum)//连接后一个点
					{
						per_date_float = (*(((short *)pObj->Trend_Info[index].ParaDateBuf_Short) + cnt + 1));
						if(per_date_float >= pObj->XYAD_INFO.YMax)
						{
							per_date_float = Y_Top;
						}
						else 
						{
							per_date_float = datY -  (per_date_float*YStep);
						}
						GUI_DrawLine(cnt + dotX,date_float,(cnt + 1 + dotX),per_date_float);
					}
                    //GUI_DrawPoint(cnt + dotX,date_float);
				}
			}

			pObj->lastxscan = pObj->xscale;
		}
    }
    

	//绘制光标(绘制光标是否应该放在绘制波形，然后绘制光标？)
    GUI_SetColor(TREND_COORDINATEXSIGN_COLOR);
    GUI_DrawVLine(pObj->xscale+pObj->DATAINFO.XDotStart,0,pObj->DATAINFO.YDotStart);     

    //绘制波形后，重新绘制坐标******************************
    GUI_SetColor(TREND_COORDINATE_COLOR);
    //Y 轴坐标轴
    GUI_DrawVLine(pObj->DATAINFO.XDotStart -1,Y_Top,pObj->DATAINFO.YDotStart);
	//X 轴坐标轴
    if(pObj->XYAD_INFO.XSign == 'T')
    {
        GUI_SetColor(TREND_COORDINATEXSIGN_COLOR);
    }
    else 
    {
        GUI_SetColor(TREND_COORDINATE_COLOR);
    }
    GUI_DrawHLine(pObj->DATAINFO.YDotStart,pObj->DATAINFO.XDotStart-3,pObj->DATAINFO.XDotStart + pObj->TrendPixelNum);       
    //BUFFER 绘制完成后，获取上一次光标的位置
    pObj->lastxscan = pObj->xscale;          


}

/***************************************************************************************************
*       _DrawCVBuffer
*
*       功能：通过buffer内的数据重绘数据显示区
*/
static void _DrawCVBuffer(TracingCurve_Obj* pObj)
{
    int y,yold,xold=0;
	int y2,yold2;
    //int size;
    GUI_COLOR color;
    float data,data2;
    int i,len,doubleFlag = 0 ;
    if(pObj->nStopRemove)
    {
        len = pObj->DATAINFO.XDotEnd - pObj->DATAINFO.XDot;
    }
    else
    {
        len = pObj->xscale;
    }
    if(len>0)
    {
		if(((!strcmp(pObj->XYAD_INFO.XYText,Sys_Lib_GetDispStr(DP_PLETH)))&&(SpO2_Choose == 2)) 
			|| (pObj->Data.DoubleFlag))
		{
			doubleFlag = 1;
		}
		else
		{
			doubleFlag = 0;
		}
        for(i=1; i<len; i++)
        {
            data = (pObj->Data.DataDotInfo+i)->Datay;
			color = (pObj->Data.DataDotInfo+i)->DataColor;
			y = (pObj->DATAINFO.YDot)-(pObj->DATAINFO.YDotStep)*data/((pObj->XYAD_INFO.YStep)*(pObj->Range));
			if(doubleFlag)
			{
				data2 = (pObj->Data.DataDotInfo+i)->Datax;
				y2 = (pObj->DATAINFO.YDot)-(pObj->DATAINFO.YDotStep)*data2/((pObj->XYAD_INFO.YStep)*(pObj->Range));
			}
			GUI_SetColor(color);
            //画出表现数据的方式
            if(pObj->Data.Mode==0)
            {
                //画曲线的信息
                if(i==1)
                {
                    GUI_SetPenSize(1);
                    GUI_DrawPoint((int)(pObj->DATAINFO.XDot),y);
					if(doubleFlag)
					{
						if(!strcmp(pObj->XYAD_INFO.XYText,Sys_Lib_GetDispStr(DP_PLETH)))
						{
							GUI_SetColor(GUI_GREEN);
						}
						GUI_DrawPoint((int)(pObj->DATAINFO.XDot),y2);
					}
                }
                else
                {
                    GUI_SetPenSize(1);
                    GUI_DrawLine(xold,yold,(i+(int)(pObj->DATAINFO.XDot)),y);
					if(doubleFlag)
					{
						if(!strcmp(pObj->XYAD_INFO.XYText,Sys_Lib_GetDispStr(DP_PLETH)))
						{
							GUI_SetColor(GUI_GREEN);
						}
						GUI_DrawLine(xold,yold2,(i+(int)(pObj->DATAINFO.XDot)),y2);
					}
                }
                yold = y;   //前一个点的y值
				if(doubleFlag)
				{
					yold2 = y2;   //前一个点的y值
				}
                xold = i+(int)(pObj->DATAINFO.XDot);
            }
            else
            {
                //画柱形的信息
                if(data>=0)
                {
                    GUI_DrawVLine((i+(pObj->DATAINFO.XDot)),y,pObj->DATAINFO.YDot);
                }
                else
                {
                    GUI_DrawVLine((i+(pObj->DATAINFO.XDot)),pObj->DATAINFO.YDot,y);
                }
            }
        }
    }
    GUI_SetColor(pObj->XYAD_INFO.XYColor);
    if(pObj->DATAINFO.YDotStart!=pObj->DATAINFO.YDot)
    {
        GUI_DrawHLine(pObj->DATAINFO.YDot,pObj->DATAINFO.XDotStart,pObj->DATAINFO.XDotEnd);    //水平方向 0
    }
    if(pObj->XYAD_INFO.XSign=='T')
    {
        //重新画X轴Y轴
        GUI_SetColor(pObj->XYAD_INFO.XYColor);
        //将有标尺的XY轴向下、左移动一个像素点
//        GUI_DrawHLine(pObj->DATAINFO.YDot,pObj->DATAINFO.XDotStart,pObj->DATAINFO.XDotEnd-1);//水平方向 djq curve flash
        GUI_DrawVLine(pObj->DATAINFO.XDot,pObj->DATAINFO.YDotEnd,pObj->DATAINFO.YDotStart);         //垂直方向
    }
}
/***************************************************************************************************
*       _DrawLoopBuffer
*
*       功能：通过buffer内的数据重绘数据显示区
*/
void _DrawLoopBuffer(TracingCurve_Obj* pObj)
{
    GUI_COLOR color;
    static GUI_COLOR color1 = GUI_RED;
    float datax=0,datay=0;
    int i,x=0,y=0,yold=0,xold=0;
    //int size;
    int len = 0;
    len = pObj->xscale;
    if(len>0)
    {
        for(i=1; i<len; i++)
        {
            datay = (pObj->Data.DataDotInfo+i)->Datay;
            datax = (pObj->Data.DataDotInfo+i)->Datax;
            color = (pObj->Data.DataDotInfo+i)->DataColor;
            x = (pObj->DATAINFO.XDot)+(pObj->DATAINFO.XDotStep)*datax/((pObj->XYAD_INFO.XStep)*(pObj->RangeX));
            y = (pObj->DATAINFO.YDot)-(pObj->DATAINFO.YDotStep)*datay/((pObj->XYAD_INFO.YStep)*(pObj->Range));
            GUI_SetColor(color);
            //画出表现数据的方式
            if(i==1)
            {
                GUI_SetPenSize(1);
                GUI_DrawPoint(x,y);
            }
            else
            {
                GUI_SetPenSize(1);
                GUI_DrawLine(xold,yold,x,y);
            }
            xold = x;
            yold = y;
        }
    }
    GUI_SetColor(pObj->XYAD_INFO.XYColor);
    if(pObj->DATAINFO.YDotStart!=pObj->DATAINFO.YDot)
    {
        GUI_DrawHLine(pObj->DATAINFO.YDot,pObj->DATAINFO.XDotStart,pObj->DATAINFO.XDotEnd);    //水平方向 0
    }
    if(pObj->XYAD_INFO.XSign=='T')
    {
        //重新画X轴Y轴
        GUI_SetColor(pObj->XYAD_INFO.XYColor); //cpp md 21030605
        //将有标尺的XY轴向下、左移动一个像素点
        GUI_DrawHLine(pObj->DATAINFO.YDot,pObj->DATAINFO.XDotStart,pObj->DATAINFO.XDotEnd);         //水平方向
        GUI_DrawVLine(pObj->DATAINFO.XDot,pObj->DATAINFO.YDotEnd,pObj->DATAINFO.YDotStart);         //垂直方向
    }
}
/***************************************************************************************************
*       _DrawVigilanceLine
*
*       功能：警戒线
*/
void _DrawVigilanceLine(TracingCurve_Obj* pObj)
{
    unsigned int i;
    int y;
    if(pObj->VigilanceSize.ViewSign == 'T')
    {
        GUI_SetColor(pObj->VigilanceSize.color);
        y = (pObj->DATAINFO.YDot)-(pObj->DATAINFO.YDotStep)*(pObj->VigilanceSize.size)/(pObj->XYAD_INFO.YStep);
        //GUI_DrawHLine(y,pObj->DATAINFO.XDotStart,pObj->DATAINFO.XDotEnd);
        for(i=pObj->DATAINFO.XDotStart; i<pObj->DATAINFO.XDotEnd; i++)
        {
            GUI_DrawPixel(i,y);
            i++;
        }
    }
}
/***************************************************************************************************
*       _DrawSPO2Bar
*
*       功能：血氧棒图
*/
void _DrawSPO2Bar(TracingCurve_Obj* pObj)
{
    int xSize,ySize;
    int yWidth,y1;
    int i;
    if(1 == pObj->nSPO2Bar)         ///波形显示的另外情况，为SPO2时，波形显示区域的右侧显示棒图
    {
        xSize = pObj->Widget.Win.Rect.x1 - pObj->Widget.Win.Rect.x0;
        ySize = pObj->Widget.Win.Rect.y1 - pObj->Widget.Win.Rect.y0;
        yWidth = (ySize-TRACINGCURVE_DEFAULT_SPO2Y*3)/TRACINGCURVE_DEFAULT_SPO2MAX;
        y1 = ySize-TRACINGCURVE_DEFAULT_SPO2Y*2;
        if(pObj->nSPO2Count>15)
        {
            pObj->nSPO2Count = 15;
        }
        ///SPO2棒图大小
        GUI_SetColor(TRACINGCURVE_DEFAULT_SPO2PROCOLR);
        for(i=0; i<=(pObj->nSPO2Count-1); i++)
        {
            GUI_FillRect(pObj->DATAINFO.XDotEnd+nTracingCurveClearLine/*TRACINGCURVE_CLEARLINE_DEFAULT*/,y1-yWidth*(i*2+1),xSize,y1-yWidth*i*2);
        }
        ///边框
        GUI_SetColor(TRACINGCURVE_DEFAULT_FRAMECOLOR);
        GUI_DrawRect(pObj->DATAINFO.XDotEnd+nTracingCurveClearLine/*TRACINGCURVE_CLEARLINE_DEFAULT*/, y1-yWidth*TRACINGCURVE_DEFAULT_SPO2MAX, xSize, y1);
    }
}
/***************************************************************************************************
*                       _OnTouch
*       功能：响应按键消息，并有按键台起来发送一个消息WM_NOTIFICATION_RELEASED_TracingCurve（自定义）
*/
static void _OnTouch(TracingCurve_Handle hWin, TracingCurve_Obj* pObj, WM_MESSAGE* pMsg)
{
    //GUI_TOUCH_tState* pState;
    WM_MESSAGE Msg;
    const GUI_PID_STATE* pState = (const GUI_PID_STATE*)pMsg->Data.p;
    if(!WM_IsWindow(hWin))
    {
        return;
    }
    pState = (GUI_PID_STATE*)pMsg->Data.p;
    if (pMsg->Data.p)
    {
        if (pState->Pressed)
        {
            if(pObj->nTrendType == 1)//趋势图触摸
            {
                if((pState->x >= pObj->DATAINFO.XDotStart) && (pState->x < pObj->DATAINFO.XDotStart + pObj->TrendPixelNum))
                {
                    //pObj->xscale = pState->x - pObj->DATAINFO.XDotStart;
                    Trend_Xsacn = pState->x - pObj->DATAINFO.XDotStart;

                }
            }
            else 
            {
                Msg.MsgId   = WM_NOTIFY_PARENT;
                Msg.Data.v  = WM_NOTIFICATION_CLICKED;
                WM_SendToParent(hWin, &Msg);
            }
        }
        else
        {
            if(pObj->nTrendType == 1)//趋势图触摸
            {
                if((pState->x >= pObj->DATAINFO.XDotStart) && (pState->x < pObj->DATAINFO.XDotStart + pObj->TrendPixelNum))
               {
                    Msg.MsgId   = WM_NOTIFY_PARENT;
                //    Msg.Data.v  = WM_PAINT;
                    Msg.Data.v  = WM_NOTIFICATION_CLICKED;
                    WM_SendToParent(hWin, &Msg);
//                    WM__SendMessageNoPara(hWin,WM_PAINT);
                }
            }
            /*if ((pObj->Widget.State & BUTTON_STATE_PRESSED) == 0){
              Msg.MsgId   = WM_NOTIFY_PARENT;
              Msg.Data.v  = WM_NOTIFICATION_RELEASED;
              WM_SendToParent(hWin, &Msg);
              }
            }*/
        }
    }
}
/***************************************************************************************************
*           _Paint
*               窗口回调函数
*/
static void _Paint(TracingCurve_Obj* pObj)
{
    /* Draw Frame */
    //GUI_GetClientRect(&r);
    int xsize = pObj->Widget.Win.Rect.x1 - pObj->Widget.Win.Rect.x0;
    int ysize = pObj->Widget.Win.Rect.y1 - pObj->Widget.Win.Rect.y0;
    //GUI_Clear();
//    _WIDGET_EFFECT_3D_DrawDown();  //sun del 20140520
    if(pObj->nTrendType != 1)
    {
        GUI_SetColor  (pObj->DIALOGINFO.BKColor);
        GUI_FillRect  (0, 0, xsize, ysize);
        _DrawPaintCoordinate(pObj);

    }
    else //趋势图
    {
        if((pObj->lastxscan == pObj->xscale)||(pObj->TrendPaintFlag == 1))//初始化的时候重绘
        {
            GUI_SetColor  (pObj->DIALOGINFO.BKColor);
            GUI_FillRect  (0, 0, xsize, ysize);
        }
        _DrawPaintTrendCoordinate(pObj);
    }

    if(pObj->Data.loopSign=='T')              //判断是否是环形图
    {
        _DrawLoopBuffer(pObj);
    }
    else if(pObj->nTrendType == 1)
    {
        _DrawTrendBuffer(pObj);
    }
    else
    {
        _DrawCVBuffer(pObj);
        //_DrawVigilanceLine(pObj);
    }
    
    if(pObj->TrendPaintFlag)
    {
        pObj->TrendPaintFlag = 0;
    }

    //WIDGET_EFFECT_3D_DrawUp();
}


/***********************************************************************************************
*               TracingCurve_AllocDispBuffer
*               功能：为一个波形图创建显示buffer
*/
int TracingCurve_AllocDispBuffer(TracingCurve_Handle hObj,int BufferSize)
{
    TracingCurve_DataDotInfo *DispBuffer;
    GUI_HMEM hMem;
    unsigned char ret = 0;
    int cnt;
    if (hObj)
    {
        TracingCurve_Obj* pObj;
        WM_LOCK();
        pObj = TracingCurve_H2P(hObj);

        hMem = GUI_ALLOC_AllocZero(sizeof(TracingCurve_DataDotInfo)*(BufferSize+1));//因为pObj->xscale从1开始，所以buffer应该比实际大1
        if(hMem == NULL)//创建buffer失败，返回0x55
        {
            ret = 0x55; 
            pObj->BufferSize = 0;
        }
        else//创建成功 
        {
            DispBuffer = (TracingCurve_DataDotInfo *)GUI_ALLOC_h2p(hMem);
            for(cnt = 0;cnt < BufferSize+1;cnt++)
            {
                (DispBuffer + cnt)->Datax = 0;
                (DispBuffer + cnt)->Datay = 0;
                (DispBuffer + cnt)->DataColor = pObj->DIALOGINFO.BKColor;
            }
            pObj->Data.DataDotInfo = DispBuffer;
            pObj->Data_hMem = hMem;
            ret = 0;
            pObj->BufferSize = BufferSize;
        }
        WM_UNLOCK();
    }
    else 
    {
        ret = 0x55;
    }
    return ret;
}

/***********************************************************************************************
*               TracingCurve_FreeDispBuffer
*               功能：为一个波形图创建显示buffer
*/
int TracingCurve_FreeDispBuffer(TracingCurve_Handle hObj)
{
    unsigned char ret = 0;
    int cnt;
    if (hObj)
    {
        TracingCurve_Obj* pObj;
        WM_LOCK();
        pObj = TracingCurve_H2P(hObj);
        if(pObj->Data_hMem == 0)//未创建Buffer
        {
            ret = 0x55; 
        }
        else 
        {
            GUI_ALLOC_Free(pObj->Data_hMem);
            pObj->Data_hMem = 0;
        }
        WM_UNLOCK();
    }
    return ret;
}

/***************************************************************************************************
*           _TracingCurve_Callback
*               窗口回调函数
*/
static void _TracingCurve_Callback (WM_MESSAGE *pMsg)
{
    TracingCurve_Handle hWin = (TracingCurve_Handle)(pMsg->hWin);
    TracingCurve_Obj* pObj = TracingCurve_H2P(hWin);
    //GUI_RECT* pRect = (GUI_RECT*)(pMsg->Data.p);
    /* Let widget handle the standard messages */
    if (WIDGET_HandleActive(hWin, pMsg) == 0)
    {
        return;
    }
    switch (pMsg->MsgId)
    {
    case WM_PAINT:
        _Paint(pObj);
        return;
    case WM_TOUCH:
        _OnTouch(hWin, pObj, pMsg);
        return;
    case WM_SIZE:
        return;
    case WM_DELETE://需要先释放buffer,波形buffer或者是趋势图buffer
        //if(pObj->nTrendType == 1)//趋势图波形
        //{
        //    TracingTrend_FreeBffer(hWin);
        //}
        //else //正常波形
        //{
        //    TracingCurve_FreeDispBuffer(hWin);
        //}
		if(pObj->nTrendType == 1)//趋势图波形
        {
            TracingTrend_FreeBffer(hWin);
            Trend_Xsacn = TREND_DEFAULT_POSITION;
        }
        else //正常波形
        {
            TracingCurve_FreeDispBuffer(hWin);
        }
        break;       
    }
    WM_DefaultProc(pMsg);
}
/***************************************************************************************************
*                   TracingCurve_CreateAsChild
*
*/
TracingCurve_Handle TracingCurve_CreateAsChild(
    int x0, int y0, int xsize, int ysize, WM_HWIN hParent,int Id,
    WM_CALLBACK* cb, int Flags)
{
    unsigned int i;
    TracingCurve_Handle hObj;
    /* Create the window */
    WM_LOCK();
    hObj = WM_CreateWindowAsChild(x0, y0, xsize/*+2*HBorder*/, ysize/*+TBorder+BBorder*/, hParent,
                                  Flags, _TracingCurve_Callback, sizeof(TracingCurve_Obj) - sizeof(WM_Obj));
    if (hObj)
    {
        TracingCurve_Obj* pObj = TracingCurve_H2P(hObj);
        /* init widget specific variables */
        WIDGET__Init(&pObj->Widget,Id, WIDGET_STATE_FOCUSSABLE);
        pObj->XYAD_INFO = xyAxes_DEFAULT_Data;
        pObj->DIALOGINFO.BKColor = TRACINGCURVE_BKCOLOR_DEFAULT;
        //pObj->Data = data_DEFAULT;
        pObj->xscale = 1;
        //pObj->yoldscale = 0;
        pObj->Data.Mode = TRACINGCURVE_DEFAULT_MODE;                    ///画出表现数据的方式  0:曲线方式  1:柱形方式
        ///初始化BUFFER数据
//        for(i=0; i<TRACINGCURVE_DATALEN_DEFAULT; i++)
//        {
//            (pObj->Data.DataDotInfo+i)->Datax = 0;
//            (pObj->Data.DataDotInfo+i)->Datay = 0;
//            (pObj->Data.DataDotInfo+i)->DataColor = pObj->DIALOGINFO.BKColor;
//        }
        pObj->VigilanceSize.ViewSign = 'F';                             ///警戒线是否可见    ‘T’：可见   ‘F’：隐藏
        pObj->PenSize = TRACINGCURVE_DEFAULT_PENSIZE;
        //pObj->Widget.Id     = Id;
        pObj->nStopRemove = 0;
        pObj->nLoopsRef = 0;                                            ///0：不保存、显示（默认值） 2：LOOPS保存显示一个周期
        pObj->nSPO2Bar = TRACINGCURVE_DEFAULT_SPO2BAR;                  ///0：不显示（默认值） 1：显示
        pObj->nSPO2Count = TRACINGCURVE_DEFAULT_SPO2COUNT;              ///SPO2棒图，显示数据大小  [0，16）
        pObj->nTrendType = 0;                                           ///为趋势时，X轴显示的信息  0：默认 1：显示
        pObj->ParameterId = Id;
        pObj->XYTextColor = TRACINGCURVE_DEFAULT_XYTEXTCOLOR;           ///图右上角的TEXT颜色
        pObj->ExceedRangeFlg = 0;
        pObj->Range = 1;
		pObj->CurveRangUpdate = 0;
		pObj->CurveRefreshCycleIndex = 0;

        pObj->RangeX = 1;
        pObj->Data_Max_High = -0xFFFF;
        pObj->Data_Max_Low = 0xFFFF;
        pObj->Data_Max_HighX = -0xFFFF;
        pObj->Data_Max_LowX = 0xFFFF;
    }
    else
    {
        //GUI_DEBUG_ERROROUT_IF(hObj==0, "TracingCurve_Create failed");
    }
    WM_UNLOCK();
    return hObj;
}
/***************************************************************************************************
*                       TracingCurve_Create
*
*/
TracingCurve_Handle TracingCurve_Create(WM_CALLBACK* cb,
                                        int Flags,
                                        int x0, int y0, int xsize, int ysize,int Id)
{
    return TracingCurve_CreateAsChild(x0, y0, xsize, ysize, WM_HWIN_NULL,Id, cb, Flags);
}
/***************************************************************************************************
*
*               TracingCurve_CreateIndirect
*/
TracingCurve_Handle TracingCurve_CreateIndirect(const GUI_WIDGET_CREATE_INFO* pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK* pCallback)
{
    TracingCurve_Handle hObj;
    hObj = TracingCurve_CreateAsChild(
               pCreateInfo->x0 + x0, pCreateInfo->y0 + y0, pCreateInfo->xSize, pCreateInfo->ySize, hWinParent, pCreateInfo->Id,
               pCallback, 0);
    return hObj;
}

#else
void WIDGET_viewWin(void) {} /* avoid empty object files */
#endif /* GUI_WINSUPPORT */
