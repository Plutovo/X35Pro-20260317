#ifndef _KeyBoardm_H
#define _KeyBoardm_H

#include "WM.h" 

#ifndef NULL
#define NULL 0
#endif

typedef struct /*创建键盘的参数结构*/
{

	WM_HWIN hWin;
	char *keymap;
	int x0,y0,xSize,ySize,xSpan,ySpan;

	int num;
}KEYBOARDCFG;



void KeyBoard_clear(WM_HWIN hWin);

WM_HWIN KeyBoard_Create();


void KeyBoard_SetHook(WM_HWIN hWin);
void KeyBoard_deleteWindow(void);
void KetBoard_SetPos(int x, int y);
WM_HWIN KeyBoard_GetHandle();

#endif 