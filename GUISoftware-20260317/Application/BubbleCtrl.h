#ifndef BUBBLECTRL_H
#define BUBBLECTRL_H

#include "WM.H"
#include "GUI.H"

#if GUI_WINSUPPORT
#include "WIDGET.h"
#include "Dialog.h"

// 1. 定义句柄 (Handle) 类型，ucGUI通过句柄管理内存
typedef WM_HMEM BubbleCtrl_Handle;

// 2. 定义属性配置结构体 (管理颜色、字体等)
typedef struct {
    GUI_COLOR BkColor;      // 背景色 (默认白色)
    GUI_COLOR TextColor;    // 文本颜色 (默认黑色)
    GUI_COLOR FrameColor;   // 边框颜色 (默认黑色)
    const GUI_FONT GUI_UNI_PTR *pFont; // 字体
    //char* pText;            // 显示文本
	char aText[32];
} BubbleCtrlInfo;

// 3. 定义控件对象结构体 (核心数据结构)
typedef struct {
    WIDGET Widget;          // 【极其关键】：必须放在结构体的最开头，实现继承
    BubbleCtrlInfo Info;    // 控件属性
    int TailWidth;          // 尾部三角形的宽度
    int TailHeight;         // 尾部三角形的高度
} BubbleCtrl_Obj;

// 4. 定义内存转换宏 (将 Handle 转换为 物理指针)
#define BubbleCtrl_H2P(h) (BubbleCtrl_Obj*) GUI_ALLOC_h2p(h)

// --- 对外提供的 API 函数声明 ---

// 创建控件作为子窗口
BubbleCtrl_Handle BubbleCtrl_CreateAsChild(int x0, int y0, int xsize, int ysize, 
                                           WM_HWIN hParent, int Id, int Flags);

// 支持通过资源表 (GUI_WIDGET_CREATE_INFO) 在对话框中自动创建控件
BubbleCtrl_Handle BubbleCtrl_CreateIndirect(const GUI_WIDGET_CREATE_INFO* pCreateInfo, 
											WM_HWIN hWinParent, int x0, int y0, 
											WM_CALLBACK* pCallback);

// 设置显示的文本
void BubbleCtrl_SetText(BubbleCtrl_Handle hObj, const char* pText);

// 设置气泡底色
void BubbleCtrl_SetBkColor(BubbleCtrl_Handle hObj, GUI_COLOR Color);

#endif // GUI_WINSUPPORT
#endif // BUBBLECTRL_H