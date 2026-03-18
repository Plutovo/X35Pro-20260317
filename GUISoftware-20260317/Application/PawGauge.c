#include <string.h>
#include <stdio.h>
#include <math.h>
#include "GUI.H"
#include "GUI_Protected.h"
#include "WIDGET.h"
#include "PawGauge.h"

#if GUI_WINSUPPORT

// ==========================================
// 辅助函数 1：多重安全画粗圆弧
// ==========================================
static void _DrawThickArcSafe(int x0, int y0, int r, int thickness, int a0, int a1) {
	int i; 
	for (i = 0; i < thickness; i++) {
		GUI_DrawArc(x0, y0, r - i, r - i, a0, a1);
	}
}

// ==========================================
// 辅助函数 2：手工像素级防畸变粗线引擎 (已验证的完美画线)
// ==========================================
static void _DrawPerfectThickLine(int x1, int y1, int x2, int y2) {
	int dx = x2 - x1;
	int dy = y2 - y1;
	int abs_dx = (dx > 0) ? dx : -dx;
	int abs_dy = (dy > 0) ? dy : -dy;
	int steps = (abs_dx > abs_dy) ? abs_dx : abs_dy;
	int i;
	float x_inc, y_inc, cur_x, cur_y;

	if (steps == 0) return;

	x_inc = (float)dx / (float)steps;
	y_inc = (float)dy / (float)steps;
	cur_x = (float)x1;
	cur_y = (float)y1;

	for (i = 0; i <= steps; i++) {
		int px = (int)(cur_x + 0.5f);
		int py = (int)(cur_y + 0.5f);
		if (abs_dy > abs_dx) {
			GUI_DrawHLine(py, px, px + 1);
		} else {
			GUI_DrawVLine(px, py, py + 1);
		}
		cur_x += x_inc;
		cur_y += y_inc;
	}
}

// ==========================================
// 核心渲染逻辑
// ==========================================
static void _Paint(PawGauge_Obj* pObj) {
	// --------------------------------------------------------
	// 【变量声明区】
	// --------------------------------------------------------
	GUI_RECT r_client;

	// ★ 本地缓存变量，防止 ucGUI 内存碎片化导致 pObj 失效
	int local_data;
	char local_unit[16];
	GUI_COLOR local_bk_color, local_track_color, local_fill_color, local_tick_color, local_text_color;

	int cx, cy;
	int r, thick;
	int end_angle;

	int val;
	int tick_length, r_out, r_in;
	float angle_deg_f, angle_rad_f; 
	float fx1, fy1, fx2, fy2;
	int x1, y1, x2, y2;

	int r_text, xt, yt;
	int text_w, text_h;
	char buf[8];

	// --------------------------------------------------------
	// 【执行逻辑区】
	// --------------------------------------------------------
	GUI_GetClientRect(&r_client);

	// ★ 第一时间提取所有数据到栈内存缓存！坚决杜绝野指针！
	local_data = pObj->Data;
	strncpy(local_unit, pObj->Unit, 15);
	local_unit[15] = '\0'; // 绝对安全的防溢出截断
	local_bk_color = pObj->BkColor;
	local_track_color = pObj->TrackColor;
	local_fill_color = pObj->FillColor;
	local_tick_color = pObj->TickColor;
	local_text_color = pObj->TextColor;

	// --- 坐标与尺寸 ---
	cx = (r_client.x0 + r_client.x1) / 2;      
	// 整体位置向上提：只偏下 5 像素（原先是 +25），你可以改这里微调
	cy = (r_client.y0 + r_client.y1) / 2 + 12; 
	r = 85;        
	thick = 22;    

	// --- 全局背景 ---
	GUI_SetColor(local_bk_color);
	GUI_FillRect(r_client.x0, r_client.y0, r_client.x1, r_client.y1);

	// --- 深灰色底轨 ---
	GUI_SetColor(local_track_color);
	_DrawThickArcSafe(cx, cy, r, thick, 0, 225);
	_DrawThickArcSafe(cx, cy, r, thick, 315, 360);

	// --- 动态填充条 ---
	GUI_SetColor(local_fill_color);
	if (local_data > 100) local_data = 100;
	if (local_data < -20) local_data = -20;

	if (local_data > 0) {
		end_angle = 180 - (local_data * 9) / 4; 
		if (end_angle >= 0) {
			_DrawThickArcSafe(cx, cy, r, thick, end_angle, 180);
		} else {
			_DrawThickArcSafe(cx, cy, r, thick, 0, 180);
			_DrawThickArcSafe(cx, cy, r, thick, 360 + end_angle, 360);
		}
	} 
	else if (local_data < 0) {
		end_angle = 180 - (local_data * 9) / 4; 
		_DrawThickArcSafe(cx, cy, r, thick, 180, end_angle);
	}

	// --- 外围刻度线与数字 ---
	GUI_SetColor(local_tick_color); 
	GUI_SetTextAlign(GUI_TA_LEFT | GUI_TA_TOP); 
	GUI_SetTextMode(GUI_TM_TRANS);

	r_out = r - 1; 

	for (val = -20; val <= 100; val += 10) {
		angle_deg_f = 180.0f - ((float)val * 9.0f) / 4.0f;
		angle_rad_f = angle_deg_f * 3.1415926f / 180.0f;

		if (val % 20 == 0) {
			tick_length = 14; 
		} else {
			tick_length = 7;  
		}
		r_in = r_out - tick_length;

		fx1 = (float)cx + (float)r_in * cos(angle_rad_f);
		fy1 = (float)cy - (float)r_in * sin(angle_rad_f);
		fx2 = (float)cx + (float)r_out * cos(angle_rad_f);
		fy2 = (float)cy - (float)r_out * sin(angle_rad_f);

		x1 = (int)(fx1 + 0.5f);
		y1 = (int)(fy1 + 0.5f);
		x2 = (int)(fx2 + 0.5f);
		y2 = (int)(fy2 + 0.5f);

		_DrawPerfectThickLine(x1, y1, x2, y2);

		if (val % 20 == 0) {
			r_text = r + 12; 
			xt = (int)((float)cx + (float)r_text * cos(angle_rad_f) + 0.5f);
			yt = (int)((float)cy - (float)r_text * sin(angle_rad_f) + 0.5f);

			sprintf(buf, "%d", val);
			GUI_SetColor(local_text_color);
			GUI_SetFont(&GUI_Font_16); 

			text_w = GUI_GetStringDistX(buf);
			text_h = GUI_GetFontDistY();
			GUI_DispStringAt(buf, xt - (text_w / 2), yt - (text_h / 2));

			GUI_SetColor(local_tick_color); 
		}
	}

	// --- 中心纯净文本 ---
	GUI_SetColor(local_text_color);

	// 主标题 "Paw"
	GUI_SetFont(&GUI_Font_32); 
	text_w = GUI_GetStringDistX("Paw");
	text_h = GUI_GetFontDistY();
	GUI_DispStringAt("Paw", cx - (text_w / 2), cy - 10 - (text_h / 2));

	// ★ 动态单位绘制：使用极其安全的 local_unit ★
	GUI_SetFont(&GUI_Font_20); 
	text_w = GUI_GetStringDistX(local_unit);
	text_h = GUI_GetFontDistY();
	GUI_DispStringAt(local_unit, cx - (text_w / 2), cy + 20 - (text_h / 2));
}

