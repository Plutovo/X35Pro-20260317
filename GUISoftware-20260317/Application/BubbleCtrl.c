#include <string.h>
#include <stdlib.h>
#include "GUI.H"
#include "GUI_Protected.h"
#include "WIDGET.h"
#include "BubbleCtrl.h"

#if GUI_WINSUPPORT

// 预设默认参数
static const BubbleCtrlInfo DefaultInfo = {
    0x59DDE5,      // 默认白底
    GUI_BLACK,      // 默认黑字
    GUI_BLACK,      // 默认黑边框
    &GUI_Font_16,  // 默认字体
    " "        // 默认文本
};

// 内部绘图函数 (结合了我们上一讲算好的坐标系)
static void _Paint(BubbleCtrl_Obj* pObj) {
	GUI_RECT r;
	GUI_POINT aPoints[3];

	int tail_width;
	int tail_height;
	int rect_bottom;
	int r_corner;
	int text_center_y;

	GUI_GetClientRect(&r);

	// --- 1. 几何参数配置 ---
	tail_width = 16;               // 尾部三角形宽度
	tail_height = 10;              // 尾部三角形高度
	rect_bottom = r.y1 - tail_height; // 主矩形的底边 Y 坐标

	// 【关键】：定义圆角半径
	r_corner = 2; 

	// 计算三角形的三个顶点
	aPoints[0].x = (r.x0 + r.x1) / 2 - tail_width / 2;
	aPoints[0].y = rect_bottom;
	aPoints[1].x = (r.x0 + r.x1) / 2 + tail_width / 2;
	aPoints[1].y = rect_bottom;
	aPoints[2].x = (r.x0 + r.x1) / 2;
	aPoints[2].y = r.y1;

	// ==========================================
	// --- 2. 绘制实心背景图层 (积木拼接法) ---
	// ==========================================
	GUI_SetColor(pObj->Info.BkColor);

	// 步骤 2.1：画四个角落的实心圆
	GUI_FillCircle(r.x0 + r_corner, r.y0 + r_corner, r_corner);         // 左上角
	GUI_FillCircle(r.x1 - r_corner, r.y0 + r_corner, r_corner);         // 右上角
	GUI_FillCircle(r.x0 + r_corner, rect_bottom - r_corner, r_corner);  // 左下角
	GUI_FillCircle(r.x1 - r_corner, rect_bottom - r_corner, r_corner);  // 右下角

	// 步骤 2.2：画两个互相垂直的矩形，填满中间的十字区域
	// 横向宽矩形 (左右顶到边缘，上下避开圆角)
	GUI_FillRect(r.x0, r.y0 + r_corner, r.x1, rect_bottom - r_corner);
	// 纵向高矩形 (上下顶到边缘，左右避开圆角)
	GUI_FillRect(r.x0 + r_corner, r.y0, r.x1 - r_corner, rect_bottom);

	// 步骤 2.3：画底部的实心三角形
	GUI_FillPolygon(aPoints, 3, 0, 0);


	// ==========================================
	// --- 3. 绘制外边框图层 (精准轮廓线) ---
	// ==========================================
	GUI_SetColor(pObj->Info.FrameColor);
	GUI_SetPenSize(1);

	// 步骤 3.1：画四个角的 1/4 圆弧
	// ucGUI的圆弧角度规定：以三点钟方向为0度，逆时针增加
	GUI_DrawArc(r.x1 - r_corner, r.y0 + r_corner, r_corner, r_corner, 0, 90);    // 右上角 (0~90度)
	GUI_DrawArc(r.x0 + r_corner, r.y0 + r_corner, r_corner, r_corner, 90, 180);  // 左上角 (90~180度)
	GUI_DrawArc(r.x0 + r_corner, rect_bottom - r_corner, r_corner, r_corner, 180, 270); // 左下角 (180~270度)
	GUI_DrawArc(r.x1 - r_corner, rect_bottom - r_corner, r_corner, r_corner, 270, 360); // 右下角 (270~360度)

	// 步骤 3.2：画连接圆弧的直线段
	GUI_DrawHLine(r.y0, r.x0 + r_corner, r.x1 - r_corner); // 顶部水平线
	GUI_DrawVLine(r.x0, r.y0 + r_corner, rect_bottom - r_corner); // 左侧垂直线
	GUI_DrawVLine(r.x1, r.y0 + r_corner, rect_bottom - r_corner); // 右侧垂直线

	// 步骤 3.3：画底部的边框线 (在此处断开，留给三角形尾巴)
	GUI_DrawHLine(rect_bottom, r.x0 + r_corner, aPoints[0].x); // 底边左半段
	GUI_DrawHLine(rect_bottom, aPoints[1].x, r.x1 - r_corner); // 底边右半段

	// 步骤 3.4：画三角形尾巴的两条斜线
	GUI_DrawLine(aPoints[0].x, aPoints[0].y, aPoints[2].x, aPoints[2].y); // 左斜线
	GUI_DrawLine(aPoints[1].x, aPoints[1].y, aPoints[2].x, aPoints[2].y); // 右斜线

	// --- 4. 绘制居中文字 ---
	// 【关键修改】：判断数组首字符是否为空，并使用 aText 渲染
	if (pObj->Info.aText[0] != '\0') {
		GUI_SetColor(pObj->Info.TextColor);
		GUI_SetTextMode(GUI_TM_TRANS);
		GUI_SetFont(pObj->Info.pFont);
		GUI_SetTextAlign(GUI_TA_HCENTER | GUI_TA_VCENTER);

		text_center_y = (r.y0 + rect_bottom) / 2;
		GUI_DispStringAt(pObj->Info.aText, (r.x0 + r.x1) / 2, text_center_y);
	}
}

