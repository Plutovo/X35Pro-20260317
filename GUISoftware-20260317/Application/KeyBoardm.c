#include "KeyBoardm.h"

#include "GUI.h" 

#include "BUTTON.h" 
#include "DIALOG.h" 

#include "FRAMEWIN.h"

#include "GUI_HOOK.h"
#include "FRAMEWIN_Private.h" 
#include <string.h>
#include <ctype.h>



#define  KEYBOARD_CLIENT_BK_COLOR 0xF7D7BD

#define  KEYBOARD_BUTTON_BKCOLOR 0xf6d6bd
#define  KEYBOARD_BUTTON_BKCOLOR_Shift_CapOn 0x3B81ff


/************************************************************************/
/*             键盘的按键配置和键盘布局全局变量                                                         */
/************************************************************************/

//EDIT_Handle hEdtDebug,hEdtMsgC;
static WM_HWIN ghTarget;
static char* gCurrentKeyMap;
static unsigned char CapsFlag,ShiftFlag; //CapsLock 与Shift 按键开启状态

static WM_HWIN gKeyBoard;   

WM_HWIN old_hwin = NULL;
static GUI_HOOK gHook;
/************************************************************************/
/* 键盘按键 ID :以标准ASCII码设置。消息id也为该ASCII码                  */
/************************************************************************/
static char keysL[]=
{
	'`', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 'x',
	'x', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\\',
	'x', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', 'x', 'n',
	'x', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 'n', 'n', 'n',
};
static char keysL1[]=
{
	'~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', 'x',
	'x', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', ']', '}', '\|',
	'x', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ':', '\"', 'x', 'n',
	'x', 'z', 'x', 'c', 'v', 'b', 'n', 'm', '.', '>', '?', 'n', 'n', 'n',
};
static char keysU[]=
{
	'`', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 'X',
	'X', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', '\\',
	'X', 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'', 'X', 'N',
	'X', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', '/', 'N', 'N', 'N',
};

static char keysU1[]=
{
	'~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', 'X',
		'X', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', ']', '}', '\|',
	'X', 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '\"', 'X', 'N',
	'X', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '.', '>', '?', 'N', 'N', 'N',
};


static int _hookFW(WM_MESSAGE * pMsg)//目标窗口的挂钩函数
{

	switch (pMsg->MsgId)
	{
	case WM_TOUCH_CHILD:
		{
			GUI_PID_STATE* pState;
			pState = ( GUI_PID_STATE * )( ( WM_MESSAGE * )pMsg->Data.p )->Data.p;
			if (( pState!=0 )&&(pState->Pressed))
			{
				I32 NCode =pMsg->Data.v;
				if ((pMsg->hWinSrc)&&(NCode!=WM_NOTIFICATION_LOST_FOCUS )&&(NCode!=WM_NOTIFICATION_LOST_FOCUS)&&(NCode!=WM_NOTIFICATION_CHILD_DELETED))
				{
					ghTarget = pMsg->hWinSrc;
				}

			}
		}
		break;
	case WM_NOTIFY_CHILD_HAS_FOCUS:
		break;
	}
	return 0;

}
/************************************************************************/
/* 初始化键盘信息结构体                                                 */
/************************************************************************/
static void _Init_KEYBOARDCFG(KEYBOARDCFG* kbcfg,WM_HWIN hWin)
{
	kbcfg->hWin = hWin;
	kbcfg->keymap = gCurrentKeyMap;
	kbcfg->x0 = 10;
	kbcfg->y0 = 15;
	kbcfg->xSize = (664 -14*1)/14 * 1.06;
	kbcfg->ySize = ((224 - 5*2)/5)*0.9;
	kbcfg->xSpan= 1;
	kbcfg->ySpan = 2;
	kbcfg->num = sizeof(keysL)/sizeof(char);
}

