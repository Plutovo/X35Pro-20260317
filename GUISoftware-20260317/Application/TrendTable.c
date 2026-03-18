#include "TrendTable.h"
//#include "UserConfig.h"
#include "GUI.h"

/*
*只显示每一页的数据，可以切换每一行显示。控件切换只是重新刷新显示buffer其中的一段，buffer中的数据应该有应用程序提供。
*空间并不判断页数是否在范围之内，页数只是用来计算当前的时间点。
*/

#ifndef  TRENDTABLE_DEFAULT_ROWNUMBER
#define  TRENDTABLE_DEFAULT_ROWNUMBER  10
#endif

#ifndef  TRENDTABLE_DEFAULT_COLUMNNUMBER
#define  TRENDTABLE_DEFAULT_COLUMNNUMBER  0
#endif

#ifndef  TRENDTABLE_DEFAULT_PAGEPOSITION
#define  TRENDTABLE_DEFAULT_PAGEPOSITION  0
#endif

#ifndef  TRENDTABLE_DEFAULT_ROWPOSITION
#define  TRENDTABLE_DEFAULT_ROWPOSITION  0
#endif

#ifndef  TRENDTABLE_DEFAULT_BOTTOMINTERVAL
#define  TRENDTABLE_DEFAULT_BOTTOMINTERVAL  40
#endif

#ifndef  TRENDTABLE_DEFAULT_ITEMNUMBER
#define  TRENDTABLE_DEFAULT_ITEMNUMBER  360
#endif

#ifndef  TRENDTABLE_DEFAULT_DATEANDTIME_INTERVAL    //日期和时间轴的间距
#define  TRENDTABLE_DEFAULT_DATEANDTIME_INTERVAL  40
#endif

#ifndef  TRENDTABLE_DEFAULT_BK_COLOR
#define  TRENDTABLE_DEFAULT_BK_COLOR  GUI_BLACK
#endif

#ifndef  TRENDTABLE_DEFAULT_PRESSED_BK_COLOR
#define  TRENDTABLE_DEFAULT_PRESSED_BK_COLOR  0xFB9E85
#endif

#ifndef  TRENDTABLE_DEFAULT_FONT_COLOR
#define  TRENDTABLE_DEFAULT_FONT_COLOR  GUI_WHITE
#endif

#ifndef  TRENDTABLE_DEFAULT_FONT_PRESSED_COLOR
#define  TRENDTABLE_DEFAULT_FONT_PRESSED_COLOR  GUI_BLACK
#endif

#ifndef  TRENDTABLE_DEFAULT_FRAME_COLOR
#define  TRENDTABLE_DEFAULT_FRAME_COLOR  0xF7D7BD
#endif

#ifndef  TRENDTABLE_DEFAULT_TIME_COLOR
#define  TRENDTABLE_DEFAULT_TIME_COLOR  0xFB9E85
#endif

#ifndef  TRENDTABLE_DEFAULT_FONT
#define  TRENDTABLE_DEFAULT_FONT  GUI_Font_16//GUI_Font16_ASCII
#endif


//声明函数
static RTC_TIMER TrendTable_GetTime(RTC_TIMER CurrentRtc,int ItemNum,int Pos,int TimeInterval);


