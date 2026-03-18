//
#include "POWER_INFO.h"
/***************************************************************************************************
*
*       Private config defaults
*
****************************************************************************************************
*/
/* Support for 3D effects */
#ifndef   POWER_INFO_USE_3D
#define POWER_INFO_USE_3D 1
#endif
/* Define colors */
#ifndef   POWER_INFO_BKCOLOR_DEFAULT
#define POWER_INFO_BKCOLOR_DEFAULT   0x383736
#endif
#ifndef   POWER_INFO_INDICATE_COLOR_DEFAULT
#define POWER_INFO_INDICATE_COLOR_DEFAULT  0xa1a545//0x00FF00
#endif
#ifndef   POWER_INFO_BATCOLOR_DEFAULT
#define POWER_INFO_BATCOLOR_DEFAULT  0x005B00
#endif
#ifndef   POWER_INFO_STATE_DEFAULT
#define POWER_INFO_STATE_DEFAULT  AC_WORK
#endif
// extern GUI_CONST_STORAGE GUI_BITMAP bmac_icon;
// extern GUI_CONST_STORAGE GUI_BITMAP bmbatt_cali; //  20130426
// extern GUI_CONST_STORAGE GUI_BITMAP bmbattery;
// 
// extern GUI_CONST_STORAGE GUI_BITMAP bmPlug_WB_Icon;//ylk modify 20180505
// extern GUI_CONST_STORAGE GUI_BITMAP bmBattery_WB_Bg;//ylk modify 20180505
extern GUI_CONST_STORAGE GUI_BITMAP bmelectric_quantity;//ylk modify 20180505

extern GUI_CONST_STORAGE GUI_BITMAP bmIconBattery;
extern GUI_CONST_STORAGE GUI_BITMAP bmIconAC;
//extern char alarm_battery_cali;
char alarm_battery_cali_index = 0; //校验停止标识
/***************************************************************************************************
*
*       Static routines
*
****************************************************************************************************
*/

