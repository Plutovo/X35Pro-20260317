/*************************************************************
标题:  KeyBoard.c
功能:  创建扁平化按键面板
说明:  支持触摸选择和按键控制，只能通过按键按下确认
当前版本： 0.1.0
修改记录：
**************************************************************/
#include "GUI_Protected.h"
#include "KeyBoard.h"
#include "MainTask.h"
#include "DispStringLib.h"

#include "WM.H"
#include "MainTask.h"
#include <stdlib.h>
#include <string.h>
//#include "UserConfig.h"

#if GUI_WINSUPPORT
    //支持窗体
#endif  //GUI_WINSUPPORT


//#define KEYBOARD_DEFAULT_XSIZE        (40)      //单个按钮的宽度
//#define KEYBOARD_DEFAULT_YSIZE        (25)      //单个按钮的高度
#define KEYBOARD_DEFAULT_KEYNUMBER    (6)       //总共的按钮数
#define KEYBOARD_DEFAULT_X_KEYNUMBER  (3)       //X轴的按钮数
#define KEYBOARD_DEFAULT_Y_KEYNUMBER  (2)       //Y轴的按钮数
#define KEYBOARD_DEFAULT_PRESSKEY_POS (1)       //被选中的按钮编号
#define KEYBOARD_DEFAULT_KEY_BKCOLOR      (0xD26F00)//背景颜色
#define KEYBOARD_DEFAULT_KEY_COLOR        (0xAD3000)    //选中的按钮颜色
#define KEYBOARD_DEFAULT_KEY_DISBKCOLOR   (0xF7D7BD) //按键禁止态背景色
#define KEYBOARD_DEFAULT_FONT_BKCOLOR     (GUI_WHITE)    //正常按钮的字体颜色
#define KEYBOARD_DEFAULT_FONT_COLOR       (GUI_WHITE)    //选中的按钮的字体颜色
#define KEYBOARD_DEFAULT_FONT             BUTTON_FONT_DEFAULT//&GUI_Font16B_ASCII
//#define KEYBOARD_DEFAULT_KEYNAME          ("hello")
const char *KEYBOARD_DEFAULT_KEYNAME = "hello";


/*********************************************************************
*
*       _Paint
*/