/************************************************************************/
/*   创建按键                                                           */
/************************************************************************/
static void _CreateKeys(KEYBOARDCFG* kbcfg)
{
	int x0,y0,xSize,ySize,xSpan,ySpan;
	int i,startX,startY,num;
	WM_HWIN hWin;
	BUTTON_Handle hbtn;
	char tmp[20],*keyMap;
	x0 = kbcfg->x0;
	y0 = kbcfg->y0;
	xSize = kbcfg->xSize;
	ySize = kbcfg->ySize;
	xSpan = kbcfg->xSpan;
	ySpan = kbcfg->ySpan;
	num = kbcfg->num;
	hWin=kbcfg->hWin;
	keyMap = kbcfg->keymap;
	startX = x0;
	startY = y0;
	for (i=0; i<num; i++)
	{
		memset(tmp,0,20);
		tmp[0] = keyMap[i];
		switch( (i/14))
		{
		case 0: //第一行
			if ( ((i + 1) % 14 == 12)) //backspace
			{
				xSize = xSize * 1.5;
				hbtn = BUTTON_CreateAsChild(x0,y0,xSize,ySize,hWin,GUI_KEY_BACKSPACE,BUTTON_CF_SHOW);
				BUTTON_SetText(hbtn,"Backspace");
				BUTTON_SetTextWrapMode(hbtn,GUI_WRAPMODE_CHAR);
				BUTTON_SetTextAlign(hbtn,GUI_TA_VCENTER|GUI_TA_HCENTER);
				BUTTON_SetFont(hbtn,&GUI_Font_16);
				BUTTON_SetBkColor(hbtn,BUTTON_CI_UNPRESSED,KEYBOARD_BUTTON_BKCOLOR);
				BUTTON_SetBkColor(hbtn,3,KEYBOARD_CLIENT_BK_COLOR);

			}
			else if( ((i + 1) % 14 == 13) || ((i + 1) % 14 == 0 ) ) //not used
			{
				; 
			}
			else //others number and char
			{
				hbtn = BUTTON_CreateAsChild(x0,y0,xSize,ySize,hWin,keyMap[i],BUTTON_CF_SHOW);
				BUTTON_SetText(hbtn,tmp);
				BUTTON_SetBkColor(hbtn,BUTTON_CI_UNPRESSED,KEYBOARD_BUTTON_BKCOLOR);
				BUTTON_SetFont(hbtn,&GUI_Font_20);
				BUTTON_SetBkColor(hbtn,3,KEYBOARD_CLIENT_BK_COLOR);
			}

			break;
		case  1:
			if ( ((i+1)%14==1) )//Tab
			{
				xSize = xSize * 1;
				hbtn = BUTTON_CreateAsChild(x0,y0,xSize,ySize,hWin,GUI_KEY_TAB,BUTTON_CF_SHOW);
				BUTTON_SetText(hbtn,"Tab");
				BUTTON_SetFont(hbtn,&GUI_Font_20);
				BUTTON_SetBkColor(hbtn,BUTTON_CI_UNPRESSED,KEYBOARD_BUTTON_BKCOLOR);
				BUTTON_SetBkColor(hbtn,3,KEYBOARD_CLIENT_BK_COLOR);
			}
			else if (((i+1)%14==13) || ((i+1)%14==0))
			{
				; //not used
			}
			else //others number and char
			{
				hbtn = BUTTON_CreateAsChild(x0,y0,xSize,ySize,hWin,keyMap[i],BUTTON_CF_SHOW);
				BUTTON_SetText(hbtn,tmp);
				BUTTON_SetBkColor(hbtn,BUTTON_CI_UNPRESSED,KEYBOARD_BUTTON_BKCOLOR);
				BUTTON_SetFont(hbtn,&GUI_Font_20);
				BUTTON_SetBkColor(hbtn,3,KEYBOARD_CLIENT_BK_COLOR);
			}
			break;
		case 2:
			if ( ((i+1)%14==1)  )//Caps
			{
				xSize = xSize * 1.8;
				hbtn = BUTTON_CreateAsChild(x0,y0,xSize,ySize,hWin,GUI_KEY_CAPITAL,BUTTON_CF_SHOW);
				BUTTON_SetText(hbtn,"Caps");
				BUTTON_SetFont(hbtn,&GUI_Font_20);
				if (CapsFlag)
				{
					BUTTON_SetBkColor(hbtn,BUTTON_CI_UNPRESSED,KEYBOARD_BUTTON_BKCOLOR_Shift_CapOn);
				}
				else
				{
					BUTTON_SetBkColor(hbtn,BUTTON_CI_UNPRESSED,KEYBOARD_BUTTON_BKCOLOR);
				}
				BUTTON_SetBkColor(hbtn,3,KEYBOARD_CLIENT_BK_COLOR);
			}
			else if (((i+1)%14==11) )//Enter
			{
				xSize = xSize * 1.74;
				hbtn = BUTTON_CreateAsChild(x0,y0,xSize,ySize,hWin,GUI_KEY_ENTER,BUTTON_CF_SHOW);
				BUTTON_SetText(hbtn,"Enter");
				BUTTON_SetFont(hbtn,&GUI_Font_20);
				BUTTON_SetBkColor(hbtn,BUTTON_CI_UNPRESSED,KEYBOARD_BUTTON_BKCOLOR);
				BUTTON_SetBkColor(hbtn,3,KEYBOARD_CLIENT_BK_COLOR);
			}
			else if ((((i+1)%14==12) ||(i+1)%14==13) || ((i+1)%14==0))
			{
				; //not used
			}
			else //others number and char
			{
				hbtn = BUTTON_CreateAsChild(x0,y0,xSize,ySize,hWin,keyMap[i],BUTTON_CF_SHOW);
				BUTTON_SetText(hbtn,tmp);
				BUTTON_SetBkColor(hbtn,BUTTON_CI_UNPRESSED,KEYBOARD_BUTTON_BKCOLOR);
				BUTTON_SetFont(hbtn,&GUI_Font_20);
				BUTTON_SetBkColor(hbtn,3,KEYBOARD_CLIENT_BK_COLOR);
			}
			break;
		case 3:
			if ( ((i+1)%14 == 1 ) || ((i+1)%14 == 10))//Shift
			{
				xSize = xSize * 2.29;
				hbtn = BUTTON_CreateAsChild(x0,y0,xSize - 1,ySize,hWin,GUI_KEY_SHIFT,BUTTON_CF_SHOW);
				BUTTON_SetText(hbtn,"Shift");
				BUTTON_SetFont(hbtn,&GUI_Font_20);
				if (ShiftFlag)
				{
					BUTTON_SetBkColor(hbtn,BUTTON_CI_UNPRESSED,KEYBOARD_BUTTON_BKCOLOR_Shift_CapOn);
				}
				else
				{
					BUTTON_SetBkColor(hbtn,BUTTON_CI_UNPRESSED,KEYBOARD_BUTTON_BKCOLOR);
				}
				BUTTON_SetBkColor(hbtn,3,KEYBOARD_CLIENT_BK_COLOR);
			}
			else if ((((i+1)%14 >= 11) || (i+1)%14==0) )
			{
				; //not used
			}
			else //others number and char
			{
				hbtn = BUTTON_CreateAsChild(x0,y0,xSize,ySize,hWin,keyMap[i],BUTTON_CF_SHOW);
				BUTTON_SetText(hbtn,tmp);
				BUTTON_SetBkColor(hbtn,BUTTON_CI_UNPRESSED,KEYBOARD_BUTTON_BKCOLOR);
				BUTTON_SetFont(hbtn,&GUI_Font_20);
				BUTTON_SetBkColor(hbtn,3,KEYBOARD_CLIENT_BK_COLOR);
			}
			break;

		default:
			break;
		}

		x0 = x0 + xSize +xSpan;
		if (((i+1)%14==1) && (i/14 ==1) || ((i+1)%14==1) && (i/14 ==2)||((i+1)%14==1) && (i/14 ==3) ||((i+1)%14==13) && (i/14 ==2))
		{
			xSize = kbcfg->xSize;
		}

		if ((i+1)%14 == 0)
		{
			y0 = y0 + ySize + ySpan;
			x0 = startX;
		}


	}

	hbtn = BUTTON_CreateAsChild(x0=x0 + xSpan+0*xSize,y0,(xSize*0.5),ySize,hWin,GUI_KEY_ESCAPE,BUTTON_CF_SHOW);//ESC
	BUTTON_SetText(hbtn,"ESC");
	BUTTON_SetBkColor(hbtn,BUTTON_CI_UNPRESSED,KEYBOARD_BUTTON_BKCOLOR);
	BUTTON_SetFont(hbtn,&GUI_Font_16);
	BUTTON_SetBkColor(hbtn,3,KEYBOARD_CLIENT_BK_COLOR);

	hbtn = BUTTON_CreateAsChild(x0=x0 + xSpan+0.5*xSize,y0,(xSize*0.5),ySize,hWin,GUI_KEY_ESCAPE,BUTTON_CF_SHOW);//Clear
	BUTTON_SetText(hbtn,"Clear");
	BUTTON_SetBkColor(hbtn,BUTTON_CI_UNPRESSED,KEYBOARD_BUTTON_BKCOLOR);
	BUTTON_SetFont(hbtn,&GUI_Font_16);
	BUTTON_SetBkColor(hbtn,3,KEYBOARD_CLIENT_BK_COLOR);


	hbtn = BUTTON_CreateAsChild(x0=x0 + xSpan+0.5*xSize,y0,(xSize*0.5),ySize,hWin,GUI_KEY_DELETE,BUTTON_CF_SHOW);//delete
	BUTTON_SetText(hbtn,"Del");
	BUTTON_SetBkColor(hbtn,BUTTON_CI_UNPRESSED,KEYBOARD_BUTTON_BKCOLOR);
	BUTTON_SetFont(hbtn,&GUI_Font_16);
	BUTTON_SetBkColor(hbtn,3,KEYBOARD_CLIENT_BK_COLOR);

	hbtn = BUTTON_CreateAsChild(x0=x0 + xSpan+0.5*xSize,y0,(xSize*2.5+3*xSpan),ySize,hWin,' ',BUTTON_CF_SHOW);//space
	BUTTON_SetText(hbtn,"");
	BUTTON_SetBkColor(hbtn,BUTTON_CI_UNPRESSED,KEYBOARD_BUTTON_BKCOLOR);
	BUTTON_SetFont(hbtn,&GUI_Font_16);
	BUTTON_SetBkColor(hbtn,3,KEYBOARD_CLIENT_BK_COLOR);

	hbtn = BUTTON_CreateAsChild(x0=x0 + xSpan+2.55*xSize,y0,(xSize*0.7+xSpan),ySize,hWin,GUI_KEY_LEFT,BUTTON_CF_SHOW);//Left
	BUTTON_SetText(hbtn,"Left");
	BUTTON_SetBkColor(hbtn,BUTTON_CI_UNPRESSED,KEYBOARD_BUTTON_BKCOLOR);
	BUTTON_SetFont(hbtn,&GUI_Font_16);
	BUTTON_SetBkColor(hbtn,3,KEYBOARD_CLIENT_BK_COLOR);

	hbtn = BUTTON_CreateAsChild(x0=x0 + xSpan+0.75*xSize,y0,(xSize*0.7 + xSpan),ySize,hWin,GUI_KEY_RIGHT,BUTTON_CF_SHOW);//Right
	BUTTON_SetText(hbtn,"Right");
	BUTTON_SetBkColor(hbtn,BUTTON_CI_UNPRESSED,KEYBOARD_BUTTON_BKCOLOR);
	BUTTON_SetFont(hbtn,&GUI_Font_16);
	BUTTON_SetBkColor(hbtn,3,KEYBOARD_CLIENT_BK_COLOR);

}