/***************************************************************************************************
*
*       _Paint
*/
static void _Paint(POWER_INFO_Obj* pObj, POWER_INFO_Handle hObj)
{
// unsigned int Index;
    GUI_RECT rInside;
	int xSize,ySize;
    GUI_GetClientRect(&rInside);
    LCD_SetBkColor (pObj->BkColor);
    WM_SetUserClipRect(&rInside);
    GUI_Clear();
    /* Draw bitmap.*/
	xSize = rInside.x1-rInside.x0;
	ySize = rInside.y1-rInside.y0;

	GUI_DrawBitmap(&bmIconAC,rInside.x0+40,rInside.y0+ySize/4-8); // AC icon
	GUI_DrawBitmap(&bmIconBattery,rInside.x0+30,rInside.y0+ySize*3/4-10); // BAT icon

	//GUI_DrawBitmap(&bmIconAC,rInside.x0+25,rInside.y0+ySize/4-8); // AC icon
	//GUI_DrawBitmap(&bmIconBattery,rInside.x0+85,rInside.y0+ySize/4-6); // BAT icon

    GUI_SetColor(GUI_WHITE);
	GUI_DrawCircle(rInside.x0+10,rInside.y0+18,6); // circle
	GUI_DrawCircle(rInside.x0+10,rInside.y0+45,6); // circle

    GUI_SetColor(pObj->StateColor);

    if(pObj->PowerState==0x11)  //AC_WORK = 0x11
    {
        GUI_FillCircle(rInside.x0+10,rInside.y0+18,5);
    }
    else if(pObj->PowerState == 0x33) //BAT_CAL  //  20130426
    {
		GUI_DrawBitmap(&bmelectric_quantity,rInside.x0+30,rInside.y0+26);
    }
    else  //BAT_WORK  = 0x22
    {
		GUI_FillCircle(rInside.x0+10,rInside.y0+45,5);
    }   

	GUI_SetColor(pObj->BkColor);
	//GUI_SetColor(GUI_WHITE);
	GUI_FillRect(rInside.x0+31,rInside.y0+39,rInside.x0+68,rInside.y0+53);
	GUI_SetColor(pObj->BatteryColor);
	switch(pObj->BatteryState)
	{
	default:
	case 1:
		// 		GUI_SetColor(pObj->BkColor);
		// 		GUI_FillRect(rInside.x0+31,rInside.y0+39,rInside.x0+40,rInside.y0+53);
		// 		GUI_FillRect(rInside.x0+40,rInside.y0+39,rInside.x0+49,rInside.y0+53);
		// 		GUI_FillRect(rInside.x0+49,rInside.y0+39,rInside.x0+58,rInside.y0+53);
		// 		GUI_FillRect(rInside.x0+58,rInside.y0+39,rInside.x0+68,rInside.y0+53);
		break;
	case 2:
		GUI_FillRect(rInside.x0+31,rInside.y0+39,rInside.x0+43,rInside.y0+53);
		// 		GUI_SetColor(pObj->BkColor);
		// 		GUI_FillRect(rInside.x0+40,rInside.y0+39,rInside.x0+49,rInside.y0+53);
		// 		GUI_FillRect(rInside.x0+49,rInside.y0+39,rInside.x0+58,rInside.y0+53);
		// 		GUI_FillRect(rInside.x0+58,rInside.y0+39,rInside.x0+68,rInside.y0+53);
		break;
	case 3:
		GUI_FillRect(rInside.x0+31,rInside.y0+39,rInside.x0+43,rInside.y0+53);
		GUI_FillRect(rInside.x0+43,rInside.y0+39,rInside.x0+55,rInside.y0+53);
		// 		GUI_SetColor(pObj->BkColor);
		// 		GUI_FillRect(rInside.x0+49,rInside.y0+39,rInside.x0+58,rInside.y0+53);
		// 		GUI_FillRect(rInside.x0+58,rInside.y0+39,rInside.x0+68,rInside.y0+53);
		break;
	case 4:
		GUI_FillRect(rInside.x0+31,rInside.y0+39,rInside.x0+43,rInside.y0+53);
		GUI_FillRect(rInside.x0+43,rInside.y0+39,rInside.x0+55,rInside.y0+53);
		GUI_FillRect(rInside.x0+55,rInside.y0+39,rInside.x0+68,rInside.y0+53);
		// 		GUI_SetColor(pObj->BkColor);
		// 		GUI_FillRect(rInside.x0+58,rInside.y0+39,rInside.x0+68,rInside.y0+53);
		break;
	}
    WM_SetUserClipRect(0);
}
/***************************************************************************************************
*       电源指示状态改变更新
*       _StatusChangedUpdate
*/
static void _StatusChangedUpdate(POWER_INFO_Obj* pObj, POWER_INFO_Handle hObj)
{
    GUI_RECT rInside;

    GUI_GetClientRect(&rInside);
    WM_SetUserClipRect(&rInside);
    //
//     GUI_SetColor(GUI_WHITE);
//     GUI_DrawCircle(rInside.x0+10,rInside.y0+18,6);
//     GUI_DrawCircle(rInside.x0+10,rInside.y0+45,6);

	//
    GUI_SetColor(pObj->StateColor);

    if(pObj->PowerState==0x11)//AC_WORK 0x11 
    {
		GUI_SetColor(pObj->StateColor);
		GUI_FillCircle(rInside.x0+10,rInside.y0+18,5);
		GUI_SetColor(pObj->BkColor);
		GUI_FillCircle(rInside.x0+10,rInside.y0+45,5);
    }
    else if(pObj->PowerState == 0x33) //BAT_CAL  //  20130426
    {
		GUI_DrawBitmap(&bmelectric_quantity,rInside.x0+30,rInside.y0+5);//ylk modify 20180505
    }    
    else //BAT_WORK 0x22
    {
		GUI_SetColor(pObj->BkColor);
		GUI_FillCircle(rInside.x0+10,rInside.y0+18,5);
		GUI_SetColor(pObj->StateColor);
		GUI_FillCircle(rInside.x0+10,rInside.y0+45,5);
    }
	//
	GUI_SetColor(pObj->BkColor);
//     GUI_SetColor(GUI_WHITE);
	GUI_FillRect(rInside.x0+31,rInside.y0+39,rInside.x0+68,rInside.y0+53);
	GUI_SetColor(pObj->BatteryColor);
	switch(pObj->BatteryState)
	{
	default:
	case 1:
		// 		GUI_SetColor(pObj->BkColor);
		// 		GUI_FillRect(rInside.x0+31,rInside.y0+39,rInside.x0+40,rInside.y0+53);
		// 		GUI_FillRect(rInside.x0+40,rInside.y0+39,rInside.x0+49,rInside.y0+53);
		// 		GUI_FillRect(rInside.x0+49,rInside.y0+39,rInside.x0+58,rInside.y0+53);
		// 		GUI_FillRect(rInside.x0+58,rInside.y0+39,rInside.x0+68,rInside.y0+53);
		break;
	case 2:
		GUI_FillRect(rInside.x0+31,rInside.y0+39,rInside.x0+43,rInside.y0+53);
		// 		GUI_SetColor(pObj->BkColor);
		// 		GUI_FillRect(rInside.x0+40,rInside.y0+39,rInside.x0+49,rInside.y0+53);
		// 		GUI_FillRect(rInside.x0+49,rInside.y0+39,rInside.x0+58,rInside.y0+53);
		// 		GUI_FillRect(rInside.x0+58,rInside.y0+39,rInside.x0+68,rInside.y0+53);
		break;
	case 3:
		GUI_FillRect(rInside.x0+31,rInside.y0+39,rInside.x0+43,rInside.y0+53);
		GUI_FillRect(rInside.x0+43,rInside.y0+39,rInside.x0+55,rInside.y0+53);
		// 		GUI_SetColor(pObj->BkColor);
		// 		GUI_FillRect(rInside.x0+49,rInside.y0+39,rInside.x0+58,rInside.y0+53);
		// 		GUI_FillRect(rInside.x0+58,rInside.y0+39,rInside.x0+68,rInside.y0+53);
		break;
	case 4:
		GUI_FillRect(rInside.x0+31,rInside.y0+39,rInside.x0+43,rInside.y0+53);
		GUI_FillRect(rInside.x0+43,rInside.y0+39,rInside.x0+55,rInside.y0+53);
		GUI_FillRect(rInside.x0+55,rInside.y0+39,rInside.x0+68,rInside.y0+53);
		// 		GUI_SetColor(pObj->BkColor);
		// 		GUI_FillRect(rInside.x0+58,rInside.y0+39,rInside.x0+68,rInside.y0+53);
		break;
	}
    WM_SetUserClipRect(0);
}
/***************************************************************************************************
*
*
*/
void POWER_INFO_Callback(WM_MESSAGE *pMsg)
{
    POWER_INFO_Handle hObj = pMsg->hWin;
    POWER_INFO_Obj* pObj = POWER_INFO_H2P(hObj);
    /* Let widget handle the standard messages */
    if (WIDGET_HandleActive(hObj, pMsg) == 0)
    {
        return;
    }
    switch (pMsg->MsgId)
    {
    case WM_TOUCH:
        return;      /* Message handled. Do not call WM_DefaultProc, because the window may have been destroyed */
    case WM_PAINT:
        _Paint(pObj, hObj);
        return;
    case WM_DELETE:
        break;       /* No return here ... WM_DefaultProc needs to be called */
    case WM_STATUS_CHANGED:
        _StatusChangedUpdate(pObj, hObj);
        break;
    }
    WM_DefaultProc(pMsg);
}
/***************************************************************************************************
*
*       Exported routines:  Create
*
****************************************************************************************************
*/
POWER_INFO_Handle POWER_INFO_Create(int x0, int y0, int xsize, int ysize, int Id, int Flags)
{
    return POWER_INFO_CreateEx(x0, y0, xsize, ysize, WM_HMEM_NULL, Flags, 0, Id);
}
/***************************************************************************************************
*
*
*/
POWER_INFO_Handle POWER_INFO_CreateEx(int x0, int y0, int xsize, int ysize, WM_HWIN hParent, int WinFlags, int ExFlags, int Id)
{
    POWER_INFO_Handle hObj;
    GUI_USE_PARA(ExFlags);
    /* Create the window */
    WM_LOCK();
    hObj = WM_CreateWindowAsChild(x0, y0, xsize, ysize, hParent, WinFlags, POWER_INFO_Callback,
                                  sizeof(POWER_INFO_Obj) - sizeof(WM_Obj));
    if (hObj)
    {
        POWER_INFO_Obj* pObj = POWER_INFO_H2P(hObj);
        /* init widget specific variables */
        WIDGET__Init(&pObj->Widget, Id, WIDGET_STATE_FOCUSSABLE);
        pObj->BkColor = POWER_INFO_BKCOLOR_DEFAULT;
        pObj->StateColor = POWER_INFO_INDICATE_COLOR_DEFAULT;
        pObj->BatteryColor = POWER_INFO_BATCOLOR_DEFAULT;
        pObj->PowerState = 0x11;
        pObj->BatteryState = 0;
        pObj->PowerInfo_Use3D = 0;
    }
    else
    {
        GUI_DEBUG_ERROROUT_IF(hObj==0, "BUTTON_Create failed")
    }
    WM_UNLOCK();
    return hObj;
}
/***************************************************************************************************
*
*       POWER_INFO_CreateIndirect
*/
POWER_INFO_Handle POWER_INFO_CreateIndirect(const GUI_WIDGET_CREATE_INFO* pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK* cb)
{
    POWER_INFO_Handle  hThis;
    //GUI_USE_PARA(cb);
    hThis = POWER_INFO_CreateEx(pCreateInfo->x0 + x0, pCreateInfo->y0 + y0,
                                pCreateInfo->xSize, pCreateInfo->ySize,
                                hWinParent, 0, pCreateInfo->Flags, pCreateInfo->Id);
    return hThis;
}
/***************************************************************************************************
*
*       POWER_INFO_SetBkColor
*/
void POWER_INFO_SetBkColor(POWER_INFO_Handle hObj,GUI_COLOR Color)
{
    if (hObj)
    {
        POWER_INFO_Obj* pObj;
        WM_LOCK();
        pObj = POWER_INFO_H2P(hObj);
        pObj->BkColor = Color;
        POWER_INFO_Invalidate(hObj);
        WM_UNLOCK();
    }
}
void POWER_INFO_SetStateColor(POWER_INFO_Handle hObj,GUI_COLOR Color)
{
	if (hObj)
	{
		POWER_INFO_Obj* pObj;
		WM_LOCK();
		pObj = POWER_INFO_H2P(hObj);
		pObj->StateColor = Color;
		POWER_INFO_Invalidate(hObj);
		WM_UNLOCK();
	}
}
void POWER_INFO_SetBatteryColor(POWER_INFO_Handle hObj,GUI_COLOR Color)
{
	if (hObj)
	{
		POWER_INFO_Obj* pObj;
		WM_LOCK();
		pObj = POWER_INFO_H2P(hObj);
		pObj->BatteryColor = Color;
		POWER_INFO_Invalidate(hObj);
		WM_UNLOCK();
	}
}
/***************************************************************************************************
*
*       POWER_INFO_SetBatteryState
*/
void POWER_INFO_SetBatteryState(POWER_INFO_Handle hObj,int state)
{
    if (hObj)
    {
        POWER_INFO_Obj* pObj;
        WM_LOCK();
        pObj = POWER_INFO_H2P(hObj);
        pObj->BatteryState = state;
        //POWER_INFO_Invalidate(hObj);
        WM_UNLOCK();
    }
}
/***************************************************************************************************
*
*       POWER_INFO_SetPowerState
*/
void POWER_INFO_SetPowerState(POWER_INFO_Handle hObj,int state)
{
    if (hObj)
    {
        POWER_INFO_Obj* pObj;
        WM_LOCK();
        pObj = POWER_INFO_H2P(hObj);
        pObj->PowerState = state;
        //POWER_INFO_Invalidate(hObj);
        WM_UNLOCK();
    }
}
/***************************************************************************************************
*
*       POWER_INFO_GetBatteryState
*/
int POWER_INFO_GetBatteryState(POWER_INFO_Handle hObj)
{
    int state = 0;
    if (hObj)
    {
        POWER_INFO_Obj* pObj;
        WM_LOCK();
        pObj = POWER_INFO_H2P(hObj);
        state = pObj->BatteryState;
        POWER_INFO_Invalidate(hObj);
        WM_UNLOCK();
    }
    return state;
}
/***************************************************************************************************
*
*       POWER_INFO_GetPowerState
*/
int POWER_INFO_GetPowerState(POWER_INFO_Handle hObj)
{
    int state = 0;
    if (hObj)
    {
        POWER_INFO_Obj* pObj;
        WM_LOCK();
        pObj = POWER_INFO_H2P(hObj);
        state = pObj->PowerState;
        POWER_INFO_Invalidate(hObj);
        WM_UNLOCK();
    }
    return state;
}