// 控件核心回调函数 (处理各种系统消息)
static void _BubbleCtrl_Callback(WM_MESSAGE *pMsg) {
    BubbleCtrl_Handle hWin = pMsg->hWin;
    BubbleCtrl_Obj* pObj;

    // 先让系统处理基础的 Widget 消息
    if (WIDGET_HandleActive(hWin, pMsg) == 0) {
        return;
    }

    // 锁定内存并转换指针
    pObj = BubbleCtrl_H2P(hWin);

    switch (pMsg->MsgId) {
        case WM_PAINT: // 收到重绘请求
            _Paint(pObj);
            break;
        default:
            WM_DefaultProc(pMsg); // 其他未处理消息交还系统
    }
}

// API 1：创建控件
BubbleCtrl_Handle BubbleCtrl_CreateAsChild(int x0, int y0, int xsize, int ysize, 
                                           WM_HWIN hParent, int Id, int Flags) 
{
    BubbleCtrl_Handle hObj;

    WM_LOCK(); // 多任务安全保护
    // 向系统申请一块内存来创建窗口，大小为 WIDGET_Obj + 自定义的私有数据
    hObj = WM_CreateWindowAsChild(x0, y0, xsize, ysize, hParent, Flags, 
                                  _BubbleCtrl_Callback, 
                                  sizeof(BubbleCtrl_Obj) - sizeof(WM_Obj));
    if (hObj) {
        BubbleCtrl_Obj* pObj = BubbleCtrl_H2P(hObj);
        // 初始化基础 Widget 参数
        WIDGET__Init(&pObj->Widget, Id, WIDGET_STATE_FOCUSSABLE);
        
        // 初始化私有属性
        pObj->Info = DefaultInfo;
        pObj->TailWidth = 16;  // 尾部默认宽 16 像素
        pObj->TailHeight = 10; // 尾部默认高 10 像素
    }
    WM_UNLOCK();
    return hObj;
}

// API 4：间接创建控件 (用于 Dialog)
BubbleCtrl_Handle BubbleCtrl_CreateIndirect(const GUI_WIDGET_CREATE_INFO* pCreateInfo, 
											WM_HWIN hWinParent, int x0, int y0, 
											WM_CALLBACK* pCallback) 
{
	BubbleCtrl_Handle hObj;

	// 直接调用我们之前写好的 CreateAsChild
	hObj = BubbleCtrl_CreateAsChild(
		pCreateInfo->x0 + x0,       // 控件的 X 坐标 = 对话框基准 X + 控件相对 X
		pCreateInfo->y0 + y0,       // 控件的 Y 坐标 = 对话框基准 Y + 控件相对 Y
		pCreateInfo->xSize,         // 控件的宽度
		pCreateInfo->ySize,         // 控件的高度
		hWinParent,                 // 挂载的父窗口 (通常是对话框本身)
		pCreateInfo->Id,            // 控件的 ID
		pCreateInfo->Flags          // 控件的标志位 (也可以像原代码一样直接传 0)
		);

	// 注意：在 ucGUI 中，pCallback 通常是对话框的外部回调。
	// 因为我们的 BubbleCtrl 内部已经绑定了 _BubbleCtrl_Callback 来处理绘制，
	// 所以这里不需要把 pCallback 强行塞给底层窗口（原版 ButtonExCtrl 虽然传了 cb，但底层也是忽略它的）。

	return hObj;
}

// API 2：动态修改文本
void BubbleCtrl_SetText(BubbleCtrl_Handle hObj, const char* pText) {
	if (hObj && pText) {  // 增加对 pText 不为空的保护
		BubbleCtrl_Obj* pObj;
		WM_LOCK();
		pObj = BubbleCtrl_H2P(hObj);

		// 【关键修改】：深拷贝字符串内容到控件私有内存中
		// 使用 strncpy 防溢出，并将最后一个字符强制置0，保证字符串安全结束
		strncpy(pObj->Info.aText, pText, sizeof(pObj->Info.aText) - 1);
		pObj->Info.aText[sizeof(pObj->Info.aText) - 1] = '\0'; 

		WM_InvalidateWindow(hObj); 
		WM_UNLOCK();
	}
}

// API 3：动态修改底色
void BubbleCtrl_SetBkColor(BubbleCtrl_Handle hObj, GUI_COLOR Color) {
    if (hObj) {
        BubbleCtrl_Obj* pObj;
        WM_LOCK();
        pObj = BubbleCtrl_H2P(hObj);
        pObj->Info.BkColor = Color;
        WM_InvalidateWindow(hObj);
        WM_UNLOCK();
    }
}

#endif // GUI_WINSUPPORT