static void _Paint(KeyBoard_Handle hObj, KeyBoard_Obj* pObj)
{
    int cnt;
    int index = pObj->PressKey_Pos,per_index = pObj->Per_PressKey_Pos;
    int X_Offset,Y_Offset;
    GUI_RECT r;
    WM_GetWindowRect(&r);
    //取得控件边长
    pObj->Width = r.x1 - r.x0 + 1;
    pObj->Heigth = r.y1 - r.y0 + 1;
    //取得控件中每个按钮的大小
    pObj->X_Size = pObj->Width/pObj->X_KeyNumber;
    pObj->Y_Size = pObj->Heigth/pObj->Y_KeyNumber;

    //if(!(pObj->KeyBoard_State[index]))
    {
        //设置文本模式
        GUI_SetTextMode(GUI_TM_TRANS);
        if(pObj->Per_PressKey_Pos == pObj->PressKey_Pos)//初始化
        {
            //绘制控件背景颜色
                LCD_SetBkColor(pObj->Key_BkColor);
                GUI_Clear();
            
            //写按键名
            LCD_SetColor(pObj->Font_BkColor);
            GUI_SetFont(pObj->Key_Font);
            for(cnt = 0;cnt < pObj->KeyNumber;cnt++)
            {
                X_Offset = (pObj->X_Size - GUI_GetStringDistX(*pObj->KeyBoard_Name[cnt]))/2;
                Y_Offset = (pObj->Y_Size - GUI_GetFontDistY())/2;
				if(pObj->KeyBoard_State[cnt] == 1)//按键被禁止
				{
				    LCD_SetColor(pObj->Key_DisBkColor);
				}
				else
				{
				    LCD_SetColor(pObj->Font_BkColor);
				}
                GUI_DispStringAt(*pObj->KeyBoard_Name[cnt],pObj->KeyBoard_Pos[cnt][0]+X_Offset,pObj->KeyBoard_Pos[cnt][1]+Y_Offset);
            }
        }
        else//按键索引值改变
        {
            //将上一次被选中的按键还原
            LCD_SetColor(pObj->Key_BkColor);
            GUI_FillRect(pObj->KeyBoard_Pos[per_index][0],pObj->KeyBoard_Pos[per_index][1],pObj->KeyBoard_Pos[per_index][0] + pObj->X_Size - 1,pObj->KeyBoard_Pos[per_index][1] + pObj->Y_Size - 1);
    
            //LCD_SetColor(pObj->Font_BkColor);
			if(pObj->KeyBoard_State[per_index] == 1)//按键被禁止
			{
				LCD_SetColor(pObj->Key_DisBkColor);
			}
			else
			{
				LCD_SetColor(pObj->Font_BkColor);
			}
            GUI_SetFont(pObj->Key_Font);
            X_Offset = (pObj->X_Size - GUI_GetStringDistX(*pObj->KeyBoard_Name[per_index]))/2;
            Y_Offset = (pObj->Y_Size - GUI_GetFontDistY())/2;
            GUI_DispStringAt(*pObj->KeyBoard_Name[per_index],pObj->KeyBoard_Pos[per_index][0]+X_Offset,pObj->KeyBoard_Pos[per_index][1]+Y_Offset);
    
        }
    
        //绘制选中的按钮
        LCD_SetColor(pObj->Key_Color);
        GUI_FillRect(pObj->KeyBoard_Pos[index][0],pObj->KeyBoard_Pos[index][1],pObj->KeyBoard_Pos[index][0] + pObj->X_Size - 1,pObj->KeyBoard_Pos[index][1] + pObj->Y_Size - 1);
        //绘制选中按钮的字体
        LCD_SetColor(pObj->TEXTFont_Color);
        X_Offset = (pObj->X_Size - GUI_GetStringDistX(*pObj->KeyBoard_Name[index]))/2;
        Y_Offset = (pObj->Y_Size - GUI_GetFontDistY())/2;
        GUI_DispStringAt(*pObj->KeyBoard_Name[index],pObj->KeyBoard_Pos[index][0]+X_Offset,pObj->KeyBoard_Pos[index][1]+Y_Offset);
    
    
        LCD_SetColor(pObj->TEXTFont_Color);
        X_Offset = (pObj->X_Size - GUI_GetStringDistX(*pObj->KeyBoard_Name[index]))/2;
        Y_Offset = (pObj->Y_Size - GUI_GetFontDistY())/2;
        GUI_DispStringAt(*pObj->KeyBoard_Name[index],pObj->KeyBoard_Pos[index][0]+X_Offset,pObj->KeyBoard_Pos[index][1]+Y_Offset);
    }
    pObj->PaintDisable = 0;
}