/*
*TrendTable_SetInfo
*设置趋势表的配置信息
*/
void TrendTable_SetInfo(TrendTable_Handle hObj,int RowNumber,int ColumnNumber,int RowPosition,int ItemNumber,RTC_TIMER Time,int TimeRange)
{
    int cnt,column;   
    if (hObj)
    {
        TrendTable_Obj* pObj;
        WM_LOCK();
        pObj = TrendTable_H2P(hObj);    

        pObj->ItemNumber   = ItemNumber;
        pObj->RowNumber    = RowNumber;
        pObj->ColumnNumber = ColumnNumber+1;
        pObj->RowPosition  = RowPosition;

        pObj->PaintFlag = Paint_All;

        pObj->Heigth = (pObj->Widget.Win.Rect.y1-pObj->Widget.Win.Rect.y0 + 1 - pObj->Interval)/(RowNumber);
        pObj->Width  = (pObj->Widget.Win.Rect.x1-pObj->Widget.Win.Rect.x0)/(ColumnNumber+1);

        //时间信息
        pObj->TrendTable_Time.RTC_Year = Time.RTC_Year;
        pObj->TrendTable_Time.RTC_Mon  = Time.RTC_Mon;
        pObj->TrendTable_Time.RTC_Mday = Time.RTC_Mday;
        pObj->TrendTable_Time.RTC_Hour = Time.RTC_Hour;
        pObj->TrendTable_Time.RTC_Min  = Time.RTC_Min;
        pObj->TrendTable_Time.RTC_Sec  = Time.RTC_Sec;
        pObj->TimeRange = TimeRange;
        pObj->TimeInterval = TimeRange*3600/pObj->ItemNumber;

        //时间坐标轴的位置信息          LCD_HL_DrawHLine(r.x0,r.y0+(r.y1 - r.y0)/2+5,r.x0+pObj->ItemNumber-1);//时间

        pObj->TimeRect.x0 = pObj->Widget.Win.Rect.x0 + pObj->Width/2;
        pObj->TimeRect.x1 = pObj->TimeRect.x0 + pObj->ItemNumber-1;
        pObj->TimeRect.y0 = (pObj->Widget.Win.Rect.y1 - pObj->Interval)+pObj->Interval/2+5;
        pObj->TimeRect.y1 = pObj->Widget.Win.Rect.y1;

        //在创建函数中动态分配地址，在回调函数Delete中释放
        pObj->ParaDispDecimal = (unsigned char *)malloc(sizeof(unsigned char)*pObj->ColumnNumber); 
        if(pObj->ParaDispDecimal != NULL)
        {
            for(cnt = 0;cnt < pObj->ColumnNumber;cnt++)//初始化显示数据的位数
            {
                *(pObj->ParaDispDecimal + cnt) = 0;
            }
        }
        else 
        {
            return;
        }
        //short **TrendTable_Buffer;
        pObj->TrendTable_Buffer = (short **)malloc(sizeof(short *)*pObj->ColumnNumber);//创建列指针
        if(pObj->TrendTable_Buffer != NULL)
        {
            for(cnt = 0;cnt < pObj->ColumnNumber;cnt++)//创建行指针
            {
				*(pObj->TrendTable_Buffer + cnt) = (short*)malloc(sizeof(short)*pObj->ItemNumber);
                if(*(pObj->TrendTable_Buffer + cnt) == NULL)//判断错误
                {
                    return;
                }
				memset(*(pObj->TrendTable_Buffer + cnt),0,pObj->ItemNumber*sizeof(short));//初始化
            }
        }
        else 
        {
            return;
        }

        TrendTable_Invalidate(hObj);
        WM_UNLOCK();
    }
}
/*
*_TrendTableDelete
*删除控件之后，释放动态创建的buffer
*/
static void _TrendTableDelete(TrendTable_Handle hObj)
{
    int cnt;
    TrendTable_Obj* pObj;
    if (hObj)
    {
        WM_LOCK();
        pObj = TrendTable_H2P(hObj);
        if(pObj->ParaDispDecimal != NULL)
        {
            free(pObj->ParaDispDecimal);
        }
        if(pObj->TrendTable_Buffer != NULL)
        {
            for(cnt = 0;cnt < pObj->ColumnNumber;cnt++)//创建行指针
            {
                if(*(pObj->TrendTable_Buffer + cnt) != NULL)//判断错误
                {
                    free(*(pObj->TrendTable_Buffer + cnt));
                }
            }
            free(pObj->TrendTable_Buffer);
         }
        WM_UNLOCK();
    }
}

