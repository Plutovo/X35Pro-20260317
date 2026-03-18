//
/***************************************************************************************************
*                                                uC/GUI
*                        Universal graphic software for embedded applications
*
*                       (c) Copyright 2002, Micrium Inc., Weston, FL
*                       (c) Copyright 2002, SEGGER Microcontroller Systeme GmbH
*
*              µC/GUI is protected by international copyright laws. Knowledge of the
*              source code may not be used to write a similar product. This file may
*              only be used in accordance with a license and should not be redistributed
*              in any way. We appreciate your understanding and fairness.
*
----------------------------------------------------------------------------------------------------
File        : DATABAR.h
Purpose     : Progressbar include
--------------------END-OF-HEADER-------------------------------------------------------------------
*/
#ifndef DATABAR_H        /* Avoid multiple inclusion  */
#define DATABAR_H
#include "WM.h"
#include "DIALOG_Intern.h"      /* Req. for Create indirect data structure */
#if GUI_WINSUPPORT
#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif
//
typedef WM_HMEM DATABAR_Handle;
/***************************************************************************************************
*
*           Create
*
****************************************************************************************************
*/
//DATABAR_Handle DATABAR_Create        (int x0, int y0, int xsize, int ysize, int Flags);
//DATABAR_Handle DATABAR_CreateAsChild (int x0, int y0, int xsize, int ysize, 
//                                        WM_HWIN hParent, int Id, int Flags);

DATABAR_Handle DATABAR_CreateIndirect(const GUI_WIDGET_CREATE_INFO* pCreateInfo, 
                                      WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK* cb);

DATABAR_Handle DATABAR_CreateEx(int x0, int y0, int xsize, int ysize, 
                                WM_HWIN hParent,int WinFlags, int ExFlags, int Id);
/***************************************************************************************************
*
*                 Standard member functions
*
****************************************************************************************************
*/
#define DATABAR_EnableMemdev(hObj)  WM_EnableMemdev(hObj)
#define DATABAR_DisableMemdev(hObj) WM_DisableMemdev(hObj)
#define DATABAR_Delete(hObj)        WM_DeleteWindow(hObj)
#define DATABAR_Paint(hObj)         WM_Paint(hObj)


/***************************************************************************************************
*
*           Individual member functions
*
****************************************************************************************************
*/
void            DATABAR_SetBarColor (DATABAR_Handle hObj, unsigned int index, GUI_COLOR color);
void            DATABAR_SetFont     (DATABAR_Handle hObj, const GUI_FONT GUI_UNI_PTR * pfont);
void            DATABAR_SetMinMax   (DATABAR_Handle hObj, int Min, int Max);
void            DATABAR_SetText     (DATABAR_Handle hObj, const char* s);
void            DATABAR_SetTextAlign(DATABAR_Handle hObj, int Align);
void            DATABAR_SetTextColor(DATABAR_Handle hObj, unsigned int index, GUI_COLOR color);
void            DATABAR_SetTextPos  (DATABAR_Handle hObj, int XOff, int YOff);
void            DATABAR_SetValue    (DATABAR_Handle hObj, int v);

#if defined(__cplusplus)
}
#endif
#endif   /* GUI_WINSUPPORT */
#endif   /* DATABAR_H */