static void _cbKeyBoard(WM_MESSAGE * pMsg)/*键盘回调函数*/
{
	switch(pMsg->MsgId)
	{
	case WM_PAINT:
		GUI_SetBkColor(KEYBOARD_CLIENT_BK_COLOR);
		GUI_Clear();
		break;
	case WM_NOTIFY_PARENT:
		{
			I32 Id;
			if (pMsg->Data.v!=WM_NOTIFICATION_RELEASED)
			{
				break;
			}
			//Id = GUI_GetKey();
			Id=WM_GetId(pMsg->hWinSrc);
			if (Id != NULL)//&&(pMsg->hWin!=pMsg->hWinSrc))
			{
				switch(Id)
				{
				case GUI_KEY_CAPITAL:
				case GUI_KEY_SHIFT:
					{
						KEYBOARDCFG kbcfg = {0};
						KeyBoard_clear(WM_GetClientWindow(pMsg->hWin));

						if (Id == GUI_KEY_CAPITAL)
						{
							CapsFlag = ~CapsFlag;
						}
						else if (Id == GUI_KEY_SHIFT)
						{
							
							ShiftFlag = ~ShiftFlag;
						}

						if (CapsFlag && ShiftFlag)
						{
							gCurrentKeyMap = keysL1;
						}
						else if(CapsFlag && !ShiftFlag)
						{
							gCurrentKeyMap = keysU;
						}
						else if(!CapsFlag && ShiftFlag)
						{
							gCurrentKeyMap = keysU1;
						}
						else if (!CapsFlag && !ShiftFlag)
						{
							gCurrentKeyMap = keysL;
						}	
                        _Init_KEYBOARDCFG(&kbcfg, pMsg->hWin);
						_CreateKeys(&kbcfg);
			
					}
					break;
				case GUI_KEY_TAB:
					{
						int tmp  = WM_GetNextSibling(ghTarget);		
						if(tmp != 0)
						{
							//WM_GetPrevSibling
							ghTarget = tmp;
							WM_SetFocus(ghTarget); 
						}
						BUTTON_SetPressed(pMsg->hWinSrc,BUTTON_CI_UNPRESSED); //按键弹起
					}
					break;
				case GUI_KEY_ESCAPE:				
						BUTTON_SetPressed(pMsg->hWinSrc,BUTTON_CI_UNPRESSED);
						WM_HideWindow(KeyBoard_GetHandle());	
					break;
				default:
					if (WM_IsWindow(ghTarget)&&(iscntrl(Id)||isprint(Id))&&(Id<0x7f))
					{
						WM_KEY_INFO key = {0};
						WM_MESSAGE msg = {0};
						key.Key = Id;
						key.PressedCnt = 1;
						msg.MsgId = WM_KEY;
						msg.Data.p = &key;
						BUTTON_SetPressed(pMsg->hWinSrc,BUTTON_CI_UNPRESSED); 
						WM_SendMessage(ghTarget,&msg);
						WM_SetFocus(ghTarget);
					}
					//BUTTON_SetPressed(pMsg->hWinSrc,BUTTON_CI_UNPRESSED); 
					break;
				}

			}

		} //end case WM_NOTIFY_PARENT
		break;
	default:
		WM_DefaultProc(pMsg);
	}

}