//绘制表格边框
static void _DrawFrame(TrendTable_Obj* pObj)
{
    int row,column;
    GUI_RECT r;
    WM_GetWindowRect(&r);

    LCD_SetColor(pObj->Frame_Color);
    for(row = 0;row <= pObj->RowNumber;row++)//画行线
    {
        LCD_HL_DrawHLine(r.x0,r.y0+pObj->Heigth*row,r.x1);
    }
    for(column = 0;column <= pObj->ColumnNumber;column++)//画列线
    {
        LCD_DrawVLine(r.x0 + column*pObj->Width,r.y0,r.y1-pObj->Interval);
    }

}
//绘制选中行，并且还原上一次选中的行
static void _DrawRowPosition(TrendTable_Obj* pObj)
{
    int column;
    GUI_RECT r;
    WM_GetWindowRect(&r);
    //绘制前一次选中的行
    if(pObj->Per_RowPosition != pObj->RowPosition)
    {
        LCD_SetColor(pObj->Table_BkColor);
        LCD_FillRect(r.x0,r.y0+pObj->Heigth*pObj->Per_RowPosition+1,r.x1,r.y0+pObj->Heigth*(pObj->Per_RowPosition+1)-1);
        LCD_SetColor(pObj->Frame_Color);
        for(column = 0;column <= pObj->ColumnNumber;column++)//画列线
        {
            LCD_DrawVLine(r.x0 + column*pObj->Width,r.y0+pObj->Heigth*pObj->Per_RowPosition+1,r.y0+pObj->Heigth*(pObj->Per_RowPosition+1)-1);
        }
    }
    //绘制当前选中行
    LCD_SetColor(pObj->Table_Pressed_BkColor);
    LCD_FillRect(r.x0,r.y0+pObj->Heigth*pObj->RowPosition+1,r.x1,r.y0+pObj->Heigth*(pObj->RowPosition+1)-1);
    for(column = 0;column <= pObj->ColumnNumber;column++)//画列线
    {
        LCD_DrawVLine(r.x0 + column*pObj->Width,r.y0+pObj->Heigth*pObj->RowPosition+1,r.y0+pObj->Heigth*(pObj->RowPosition+1)-1);
    }    
}
//绘制时间和数据
//Flag :  0:全部刷新    1:只刷新当前行和前一行   2:只刷新某一列   
static void _DrawText(TrendTable_Obj* pObj,int Flag)
{
    int row,column;
    char DispBuff[15]={0};
    GUI_RECT r,rect;
    RTC_TIMER Time;
    WM_GetWindowRect(&r);
    
    GUI_SetFont(&pObj->Table_Font);

    if(Flag == 0)//全部刷新，有可能是切换了页面，所以要重新计算时间
    {
		//时间一列可以省略，不创建，应为暂时没有用到
   //     for(row = 0;row < pObj->RowNumber;row++)//按列刷新显示数据
   //     {
			//pObj->TrendTable_Buffer[0][row+pObj->Page_Position*pObj->RowNumber] = pObj->Page_Position*pObj->RowNumber + row;
   //     }
    }

    for(column = 0;column < pObj->ColumnNumber;column++)//按列刷新显示数据
    {
        if((Flag == 2)&&(column != pObj->ColumnPosition))//如果刷新某一列，则判断当前列是否符合
        {
            continue;
        }
        for(row = 0;row < pObj->RowNumber;row++)
        {
            if((Flag == 1)&&(row != pObj->RowPosition)&&(row != pObj->Per_RowPosition))
            {
                continue;
            }
            if(column == 0)//时间列
            {
                if(row == pObj->RowPosition)//当前选中行
                {
                    GUI_SetColor(pObj->Font_Pressed_Color);
                }
                else
                {
                    LCD_SetColor(pObj->Time_Color);
                }
                Time = TrendTable_GetTime(pObj->TrendTable_Time,pObj->ItemNumber,(pObj->ItemNumber-1-pObj->Page_Position*pObj->RowNumber-row),pObj->TimeInterval);
                sprintf(DispBuff,"%02d:%02d:%02d",Time.RTC_Hour,Time.RTC_Min,Time.RTC_Sec);
            }
            else 
            {
                if(row == pObj->RowPosition)//当前选中行
                {
                    GUI_SetColor(pObj->Font_Pressed_Color);
                }
                else
                {
                    GUI_SetColor(pObj->Font_Color);
                }
                if((pObj->ParaDispDecimal[column] == 1)&&(pObj->TrendTable_Buffer[column][row+pObj->Page_Position*pObj->RowNumber] != 0))
                {
                    sprintf(DispBuff,"%.1f",(float)pObj->TrendTable_Buffer[column][row+pObj->Page_Position*pObj->RowNumber]/10);
                }
                else if((pObj->ParaDispDecimal[column] == 0)||(pObj->TrendTable_Buffer[column][row+pObj->Page_Position*pObj->RowNumber] == 0))
                {
                    sprintf(DispBuff,"%d",pObj->TrendTable_Buffer[column][row+pObj->Page_Position*pObj->RowNumber]);
                }
            }
            rect.x0 = column*pObj->Width;
            rect.x1 = (column+1)*pObj->Width;
            rect.y0 = row*pObj->Heigth;
            rect.y1 = (row+1)*pObj->Heigth;
            GUI_SetTextMode(GUI_TM_TRANS);
            GUI_DispStringInRect(DispBuff,&rect,TEXT_CF_HCENTER|TEXT_CF_VCENTER);
        }
    }
}


