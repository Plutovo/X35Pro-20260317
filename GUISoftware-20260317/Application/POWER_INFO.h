#ifndef POWER_INFO_H
#define POWER_INFO_H
#endif
#include "WM.h"
#include "WIDGET.h"
typedef struct
{
    WIDGET Widget;
    GUI_COLOR BkColor;
    GUI_COLOR StateColor;
    GUI_COLOR BatteryColor;
    int PowerState;
    int BatteryState;//(1 t0 4)
    int PowerInfo_Use3D;
} POWER_INFO_Obj;
#define POWER_INFO_H2P(h) (POWER_INFO_Obj*) GUI_ALLOC_h2p(h)
#define POWER_INFO_Invalidate(hObj)    WM_InvalidateWindow(hObj)
/************************************************************
*
*       Types
*
*************************************************************
*/
typedef WM_HMEM POWER_INFO_Handle;
/************************************************************
*
*       Create function(s)
  Note: the parameters to a create function may vary.
         Some widgets may have multiple create functions
*/
POWER_INFO_Handle POWER_INFO_Create        (int x0, int y0, int xsize, int ysize, int ID, int Flags);
POWER_INFO_Handle POWER_INFO_CreateEx      (int x0, int y0, int xsize, int ysize, WM_HWIN hParent,
        int WinFlags, int ExFlags, int Id);
POWER_INFO_Handle POWER_INFO_CreateIndirect(const GUI_WIDGET_CREATE_INFO* pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK* cb);
/***************************************************************************************************
*
*       Standard member functions
*
**********************************************************************
*/
void POWER_INFO_SetBkColor  (POWER_INFO_Handle hObj,GUI_COLOR Color);
void POWER_INFO_SetStateColor(POWER_INFO_Handle hObj,GUI_COLOR Color);
void POWER_INFO_SetBatteryColor(POWER_INFO_Handle hObj,GUI_COLOR Color);
/***************************************************************************************************
*
*       The callback ...
*
* Do not call it directly ! It is only to be used from within an
* overwritten callback.
*/
void POWER_INFO_Callback(WM_MESSAGE *pMsg);