/*********************************************************************
*
*       _OnTouch
*/
static void _OnTouch(KeyBoard_Handle hObj, KeyBoard_Obj* pObj, WM_MESSAGE*pMsg)
{
    int Notification;
    int Hit = 0;
    int tmp_x = 0xA0A0,tmp_y = 0xA0A0;
    int cnt,flag = 0;
	int Index = 0;
    GUI_PID_STATE* pState = (GUI_PID_STATE*)pMsg->Data.p;
    if (pMsg->Data.p)    /* Something happened in our area (pressed or released) */
    {
        if (pState->Pressed)
        {
            //if (y <= pObj->Height)
            //{
            //    RADIO_SetValue(hObj, Sel);
            //}
            for(cnt = 0;cnt < pObj->KeyNumber;cnt++)
            {
                if((pObj->KeyBoard_Pos[cnt][0] + pObj->X_Size > pState->x) && (tmp_x == 0xA0A0))
                {
                    tmp_x = cnt%pObj->X_KeyNumber;//获得x索引号
                }
                if((pObj->KeyBoard_Pos[cnt][1] + pObj->Y_Size > pState->y) && (tmp_y == 0xA0A0))
                {
                    tmp_y = cnt/pObj->X_KeyNumber;//获得y索引号 
                }
                if((tmp_x != 0xA0A0) && (tmp_y != 0xA0A0))
                {
                    flag = 1; //获得控件索引号
                    KEYBOARD_SetIndex(hObj,tmp_x + pObj->X_KeyNumber*tmp_y, 0);
                    break;
                }
            }
            if(flag == 0)//按键超出控件范围
            {
                return;
            }
            if (WM_IsFocussable(hObj))
            {
                WM_SetFocus(hObj);
            }
            Notification = WM_NOTIFICATION_CLICKED;
        }
        else
        {
            Hit = 1;
            Notification = WM_NOTIFICATION_RELEASED;
        }
    }
    else
    {
        Notification = WM_NOTIFICATION_MOVED_OUT;
    }
	if((tmp_x + pObj->X_KeyNumber*tmp_y < 16) && (tmp_x + pObj->X_KeyNumber*tmp_y >= 0))
	{
        Index = tmp_x + pObj->X_KeyNumber*tmp_y;
	}
	else
	{
	    Index = 15;
	}
	if((pObj->KeyBoard_State[Index] != 1) &&(Index < pObj->KeyNumber))
	{
		WM_NotifyParent(hObj, Notification);
	}
    if (Hit == 1)
    {
        GUI_DEBUG_LOG("RADIO: Hit\n");
        GUI_StoreKey(pObj->Widget.Id);
    }
}

/*********************************************************************
*
*       _OnKey
*/
static void  _OnKey(KeyBoard_Handle hObj, WM_MESSAGE* pMsg)
{
    WM_KEY_INFO* pKeyInfo;
    pKeyInfo = (WM_KEY_INFO*)(pMsg->Data.p);
    if (pKeyInfo->PressedCnt > 0)
    {
        switch (pKeyInfo->Key)
        {
        case GUI_KEY_RIGHT:
            KEYBOARD_Inc(hObj);
            break;                    /* Send to parent by not doing anything */
        case GUI_KEY_LEFT:
            KEYBOARD_Dec(hObj);
            break;                    /* Send to parent by not doing anything */
        default:
            return;
        }
    }
}

static void _KeyBoard_Callback (WM_MESSAGE*pMsg)
{
    KeyBoard_Handle hObj = pMsg->hWin;
    KeyBoard_Obj* pObj = KeyBoard_H2P(hObj);
    /* Let widget handle the standard messages */
    if (WIDGET_HandleActive(hObj, pMsg) == 0)
    {
        return;
    }
    switch (pMsg->MsgId)
    {
    case WM_PAINT:
         GUI_DEBUG_LOG("TEXT: _Callback(WM_PAINT)\n");
        _Paint(hObj, pObj);
        return;
    case WM_TOUCH:
        _OnTouch(hObj, pObj, pMsg);
        return;
    case WM_KEY:
        _OnKey(hObj, pMsg);
        return;
    case WM_DELETE:
        GUI_DEBUG_LOG("TEXT: _Callback(WM_DELETE)\n");
//        _Delete(pObj);
        break;       /* No return here ... WM_DefaultProc needs to be called */
    }
    WM_DefaultProc(pMsg);
}


