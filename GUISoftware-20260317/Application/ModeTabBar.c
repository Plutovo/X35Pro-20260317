#include <string.h>
#include "GUI.H"
#include "GUI_Protected.h"
#include "WIDGET.h"
#include "ModeTabBar.h"

#if GUI_WINSUPPORT

// ==========================================
// 辅助函数 1：兼容 ucGUI 3.90 的空心圆角矩形 (全圆角，用于内部蓝框)
// ==========================================
static void _DrawRoundedRect(int x0, int y0, int x1, int y1, int r) {
	if (r <= 0) {
		GUI_DrawRect(x0, y0, x1, y1);
		return;
	}
	GUI_DrawArc(x1 - r, y0 + r, r, r, 0, 90);
	GUI_DrawArc(x0 + r, y0 + r, r, r, 90, 180);
	GUI_DrawArc(x0 + r, y1 - r, r, r, 180, 270);
	GUI_DrawArc(x1 - r, y1 - r, r, r, 270, 360);

	GUI_DrawHLine(y0, x0 + r, x1 - r);
	GUI_DrawHLine(y1, x0 + r, x1 - r);
	GUI_DrawVLine(x0, y0 + r, y1 - r);
	GUI_DrawVLine(x1, y0 + r, y1 - r);
}

// ==========================================
// 辅助函数 2：专属定制的“上圆下平”实心底板 (用于外部大底板)
// ==========================================
static void _FillTopRoundedRect(int x0, int y0, int x1, int y1, int r) {
	if (r <= 0) {
		GUI_FillRect(x0, y0, x1, y1);
		return;
	}
	// 1. 仅仅画上方左右两个角落的实心圆
	GUI_FillCircle(x0 + r, y0 + r, r);
	GUI_FillCircle(x1 - r, y0 + r, r);

	// 2. 画顶部的中间矩形（填补两个实心圆之间的顶部空隙）
	GUI_FillRect(x0 + r, y0, x1 - r, y0 + r);

	// 3. 画主体和底部矩形（从圆心所在的Y轴一直向下画，直到底部，形成平底直角）
	GUI_FillRect(x0, y0 + r, x1, y1);
}

