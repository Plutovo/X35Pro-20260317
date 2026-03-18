/*
****************************************************************************************************
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
File        : BUTTON.h
Purpose     : BUTTON public header file (API)
--------------------END-OF-HEADER-------------------------------------------------------------------
*/
#ifndef BUTTON_MENU_H
#define BUTTON_MENU_H
//
#include "WM.h"
#include "DIALOG_Intern.h"      /* Req. for Create indirect data structure */
#include "WIDGET.h"
#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

#if GUI_WINSUPPORT
/***************************************************************************************************
*
*       Create flags
*/
/* For compatibility only ! */
#define BUTTON_MENU_CF_HIDE   WM_CF_HIDE
#define BUTTON_MENU_CF_SHOW   WM_CF_SHOW
#define BUTTON_MENU_CF_MEMDEV WM_CF_MEMDEV

/***************************************************************************************************
*
*       Color indices
*/
#define BUTTON_MENU_CI_UNPRESSED 0
#define BUTTON_MENU_CI_PRESSED   1
#define BUTTON_MENU_CI_DISABLED  2

/***************************************************************************************************
*
*       Bitmap indices
*/
#define BUTTON_MENU_BI_UNPRESSED 0
#define BUTTON_MENU_BI_PRESSED   1
#define BUTTON_MENU_BI_DISABLED  2

/***************************************************************************************************
*
*       Messages
*/


/***************************************************************************************************
*
*       States
*/
#define BUTTON_MENU_STATE_FOCUS      WIDGET_STATE_FOCUS
#define BUTTON_MENU_STATE_PRESSED    WIDGET_STATE_USER0


/***************************************************************************************************
*
*       Types
*/
typedef WM_HMEM BUTTON_MENU_Handle;


/***************************************************************************************************
*
*       Create function(s)
  Note: the parameters to a create function may vary.
         Some widgets may have multiple create functions
*/
BUTTON_MENU_Handle BUTTON_MENU_Create        (int x0, int y0, int xsize, int ysize, 
                                              int ID, int Flags);

BUTTON_MENU_Handle BUTTON_MENU_CreateAsChild (int x0, int y0, int xsize, int ysize, 
                                              WM_HWIN hParent, int Id, int Flags);

BUTTON_MENU_Handle BUTTON_MENU_CreateIndirect(const GUI_WIDGET_CREATE_INFO* pCreateInfo, 
                                              WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK* cb);

BUTTON_MENU_Handle BUTTON_MENU_CreateEx      (int x0, int y0, int xsize, int ysize, 
                                              WM_HWIN hParent,int WinFlags, int ExFlags, int Id);
/***************************************************************************************************
*
*       Standard member functions
*
*/
GUI_COLOR                    BUTTON_MENU_GetDefaultBkColor  (unsigned Index);
const GUI_FONT GUI_UNI_PTR * BUTTON_MENU_GetDefaultFont     (void);
int                          BUTTON_MENU_GetDefaultTextAlign(void);
GUI_COLOR                    BUTTON_MENU_GetDefaultTextColor(unsigned Index);
void                         BUTTON_MENU_SetDefaultBkColor  (GUI_COLOR Color, unsigned Index);
void                         BUTTON_MENU_SetDefaultFont     (const GUI_FONT GUI_UNI_PTR * pFont);
void                         BUTTON_MENU_SetDefaultTextAlign(int Align);
void                         BUTTON_MENU_SetDefaultTextColor(GUI_COLOR Color, unsigned Index);
//
#define BUTTON_MENU_EnableMemdev(hObj)  WM_EnableMemdev    (hObj)
#define BUTTON_MENU_DisableMemdev(hObj) WM_DisableMemdev   (hObj)
#define BUTTON_MENU_Delete(hObj)        WM_DeleteWindow    (hObj)
#define BUTTON_MENU_Paint(hObj)         WM_Paint           (hObj)
#define BUTTON_MENU_Invalidate(hObj)    WM_InvalidateWindow(hObj)
/***************************************************************************************************
*
*       The callback ...
*
* Do not call it directly ! It is only to be used from within an
* overwritten callback.
*/
void BUTTON_MENU_Callback(WM_MESSAGE *pMsg);

/***************************************************************************************************
*
*       Member functions
*
***************************************************************************************************/
//
//GUI_COLOR        BUTTON_GetBkColor         (BUTTON_Handle hObj, unsigned int Index);
//const GUI_FONT GUI_UNI_PTR * BUTTON_GetFont(BUTTON_Handle hObj);
//void             BUTTON_GetText            (BUTTON_Handle hObj, char * pBuffer, int MaxLen);
//GUI_COLOR        BUTTON_GetTextColor       (BUTTON_Handle hObj, unsigned int Index);
//unsigned         BUTTON_IsPressed          (BUTTON_Handle hObj);
//void             BUTTON_SetBitmap          (BUTTON_Handle hObj, unsigned int Index, const GUI_BITMAP * pBitmap);
//void             BUTTON_SetBitmapEx        (BUTTON_Handle hObj, unsigned int Index, const GUI_BITMAP * pBitmap, int x, int y);
  void             BUTTON_MENU_SetBkColor    (BUTTON_MENU_Handle hObj, unsigned int Index, GUI_COLOR Color);
//void             BUTTON_SetBMP             (BUTTON_Handle hObj, unsigned int Index, const void * pBitmap);
//void             BUTTON_SetBMPEx           (BUTTON_Handle hObj, unsigned int Index, const void * pBitmap, int x, int y);
  void             BUTTON_MENU_SetFont       (BUTTON_MENU_Handle hObj, const GUI_FONT GUI_UNI_PTR * pfont);
  void             BUTTON_MENU_SetState      (BUTTON_MENU_Handle hObj, int State);                                    /* Not to be doc. */
  void             BUTTON_MENU_SetPressed    (BUTTON_MENU_Handle hObj, int State);
//void             BUTTON_SetFocussable      (BUTTON_Handle hObj, int State);
//void             BUTTON_SetStreamedBitmap  (BUTTON_Handle hObj, unsigned int Index, const GUI_BITMAP_STREAM * pBitmap);
//void             BUTTON_SetStreamedBitmapEx(BUTTON_Handle hObj, unsigned int Index, const GUI_BITMAP_STREAM * pBitmap, int x, int y);
  void             BUTTON_MENU_SetText       (BUTTON_MENU_Handle hObj, const char* s);
  void             BUTTON_MENU_SetTextAlign  (BUTTON_MENU_Handle hObj, int Align);
  void             BUTTON_MENU_SetTextColor  (BUTTON_MENU_Handle hObj, unsigned int Index, GUI_COLOR Color);
//void             BUTTON_SetSelfDrawEx      (BUTTON_Handle hObj, unsigned int Index, void (*pDraw)(void), int x, int y); /* Not to be doc. */
//void             BUTTON_SetSelfDraw        (BUTTON_Handle hObj, unsigned int Index, void (*pDraw)(void));               /* Not to be doc. */

/***************************************************************************************************
*
*       States
*/
#define BUTTON_MENU_STATE_HASFOCUS 0
#endif   /* BUTTON_H */
#if defined(__cplusplus)
}
#endif
#endif   /* BUTTON_H */