KeyBoard_Handle KeyBoard_CreateAsChild(
    int x0, int y0, int xsize, int ysize, WM_HWIN hParent,int Id,
    WM_CALLBACK* cb, int Flags)
{
    KeyBoard_Handle hObj;
    int cnt;
    /* Create the window */
    WM_LOCK();
    hObj = WM_CreateWindowAsChild(x0, y0, xsize/*+2*HBorder*/, ysize/*+TBorder+BBorder*/, hParent,
                                  Flags, _KeyBoard_Callback, sizeof(KeyBoard_Obj) - sizeof(WM_Obj));
    if (hObj)
    {
        KeyBoard_Obj* pObj = KeyBoard_H2P(hObj);
        /* init widget specific variables */
        WIDGET__Init(&pObj->Widget,Id, WIDGET_STATE_FOCUSSABLE);
		
	   //按钮数量
		pObj->KeyNumber = KEYBOARD_DEFAULT_KEYNUMBER;//按钮的总共个数（如果键盘按钮是满的，则个数为X * Y，如果没有排列满，则按照从左至右从上到下排列，剩下的为失效态。）
		pObj->X_KeyNumber = KEYBOARD_DEFAULT_X_KEYNUMBER;//X轴按键个数
	    pObj->Y_KeyNumber = KEYBOARD_DEFAULT_Y_KEYNUMBER;//Y轴按键个数

		pObj->PressKey_Pos     = KEYBOARD_DEFAULT_PRESSKEY_POS; //被选中的按键的位置（按照从左至右、从上到下的顺序排列，首个按键的编号为0，一次向后排列）
        pObj->Per_PressKey_Pos = KEYBOARD_DEFAULT_PRESSKEY_POS;

		//颜色
		pObj->Key_BkColor = KEYBOARD_DEFAULT_KEY_BKCOLOR;//按钮的背景颜色
		pObj->Key_Color = KEYBOARD_DEFAULT_KEY_COLOR;//按钮选中的颜色
        pObj->Key_DisBkColor = KEYBOARD_DEFAULT_KEY_DISBKCOLOR;//禁止态背景色
		pObj->Font_BkColor = KEYBOARD_DEFAULT_FONT_BKCOLOR;//未选中按钮的字体颜色
		pObj->TEXTFont_Color = KEYBOARD_DEFAULT_FONT_COLOR;//选中按钮的字体颜色
        
        pObj->PaintDisable = 1;

        if(LanguageType == Disp_Lg_Chinese)
		{
			pObj->Key_Font = (GUI_FONT *)(&GUI_Font_15);
		}
		else
		{
			pObj->Key_Font = (GUI_FONT *)(&GUI_Font_16);
		}

        //创建边框的位置信息数组和按键的名称数组
//        pObj->Key_Name = (char **)KeyBoard_Name;
//        pObj->Key_Pos  = (char **)KeyBoard_Pos;


        for(cnt = 0;cnt < pObj->X_KeyNumber*pObj->Y_KeyNumber;cnt++)  //位置信息初始化
        {
            pObj->KeyBoard_Pos[cnt][0] = (xsize/pObj->X_KeyNumber)*(cnt%pObj->X_KeyNumber);
            if((cnt%pObj->X_KeyNumber == pObj->X_KeyNumber -1)&&(xsize%pObj->X_KeyNumber))//最后一行，并且宽度不能被按键数整除，将多余的宽度放到最后行中
            {
                pObj->KeyBoard_Pos[cnt][0] += xsize%pObj->X_KeyNumber;
            }
            pObj->KeyBoard_Pos[cnt][1] = (ysize/pObj->Y_KeyNumber)*(cnt/pObj->X_KeyNumber);
            if((cnt/pObj->X_KeyNumber == pObj->Y_KeyNumber -1)&&(ysize%pObj->Y_KeyNumber))//最后一列，并且高度不能被按键数整除，将多余的高度放到最后列中
            {
                pObj->KeyBoard_Pos[cnt][1] += ysize%pObj->Y_KeyNumber;
            }
        }
        for(cnt = 0;cnt < pObj->X_KeyNumber*pObj->Y_KeyNumber;cnt++)  //按键名初始化
        {
            //strcpy(KeyBoard_Name[cnt],KEYBOARD_DEFAULT_KEYNAME);
            (pObj->KeyBoard_Name[cnt])=&KEYBOARD_DEFAULT_KEYNAME;
        }
        //for(cnt = 0;cnt < pObj->X_KeyNumber*pObj->Y_KeyNumber;cnt++)  //按键名初始化
        //{
        //    (pObj->KeyBoard_State[cnt])=0;
        //}
	}
    WM_UNLOCK();
    return hObj;
}

