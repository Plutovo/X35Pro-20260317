#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include "WM.h"
#include "DIALOG_Intern.h"      /* Req. for Create indirect data structure */
#include "WIDGET.h"      /* Req. for Create indirect data structure */
#include "GUIDebug.h" /* Req. for GUI_DEBUG_LEVEL */

typedef WM_HMEM KeyBoard_Handle;



typedef struct
{
    WIDGET Widget;//GUI结构体
    //外形大小
    int Width; //宽度
    int Heigth;
    //间隔距离
    int X_Size;
    int Y_Size;
    //按钮数量
    int KeyNumber;//实际按钮的总共个数（如果键盘按钮是满的，则个数为X * Y，如果没有排列满，则按照从左至右从上到下排列，剩下的为失效态。）
    int X_KeyNumber;//X轴最大按键个数
    int Y_KeyNumber;//Y轴最大按键个数

    int PressKey_Pos; //被选中的按键的位置（按照从左至右、从上到下的顺序排列，首个按键的编号为0，一次向后排列）
    int Per_PressKey_Pos; //上一次被选中的按键索引号，只有在初始化的时候这两个值才相等，其他情况均不能相同。

    //颜色
    int Key_BkColor;//按钮的背景颜色
    int Key_Color;//按钮选中的颜色
    int Font_BkColor;//未选中按钮的字体颜色
    int TEXTFont_Color;//选中按钮的字体颜色
    int Key_DisBkColor;//禁止态的背景颜色

    unsigned char PaintDisable;
    GUI_FONT *Key_Font;//字体
   //FLAG_GUI  这3个数组应该改成动态创建动态释放的，并且在删除事件中释放。
    unsigned short KeyBoard_Pos[25][2];
    const    char **KeyBoard_Name[25]; 
    unsigned char KeyBoard_State[25];//0：使能   1：禁止

    //char **Key_Name;  //每个按键对应的名称
    //unsigned char **Key_Pos;   //按键的位置信息（横纵坐标）
} KeyBoard_Obj;

#define KeyBoard_H2P(h) (KeyBoard_Obj*) GUI_ALLOC_h2p(h)
#define KeyBoard_Invalidate(hObj) WM_InvalidateWindow(hObj)









KeyBoard_Handle KEYBOARD_CreateIndirect(const GUI_WIDGET_CREATE_INFO* pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK* cb);


void KEYBOARD_SetIndex(KeyBoard_Handle hObj,int Index,int Flag);
int KEYBOARD_GetIndex(KeyBoard_Handle hObj);
int KEYBOARD_GetPerIndex(KeyBoard_Handle hObj);

void KEYBOARD_Inc(KeyBoard_Handle hObj);
void KEYBOARD_Dec(KeyBoard_Handle hObj);

#endif  //_KEYBOARD_H_