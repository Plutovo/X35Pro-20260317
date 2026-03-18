#ifndef PAWGAUGE_H
#define PAWGAUGE_H

#include "WM.H"
#include "GUI.H"

#if GUI_WINSUPPORT
#include "WIDGET.h"
#include "Dialog.h"

// 1. 定义句柄
typedef WM_HMEM PawGauge_Handle;

// 2. 控件对象结构体
typedef struct {
    WIDGET Widget;               // 必须在第一位，实现继承
    
    int Data;                    // 当前气道压力值
    char Unit[16];
    // 颜色配置
    GUI_COLOR BkColor;           // 控件整体背景色 (通常为黑色)
    GUI_COLOR TrackColor;        // 仪表盘底轨颜色 (深灰色)
    GUI_COLOR FillColor;         // 动态填充颜色 (浅绿色)
    GUI_COLOR TickColor;         // 刻度线颜色
    GUI_COLOR TextColor;         // 文本颜色
} PawGauge_Obj;

// 3. 句柄转指针宏
#define PawGauge_H2P(h) (PawGauge_Obj*) GUI_ALLOC_h2p(h)

// --- API 声明 ---

// 创建控件
PawGauge_Handle PawGauge_CreateAsChild(int x0, int y0, int xsize, int ysize, 
                                       WM_HWIN hParent, int Id, int Flags);

// 间接创建 (用于 Dialog)
PawGauge_Handle PawGauge_CreateIndirect(const GUI_WIDGET_CREATE_INFO* pCreateInfo, 
                                        WM_HWIN hWinParent, int x0, int y0, 
                                        WM_CALLBACK* pCallback);

// 动态设置压力值 (触发刷新)
void PawGauge_SetData(PawGauge_Handle hObj, int Data);

// 获取当前压力值
int PawGauge_GetData(PawGauge_Handle hObj);

void PawGauge_SetUnit(PawGauge_Handle hObj, const char* pUnit);

#endif // GUI_WINSUPPORT
#endif // PAWGAUGE_H