KeyBoard_Handle KEYBOARD_CreateIndirect(const GUI_WIDGET_CREATE_INFO* pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK* cb)
{
    KeyBoard_Handle  hThis;
//    int NumItems = (pCreateInfo->Para)       & 0xFF;
//    GUI_USE_PARA(cb);
    hThis = KeyBoard_CreateAsChild(pCreateInfo->x0 + x0, pCreateInfo->y0 + y0, pCreateInfo->xSize, pCreateInfo->ySize,
                           hWinParent,pCreateInfo->Id,cb,pCreateInfo->Flags);
    return hThis;
}


/*设置当前选中按钮的位置*/
void KEYBOARD_SetIndex(KeyBoard_Handle hObj,int Index,int Flag)
{
	int cnt = 0,cnt1 = 0;
    if (hObj)
    {
        KeyBoard_Obj* pObj;
        WM_LOCK();
        pObj = KeyBoard_H2P(hObj);
        if(!pObj->PaintDisable)
        {
			if((Index < 16) && (Index >= 0))
			{
				cnt = Index;
			}
			else
			{
				cnt = 15;
			}
			if(Flag)
			{
			    cnt1 = pObj->KeyNumber + 1;
			}
			else
			{
			    cnt1 = pObj->KeyNumber;
			}
			if((pObj->KeyBoard_State[cnt] != 1) &&(cnt < cnt1))
			{
				if(Index < 0)
				{
					pObj->Per_PressKey_Pos = pObj->PressKey_Pos;//保存上一次的索引值
					pObj->PressKey_Pos = pObj->KeyNumber-1;
				}
				else if(Index >= pObj->KeyNumber)
				{
					pObj->Per_PressKey_Pos = pObj->PressKey_Pos;//保存上一次的索引值
					pObj->PressKey_Pos = 0;
				}
				else 
				{
					pObj->Per_PressKey_Pos = pObj->PressKey_Pos;//保存上一次的索引值
					pObj->PressKey_Pos = Index;
				}
			}
			pObj->PaintDisable = 1;
			KeyBoard_Invalidate(hObj);
        }
        WM_UNLOCK();
    }
}
/*获得当前选中按钮的位置*/
int KEYBOARD_GetIndex(KeyBoard_Handle hObj)
{
    int ret;
    if (hObj)
    {
        KeyBoard_Obj* pObj;
        WM_LOCK();
        pObj = KeyBoard_H2P(hObj);
        ret = pObj->PressKey_Pos;
        WM_UNLOCK();
    }
    return ret;
}
/*获得上一个选中按钮的位置*/
int KEYBOARD_GetPerIndex(KeyBoard_Handle hObj)
{
    int ret;
    if (hObj)
    {
        KeyBoard_Obj* pObj;
        WM_LOCK();
        pObj = KeyBoard_H2P(hObj);
        ret = pObj->Per_PressKey_Pos;
        WM_UNLOCK();
    }
    return ret;
}
/*向后移动一个位置*/
void KEYBOARD_Inc(KeyBoard_Handle hObj)
{
    static int Item = 1;
    if (hObj)
    {
        KeyBoard_Obj* pObj;
        WM_LOCK();
        pObj = KeyBoard_H2P(hObj);
        while(pObj->KeyBoard_State[pObj->PressKey_Pos + Item] == 1)
        {
            Item++;
            if(pObj->PressKey_Pos + Item >= pObj->KeyNumber)
            {
                Item = 0 - pObj->PressKey_Pos;
            }
        }
        KEYBOARD_SetIndex(hObj,pObj->PressKey_Pos + Item, 1);
        Item = 1;
        WM_UNLOCK();
    }
}
/*向前移动一个位置*/
void KEYBOARD_Dec(KeyBoard_Handle hObj)
{
    static int Item1 = 1;
    if (hObj)
    {
        KeyBoard_Obj* pObj;
        WM_LOCK();
        pObj = KeyBoard_H2P(hObj);
		if(pObj->PressKey_Pos == 0)
		{
		    Item1 = 0 - pObj->KeyNumber + 1;
		}
        while(pObj->KeyBoard_State[pObj->PressKey_Pos - Item1] == 1)
        {
            Item1++;
            if(pObj->PressKey_Pos - Item1 < 0)
            {
                Item1 = pObj->PressKey_Pos - pObj->KeyNumber + 1;
            }
        }
        KEYBOARD_SetIndex(hObj,pObj->PressKey_Pos - Item1,1);
        Item1 = 1;
        WM_UNLOCK();
    }
}
/*获取按钮状态*/
int KEYBOARD_GetDisable(KeyBoard_Handle hObj,int Index)
{
    int Flag = 0;
    if (hObj)
    {
        KeyBoard_Obj* pObj;
        WM_LOCK();
        pObj = KeyBoard_H2P(hObj);
        Flag = pObj->KeyBoard_State[Index];
        KeyBoard_Invalidate(hObj);
        WM_UNLOCK();
    }
    return Flag;
}

