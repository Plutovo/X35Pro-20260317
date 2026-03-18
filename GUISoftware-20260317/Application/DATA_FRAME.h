//
#ifndef DATA_FRAME_H
#define DATA_FRAME_H
#endif
//
#include "WIDGET.h"
#include "Dialog.h"
#include "WM.h"
#include "GUI_Protected.h"
//
typedef struct
{
    WIDGET Widget;
    GUI_COLOR BkColor;
    GUI_COLOR FrameColor;
    GUI_COLOR DataColor;
    GUI_COLOR TextColor;
    const GUI_FONT GUI_UNI_PTR * pTextFont;               ///Text×ÖÌå´óÐ¡
    const GUI_FONT GUI_UNI_PTR * pHeadFont;
    const GUI_FONT GUI_UNI_PTR * pUnitFont;
    const GUI_FONT GUI_UNI_PTR * pDataFont;
    const GUI_FONT GUI_UNI_PTR * pRealtimeDataFont;
    int data0;
    int data1;
    int data2;
    int data3;
    int data4;
    int data5;
    int data6;
    char * data0_text;
    char * data1_text;
    char * data2_text;
    char * data3_text;
    char * data4_text;
    char * data5_text;
    char * data6_text;
    char * HeadText;
    char * RT_Unit;
    char * data0_Unit;
    char * data1_Unit;
    char * data2_Unit;
    char * data3_Unit;
    char * data4_Unit;
    char * data5_Unit;
    char * data6_Unit;
    int ParameterNum;
    int RT_Value;
    int DisplayFlag;
} DATA_FRAME_Obj;
//
#define DATA_FRAME_H2P(h) (DATA_FRAME_Obj*) GUI_ALLOC_h2p(h)
#define DATA_FRAME_Invalidate(hObj)    WM_InvalidateWindow(hObj)
/***************************************************************************************************
*
*       Types
*
***************************************************************************************************/
typedef WM_HMEM DATA_FRAME_Handle;

/***************************************************************************************************
*
*       Create function(s)
  Note: the parameters to a create function may vary.
         Some widgets may have multiple create functions
*/
DATA_FRAME_Handle DATA_FRAME_Create(int x0, int y0, int xsize, int ysize, int ID, int Flags);

DATA_FRAME_Handle DATA_FRAME_CreateEx(int x0, int y0, int xsize, int ysize, 
                                      WM_HWIN hParent, int Id,WM_CALLBACK* cb,int WinFlags);

DATA_FRAME_Handle DATA_FRAME_CreateIndirect(const GUI_WIDGET_CREATE_INFO* pCreateInfo, 
                                            WM_HWIN hWinParent, int x0, int y0, 
                                            WM_CALLBACK* pCallback);

/***************************************************************************************************
*
*       Standard member functions
*
***************************************************************************************************/
void DATA_FRAME_SetBkColor  (DATA_FRAME_Handle hObj,GUI_COLOR Color);
void DATA_FRAME_SetHeadText(DATA_FRAME_Handle hObj,char * text);
void DATA_FRAME_SetDispFlag(DATA_FRAME_Handle hObj,int flag);
void DATA_FRAME_SetUnitText(DATA_FRAME_Handle hObj,char * text);
void DATA_FRAME_SetParameterNum(DATA_FRAME_Handle hObj,int num);
void DATA_FRAME_SetParamterText(DATA_FRAME_Handle hObj,char * text,int num);
void DATA_FRAME_SetParamterData(DATA_FRAME_Handle hObj,int data,int num);

/***************************************************************************************************
*
*       The callback ...
*
* Do not call it directly ! It is only to be used from within an
* overwritten callback.
*/
void DATA_FRAME_Callback(WM_MESSAGE *pMsg);




