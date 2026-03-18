#ifndef MODETABBAR_H
#define MODETABBAR_H

#include "WM.H"
#include "GUI.H"

#if GUI_WINSUPPORT
#include "WIDGET.h"
#include "Dialog.h"

// 1. 定义句柄
typedef WM_HMEM ModeTabBar_Handle;

// 2. 预设最大支持的模式数量
#define MODETABBAR_MAX_ITEMS 16

// 3. 控件对象结构体
typedef struct {
    WIDGET Widget;               // 必须在第一位，实现继承
    
    // 颜色配置
    GUI_COLOR BkColor;           // 背景色 (深灰色)
    GUI_COLOR TextColor;         // 文本颜色 (默认白色)
    GUI_COLOR ActiveBorderColor; // 选中状态的边框颜色 (蓝色)
    const GUI_FONT GUI_UNI_PTR *pFont; // 字体
    
    // 核心数据模型
    const char* ModeNames[MODETABBAR_MAX_ITEMS]; // 存储所有模式名称的数组
    int TotalModes;              // 总模式数量
    int ItemsPerPage;            // 每页显示的模式数量 (不含翻页按钮)
    
    // 状态控制
    int CurrentSelection;        // 当前选中的模式索引 (0 ~ TotalModes-1)
    int CurrentPage;             // 当前页码 (0: 第一页, 1: 第二页)
} ModeTabBar_Obj;

// 4. 句柄转指针宏
#define ModeTabBar_H2P(h) (ModeTabBar_Obj*) GUI_ALLOC_h2p(h)

// --- API 声明 ---

// 创建控件
ModeTabBar_Handle ModeTabBar_CreateAsChild(int x0, int y0, int xsize, int ysize, 
                                           WM_HWIN hParent, int Id, int Flags);

// 间接创建 (用于 Dialog)
ModeTabBar_Handle ModeTabBar_CreateIndirect(const GUI_WIDGET_CREATE_INFO* pCreateInfo, 
                                            WM_HWIN hWinParent, int x0, int y0, 
                                            WM_CALLBACK* pCallback);

// 获取当前选中的模式索引
int ModeTabBar_GetSelection(ModeTabBar_Handle hObj);

// 代码设置选中项 (例如开机默认选中某个模式)
void ModeTabBar_SetSelection(ModeTabBar_Handle hObj, int Index);

#endif // GUI_WINSUPPORT
#endif // MODETABBAR_H