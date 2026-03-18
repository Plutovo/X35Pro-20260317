/***********************************************************************
*Author：djq
*Date：20160930
*Fuction：
    Process_Widget是一个过程进度显示的小控件，可以自定义控件的大小显示状态、
显示形式、显示颜色和大小等等；例如：整个控件有5步，当前为第二步，如下图所示。
                       ○●○○○
************************************************************************/

#ifndef _PROCESS_WIDGET_H_
#define _PROCESS_WIDGET_H_

#include "WM.h"
#include "DIALOG_Intern.h"      /* Req. for Create indirect data structure */
#include "WIDGET.h"      /* Req. for Create indirect data structure */
#include "GUIDebug.h" /* Req. for GUI_DEBUG_LEVEL */

typedef WM_HMEM ProcessWidget_Handle;

typedef enum {Rect,Round,HLine,VLine}Style_E;//表现形式□ ○ | —

typedef struct
{
    WIDGET Widget;//GUI结构体
    
    int Size;//单个状态标志的大小
    int Interval;//图形之间的间距
    int Number;//标志的总数
    int Index;//当前选中标志的索引
    int RowNum;//行数
    Style_E Style;//表现形式
    //非选中状态
    GUI_COLOR InactiveColor;
    //选中状态
    GUI_COLOR ActiveColor;
    //背景颜色
    GUI_COLOR BkColor;


//    int Align;//对齐方式
} ProcessWidget_Obj;

#define ProcessWidget_H2P(h) (ProcessWidget_Obj*) GUI_ALLOC_h2p(h)
#define ProcessWidget_Invalidate(hObj) WM_InvalidateWindow(hObj)

ProcessWidget_Handle ProcessWidget_CreateAsChild(
    int x0, int y0, int xsize, int ysize, WM_HWIN hParent,int Id,
    WM_CALLBACK* cb, int Flags);
ProcessWidget_Handle ProcessWidget_CreateIndirect(const GUI_WIDGET_CREATE_INFO* pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK* cb);


void ProcessWidget_SetBkColor(ProcessWidget_Handle hObj,GUI_COLOR BkColor);
void ProcessWidget_SetActiveColor(ProcessWidget_Handle hObj,GUI_COLOR ActiveColor);
void ProcessWidget_SetInactiveColor(ProcessWidget_Handle hObj,GUI_COLOR InactiveColor);
int  ProcessWidget_SetIndexNumber(ProcessWidget_Handle hObj,int Index);


#endif //_PROCESS_WIDGET_H_