//绘制时间轴
//Flag :  0:全部刷新(时间范围和光标位置)    1:只刷光标位置
static void _DrawTimeShaft(TrendTable_Obj* pObj,int Flag)
{
    int Cursor = pObj->Page_Position*pObj->RowNumber + pObj->RowPosition;
    int PerCursor = pObj->Per_Page_Position*pObj->RowNumber + pObj->Per_RowPosition;
    char DispBuff[20]={0};
    RTC_TIMER Time;
    GUI_RECT r,rect;
    r = pObj->TimeRect;
    GUI_SetFont(&pObj->Table_Font);


    LCD_SetColor(pObj->Time_Color);
    if(Flag == 0)
    {
        LCD_HL_DrawHLine(r.x0,r.y0,r.x1);//时间
        LCD_DrawVLine(r.x0+pObj->ItemNumber-1-Cursor,r.y0-10,r.y0-1);//光标
        //首个时间点
        Time = TrendTable_GetTime(pObj->TrendTable_Time,pObj->ItemNumber,(0),pObj->TimeInterval);
        sprintf(DispBuff,"%02d:%02d:%02d",Time.RTC_Hour,Time.RTC_Min,Time.RTC_Sec);
        GUI_DispStringAt(DispBuff,r.x0-pObj->Widget.Win.Rect.x0-GUI_GetStringDistX(DispBuff)/2,r.y0-pObj->Widget.Win.Rect.y0+3);
        //中间时间点
        Time = TrendTable_GetTime(pObj->TrendTable_Time,pObj->ItemNumber,(pObj->ItemNumber/2-1),pObj->TimeInterval);
        sprintf(DispBuff,"%02d:%02d:%02d",Time.RTC_Hour,Time.RTC_Min,Time.RTC_Sec);
        GUI_DispStringAt(DispBuff,r.x0-pObj->Widget.Win.Rect.x0-GUI_GetStringDistX(DispBuff)/2+pObj->ItemNumber/2,r.y0-pObj->Widget.Win.Rect.y0+3);
        //末尾时间点
        Time = TrendTable_GetTime(pObj->TrendTable_Time,pObj->ItemNumber,(pObj->ItemNumber-1),pObj->TimeInterval);
        sprintf(DispBuff,"%02d:%02d:%02d",Time.RTC_Hour,Time.RTC_Min,Time.RTC_Sec);
        GUI_DispStringAt(DispBuff,r.x0-pObj->Widget.Win.Rect.x0-GUI_GetStringDistX(DispBuff)/2+pObj->ItemNumber,r.y0-pObj->Widget.Win.Rect.y0+3);

    }
    else if(Flag == 1)
    {
        if(Cursor != PerCursor)
        {

            LCD_SetColor(pObj->Table_BkColor);
            LCD_DrawVLine(r.x0+pObj->ItemNumber-1-PerCursor,r.y0-10,r.y0-1);//光标
            PerCursor = pObj->Page_Position*pObj->RowNumber + pObj->Per_RowPosition;
            LCD_DrawVLine(r.x0+pObj->ItemNumber-1-PerCursor,r.y0-10,r.y0-1);//光标

            LCD_SetColor(pObj->Time_Color);
            LCD_DrawVLine(r.x0+pObj->ItemNumber-1-Cursor,r.y0-10,r.y0-1);//光标
        }
    }

    //刷新坐标轴上的时间点标志
    LCD_SetColor(GUI_WHITE);
    LCD_DrawVLine(r.x0+pObj->ItemNumber/2-1,r.y0-7,r.y0-1);//行中标志
    LCD_DrawVLine(r.x0,r.y0-5,r.y0-1);//行首标志
    LCD_DrawVLine(r.x0+pObj->ItemNumber-1,r.y0-5,r.y0-1);//行末标志
    //当前时间显示
    LCD_SetColor(pObj->Table_BkColor);
    LCD_FillRect(r.x0+pObj->ItemNumber+TRENDTABLE_DEFAULT_DATEANDTIME_INTERVAL,
                 r.y0-GUI_GetFontSizeY()-1,
                 pObj->Widget.Win.Rect.x1,
                 pObj->Widget.Win.Rect.y1);

    LCD_SetColor(pObj->Time_Color);
    Time = TrendTable_GetTime(pObj->TrendTable_Time,pObj->ItemNumber,(pObj->ItemNumber-1-pObj->RowPosition-pObj->Page_Position*pObj->RowNumber),pObj->TimeInterval);
    sprintf(DispBuff,"%04d-%02d-%02d",Time.RTC_Year,Time.RTC_Mon,Time.RTC_Mday);
    GUI_DispStringAt(DispBuff,r.x0-pObj->Widget.Win.Rect.x0+pObj->ItemNumber+TRENDTABLE_DEFAULT_DATEANDTIME_INTERVAL,r.y0-pObj->Widget.Win.Rect.y0-GUI_GetFontSizeY()-1);
    sprintf(DispBuff,"%02d:%02d:%02d",Time.RTC_Hour,Time.RTC_Min,Time.RTC_Sec);
    GUI_DispStringAt(DispBuff,r.x0-pObj->Widget.Win.Rect.x0+pObj->ItemNumber+TRENDTABLE_DEFAULT_DATEANDTIME_INTERVAL,r.y0-pObj->Widget.Win.Rect.y0+3);

}


//刷新数据之前，应该先填充数据Buffer
static void _TrendTablePaint(TrendTable_Handle hObj, TrendTable_Obj* pObj)
{
    int cnt;
    int X_Offset,Y_Offset;
    GUI_RECT r;
    WM_GetWindowRect(&r);

    switch(pObj->PaintFlag)
    {
    case Paint_Row:
        _DrawRowPosition(pObj);//绘制选中行
        _DrawText(pObj,1);//刷新数据buffer
        _DrawTimeShaft(pObj,1);
        break;
    case Paint_Column:
        _DrawText(pObj,2);//刷新数据buffer
        break;
    case Paint_All:  //只有当前选项才刷新边框
    case Paint_AllData:
        LCD_SetBkColor(pObj->Table_BkColor);
	    GUI_Clear();
        _DrawFrame(pObj);   //绘制边框
        _DrawRowPosition(pObj);  //绘制选中行
        _DrawText(pObj,0);  //刷新数据buffer
        _DrawTimeShaft(pObj,0);   //刷新时间  
        //if(pObj->PaintFlag == Paint_All)
        //{
        //    _DrawTimeShaft(pObj,0);   //刷新时间  
        //}
        //else if(pObj->PaintFlag == Paint_AllData)
        //{
        //    _DrawTimeShaft(pObj,1);   //刷新时间  
        //}
        break;
    default:
        break;
    }
    pObj->PaintFlag = 255;//默认重绘动作，不选择任何一个。如果本次重绘不是人为触发的，则不进行任何响应
}