WM_HWIN KeyBoard_Create()
{
	WM_HWIN hFW,hWin;
	KEYBOARDCFG kbcfg;
	if (WM_IsWindow(gKeyBoard))
	{
		return gKeyBoard;
	}
	hFW = FRAMEWIN_CreateAsChild(3, 142, 560+100-26, 210+32-18, 0, "KeyBaord", NULL, WM_CF_SHOW);
	FRAMEWIN_SetClientColor(hFW, KEYBOARD_CLIENT_BK_COLOR);
	FRAMEWIN_SetBorderSize(hFW,0);
    FRAMEWIN_SetTitleVis(hFW,0); //隐藏标题栏
	hWin = WM_GetClientWindow(hFW);
	gKeyBoard = hFW;
	WM_SetStayOnTop(hFW,1);
	gCurrentKeyMap = keysL;
	 _Init_KEYBOARDCFG(&kbcfg, hWin);
	_CreateKeys(&kbcfg);
	WM_SetCallback(hWin,_cbKeyBoard);

	return hFW;
}


void KeyBoard_clear(WM_HWIN hWin)
{
	WM_HWIN hCWin;
	for (hCWin=WM_GetFirstChild(hWin); hCWin!=0; hCWin=WM_GetFirstChild(hWin))
	{
		//hCWin = WM_GetNextSibling(hCWin);
		WM_DeleteWindow(hCWin);
	}
}

/************************************************************************/
/* 设置键盘位置                                                         */
/************************************************************************/
void KetBoard_SetPos(int x, int y)
{
	WM_MoveTo(gKeyBoard, x, y);
}

void KeyBoard_deleteWindow(void)
{
	if(WM_IsWindow(gKeyBoard))
	{
		WM_DeleteWindow(gKeyBoard);
		gKeyBoard = 0;
	}
}

WM_HWIN KeyBoard_GetHandle()
{
	return gKeyBoard;
}

void KeyBoard_SetHook(WM_HWIN hWin)
{
	FRAMEWIN_Obj* pObj;
	//   if(WM_IsWindow(old_hwin))
	//   {
	//     pObj= FRAMEWIN_H2P(old_hwin);
	//     GUI_HOOK_Remove(&pObj->pFirstHook,&gHook);
	//   }
	if(WM_IsWindow(hWin))
	{
		pObj= FRAMEWIN_H2P(hWin);
		GUI_HOOK_Add(&pObj->pFirstHook,&gHook,&_hookFW);
		old_hwin = hWin;
	}
}