/*设置按钮状态*/
void KEYBOARD_SetDisable(KeyBoard_Handle hObj,int Index,int Flag)
{
    if (hObj)
    {
        KeyBoard_Obj* pObj;
        WM_LOCK();
        pObj = KeyBoard_H2P(hObj);
        pObj->KeyBoard_State[Index] = Flag;
        KeyBoard_Invalidate(hObj);
        WM_UNLOCK();
    }
}

/*初始化键盘结构体*/
void KEYBOARD_SetInfo(KeyBoard_Handle hObj,int KeyNumber,int X_KeyNumber,int Y_KeyNumber,int PressKey_Pos,const char *(*KeyBoard_Name))
{
    int cnt;
    int xsize = 0,ysize = 0;
    if (hObj)
    {
        KeyBoard_Obj* pObj;
        WM_LOCK();
        pObj = KeyBoard_H2P(hObj);
        //按钮数量
	    pObj->KeyNumber = KeyNumber;
	    pObj->X_KeyNumber = X_KeyNumber;
        pObj->Y_KeyNumber = Y_KeyNumber;
        //被选中的按键的位置（按照从左至右、从上到下的顺序排列，首个按键的编号为0，一次向后排列）
        if(PressKey_Pos < 0)
        {
            PressKey_Pos = 0;
        }
        else if(PressKey_Pos >= KeyNumber)
        {
            PressKey_Pos = KeyNumber-1;
        }
	    pObj->PressKey_Pos     = PressKey_Pos; 
        pObj->Per_PressKey_Pos = PressKey_Pos;
        xsize = pObj->Widget.Win.Rect.x1-pObj->Widget.Win.Rect.x0;
        ysize = pObj->Widget.Win.Rect.y1-pObj->Widget.Win.Rect.y0;
        for(cnt = 0;cnt < pObj->X_KeyNumber*pObj->Y_KeyNumber;cnt++)  //位置信息初始化
        {
            pObj->KeyBoard_Pos[cnt][0] = (xsize/pObj->X_KeyNumber)*(cnt%pObj->X_KeyNumber);
            if((cnt%pObj->X_KeyNumber == pObj->X_KeyNumber -1)&&(xsize%pObj->X_KeyNumber))//最后一行，并且宽度不能被按键数整除，将多余的宽度放到最后行中
            {
                pObj->KeyBoard_Pos[cnt][0] += xsize%pObj->X_KeyNumber;
            }
            pObj->KeyBoard_Pos[cnt][1] = (ysize/pObj->Y_KeyNumber)*(cnt/pObj->X_KeyNumber);
            if((cnt/pObj->X_KeyNumber == pObj->Y_KeyNumber -1)&&(ysize%pObj->Y_KeyNumber))//最后一列，并且高度不能被按键数整除，将多余的高度放到最后列中
            {
                pObj->KeyBoard_Pos[cnt][1] += ysize%pObj->Y_KeyNumber;
            }
        }
        for(cnt = 0;cnt < pObj->KeyNumber;cnt++)  //按键名初始化
        {
            (pObj->KeyBoard_Name[cnt])=KeyBoard_Name+cnt;
        }        
        KeyBoard_Invalidate(hObj);
        WM_UNLOCK();
    }
}