static void  _TrendTableOnKey(TrendTable_Handle hObj, WM_MESSAGE* pMsg)
{
    WM_KEY_INFO* pKeyInfo;
	WM_MESSAGE  Msg;
    pKeyInfo = (WM_KEY_INFO*)(pMsg->Data.p);
    if (pKeyInfo->PressedCnt > 0)
    {
        switch (pKeyInfo->Key)
        {
        case GUI_KEY_RIGHT:
			Msg.Data.v = GUI_KEY_RIGHT;
            break;                    /* Send to parent by not doing anything */
        case GUI_KEY_LEFT:
		    Msg.Data.v = GUI_KEY_LEFT;
            break;                    /* Send to parent by not doing anything */
        default:
            return;
        }
		Msg.MsgId = WM_NOTIFY_PARENT;
		WM_SendToParent(hObj,&Msg);

    }
}

static void _TrendTableOnTouch(TrendTable_Handle hObj, TrendTable_Obj* pObj, WM_MESSAGE*pMsg)
{
    int Notification;
    int Hit = 0;
    int row = 0;
    GUI_PID_STATE* pState = (GUI_PID_STATE*)pMsg->Data.p;
    if (pMsg->Data.p)    /* Something happened in our area (pressed or released) */
    {
        if (pState->Pressed)
        {
            //选中表格区域
            for(row = 0;row < pObj->RowNumber;row++)
            {
                if((row*pObj->Heigth <= pState->y)&&((row+1)*pObj->Heigth > pState->y))
                {
                    TrendTable_SetSel(hObj,row);
                    break;
                }
            }
            //选中光标区域（移动到光标所在位置）
            if((pState->y >= pObj->Heigth*pObj->RowNumber)
                &&(pState->x >= pObj->TimeRect.x0 - pObj->Widget.Win.Rect.x0)&&(pState->x <= pObj->TimeRect.x1 - pObj->Widget.Win.Rect.x0))
            {
                pObj->Per_Page_Position = pObj->Page_Position;
                if((pObj->TimeRect.x1 - pObj->Widget.Win.Rect.x0 - pState->x))
                {
                    pObj->Page_Position = (pObj->TimeRect.x1 - pObj->Widget.Win.Rect.x0 - pState->x)/pObj->RowNumber;
                }
                else 
                {
                    pObj->Page_Position = 0;
                }
                pObj->Per_RowPosition = pObj->RowPosition;
                pObj->RowPosition   = (pObj->TimeRect.x1 - pObj->Widget.Win.Rect.x0- pState->x)%pObj->RowNumber;
                pObj->PaintFlag = Paint_AllData;
                TrendTable_Invalidate(hObj);
            }
            WM_NotifyParent(hObj,WM_NOTIFICATION_CLICKED);
        }
        else
        {
        }
    }
    else
    {
    }
}



static void _TrendTable_Callback (WM_MESSAGE*pMsg)
{
    TrendTable_Handle hObj = pMsg->hWin;
    TrendTable_Obj* pObj = TrendTable_H2P(hObj);
    /* Let widget handle the standard messages */
    if (WIDGET_HandleActive(hObj, pMsg) == 0)
    {
        return;
    }
    switch (pMsg->MsgId)
    {
    case WM_PAINT:
         GUI_DEBUG_LOG("TEXT: _Callback(WM_PAINT)\n");
        _TrendTablePaint(hObj, pObj);
        return;
    case WM_TOUCH:
        _TrendTableOnTouch(hObj, pObj, pMsg);
        return;
    case WM_KEY:
//        _TrendTableOnKey(hObj, pMsg);
        return;
    case WM_DELETE:
        GUI_DEBUG_LOG("TEXT: _Callback(WM_DELETE)\n");
        _TrendTableDelete(hObj);
        return;       /* No return here ... WM_DefaultProc needs to be called */
    }
    WM_DefaultProc(pMsg);
}





