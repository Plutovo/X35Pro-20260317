/*
*********************************************************************************************************
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
----------------------------------------------------------------------
File        : LIMITBAR.h
Purpose     : Progressbar include
--------------------END-OF-HEADER-------------------------------------
*/
#ifndef LIMITBAR_H        /* Avoid multiple inclusion  */
#define LIMITBAR_H
#include "WM.h"
#include "DIALOG_Intern.h"      /* Req. for Create indirect data structure */
#if GUI_WINSUPPORT
#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif
    typedef WM_HMEM LIMITBAR_Handle;
    /************************************************************
    *
    *           Create
    *
    *************************************************************
    */
//LIMITBAR_Handle LIMITBAR_Create        (int x0, int y0, int xsize, int ysize, int Flags);
//LIMITBAR_Handle LIMITBAR_CreateAsChild (int x0, int y0, int xsize, int ysize, WM_HWIN hParent, int Id, int Flags);
    LIMITBAR_Handle LIMITBAR_CreateIndirect(const GUI_WIDGET_CREATE_INFO* pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK* cb);
    LIMITBAR_Handle LIMITBAR_CreateEx      (int x0, int y0, int xsize, int ysize, WM_HWIN hParent,
                                            int WinFlags, int ExFlags, int Id);
    /************************************************************
    *
    *                 Standard member functions
    *
    *************************************************************
    */
#define LIMITBAR_EnableMemdev(hObj)  WM_EnableMemdev(hObj)
#define LIMITBAR_DisableMemdev(hObj) WM_DisableMemdev(hObj)
#define LIMITBAR_Delete(hObj)        WM_DeleteWindow(hObj)
#define LIMITBAR_Paint(hObj)         WM_Paint(hObj)
    /************************************************************
    *
    *           Individual member functions
    *
    *************************************************************
    */
    void            LIMITBAR_SetBarColor (LIMITBAR_Handle hObj, unsigned int index, GUI_COLOR color);
    void            LIMITBAR_SetFont     (LIMITBAR_Handle hObj, const GUI_FONT GUI_UNI_PTR * pfont);
    void            LIMITBAR_SetMinMax   (LIMITBAR_Handle hObj, int Min, int Max);
    void            LIMITBAR_SetText     (LIMITBAR_Handle hObj, const char* s);
    void            LIMITBAR_SetTextAlign(LIMITBAR_Handle hObj, int Align);
    void            LIMITBAR_SetTextColor(LIMITBAR_Handle hObj, unsigned int index, GUI_COLOR color);
    void            LIMITBAR_SetTextPos  (LIMITBAR_Handle hObj, int XOff, int YOff);
    void            LIMITBAR_SetValue    (LIMITBAR_Handle hObj, int v);
#if defined(__cplusplus)
}
#endif
#endif   /* GUI_WINSUPPORT */
#endif   /* LIMITBAR_H */
