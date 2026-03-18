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
File        : IMAGETEXT.h
Purpose     : IMAGETEXT public header file (API)
--------------------END-OF-HEADER-------------------------------------
*/
#ifndef IMAGETEXT_H
#define IMAGETEXT_H
#include "WM.h"
#include "DIALOG_Intern.h"      /* Req. for Create indirect data structure */
#include "WIDGET.h"
#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif
#if GUI_WINSUPPORT
    /************************************************************
    *
    *       Create flags
    */
    /* For compatibility only ! */
#define IMAGETEXT_CF_HIDE   WM_CF_HIDE
#define IMAGETEXT_CF_SHOW   WM_CF_SHOW
#define IMAGETEXT_CF_MEMDEV WM_CF_MEMDEV
    /************************************************************
    *
    *       States
    */
    /************************************************************
    *
    *       Types
    *
    *************************************************************
    */
    typedef WM_HMEM IMAGETEXT_Handle;
    /************************************************************
    *
    *       Create function(s)
      Note: the parameters to a create function may vary.
             Some widgets may have multiple create functions
    */
    IMAGETEXT_Handle IMAGETEXT_Create        (int x0, int y0, int xsize, int ysize, int ID, int Flags);
    IMAGETEXT_Handle IMAGETEXT_CreateAsChild (int x0, int y0, int xsize, int ysize, WM_HWIN hParent, int Id, int Flags);
    IMAGETEXT_Handle IMAGETEXT_CreateIndirect(const GUI_WIDGET_CREATE_INFO* pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK* cb);
    IMAGETEXT_Handle IMAGETEXT_CreateEx      (int x0, int y0, int xsize, int ysize, WM_HWIN hParent,
            int WinFlags, int ExFlags, int Id);
    /***************************************************************************************************
    *
    *       Standard member functions
    *
    **********************************************************************
    */
#define IMAGETEXT_EnableMemdev(hObj)  WM_EnableMemdev    (hObj)
#define IMAGETEXT_DisableMemdev(hObj) WM_DisableMemdev   (hObj)
#define IMAGETEXT_Delete(hObj)        WM_DeleteWindow    (hObj)
#define IMAGETEXT_Paint(hObj)         WM_Paint           (hObj)
#define IMAGETEXT_Invalidate(hObj)    WM_InvalidateWindow(hObj)
    /***************************************************************************************************
    *
    *       The callback ...
    *
    * Do not call it directly ! It is only to be used from within an
    * overwritten callback.
    */
    void IMAGETEXT_Callback(WM_MESSAGE *pMsg);
    /***************************************************************************************************
    *
    *       Member functions
    *
    **********************************************************************
    */
    GUI_COLOR        IMAGETEXT_GetBkColor         (IMAGETEXT_Handle hObj);
    const GUI_FONT GUI_UNI_PTR * IMAGETEXT_GetFont(IMAGETEXT_Handle hObj);
    void             IMAGETEXT_GetText            (IMAGETEXT_Handle hObj, char * pBuffer, int MaxLen);
    GUI_COLOR        IMAGETEXT_GetTextColor       (IMAGETEXT_Handle hObj);
    void             IMAGETEXT_SetBitmap          (IMAGETEXT_Handle hObj, const GUI_BITMAP * pBitmap);
    void             IMAGETEXT_SetBitmapEx        (IMAGETEXT_Handle hObj, const GUI_BITMAP * pBitmap, int x, int y);
	void             IMAGETEXT_SetBkColor         (IMAGETEXT_Handle hObj, GUI_COLOR Color);
	void             IMAGETEXT_SetFrameColor      (IMAGETEXT_Handle hObj, GUI_COLOR Color);
    void             IMAGETEXT_SetBMP             (IMAGETEXT_Handle hObj, const void * pBitmap);
    void             IMAGETEXT_SetBMPEx           (IMAGETEXT_Handle hObj, const void * pBitmap, int x, int y);
    void             IMAGETEXT_SetFont            (IMAGETEXT_Handle hObj, const GUI_FONT GUI_UNI_PTR * pfont);
    void             IMAGETEXT_SetStreamedBitmap  (IMAGETEXT_Handle hObj, const GUI_BITMAP_STREAM * pBitmap);
    void             IMAGETEXT_SetStreamedBitmapEx(IMAGETEXT_Handle hObj, const GUI_BITMAP_STREAM * pBitmap, int x, int y);
    void             IMAGETEXT_SetText            (IMAGETEXT_Handle hObj, const char* s);
    void             IMAGETEXT_SetTextAlign       (IMAGETEXT_Handle hObj, int Align);
    void             IMAGETEXT_SetTextColor       (IMAGETEXT_Handle hObj, GUI_COLOR Color);
    void             IMAGETEXT_SetSelfDrawEx      (IMAGETEXT_Handle hObj, void (*pDraw)(void), int x, int y); /* Not to be doc. */
    void             IMAGETEXT_SetSelfDraw        (IMAGETEXT_Handle hObj, void (*pDraw)(void));               /* Not to be doc. */
  
	
	void IMAGETEXT_SetState(IMAGETEXT_Handle hObj, int state);
	void IMAGETEXT_SetStyle(IMAGETEXT_Handle hObj, int style);
	/************************************************************
    *
    *       States
    #endif   /* IMAGETEXT_H */
#endif   /* IMAGETEXT_H */
#if defined(__cplusplus)
}
#endif
#endif   /* IMAGETEXT_H */