// ==========================================
// 消息回调
// ==========================================
static void _PawGauge_Callback(WM_MESSAGE *pMsg) {
	PawGauge_Handle hWin = pMsg->hWin;
	PawGauge_Obj* pObj;
	if (WIDGET_HandleActive(hWin, pMsg) == 0) return;

	pObj = PawGauge_H2P(hWin);
	switch (pMsg->MsgId) {
		case WM_PAINT:
			_Paint(pObj);
			break;
		default:
			WM_DefaultProc(pMsg);
	}
}

// ==========================================
// API 实现
// ==========================================
PawGauge_Handle PawGauge_CreateAsChild(int x0, int y0, int xsize, int ysize, WM_HWIN hParent, int Id, int Flags) {
	PawGauge_Handle hObj;
	WM_LOCK();
	hObj = WM_CreateWindowAsChild(x0, y0, xsize, ysize, hParent, Flags, _PawGauge_Callback, sizeof(PawGauge_Obj) - sizeof(WM_Obj));
	if (hObj) {
		PawGauge_Obj* pObj = PawGauge_H2P(hObj);
		WIDGET__Init(&pObj->Widget, Id, WIDGET_STATE_FOCUSSABLE);

		pObj->Data = 0; 

		// ★ 绝对安全的初始化单位，强制使用 15 防止 sizeof 陷阱
		strncpy(pObj->Unit, "cmH2O", 15);
		pObj->Unit[15] = '\0';

		pObj->BkColor = GUI_BLACK;         
		pObj->TrackColor = 0x2A2A2A;       
		pObj->FillColor = 0x90A040;        
		pObj->TickColor = GUI_WHITE;       
		pObj->TextColor = GUI_WHITE;       
	}
	WM_UNLOCK();
	return hObj;
}

PawGauge_Handle PawGauge_CreateIndirect(const GUI_WIDGET_CREATE_INFO* pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK* pCallback) {
	return PawGauge_CreateAsChild(pCreateInfo->x0 + x0, pCreateInfo->y0 + y0, pCreateInfo->xSize, pCreateInfo->ySize, hWinParent, pCreateInfo->Id, pCreateInfo->Flags);
}

void PawGauge_SetData(PawGauge_Handle hObj, int Data) {
	PawGauge_Obj* pObj;
	if (hObj) {
		WM_LOCK();
		pObj = PawGauge_H2P(hObj);
		if (pObj->Data != Data) {
			pObj->Data = Data;
			WM_InvalidateWindow(hObj); 
		}
		WM_UNLOCK();
	}
}

// 【新增实现】：极其安全的单位配置 API
void PawGauge_SetUnit(PawGauge_Handle hObj, const char* pUnit) {
	PawGauge_Obj* pObj;
	if (hObj && pUnit) {
		WM_LOCK();
		pObj = PawGauge_H2P(hObj);

		// 使用 15 强制限制，避免老式编译器 sizeof 计算指针错误
		strncpy(pObj->Unit, pUnit, 15);
		pObj->Unit[15] = '\0';

		WM_InvalidateWindow(hObj); 
		WM_UNLOCK();
	}
}

int PawGauge_GetData(PawGauge_Handle hObj) {
	PawGauge_Obj* pObj;
	int val = 0;
	if (hObj) {
		WM_LOCK();
		pObj = PawGauge_H2P(hObj);
		val = pObj->Data;
		WM_UNLOCK();
	}
	return val;
}
#endif // GUI_WINSUPPORT