TrendTable_Handle TrendTable_CreateAsChild(
    int x0, int y0, int xsize, int ysize, WM_HWIN hParent,int Id,
    WM_CALLBACK* cb, int Flags)
{
    TrendTable_Handle hObj;
    int cnt;
    /* Create the window */
    WM_LOCK();
    hObj = WM_CreateWindowAsChild(x0, y0, xsize, ysize, hParent,
                                  Flags, _TrendTable_Callback, sizeof(TrendTable_Obj) - sizeof(WM_Obj));
    if (hObj)
    {
        TrendTable_Obj* pObj = TrendTable_H2P(hObj);
        /* init widget specific variables */
        WIDGET__Init(&pObj->Widget,Id, WIDGET_STATE_FOCUSSABLE);


        pObj->ItemNumber = TRENDTABLE_DEFAULT_ITEMNUMBER;
        pObj->RowNumber = TRENDTABLE_DEFAULT_ROWNUMBER;
        pObj->ColumnNumber = TRENDTABLE_DEFAULT_COLUMNNUMBER;
        pObj->Page_Position = TRENDTABLE_DEFAULT_PAGEPOSITION;
        pObj->Per_Page_Position = TRENDTABLE_DEFAULT_PAGEPOSITION;
        pObj->Interval = TRENDTABLE_DEFAULT_BOTTOMINTERVAL;
        pObj->RowPosition = TRENDTABLE_DEFAULT_ROWPOSITION;
        pObj->Per_RowPosition = TRENDTABLE_DEFAULT_ROWPOSITION;
        pObj->Table_BkColor = TRENDTABLE_DEFAULT_BK_COLOR;
        pObj->Table_Pressed_BkColor = TRENDTABLE_DEFAULT_PRESSED_BK_COLOR;
        pObj->Font_Color = TRENDTABLE_DEFAULT_FONT_COLOR;
        pObj->Font_Pressed_Color = TRENDTABLE_DEFAULT_FONT_PRESSED_COLOR;
        pObj->Frame_Color = TRENDTABLE_DEFAULT_FRAME_COLOR;
        pObj->Time_Color = TRENDTABLE_DEFAULT_TIME_COLOR;
        pObj->Table_Font = TRENDTABLE_DEFAULT_FONT;
        pObj->PaintFlag = Paint_All;

	}
    WM_UNLOCK();
    return hObj;
}

TrendTable_Handle TrendTable_CreateIndirect(const GUI_WIDGET_CREATE_INFO* pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK* cb)
{
    TrendTable_Handle  hThis;
//    int NumItems = (pCreateInfo->Para)       & 0xFF;
//    GUI_USE_PARA(cb);
    hThis = TrendTable_CreateAsChild(pCreateInfo->x0 + x0, pCreateInfo->y0 + y0, pCreateInfo->xSize, pCreateInfo->ySize,
                           hWinParent,pCreateInfo->Id,cb,pCreateInfo->Flags);
    return hThis;
}
/*
*TrendTable_SetPagePosition
*设置当前的页数，用于计算每一行对应的时间点，还有当前选中的行在标尺上的位置。
*/

void TrendTable_SetPagePosition(TrendTable_Handle hObj,int Page_Position,int RowPosition)
{
    TrendTable_Obj* pObj;
	unsigned char Page_num;
	if(hObj)
	{
        WM_LOCK();
		pObj = TrendTable_H2P(hObj);
        if((Page_Position>=0)&&(Page_Position < pObj->ItemNumber/pObj->RowNumber))
        {
            pObj->Per_RowPosition = pObj->RowPosition;;
		    pObj->RowPosition = RowPosition;
            pObj->Per_Page_Position = pObj->Page_Position;
		    pObj->Page_Position = Page_Position;
		    pObj->PaintFlag = Paint_AllData;
		    TrendTable_Invalidate(hObj);
        }
		WM_UNLOCK();
	}
}
int TrendTable_GetPagePosition(TrendTable_Handle hObj)
{
    TrendTable_Obj* pObj;
	int Page_num;
	if(hObj)
	{
        WM_LOCK();
		pObj = TrendTable_H2P(hObj);
		Page_num = pObj->Page_Position;
		WM_UNLOCK();
	}
    return Page_num;
}
/*
*TrendTable_SetSel
*设置当前页面上，被选中的项目所在的行数。
*/

void TrendTable_SetSel(TrendTable_Handle hObj,unsigned char RowPosition)
{
    TrendTable_Obj* pObj;
	if(hObj)
	{
        WM_LOCK();
		pObj = TrendTable_H2P(hObj);
        if(RowPosition > pObj->RowNumber-1)
        {
            RowPosition = pObj->RowNumber-1;
        }
		pObj->Per_RowPosition = pObj->RowPosition;
		pObj->RowPosition = RowPosition;
		pObj->PaintFlag = Paint_Row;
		TrendTable_Invalidate(hObj);
		WM_UNLOCK();
	}
}

/*
*TrendTable_GetSel
*设置当前页面上，被选中的项目所在的行数。
*/

unsigned char TrendTable_GetSel(TrendTable_Handle hObj)
{
    TrendTable_Obj* pObj;
	unsigned char RowPosition;
	if(hObj)
	{
        WM_LOCK();
		pObj = TrendTable_H2P(hObj);
		RowPosition = pObj->RowPosition;
		WM_UNLOCK();
	}
	return RowPosition;
}

/*
*TrendTable_IncSel
*选中下一个项目
*return 0:当前页面移动   1:切换到下一页
*/