// ==========================================
// 核心渲染逻辑
// ==========================================
static void _Paint(ModeTabBar_Obj* pObj) {
	GUI_RECT r;

	int total_slots;
	int slot_width;

	// 参数 A：外部大底板（上圆下平）
	int bg_margin_x = 0;       // 左右缩进很小，显得宽大
	int bg_margin_top = 2;     // 顶部留出一点点背景黑边
	int bg_margin_bottom = 0;  // 【绝杀细节】：底部缩进必须为 0！这样底板才会直接冲出控件边缘，和下方面板完美相连
	int bg_r_corner = 8;       // 顶部大圆角
	GUI_COLOR color_bg = 0x404040; // 选项卡底色

	// 参数 B：内部蓝色线框（全圆角，紧贴文字）
	int border_margin_x = 12;  // 左右向内大幅缩进
	int border_margin_y = 8;   // 上下向内大幅缩进
	int border_r_corner = 4;   // 较小的内圆角

	int i;

	GUI_GetClientRect(&r);

	// 【图层 1】：画全局背景色 (极深的黑色底板)
	GUI_SetColor(pObj->BkColor);
	GUI_FillRect(r.x0, r.y0, r.x1, r.y1);

	total_slots = pObj->ItemsPerPage + 1;
	slot_width = (r.x1 - r.x0 + 1) / total_slots;

	GUI_SetFont(pObj->pFont);
	GUI_SetTextMode(GUI_TM_TRANS); // 必须透明

	// ========================================================
	// ★ 独立控制参数区（根据你的选项卡效果进行了黄金比例预设）
	// ========================================================

	for (i = 0; i < pObj->ItemsPerPage; i++) {

		int slot_x0;
		int slot_x1;

		int mode_idx = pObj->CurrentPage * pObj->ItemsPerPage + i;
		if (mode_idx >= pObj->TotalModes) continue;

		// 当前槽位的绝对坐标
		slot_x0 = r.x0 + i * slot_width;
		slot_x1 = slot_x0 + slot_width - 1;

		if (mode_idx == pObj->CurrentSelection) {

			int border_x0;
			int border_x1;
			int border_y0;
			int border_y1;

			// 【图层 2】：先画外部选项卡大底板（上圆下平）
			int bg_x0 = slot_x0 + bg_margin_x;
			int bg_x1 = slot_x1 - bg_margin_x;
			int bg_y0 = r.y0 + bg_margin_top;
			int bg_y1 = r.y1 - bg_margin_bottom; // 这里直通 r.y1

			GUI_SetColor(color_bg);
			_FillTopRoundedRect(bg_x0, bg_y0, bg_x1, bg_y1, bg_r_corner);

			// 【图层 3】：再在底板内部画蓝色高亮线框（空心嵌套）
			border_x0 = slot_x0 + border_margin_x;
			border_x1 = slot_x1 - border_margin_x;
			border_y0 = r.y0 + border_margin_y;
			border_y1 = r.y1 - border_margin_y;

			GUI_SetColor(pObj->ActiveBorderColor);
			GUI_SetPenSize(1); // 强制为 1 像素，消除圆角重影

			// 画外圈
			_DrawRoundedRect(border_x0, border_y0, border_x1, border_y1, border_r_corner);
			// 向内缩进 1 像素画内圈
			_DrawRoundedRect(border_x0 + 1, border_y0 + 1, border_x1 - 1, border_y1 - 1, border_r_corner - 1);
		}

		// 【图层 4】：最后绘制模式文本（处于最上层）
		GUI_SetColor(pObj->TextColor);
		GUI_SetTextAlign(GUI_TA_HCENTER | GUI_TA_VCENTER);
		GUI_DispStringAt(pObj->ModeNames[mode_idx], (slot_x0 + slot_x1) / 2, (r.y0 + r.y1) / 2);
	}

	// --- 绘制最右侧的翻页按钮 ---
	{
		int btn_x0 = r.x0 + pObj->ItemsPerPage * slot_width;
		int btn_x1 = r.x1;
		GUI_SetColor(GUI_WHITE);
		GUI_SetTextAlign(GUI_TA_HCENTER | GUI_TA_VCENTER);

		if (pObj->CurrentPage == 0) {
			GUI_DispStringAt(">>", (btn_x0 + btn_x1) / 2, (r.y0 + r.y1) / 2);
		} else {
			GUI_DispStringAt("<<", (btn_x0 + btn_x1) / 2, (r.y0 + r.y1) / 2);
		}
	}
}

// ==========================================
// 触摸与消息处理回调
// ==========================================
static void _OnTouch(ModeTabBar_Handle hWin, ModeTabBar_Obj* pObj, WM_MESSAGE* pMsg) {

	int touch_x;
	int total_slots;
	int slot_width;	
	int click_slot;

    const GUI_PID_STATE* pState = (const GUI_PID_STATE*)pMsg->Data.p;
    
    if (pState && pState->Pressed) {
        GUI_RECT r;
        WM_GetWindowRectEx(hWin, &r); // 获取控件在屏幕上的绝对物理坐标
        
        // 计算相对点击位置的 X 坐标
        touch_x = pState->x - r.x0;
        
        total_slots = pObj->ItemsPerPage + 1;
        slot_width = (r.x1 - r.x0 + 1) / total_slots;
        
        // 算出点击了第几个槽位
        click_slot = touch_x / slot_width;

        // 限制越界 (防止点击最右侧边缘算到下一个槽)
        if (click_slot >= total_slots) click_slot = total_slots - 1;

        if (click_slot == pObj->ItemsPerPage) {
            // --- 情景 1：点击了最右侧的翻页按钮 (>> 或 <<) ---
            pObj->CurrentPage = (pObj->CurrentPage == 0) ? 1 : 0;
            WM_InvalidateWindow(hWin); // 触发重绘翻页
        } else {
            // --- 情景 2：点击了某个模式文本 ---
            int new_selection = pObj->CurrentPage * pObj->ItemsPerPage + click_slot;
            
            // 确保点击的是有效模式 (没有点在空白处)，且与当前选中项不同
            if (new_selection < pObj->TotalModes && new_selection != pObj->CurrentSelection) {
                pObj->CurrentSelection = new_selection;
                WM_InvalidateWindow(hWin); // 触发重绘，移动高亮框
                
                // 【核心解耦机制】：向父窗口发送“值改变”通知！
                // 父窗口只需捕获 WM_NOTIFICATION_VALUE_CHANGED，就能知道该切换下方按钮了
                WM_NotifyParent(hWin, WM_NOTIFICATION_VALUE_CHANGED);
            }
        }
    }
}

