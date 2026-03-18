/*
*********************************************************************************************************
*                                                uC/GUI
*                        Universal graphic software for embedded applications
*
*                       (c) Copyright 2002, Micrium Inc., Weston, FL
*                       (c) Copyright 2002, SEGGER Microcontroller Systeme GmbH
*
*              礐/GUI is protected by international copyright laws. Knowledge of the
*              source code may not be used to write a similar product. This file may
*              only be used in accordance with a license and should not be redistributed
*              in any way. We appreciate your understanding and fairness.
*
----------------------------------------------------------------------
File        : IMAGETEXT_Private.h
Purpose     : IMAGETEXT private header file
--------------------END-OF-HEADER-------------------------------------
*/
#ifndef IMAGETEXT_PRIVATE_H
#define IMAGETEXT_PRIVATE_H
#include "WM.h"
#include "IMAGETEXT.h"
#if GUI_WINSUPPORT
#define IMAGETEXT_ID 0x42555454
#define IMAGETEXT_H2P(h) (IMAGETEXT_Obj*) GUI_ALLOC_h2p(h)
#if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
#define IMAGETEXT_ASSERT_IS_VALID_PTR(p) GUI_DEBUG_ERROROUT_IF(p->DebugId != IMAGETEXT_ID, "IMAGETEXT.C: Wrong handle type or object not init'ed")
#define IMAGETEXT_INIT_ID(p)   p->DebugId = IMAGETEXT_ID
#define IMAGETEXT_DEINIT_ID(p) p->DebugId = IMAGETEXT_ID+1
#else
#define IMAGETEXT_ASSERT_IS_VALID_PTR(p)
#define IMAGETEXT_INIT_ID(p)
#define IMAGETEXT_DEINIT_ID(p)
#endif
typedef struct
{
    GUI_COLOR aBkColor;
    GUI_COLOR aTextColor;
	GUI_COLOR aFrameColor;
    const GUI_FONT GUI_UNI_PTR * pFont;
    I16 Align;
	GUI_WRAPMODE WrapMode;//ylk add 20190314
} IMAGETEXT_PROPS;
typedef struct
{
    WIDGET Widget;
    IMAGETEXT_PROPS Props;
    WM_HMEM hpText;
    WM_HMEM ahDrawObj;
	unsigned char BitmapTextFlag;//图片位置相较于文本位置，0Top，1bottom，2left，3right
#if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
    int DebugId;	
#endif	
	//
	int nStyle;//
	int nState;
} IMAGETEXT_Obj;
extern IMAGETEXT_PROPS IMAGETEXT__DefaultProps;
void IMAGETEXT__SetDrawObj(IMAGETEXT_Handle hObj, GUI_DRAW_HANDLE hDrawObj);
void IMAGETEXT_SetWrapMode(IMAGETEXT_Handle hObj,GUI_WRAPMODE Mode);
#endif   /* GUI_WINSUPPORT */
#endif   /* IMAGETEXT_H */