int TrendTable_IncSel(TrendTable_Handle hObj)
{
    TrendTable_Obj* pObj;
    int ret = 0;
	if(hObj)
	{
        WM_LOCK();
		pObj = TrendTable_H2P(hObj);
        if(pObj->RowPosition < pObj->RowNumber - 1)//在当前页面向下移动一个
        {
            pObj->Per_Page_Position = pObj->Page_Position;
		    pObj->Per_RowPosition = pObj->RowPosition;
		    pObj->RowPosition++;
		    pObj->PaintFlag = Paint_Row;
	        TrendTable_Invalidate(hObj);
        }
        else //切换到下一页
        {
            if(pObj->Page_Position < pObj->ItemNumber/pObj->RowNumber-1)
            {
                ret = 1;
                pObj->RowPosition = 0;
                pObj->Per_RowPosition = pObj->RowNumber - 1;
                pObj->Per_Page_Position = pObj->Page_Position;
                pObj->Page_Position++;
		        pObj->PaintFlag = Paint_AllData;
//	            TrendTable_Invalidate(hObj);//翻页后，需要填充显示buffer后，手动刷新
            }
        }
		WM_UNLOCK();
    }
    return ret;
}
/*
*TrendTable_IncSel
*选中上一个项目
*return 0:当前页面移动   1:切换到上一页
*/

int TrendTable_DecSel(TrendTable_Handle hObj)
{
    TrendTable_Obj* pObj;
    int ret = 0;
	if(hObj)
	{
        WM_LOCK();
		pObj = TrendTable_H2P(hObj);
        if(pObj->RowPosition > 0)//在当前页面向上移动一个
        {
            pObj->Per_Page_Position = pObj->Page_Position;
		    pObj->Per_RowPosition = pObj->RowPosition;
		    pObj->RowPosition--;
		    pObj->PaintFlag = Paint_Row;
	        TrendTable_Invalidate(hObj);
        }
        else //切换到上一页
        {
            if(pObj->Page_Position > 0)
            {
                ret = 1;
                pObj->RowPosition = pObj->RowNumber - 1;
                pObj->Per_RowPosition = 0;
                pObj->Per_Page_Position = pObj->Page_Position;
                pObj->Page_Position--;
		        pObj->PaintFlag = Paint_AllData;
//	            TrendTable_Invalidate(hObj); //翻页后，需要填充显示buffer后，手动刷新
            }
        }
		WM_UNLOCK();
    }
    return ret;
}

/*
*TrendTable_SetColumn
*设置想要更改参数的一列列号
*/

void TrendTable_SetColumn(TrendTable_Handle hObj,unsigned char Column)
{
    TrendTable_Obj* pObj;
	if(hObj)
	{
        WM_LOCK();
		pObj = TrendTable_H2P(hObj);
        pObj->ColumnPosition = Column;
		pObj->PaintFlag = Paint_Column;
		TrendTable_Invalidate(hObj);
		WM_UNLOCK();
	}
}
/*
*TrendTable_SetPaintType
*设置重绘的类型，并且进行一次重绘
*/
void TrendTable_SetPaintType(TrendTable_Handle hObj,int PaintType)
{
    TrendTable_Obj* pObj;
	if(hObj)
	{
        WM_LOCK();
		pObj = TrendTable_H2P(hObj);
		pObj->PaintFlag = PaintType;
		TrendTable_Invalidate(hObj);
		WM_UNLOCK();
	}
}
/*
*TrendTable_SetTimeRange
*设置时间范围，并且刷新显示
*/
void TrendTable_SetTimeRange(TrendTable_Handle hObj,int TimeRange)
{
    TrendTable_Obj* pObj;
	if(hObj)
	{
        WM_LOCK();
		pObj = TrendTable_H2P(hObj);
        pObj->TimeRange = TimeRange;
        pObj->TimeInterval = TimeRange*3600/pObj->ItemNumber;
        pObj->PaintFlag = Paint_AllData;
		TrendTable_Invalidate(hObj);
		WM_UNLOCK();
	}
}
/*
*TrendTable_GetTimeRange
*获得当前的时间范围
*/
unsigned char TrendTable_GetTimeRange(TrendTable_Handle hObj)
{
    TrendTable_Obj* pObj;
    unsigned char TimeRange;
	if(hObj)
	{
        WM_LOCK();
		pObj = TrendTable_H2P(hObj);
        TimeRange = pObj->TimeRange;
		WM_UNLOCK();
	}
    return TimeRange;
}

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