static void _ModeTabBar_Callback(WM_MESSAGE *pMsg) {
    ModeTabBar_Handle hWin = pMsg->hWin;
    ModeTabBar_Obj* pObj;

    if (WIDGET_HandleActive(hWin, pMsg) == 0) {
        return;
    }
    pObj = ModeTabBar_H2P(hWin);

    switch (pMsg->MsgId) {
        case WM_PAINT:
            _Paint(pObj);
            break;
        case WM_TOUCH:
            _OnTouch(hWin, pObj, pMsg);
            break;
        default:
            WM_DefaultProc(pMsg);
    }
}

// ==========================================
// API 实现
// ==========================================
ModeTabBar_Handle ModeTabBar_CreateAsChild(int x0, int y0, int xsize, int ysize, 
                                           WM_HWIN hParent, int Id, int Flags) 
{
    ModeTabBar_Handle hObj;
    WM_LOCK();
    hObj = WM_CreateWindowAsChild(x0, y0, xsize, ysize, hParent, Flags, 
                                  _ModeTabBar_Callback, 
                                  sizeof(ModeTabBar_Obj) - sizeof(WM_Obj));
    if (hObj) {
        ModeTabBar_Obj* pObj = ModeTabBar_H2P(hObj);
        WIDGET__Init(&pObj->Widget, Id, WIDGET_STATE_FOCUSSABLE);
        
        // 初始化默认颜色和字体
        pObj->BkColor = 0x292420;    
        pObj->TextColor = GUI_WHITE;       
        pObj->ActiveBorderColor = 0xFF8040;
        pObj->pFont = &GUI_Font_20; 
        
        pObj->ModeNames[0] = "VCV";
        pObj->ModeNames[1] = "PCV";
        pObj->ModeNames[2] = "PRVC";
        pObj->ModeNames[3] = "SIMV-V";
        pObj->ModeNames[4] = "SIMV-P";
        pObj->ModeNames[5] = "SIMV-PRVC";
        pObj->ModeNames[6] = "SPONT/PSV";
        pObj->ModeNames[7] = "APRV";
        
        // 补充的第二页模式
        pObj->ModeNames[8] = "Duophasic";
        pObj->ModeNames[9] = "VS";
        
        pObj->TotalModes = 10;      // 总共 10 个模式
        pObj->ItemsPerPage = 8;     // 前 8 个放在第一页
        
        pObj->CurrentSelection = 0; // 默认选中第一个 (VCV)
        pObj->CurrentPage = 0;      // 默认在第一页
    }
    WM_UNLOCK();
    return hObj;
}

ModeTabBar_Handle ModeTabBar_CreateIndirect(const GUI_WIDGET_CREATE_INFO* pCreateInfo, 
                                            WM_HWIN hWinParent, int x0, int y0, 
                                            WM_CALLBACK* pCallback) 
{
    return ModeTabBar_CreateAsChild(pCreateInfo->x0 + x0, pCreateInfo->y0 + y0, 
                                    pCreateInfo->xSize, pCreateInfo->ySize, 
                                    hWinParent, pCreateInfo->Id, pCreateInfo->Flags);
}

// 获取当前选中项，供父窗口查询
int ModeTabBar_GetSelection(ModeTabBar_Handle hObj) {
    int sel = 0;
    if (hObj) {
        ModeTabBar_Obj* pObj;
        WM_LOCK();
        pObj = ModeTabBar_H2P(hObj);
        sel = pObj->CurrentSelection;
        WM_UNLOCK();
    }
    return sel;
}

// 代码设置选中项
void ModeTabBar_SetSelection(ModeTabBar_Handle hObj, int Index) {
    if (hObj) {
        ModeTabBar_Obj* pObj;
        WM_LOCK();
        pObj = ModeTabBar_H2P(hObj);
        if (Index >= 0 && Index < pObj->TotalModes && Index != pObj->CurrentSelection) {
            pObj->CurrentSelection = Index;
            
            // 自动判断选中项是否在当前页，如果不在，自动翻页！
            pObj->CurrentPage = Index / pObj->ItemsPerPage;
            
            WM_InvalidateWindow(hObj);
        }
        WM_UNLOCK();
    }
}

#endif // GUI_WINSUPPORT