static RTC_TIMER TrendTable_GetTime(RTC_TIMER CurrentRtc,int ItemNum,int Pos,int TimeInterval)//使用这个函数替换趋势图中的这个名称的函数
{
    RTC_TIMER ret_time;
    int tmp= (ItemNum - (Pos + 1))*TimeInterval;//当前光标时间点 和 当前时间点的时间间隔
    if((tmp%60) <= CurrentRtc.RTC_Sec)
    {
        ret_time.RTC_Sec  = CurrentRtc.RTC_Sec - tmp%60;
        tmp = tmp/60;//Min
    }
    else  
    {
        ret_time.RTC_Sec  = 60 + CurrentRtc.RTC_Sec - tmp%60;
        tmp = tmp/60 + 1;
    }

    if((tmp > 0) && ((tmp % 60) <= CurrentRtc.RTC_Min))
    {
        ret_time.RTC_Min  = CurrentRtc.RTC_Min - tmp%60;
        tmp = tmp/60;//Hour
    }
    else if(tmp > 0)
    {
        ret_time.RTC_Min  = 60 + CurrentRtc.RTC_Min - tmp%60;
        tmp = tmp/60 + 1;//Hour
    }
    else if(tmp == 0)
    {
        ret_time.RTC_Min  = CurrentRtc.RTC_Min;
        ret_time.RTC_Hour = CurrentRtc.RTC_Hour;
        ret_time.RTC_Mday = CurrentRtc.RTC_Mday;
        ret_time.RTC_Mon  = CurrentRtc.RTC_Mon;
        ret_time.RTC_Year = CurrentRtc.RTC_Year;
    }

    if((tmp > 0) && ((tmp % 24) <= CurrentRtc.RTC_Hour))
    {
        ret_time.RTC_Hour  = CurrentRtc.RTC_Hour - tmp%24;
        tmp = tmp/24;//day
    }
    else if(tmp > 0)
    {
        ret_time.RTC_Hour  = 24 + CurrentRtc.RTC_Hour - tmp%24;
        tmp = tmp/24 + 1;//day
    }
    else if(tmp == 0)
    {
        ret_time.RTC_Hour = CurrentRtc.RTC_Hour;
        ret_time.RTC_Mday = CurrentRtc.RTC_Mday;
        ret_time.RTC_Mon  = CurrentRtc.RTC_Mon;
        ret_time.RTC_Year = CurrentRtc.RTC_Year;
    }
    if((tmp > 0) && ((tmp + 1) <= CurrentRtc.RTC_Mday))
    {
        ret_time.RTC_Mday  = CurrentRtc.RTC_Mday - tmp;
        ret_time.RTC_Mon   = CurrentRtc.RTC_Mon;
        ret_time.RTC_Year  = CurrentRtc.RTC_Year;
    }
    else if(tmp > 0)
    {
        //1、3、5、7、8、10、12为闰月，31天
        if((CurrentRtc.RTC_Mon == 2) || (CurrentRtc.RTC_Mon == 4) 
         ||(CurrentRtc.RTC_Mon == 8) || (CurrentRtc.RTC_Mon == 9)
         ||(CurrentRtc.RTC_Mon == 11)|| (CurrentRtc.RTC_Mon == 1))
        {
            ret_time.RTC_Mday  = 31 + CurrentRtc.RTC_Mday - tmp;
            if(CurrentRtc.RTC_Mon == 1)
            {
                ret_time.RTC_Mon = 12;
                ret_time.RTC_Year  = CurrentRtc.RTC_Year - 1;
            }
            else
            {
                ret_time.RTC_Mon = CurrentRtc.RTC_Mon - 1;
                ret_time.RTC_Mday  = CurrentRtc.RTC_Year;
            }
        }
        //4、6、9、11为30天
        else if((CurrentRtc.RTC_Mon == 5) || (CurrentRtc.RTC_Mon == 7) 
              ||(CurrentRtc.RTC_Mon == 10) || (CurrentRtc.RTC_Mon == 12))
        {
            ret_time.RTC_Mday  = 30 + CurrentRtc.RTC_Mday - tmp;
            ret_time.RTC_Mon   = CurrentRtc.RTC_Mon - 1;
            ret_time.RTC_Mday  = CurrentRtc.RTC_Year;

        }
        else if(CurrentRtc.RTC_Mon == 2)//2月份，分平年和闰年 
        {
            ret_time.RTC_Mday  = LeapYear(CurrentRtc.RTC_Year) + CurrentRtc.RTC_Mday - tmp;//判断平闰年
            ret_time.RTC_Mon   = CurrentRtc.RTC_Mon - 1;
            ret_time.RTC_Mday  = CurrentRtc.RTC_Year;
        }
    }
    else if(tmp == 0)
    {
        ret_time.RTC_Mday = CurrentRtc.RTC_Mday;
        ret_time.RTC_Mon  = CurrentRtc.RTC_Mon;
        ret_time.RTC_Year = CurrentRtc.RTC_Year;
    }
    return (ret_time);
}



/*
*
*填充数据buffer
*BufferIndex == 0时为时间轴，时间轴不需要应用程序赋值。
*/
void TrendTable_FillBuffer(TrendTable_Handle hObj,short *Buffer,int BufferIndex)
{
    TrendTable_Obj* pObj;
	int cnt;
	if(hObj)
	{
        WM_LOCK();
		pObj = TrendTable_H2P(hObj);
		for(cnt = 0;cnt < pObj->ItemNumber;cnt++)
		{
			pObj->TrendTable_Buffer[BufferIndex][cnt]=Buffer[cnt];
		}
		WM_UNLOCK();
	}
}

/*
*
*设置小数位数
*/
void TrendTable_SetDecimal(TrendTable_Handle hObj,unsigned char column,unsigned char DecimalLen)
{
    TrendTable_Obj* pObj;
	int cnt;
	if(hObj)
	{
        WM_LOCK();
		pObj = TrendTable_H2P(hObj);
        pObj->ParaDispDecimal[column] = DecimalLen;
        WM_UNLOCK();
	}
}

